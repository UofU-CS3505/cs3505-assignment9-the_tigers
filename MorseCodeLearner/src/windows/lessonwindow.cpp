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

    ui->backButton->setIcon(QIcon(":/icons/back.png"));
    ui->backButton->setIconSize(QSize(52, 52));

    QPixmap lightOn(QPixmap::fromImage(QImage(":/icons/light_on.png")));
    QPixmap lightOff(QPixmap::fromImage(QImage(":/icons/light_off.png")));
    ui->flashIndicator->setPixmap(lightOff);
    ui->flashIndicator->setScaledContents(true);

    // Buttons
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &lessonwindow::onBackButtonClicked);
    QObject::connect(this, &lessonwindow::backButtonClicked, lessonHandler, &LessonHandler::onBackButtonClicked);

    // Key Event Filters
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, lessonHandler, &LessonHandler::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, lessonHandler, &LessonHandler::handleSpaceReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowPressed, lessonHandler, &LessonHandler::handleLeftArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowReleased, lessonHandler, &LessonHandler::handleLeftArrowReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowPressed, lessonHandler, &LessonHandler::handleRightArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowReleased, lessonHandler, &LessonHandler::handleRightArrowReleased);

    // Morse Handler
    QObject::connect(morseHandler, &MorseHandler::decodedInput, lessonHandler, &LessonHandler::onMorseReceived);

    // Lesson Handler
    QObject::connect(lessonHandler, &LessonHandler::lightIndicatorOn, this, [=]() {ui->flashIndicator->setPixmap(lightOn);});
    QObject::connect(lessonHandler, &LessonHandler::lightIndicatorOff, this, [=]() {ui->flashIndicator->setPixmap(lightOff);});
    QObject::connect(lessonHandler, &LessonHandler::guessCorrect, this, &lessonwindow::guessCorrect);
    QObject::connect(lessonHandler, &LessonHandler::guessIncorrect, this, &lessonwindow::guessIncorrect);
    QObject::connect(lessonHandler, &LessonHandler::displayTextToUI, this, &lessonwindow::displayTextQuestion);
    QObject::connect(lessonHandler, &LessonHandler::updateInputText, this, &lessonwindow::updateInputText);
    QObject::connect(lessonHandler, &LessonHandler::completedLesson, this, &lessonwindow::onBackButtonClicked);
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
    ui->problemText->setText(text);
}

void lessonwindow::updateInputText(QString inputText) {
    ui->inputText->setText(inputText);
}

