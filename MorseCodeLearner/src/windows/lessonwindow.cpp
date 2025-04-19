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

    qDebug() << "lesson page constructed";


    ui->backButton->setIcon(QIcon(":/icons/back.png"));
    ui->backButton->setIconSize(QSize(52, 52));

    // Light indicator
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

    // Iambic paddle illustrations
    QObject::connect(lessonHandler, &LessonHandler::paddleSelected, this, [this, keyEventFilter](){
        QObject::disconnect(straightPressedConnection);
        QObject::disconnect(straightReleasedConnection);

        QPixmap paddleRight(QPixmap::fromImage(QImage(":/images/paddle_right.png")));
        QPixmap paddleLeft(QPixmap::fromImage(QImage(":/images/paddle_left.png")));
        QPixmap paddleCenter(QPixmap::fromImage(QImage(":/images/paddle_center.png")));
        ui->illustrationLabel->setPixmap(paddleCenter);
        rightPressedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowPressed, this, [this, paddleRight](){ui->illustrationLabel->setPixmap(paddleRight);});
        rightReleasedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowReleased, this, [this, paddleCenter](){ui->illustrationLabel->setPixmap(paddleCenter);});
        leftPressedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowPressed, this, [this, paddleLeft](){ui->illustrationLabel->setPixmap(paddleLeft);});
        leftReleasedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowReleased, this, [this, paddleCenter](){ui->illustrationLabel->setPixmap(paddleCenter);});
    });

    // Straight key illustrations
    QObject::connect(lessonHandler, &LessonHandler::straightKeySelected, this, [this, keyEventFilter](){
        QObject::disconnect(rightPressedConnection);
        QObject::disconnect(rightReleasedConnection);
        QObject::disconnect(leftPressedConnection);
        QObject::disconnect(leftReleasedConnection);

        QPixmap straightKeyUp(QPixmap::fromImage(QImage(":/images/straight_key_up.png")));
        QPixmap straightKeyDown(QPixmap::fromImage(QImage(":/images/straight_key_down.png")));
        ui->illustrationLabel->setPixmap(straightKeyUp);
        straightPressedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, this, [this, straightKeyDown](){ui->illustrationLabel->setPixmap(straightKeyDown);});
        straightReleasedConnection = QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, this, [this, straightKeyUp](){ui->illustrationLabel->setPixmap(straightKeyUp);});
    });
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

