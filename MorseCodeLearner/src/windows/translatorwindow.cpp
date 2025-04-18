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

    ui->backButton->setIcon(QIcon(":/icons/back.svg"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &translatorwindow::onBackButtonClicked);
    QObject::connect(ui->swapButton, &QPushButton::clicked, this, &translatorwindow::onSwapButtonClicked);
    QObject::connect(ui->inputText, &QTextEdit::textChanged, this, &translatorwindow::onInputTextTextChanged);
    QObject::connect(ui->audioPlayButton, &QPushButton::clicked, this, &translatorwindow::onAudioPlayButtonClicked);
    QObject::connect(ui->clearButton, &QPushButton::clicked, this, &translatorwindow::clearInput);

    ui->swapButton->setIcon(QIcon(":/icons/swap.svg"));
    ui->swapButton->setIconSize(QSize(52, 104));
    ui->clearButton->setIcon(QIcon(":/icons/clear.png"));
    ui->clearButton->setIconSize(QSize(58, 58));

    qApp->installEventFilter(this);

    ui->audioPlayButton->setIcon(QIcon(":/icons/play.svg"));
    ui->audioPlayButton->setIconSize(QSize(52, 52));

    QObject::connect(morseHandler, &MorseHandler::decodedInput, this, &translatorwindow::onMorseReceived);
    QObject::connect(morseHandler, &MorseHandler::playbackEnd, this, &translatorwindow::handlePlaybackStopped);
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOn, this, [=]() {ui->flashIndicator->setStyleSheet("QLabel { background-color : white; border : 1px solid black;}");});
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOff, this, [=]() {ui->flashIndicator->setStyleSheet("QLabel { background-color : gray; border : 1px solid black;}");});

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
    this->userOnThisPage = userOnThisPage;
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
}

void translatorwindow::handleSpaceReleased() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || mode == TEXT_TO_MORSE)
        return;
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
    ui->inputText->blockSignals(true);
    ui->inputText->setText(ui->inputText->toPlainText() + qmorse);
    ui->inputText->blockSignals(false);

    if (morse == " ") {
        onInputTextTextChanged();
    }
}

void translatorwindow::onAudioPlayButtonClicked() {
    if (!morseHandler->getPlayback()) {
        morseHandler->stopPlayback();
        ui->audioPlayButton->setIcon(QIcon(":/icons/pause.svg"));
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
    ui->audioPlayButton->setIcon(QIcon(":/icons/play.svg"));
    ui->inputText->setEnabled(true);
    ui->clearButton->setEnabled(true);
}

