#include "translatorwindow.h"
#include "ui_translatorwindow.h"
#include <QDebug>

translatorwindow::translatorwindow(QWidget *parent,
                                   TranslateHandler *translateHandler)
    : QWidget(parent)
    , ui(new Ui::translatorwindow)
    , translateHandler(translateHandler)
{
    ui->setupUi(this);

    // Setup ui
    ui->backButton->setIcon(QIcon(":/icons/back.png"));
    ui->backButton->setIconSize(QSize(52, 52));
    ui->swapButton->setIcon(QIcon(":/icons/swap.png"));
    ui->swapButton->setIconSize(QSize(104, 104));
    ui->clearButton->setIcon(QIcon(":/icons/clear.png"));
    ui->clearButton->setIconSize(QSize(64, 64));
    ui->audioPlayButton->setIcon(QIcon(":/icons/play.png"));
    ui->audioPlayButton->setIconSize(QSize(64, 64));
    ui->handGraphic->setPixmap(QPixmap::fromImage(QImage(":/icons/pointing_hand.png")));
    ui->handGraphic->setScaledContents(true);

    // Box2D Event Filter
    qApp->installEventFilter(this);

    QPixmap lightOn(QPixmap::fromImage(QImage(":/icons/light_on.png")));
    QPixmap lightOff(QPixmap::fromImage(QImage(":/icons/light_off.png")));
    ui->flashIndicator->setPixmap(lightOff);
    ui->flashIndicator->setScaledContents(true);

    // Changing input device
    QObject::connect(translateHandler, &TranslateHandler::paddleSelected, this, &translatorwindow::paddleSelected);
    QObject::connect(translateHandler, &TranslateHandler::straightKeySelected, this, &translatorwindow::straightKeySelected);

    // Page entering/leaving
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &translatorwindow::onBackButtonClicked);
    QObject::connect(this, &translatorwindow::leftTranslator, translateHandler, &TranslateHandler::onUserLeftTranslate);
    QObject::connect(this, &translatorwindow::enteredTranslator, translateHandler, &TranslateHandler::onUserEnteredTranslate);


    // Light indicator
    QObject::connect(translateHandler, &TranslateHandler::turnLightIndicatorOn, this, [=]() {ui->flashIndicator->setPixmap(lightOn);});
    QObject::connect(translateHandler, &TranslateHandler::turnLightIndicatorOff, this, [=]() {ui->flashIndicator->setPixmap(lightOff);});

    // Changing modes
    QObject::connect(translateHandler, &TranslateHandler::modeToEnglishToMorse, this, &translatorwindow::modeEnglishToMorse);
    QObject::connect(translateHandler, &TranslateHandler::modeToMorseToEnglish, this, &translatorwindow::modeMorseToEnglish);

    // Button handling
    QObject::connect(ui->swapButton, &QPushButton::clicked, translateHandler, &TranslateHandler::onSwapTranslateMode);
    QObject::connect(ui->audioPlayButton, &QPushButton::clicked, translateHandler, &TranslateHandler::onAudioPlayButtonClicked);
    QObject::connect(ui->clearButton, &QPushButton::clicked, translateHandler, &TranslateHandler::onClearInput);

    // Audio Icon Updating
    QObject::connect(translateHandler, &TranslateHandler::setPlayButtonPaused, this, &translatorwindow::handlePlaybackStopped);
    QObject::connect(translateHandler, &TranslateHandler::setPlayButtonPlaying, this, &translatorwindow::handlePlaybackStarted);

    // Text objects handling
    QObject::connect(ui->inputText, &QTextEdit::textChanged, translateHandler, [=]() {emit inputTextChanged(ui->inputText->toPlainText().toStdString());});
    QObject::connect(this, &translatorwindow::inputTextChanged, translateHandler, &TranslateHandler::onInputTextChanged);
    QObject::connect(translateHandler, &TranslateHandler::updateInputText, this, [=](const std::string text) {ui->inputText->setText(QString::fromStdString(text));});
    QObject::connect(translateHandler, &TranslateHandler::updateOutputText, this, [=](const std::string text) {ui->outputText->setText(QString::fromStdString(text));});
    QObject::connect(translateHandler, &TranslateHandler::updateMorsePreview, this, [=](const std::string text) {ui->morsePreview->setText(QString::fromStdString(text));});
}

translatorwindow::~translatorwindow() {
    delete ui;
}

