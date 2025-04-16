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
    ui->flashIndicator->setStyleSheet("QLabel { background-color : gray; border : 2px solid black; border-radius: 5px}");

    QObject::connect(ui->backButton, &QPushButton::clicked, this, &lessonwindow::onBackButtonClicked);
    QObject::connect(this, &lessonwindow::backButtonClicked, lessonHandler, &LessonHandler::onBackButtonClicked);

    // Key Event Filters
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, lessonHandler, &LessonHandler::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, lessonHandler, &LessonHandler::handleSpaceReleased);

    QObject::connect(morseHandler, &MorseHandler::decodedInput, lessonHandler, &LessonHandler::onMorseReceived);

    QObject::connect(lessonHandler, &LessonHandler::lightIndicatorOn, this, [=]() {ui->flashIndicator->setStyleSheet("QLabel { background-color : white; border: 1px solid black;}");});
    QObject::connect(lessonHandler, &LessonHandler::lightIndicatorOff, this, [=]() {ui->flashIndicator->setStyleSheet("QLabel { background-color : gray; border: 1px solid black;}");});
    QObject::connect(lessonHandler, &LessonHandler::guessCorrect, this, &lessonwindow::guessCorrect);
    QObject::connect(lessonHandler, &LessonHandler::guessIncorrect, this, &lessonwindow::guessIncorrect);
    QObject::connect(lessonHandler, &LessonHandler::displayTextToUI, this, &lessonwindow::displayTextQuestion);
    QObject::connect(lessonHandler, &LessonHandler::updateInputText, this, &lessonwindow::updateInputText);
}

lessonwindow::~lessonwindow()
{
    delete ui;
}

void lessonwindow::onBackButtonClicked() {
    emit backButtonClicked();
    emit goToLessonSelect();
}

void lessonwindow::guessCorrect() {
    ui->problemText->setText("Correct!");
}

void lessonwindow::guessIncorrect() {
    ui->problemText->setText("Incorrect!");
}

void lessonwindow::displayTextQuestion(QString text) {
    ui->inputText->setText("");
    ui->problemText->setText("What is " + text + " in morse?");
}

void lessonwindow::updateInputText(QString inputText) {
    ui->inputText->setText(inputText);
}

