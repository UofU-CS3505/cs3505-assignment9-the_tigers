#include "practicewindow.h"
#include "ui_practicewindow.h"
#include <ctime>

practicewindow::practicewindow(QWidget *parent, KeyEventFilter *keyEventFilter, PracticeHandler *practiceHandler)
    : QWidget(parent)
    , ui(new Ui::practicewindow)
    , keyEventFilter(keyEventFilter)
    , practiceHandler(practiceHandler)
{
    ui->setupUi(this);

    QPixmap soundPlaying(QPixmap::fromImage(QImage(":/icons/playing_audio.png")));
    QPixmap soundNotPlaying(QPixmap::fromImage(QImage(":/icons/not_playing_audio.png")));
    ui->soundDisplayLabel->hide();
    ui->soundDisplayLabel->setPixmap(soundNotPlaying);
    ui->soundDisplayLabel->setScaledContents(true);

    ui->backButton->setIcon(QIcon(":/icons/back.png"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, practiceHandler, &PracticeHandler::onBackButtonClicked);

    ui->checkInputButton->setIcon(QIcon(":/icons/check.png"));
    ui->checkInputButton->setIconSize(QSize(52, 52));
    ui->checkInputButton->hide();

    ui->skipButton->setIcon(QIcon(":/icons/skip.png"));
    ui->skipButton->setIconSize(QSize(52, 52));
    // add slight left padding
    ui->skipButton->setStyleSheet("QPushButton {border: 1px solid black;\nbackground-color: transparent;\npadding-left: 4px;\n}\nQPushButton:hover {\n    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(255, 255, 255, 255), stop:1 rgba(255, 255, 255, 0));\n}\n");

    QPixmap lightOn(QPixmap::fromImage(QImage(":/icons/light_on.png")));
    QPixmap lightOff(QPixmap::fromImage(QImage(":/icons/light_off.png")));
    ui->flashIndicator->setPixmap(lightOff);
    ui->flashIndicator->setScaledContents(true);

    // Telegraph illustrations
    QPixmap straightKeyUp(QPixmap::fromImage(QImage(":/images/straight_key_up.png")));
    QPixmap straightKeyDown(QPixmap::fromImage(QImage(":/images/straight_key_down.png")));
    ui->illustrationLabel->setScaledContents(true);
    ui->illustrationLabel->setPixmap(straightKeyUp);
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, this, [this, straightKeyDown](){ui->illustrationLabel->setPixmap(straightKeyDown);});
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, this, [this, straightKeyUp](){ui->illustrationLabel->setPixmap(straightKeyUp);});

    // Light Indicator
    QObject::connect(practiceHandler, &PracticeHandler::lightIndicatorOn, this, [=]() {ui->flashIndicator->setPixmap(lightOn);});
    QObject::connect(practiceHandler, &PracticeHandler::lightIndicatorOff, this, [=]() {ui->flashIndicator->setPixmap(lightOff);});

    // Key Event Filters
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, practiceHandler, &PracticeHandler::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, practiceHandler, &PracticeHandler::handleSpaceReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::enterPressed, practiceHandler, &PracticeHandler::checkProblem);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowPressed, practiceHandler, &PracticeHandler::handleLeftArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowReleased, practiceHandler, &PracticeHandler::handleLeftArrowReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowPressed, practiceHandler, &PracticeHandler::handleRightArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowReleased, practiceHandler, &PracticeHandler::handleRightArrowReleased);

    QObject::connect(practiceHandler, &PracticeHandler::updateInputText, this, &practicewindow::updateInputText);
    QObject::connect(practiceHandler, &PracticeHandler::updatePracticeText, this, &practicewindow::updatePracticeText);
    QObject::connect(ui->difficultySelectBox, &QComboBox::currentTextChanged, practiceHandler, &PracticeHandler::setDifficulty);

    QObject::connect(ui->inputText, &QLineEdit::textChanged, practiceHandler, [practiceHandler, this](){practiceHandler->receiveInputText(ui->inputText->text());});

    QObject::connect(ui->modeSelectBox, &QComboBox::currentTextChanged, this, [=](){
        QString newMode = ui->modeSelectBox->currentText();
        practiceHandler->setMode(ui->modeSelectBox->currentText());
        if (newMode == "Decode Morse" || newMode == "Decode Sound") {
            ui->checkInputButton->show();
            ui->skipButton->setFixedHeight(61);
            // remove left padding
            ui->skipButton->setStyleSheet("QPushButton {border: 1px solid black;\nbackground-color: transparent;\npadding-left: 0px;\n}\nQPushButton:hover {\n    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(255, 255, 255, 255), stop:1 rgba(255, 255, 255, 0));\n}\n");
            ui->skipButton->move(1110, 260);
        } else {
            ui->checkInputButton->hide();
            ui->skipButton->setFixedHeight(131);
            // add slight left padding
            ui->skipButton->setStyleSheet("QPushButton {border: 1px solid black;\nbackground-color: transparent;\npadding-left: 4px;\n}\nQPushButton:hover {\n    background: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 rgba(255, 255, 255, 255), stop:1 rgba(255, 255, 255, 0));\n}\n");
            ui->skipButton->move(1110, 190);
        }

        if (newMode == "Decode Morse") {
            ui->modeInstructionLabel->setText("Translate the morse code into English. Check your answer with the ENTER key or the button!");
            ui->soundDisplayLabel->hide();
        } else if (newMode == "Decode Sound") {
            ui->modeInstructionLabel->setText("Translate the audible morse code into English. Check your answer with the ENTER key or the button!");
            ui->soundDisplayLabel->show();
        } else {
            ui->modeInstructionLabel->setText("Translate English by inputting morse code with the spacebar or paddles. Your answers are checked automatically!");
            ui->soundDisplayLabel->hide();
        }
    }); 

    QObject::connect(practiceHandler, &PracticeHandler::isInputReadOnly, ui->inputText, &QLineEdit::setReadOnly);

    QObject::connect(ui->checkInputButton, &QPushButton::clicked, practiceHandler, &PracticeHandler::checkProblem);

    QObject::connect(practiceHandler, &PracticeHandler::focusInput, this, [this]() {ui->inputText->setFocus();});
    QObject::connect(practiceHandler, &PracticeHandler::updateScore, ui->scoreDisplayLabel, &QLabel::setText);

    QObject::connect(practiceHandler, &PracticeHandler::soundPlaying, this, [this, soundPlaying]() {ui->soundDisplayLabel->setPixmap(soundPlaying);});
    QObject::connect(practiceHandler, &PracticeHandler::soundNotPlaying, this, [this, soundNotPlaying]() {ui->soundDisplayLabel->setPixmap(soundNotPlaying);});

    QObject::connect(ui->skipButton, &QPushButton::clicked, practiceHandler, &PracticeHandler::skipProblem);
    QObject::connect(practiceHandler, &PracticeHandler::updateHighScore, ui->highScoreDisplayLabel, &QLabel::setText);
}

practicewindow::~practicewindow() {
    delete ui;
}

void practicewindow::setUserOnThisPage(bool userOnThisPage) {
    practiceHandler->setUserOnThisPage(userOnThisPage);
}

bool practicewindow::getUserOnThisPage() {
    return practiceHandler->getUserOnThisPage();
}

void practicewindow::updateInputText(QString text) {
    ui->inputText->setText(text);
}

void practicewindow::updatePracticeText(QString text) {
    ui->problemText->setText(text);
}

