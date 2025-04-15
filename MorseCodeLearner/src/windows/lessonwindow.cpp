#include "lessonwindow.h"
#include "ui_lessonwindow.h"

lessonwindow::lessonwindow(LessonHandler *lessonHandler, MorseHandler *morseHandler, KeyEventFilter *keyEventFilter, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::lessonwindow)
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

    acceptingInput = true;
}

lessonwindow::~lessonwindow()
{
    delete ui;
}

void lessonwindow::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool lessonwindow::getUserOnThisPage() {
    return userOnThisPage;
}

void lessonwindow::on_backButton_clicked()
{
    emit goToLessonSelect();
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

