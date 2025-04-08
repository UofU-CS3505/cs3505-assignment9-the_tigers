#include "translatorwindow.h"
#include "ui_translatorwindow.h"
#include <QDebug>

translatorwindow::translatorwindow(QWidget *parent,
                                   MorseHandler *morseHandler,
                                   MorseAudioHandler *audioHandler,
                                   KeyEventFilter *keyEventFilter)
    : QWidget(parent)
    , ui(new Ui::translatorwindow)
    , morseHandler(morseHandler)
    , audioHandler(audioHandler)
    , keyEventFilter(keyEventFilter)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.svg"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &translatorwindow::onBackButtonClicked);
    QObject::connect(ui->swapButton, &QPushButton::clicked, this, &translatorwindow::onSwapButtonClicked);
    QObject::connect(ui->inputText, &QTextEdit::textChanged, this, &translatorwindow::onInputTextTextChanged);
    QObject::connect(ui->audioPlayButton, &QPushButton::clicked, this, &translatorwindow::onAudioPlayButtonClicked);


    ui->swapButton->setIcon(QIcon(":/icons/swap.svg"));
    ui->swapButton->setIconSize(QSize(52, 104));

    qApp->installEventFilter(this);

    QObject::connect(morseHandler, &MorseHandler::decodedInput, this, &translatorwindow::onMorseReceived);
    QObject::connect(audioHandler, &MorseAudioHandler::playbackEnd, this, [=]() {ui->audioPlayButton->setText("Play");});
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, this, &translatorwindow::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, this, &translatorwindow::handleSpaceReleased);
}

translatorwindow::~translatorwindow()
{
    delete ui;
}

void translatorwindow::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool translatorwindow::getUserOnThisPage() {
    return userOnThisPage;
}

void translatorwindow::onBackButtonClicked() {
    audioHandler->stop();

    ui->inputText->clear();
    ui->outputText->clear();
    emit goHome();
    userOnThisPage = false;
}

void translatorwindow::onSwapButtonClicked()
{
    morseHandler->stopTimers();

    if (mode == MORSE_TO_TEXT) {
        mode = TEXT_TO_MORSE;
        ui->inputLabel->setText("Input: Text");
        ui->outputLabel->setText("Output: Morse");

    } else {
        mode = MORSE_TO_TEXT;
        ui->inputLabel->setText("Input: Morse");
        ui->outputLabel->setText("Output: Text");
    }

    ui->inputText->setText(ui->outputText->toPlainText());
}


void translatorwindow::onInputTextTextChanged()
{
    std::string text = ui->inputText->toPlainText().toStdString();
    if (mode == TEXT_TO_MORSE) {
        text = morseHandler->encodeText(text);
    } else {
        text = morseHandler->decodeMorse(text);
    }
    ui->outputText->setText(QString::fromStdString(text));
}

void translatorwindow::handleSpacePressed() {
    if (userOnThisPage == false || audioHandler->getPlayback() || mode == TEXT_TO_MORSE)
        return;
    audioHandler->start();
    morseHandler->straightKeyDown();
}

void translatorwindow::handleSpaceReleased() {
    if (userOnThisPage == false || audioHandler->getPlayback() || mode == TEXT_TO_MORSE)
        return;
    morseHandler->straightKeyUp();
    audioHandler->suspend();
}

void translatorwindow::onMorseReceived(const string morse) {
    QString qmorse = QString::fromStdString(morse);
    ui->inputText->blockSignals(true);
    ui->inputText->setText(ui->inputText->toPlainText() + qmorse);
    ui->inputText->blockSignals(false);

    if (morse == " ") {
        onInputTextTextChanged();
    }
}

void translatorwindow::onAudioPlayButtonClicked()
{
    if (audioHandler->getPlayback() == false) {
        audioHandler->suspend();
        ui->audioPlayButton->setText("Pause");
        if (mode == TEXT_TO_MORSE) {
            audioHandler->playMorse(ui->outputText->toPlainText().toStdString());
        } else {
            audioHandler->playMorse(ui->inputText->toPlainText().toStdString());
        }
    } else {
        ui->audioPlayButton->setText("Play");
        audioHandler->stop();
    }
}

