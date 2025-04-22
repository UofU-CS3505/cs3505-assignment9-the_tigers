#include "translatorwindow.h"
#include "ui_translatorwindow.h"
#include <QDebug>

translatorwindow::translatorwindow(QWidget *parent,
                                   MorseHandler *morseHandler,
                                   KeyEventFilter *keyEventFilter)
    : QWidget(parent)
    , ui(new Ui::translatorwindow)
    , morseHandler(morseHandler)
    , keyEventFilter(keyEventFilter)
{
    ui->setupUi(this);


    QObject::connect(ui->backButton, &QPushButton::clicked, this, &translatorwindow::onBackButtonClicked);
    QObject::connect(ui->swapButton, &QPushButton::clicked, this, &translatorwindow::onSwapButtonClicked);
    QObject::connect(ui->inputText, &QTextEdit::textChanged, this, &translatorwindow::onInputTextTextChanged);
    QObject::connect(ui->audioPlayButton, &QPushButton::clicked, this, &translatorwindow::onAudioPlayButtonClicked);
    QObject::connect(ui->clearButton, &QPushButton::clicked, this, &translatorwindow::clearInput);

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

    qApp->installEventFilter(this);

    QPixmap lightOn(QPixmap::fromImage(QImage(":/icons/light_on.png")));
    QPixmap lightOff(QPixmap::fromImage(QImage(":/icons/light_off.png")));
    ui->flashIndicator->setPixmap(lightOff);
    ui->flashIndicator->setScaledContents(true);

    // Light indicator
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOn, this, [=]() {ui->flashIndicator->setPixmap(lightOn);});
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOff, this, [=]() {ui->flashIndicator->setPixmap(lightOff);});

    QObject::connect(morseHandler, &MorseHandler::decodedInput, this, &translatorwindow::onMorseReceived);
    QObject::connect(morseHandler, &MorseHandler::playbackEnd, this, &translatorwindow::handlePlaybackStopped);

    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, this, &translatorwindow::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, this, &translatorwindow::handleSpaceReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowPressed, this, &translatorwindow::handleLeftArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowReleased, this, &translatorwindow::handleLeftArrowReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowPressed, this, &translatorwindow::handleRightArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowReleased, this, &translatorwindow::handleRightArrowReleased);
}

translatorwindow::~translatorwindow() {
    delete ui;
}

void translatorwindow::setUserOnThisPage(bool userOnThisPage) {
    ui->morsePreview->setText("");
    this->userOnThisPage = userOnThisPage;

    if (userOnThisPage) {
        if (morseHandler->getDevice() == MorseHandler::STRAIGHT_KEY) {
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
            straightPressedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, this, [this, straightKeyDown](){ui->illustrationLabel->setPixmap(straightKeyDown);});
            straightReleasedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, this, [this, straightKeyUp](){ui->illustrationLabel->setPixmap(straightKeyUp);});
        } else {
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
            rightPressedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowPressed, this, [this, paddleRight, paddleBoth](){
                if (paddleState == LEFT) {
                    paddleState = BOTH;
                    ui->illustrationLabel->setPixmap(paddleBoth);
                } else {
                    paddleState = RIGHT;
                    ui->illustrationLabel->setPixmap(paddleRight);
                }
            });
            rightReleasedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowReleased, this, [this, paddleCenter, paddleLeft](){
                if (paddleState == BOTH) {
                    paddleState = LEFT;
                    ui->illustrationLabel->setPixmap(paddleLeft);
                } else {
                    paddleState = CENTER;
                    ui->illustrationLabel->setPixmap(paddleCenter);
                }
            });
            leftPressedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowPressed, this, [this, paddleLeft, paddleBoth](){
                if (paddleState == RIGHT) {
                    paddleState = BOTH;
                    ui->illustrationLabel->setPixmap(paddleBoth);
                } else {
                    paddleState = LEFT;
                    ui->illustrationLabel->setPixmap(paddleLeft);
                }
            });
            leftReleasedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowReleased, this, [this, paddleCenter, paddleRight](){
                if (paddleState == BOTH) {
                    paddleState = RIGHT;
                    ui->illustrationLabel->setPixmap(paddleRight);
                } else {
                    paddleState = CENTER;
                    ui->illustrationLabel->setPixmap(paddleCenter);
                }
            });
        }
    }
}