void translatorwindow::straightKeySelected() {
    QObject::disconnect(straightPressedConnection);
    QObject::disconnect(straightReleasedConnection);
    QObject::disconnect(rightPressedConnection);
    QObject::disconnect(rightReleasedConnection);
    QObject::disconnect(leftPressedConnection);
    QObject::disconnect(leftReleasedConnection);

    // Straight key illustrations
    QPixmap straightKeyUp(QPixmap::fromImage(QImage(":/images/straight_key_up.png")));
    QPixmap straightKeyDown(QPixmap::fromImage(QImage(":/images/straight_key_down.png")));
    ui->illustrationLabel->setPixmap(straightKeyUp);
    straightPressedConnection = QObject::connect(translateHandler, &TranslateHandler::straightKeyPressed, this, [this, straightKeyDown](){ui->illustrationLabel->setPixmap(straightKeyDown);});
    straightReleasedConnection = QObject::connect(translateHandler, &TranslateHandler::straightKeyReleased, this, [this, straightKeyUp](){ui->illustrationLabel->setPixmap(straightKeyUp);});
}

void translatorwindow::paddleSelected() {
    QObject::disconnect(straightPressedConnection);
    QObject::disconnect(straightReleasedConnection);
    QObject::disconnect(rightPressedConnection);
    QObject::disconnect(rightReleasedConnection);
    QObject::disconnect(leftPressedConnection);
    QObject::disconnect(leftReleasedConnection);

    // Iambic paddle illustrations
    QPixmap paddleRight(QPixmap::fromImage(QImage(":/images/paddle_right.png")));
    QPixmap paddleLeft(QPixmap::fromImage(QImage(":/images/paddle_left.png")));
    QPixmap paddleCenter(QPixmap::fromImage(QImage(":/images/paddle_center.png")));
    QPixmap paddleBoth(QPixmap::fromImage(QImage(":/images/paddle_both.png")));
    ui->illustrationLabel->setPixmap(paddleCenter);
    rightPressedConnection = QObject::connect(translateHandler, &TranslateHandler::paddleRightPressed, this, [this, paddleRight, paddleBoth](){
        if (paddleState == LEFT) {
            paddleState = BOTH;
            ui->illustrationLabel->setPixmap(paddleBoth);
        } else {
            paddleState = RIGHT;
            ui->illustrationLabel->setPixmap(paddleRight);
        }
    });
    rightReleasedConnection = QObject::connect(translateHandler, &TranslateHandler::paddleRightReleased, this, [this, paddleCenter, paddleLeft](){
        if (paddleState == BOTH) {
            paddleState = LEFT;
            ui->illustrationLabel->setPixmap(paddleLeft);
        } else {
            paddleState = CENTER;
            ui->illustrationLabel->setPixmap(paddleCenter);
        }
    });
    leftPressedConnection = QObject::connect(translateHandler, &TranslateHandler::paddleLeftPressed, this, [this, paddleLeft, paddleBoth](){
        if (paddleState == RIGHT) {
            paddleState = BOTH;
            ui->illustrationLabel->setPixmap(paddleBoth);
        } else {
            paddleState = LEFT;
            ui->illustrationLabel->setPixmap(paddleLeft);
        }
    });
    leftReleasedConnection = QObject::connect(translateHandler, &TranslateHandler::paddleLeftReleased, this, [this, paddleCenter, paddleRight](){
        if (paddleState == BOTH) {
            paddleState = RIGHT;
            ui->illustrationLabel->setPixmap(paddleRight);
        } else {
            paddleState = CENTER;
            ui->illustrationLabel->setPixmap(paddleCenter);
        }
    });
}


void translatorwindow::setUserOnThisPage(bool userOnThisPage) {
    if (userOnThisPage)
        emit enteredTranslator();
}

void translatorwindow::onBackButtonClicked() {
    emit leftTranslator();
    emit goHome();
}

void translatorwindow::modeEnglishToMorse() {
    ui->inputLabel->setText("TEXT");
    ui->outputLabel->setText("MORSE");
}

void translatorwindow::modeMorseToEnglish() {
    ui->inputLabel->setText("MORSE");
    ui->outputLabel->setText("TEXT");
}

void translatorwindow::handlePlaybackStarted() {
    ui->audioPlayButton->setIcon(QIcon(":/icons/pause.png"));
    ui->clearButton->setEnabled(false);
    ui->inputText->setEnabled(false);
}

void translatorwindow::handlePlaybackStopped() {
    ui->audioPlayButton->setIcon(QIcon(":/icons/play.png"));
    ui->inputText->setEnabled(true);
    ui->clearButton->setEnabled(true);
}

