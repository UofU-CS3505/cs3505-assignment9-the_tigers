#include "practicewindow.h"
#include "ui_practicewindow.h"
#include <random>
#include <ctime>

practicewindow::practicewindow(QWidget *parent, MorseHandler *morseHandler, KeyEventFilter *keyEventFilter)
    : QWidget(parent)
    , ui(new Ui::practicewindow)
    , morseHandler(morseHandler)
    , keyEventFilter(keyEventFilter)
{
    QTimer timer(this);

    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.svg"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &practicewindow::onBackButtonClicked);

    QObject::connect(morseHandler, &MorseHandler::decodedInput, this, &practicewindow::onMorseReceived);

    ui->flashIndicator->setStyleSheet("QLabel { background-color : gray; border : 2px solid black; border-radius: 5px}");
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOn, this, [=]() {ui->flashIndicator->setStyleSheet("QLabel { background-color : white; border : 2px solid black; border-radius: 5px}");});
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOff, this, [=]() {ui->flashIndicator->setStyleSheet("QLabel { background-color : gray; border : 2px solid black; border-radius: 5px}");});

    // Key Event Filters
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, this, &practicewindow::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, this, &practicewindow::handleSpaceReleased);

    loadPracticeProblem();
}

practicewindow::~practicewindow() {
    delete ui;
}

void practicewindow::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool practicewindow::getUserOnThisPage() {
    return userOnThisPage;
}

void practicewindow::onBackButtonClicked() {
    ui->inputText->clear();

    emit goHome();
    userOnThisPage = false;
}

void practicewindow::handleSpacePressed() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput)
        return;
    morseHandler->straightKeyDown();
}

void practicewindow::handleSpaceReleased() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput)
        return;
    morseHandler->straightKeyUp();


}

void practicewindow::onMorseReceived(const string morse) {
    if (!userOnThisPage)
        return;
    QString qmorse = QString::fromStdString(morse);


    // Both of these checks for "/" and " " should only happen if the mode is easy.
    // For other modes we'll probably need some other logic

    if (qmorse == "/ ")
        return;

    if (qmorse == " ") {
        // check vs problem character
        if (morseHandler->decodeMorse(ui->inputText->toPlainText().toStdString()) == problemText) {
            acceptingInput = false;
            ui->problemText->setText("Correct!");
            timer.singleShot(1500, this, [this](){loadPracticeProblem();});
        }

        // if incorrect clear and try again
        if (morseHandler->decodeMorse(ui->inputText->toPlainText().toStdString()) != problemText) {
            acceptingInput = false;
            ui->problemText->setText("Try again!");
            timer.singleShot(1500, this, [this](){loadPracticeProblem(problemText);});
        }
        return;
    }

    ui->inputText->blockSignals(true);
    ui->inputText->setText(ui->inputText->toPlainText() + qmorse);
    ui->inputText->blockSignals(false);
}

void practicewindow::loadPracticeProblem() {
    // get character and display on screen
    ui->inputText->clear();

    // implement modes here
    problemText = getCharacter();
    ui->problemText->setText(QString::fromStdString(problemText));

    // reenable inputs on ui
    acceptingInput = true;
}

void practicewindow::loadPracticeProblem(std::string seedText) {
    // get character and display on screen
    ui->inputText->clear();

    // implement modes here
    problemText = seedText;
    ui->problemText->setText(QString::fromStdString(problemText));

    // reenable inputs on ui
    acceptingInput = true;
}


std::string practicewindow::getCharacter() {
    std::srand(std::time(0));
    int randomNumber = rand() % 26;
    char randomCharacter = 'a' + randomNumber;
    std::string characterString(1, randomCharacter);
    return characterString;
}