bool translatorwindow::getUserOnThisPage() {
    return userOnThisPage;
}

void translatorwindow::onBackButtonClicked() {
    morseHandler->stopPlayback();

    ui->inputText->clear();
    ui->outputText->clear();
    emit goHome();
    userOnThisPage = false;
}

void translatorwindow::onSwapButtonClicked() {
    morseHandler->stopTimers();

    if (mode == MORSE_TO_TEXT) {
        mode = TEXT_TO_MORSE;
        ui->inputLabel->setText("TEXT");
        ui->outputLabel->setText("MORSE");

    } else {
        mode = MORSE_TO_TEXT;
        ui->inputLabel->setText("MORSE");
        ui->outputLabel->setText("TEXT");
    }

    ui->inputText->setText(ui->outputText->toPlainText());
}

void translatorwindow::onInputTextTextChanged() {
    std::string text = ui->inputText->toPlainText().toStdString();
    if (mode == TEXT_TO_MORSE) {
        text = morseHandler->encodeText(text);
    } else {
        text = morseHandler->decodeMorse(text);
    }
    ui->outputText->setText(QString::fromStdString(text));
}

void translatorwindow::handleSpacePressed() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || mode == TEXT_TO_MORSE)
        return;
    morseHandler->straightKeyDown();
    QPixmap straightKeyDown(QPixmap::fromImage(QImage(":/images/straight_key_down.png")));
    ui->illustrationLabel->setPixmap(straightKeyDown);
}

void translatorwindow::handleSpaceReleased() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || mode == TEXT_TO_MORSE)
        return;
    QPixmap straightKeyUp(QPixmap::fromImage(QImage(":/images/straight_key_up.png")));
    ui->illustrationLabel->setPixmap(straightKeyUp);
    morseHandler->straightKeyUp();
}

void translatorwindow::handleLeftArrowPressed() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || mode == TEXT_TO_MORSE)
        return;
    morseHandler->paddleDotDown();
}

void translatorwindow::handleLeftArrowReleased() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || mode == TEXT_TO_MORSE)
        return;
    morseHandler->paddleDotUp();
}

void translatorwindow::handleRightArrowPressed() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || mode == TEXT_TO_MORSE)
        return;
    morseHandler->paddleDashDown();
}

void translatorwindow::handleRightArrowReleased() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || mode == TEXT_TO_MORSE)
        return;
    morseHandler->paddleDashUp();
}

void translatorwindow::onMorseReceived(const string morse) {
    if (userOnThisPage == false) {
        return;
    }
    QString qmorse = QString::fromStdString(morse);

    if (qmorse == "/ " || qmorse == " ") {
        ui->inputText->blockSignals(true);
        ui->inputText->setText(ui->inputText->toPlainText() + ui->morsePreview->text() + qmorse);
        ui->inputText->blockSignals(false);
        ui->morsePreview->clear();
    } else {
        ui->morsePreview->setText(ui->morsePreview->text() + qmorse);
    }

    if (morse == " ") {
        onInputTextTextChanged();
    }
}

void translatorwindow::onAudioPlayButtonClicked() {
    if (!morseHandler->getPlayback()) {
        morseHandler->stopPlayback();
        ui->audioPlayButton->setIcon(QIcon(":/icons/pause.png"));
        ui->clearButton->setEnabled(false);
        ui->inputText->setEnabled(false);

        if (mode == TEXT_TO_MORSE) {
            morseHandler->playMorse(ui->outputText->toPlainText().toStdString());
        } else {
            morseHandler->playMorse(ui->inputText->toPlainText().toStdString());
        }
    } else {
        handlePlaybackStopped();
        morseHandler->stopPlayback();
    }
}

void translatorwindow::clearInput() {
    morseHandler->stopPlayback();
    ui->inputText->clear();
    ui->outputText->clear();
}

void translatorwindow::handlePlaybackStopped() {
    ui->audioPlayButton->setIcon(QIcon(":/icons/play.png"));
    ui->inputText->setEnabled(true);
    ui->clearButton->setEnabled(true);
}

