#include "wordslesson.h"
#include "ui_wordslesson.h"
#include <QSettings>

WordsLesson::WordsLesson(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WordsLesson)
{
    ui->setupUi(this);
    ui->backButton->setIcon(QIcon(":/icons/back.png"));
    ui->backButton->setIconSize(QSize(52, 52));
    ui->stackedWidget->setCurrentIndex(0);

    ui->previousSlideButton->setIcon(QIcon(":/icons/back.png"));
    ui->previousSlideButton->setIconSize(QSize(52, 52));
    ui->previousSlideButton->setVisible(false);

    ui->nextSlideButton->setIcon(QIcon(QPixmap(":/icons/back.png").transformed(QTransform().scale(-1, 1)))); // Reverses the arrow icon
    ui->nextSlideButton->setIconSize(QSize(52, 52));

    ui->startPracticeButton->setVisible(false);

    QObject::connect(ui->backButton, &QPushButton::clicked, this, &WordsLesson::onBackButtonClicked);
    QObject::connect(ui->nextSlideButton, &QPushButton::clicked, this, &WordsLesson::onNextSlideClicked);
    QObject::connect(ui->previousSlideButton, &QPushButton::clicked, this, &WordsLesson::onPreviousSlideClicked);
    QObject::connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &WordsLesson::onStackedWidgetIndexChange);
    QObject::connect(ui->startPracticeButton, &QPushButton::clicked, this, &WordsLesson::startPracticeLesson);

    currentIndex = 0;
    userOnThisPage = false;
}

WordsLesson::~WordsLesson() {
    delete ui;
}

void WordsLesson::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

void WordsLesson::onBackButtonClicked() {
    emit goBack();
    userOnThisPage = false;
    currentIndex = 0;
    ui->stackedWidget->setCurrentIndex(currentIndex);
}

void WordsLesson::onNextSlideClicked() {
    if (currentIndex != 2) {
        currentIndex++;
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void WordsLesson::onPreviousSlideClicked() {
    if (currentIndex != 0) {
        currentIndex--;
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void WordsLesson::onStackedWidgetIndexChange(int index) {
    if (index == 0) {
        ui->previousSlideButton->setVisible(false);
        ui->nextSlideButton->setVisible(true);
        ui->startPracticeButton->setVisible(false);
    } else if (index == 1) {
        ui->previousSlideButton->setVisible(true);
        ui->nextSlideButton->setVisible(true);
        ui->startPracticeButton->setVisible(false);
    } else if (index == 2) {
        ui->previousSlideButton->setVisible(true);
        ui->nextSlideButton->setVisible(false);
        ui->startPracticeButton->setVisible(true);
    }
}

void WordsLesson::startPracticeLesson() {
    emit goToPracticeLesson();
    userOnThisPage = false;
    currentIndex = 0;
    ui->stackedWidget->setCurrentIndex(currentIndex);
}
