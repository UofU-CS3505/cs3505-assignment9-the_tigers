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

    ui->inputText->setFixedWidth(1081);

    QPixmap soundPlaying(QPixmap::fromImage(QImage(":/icons/playing_audio.png")));
    QPixmap soundNotPlaying(QPixmap::fromImage(QImage(":/icons/not_playing_audio.png")));
    ui->soundDisplayLabel->hide();
    ui->soundDisplayLabel->setPixmap(QPixmap::fromImage(QImage(":/icons/playing_audio.png")));
    ui->soundDisplayLabel->setScaledContents(true);

    ui->backButton->setIcon(QIcon(":/icons/back.svg"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, practiceHandler, &PracticeHandler::onBackButtonClicked);

    QObject::connect(practiceHandler, &PracticeHandler::lightIndicatorOn, this, [=]() {ui->flashIndicator->setStyleSheet("QLabel { background-color : white; border: 1px solid black;}");});
    QObject::connect(practiceHandler, &PracticeHandler::lightIndicatorOff, this, [=]() {ui->flashIndicator->setStyleSheet("QLabel { background-color : gray; border: 1px solid black;}");});

    // Key Event Filters
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, practiceHandler, &PracticeHandler::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, practiceHandler, &PracticeHandler::handleSpaceReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::enterPressed, practiceHandler, &PracticeHandler::checkProblem);

    QObject::connect(practiceHandler, &PracticeHandler::updateInputText, this, &practicewindow::updateInputText);
    QObject::connect(practiceHandler, &PracticeHandler::updatePracticeText, this, &practicewindow::updatePracticeText);
    QObject::connect(ui->difficultySelectBox, &QComboBox::currentTextChanged, practiceHandler, &PracticeHandler::setDifficulty);

    QObject::connect(ui->inputText, &QLineEdit::textChanged, practiceHandler, [practiceHandler, this](){practiceHandler->receiveInputText(ui->inputText->text());});

    QObject::connect(ui->modeSelectBox, &QComboBox::currentTextChanged, this, [=](){
        QString newMode = ui->modeSelectBox->currentText();
        practiceHandler->setMode(ui->modeSelectBox->currentText());
        if (newMode == "Decode Morse" || newMode == "Decode Sound") {
            ui->inputText->setFixedWidth(1011);
        } else {
            ui->inputText->setFixedWidth(1081);
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

    ui->checkInputButton->hide();
    QObject::connect(practiceHandler, &PracticeHandler::showInputCheck, ui->checkInputButton, &QPushButton::show);
    QObject::connect(practiceHandler, &PracticeHandler::hideInputCheck, ui->checkInputButton, &QPushButton::hide);

    QObject::connect(practiceHandler, &PracticeHandler::isInputReadOnly, ui->inputText, &QLineEdit::setReadOnly);

    QObject::connect(ui->checkInputButton, &QPushButton::clicked, practiceHandler, &PracticeHandler::checkProblem);

    QObject::connect(practiceHandler, &PracticeHandler::focusInput, this, [this]() {ui->inputText->setFocus();});
    QObject::connect(practiceHandler, &PracticeHandler::updateScore, ui->scoreDisplayLabel, &QLabel::setText);

    QObject::connect(practiceHandler, &PracticeHandler::soundPlaying, this, [this, soundPlaying]() {ui->soundDisplayLabel->setPixmap(soundPlaying);});
    QObject::connect(practiceHandler, &PracticeHandler::soundNotPlaying, this, [this, soundNotPlaying]() {ui->soundDisplayLabel->setPixmap(soundNotPlaying);});
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

