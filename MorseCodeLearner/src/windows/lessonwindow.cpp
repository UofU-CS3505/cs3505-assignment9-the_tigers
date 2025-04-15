#include "lessonwindow.h"
#include "ui_lessonwindow.h"

lessonwindow::lessonwindow(LessonHandler *lessonHandler, MorseHandler *morseHandler, KeyEventFilter *keyEventFilter, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::lessonwindow)
    , lessonHandler(lessonHandler)
    , morseHandler(morseHandler)
    , keyEventFilter(keyEventFilter)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.svg"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &lessonwindow::on_backButton_clicked);

    // Key Event Filters
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, this, &lessonwindow::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, this, &lessonwindow::handleSpaceReleased);

    QObject::connect(lessonHandler, &LessonHandler::guessCorrect, this, &lessonwindow::guessCorrect);
    QObject::connect(lessonHandler, &LessonHandler::guessIncorrect, this, &lessonwindow::guessIncorrect);
    QObject::connect(lessonHandler, &LessonHandler::displayTextToUI, this, &lessonwindow::displayTextQuestion);

    acceptingInput = false;
}

lessonwindow::~lessonwindow()
{
    delete ui;
}

void lessonwindow::setUserOnThisPage(bool userOnThisPage) {
    if (userOnThisPage) {
        acceptingInput = true;
    } else {
        acceptingInput = false;
    }

    this->userOnThisPage = userOnThisPage;
}

bool lessonwindow::getUserOnThisPage() {
    return userOnThisPage;
}

void lessonwindow::on_backButton_clicked()
{
    emit goToLessonSelect();
    acceptingInput = false;
    userOnThisPage = false;
}

void lessonwindow::handleSpacePressed() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput)
        return;
    morseHandler->straightKeyDown();
}

void lessonwindow::handleSpaceReleased() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput)
        return;
    morseHandler->straightKeyUp();
}

void lessonwindow::guessCorrect() {

}

void lessonwindow::guessIncorrect() {

}

void lessonwindow::displayTextQuestion(const std::string text) {
    ui->problemText->setText("What is " + QString::fromStdString(text) + " in morse?");
    acceptingInput = true;
}

