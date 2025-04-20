#include "numbersbasics.h"
#include <QSettings>
#include "ui_numbersbasics.h"

NumbersBasics::NumbersBasics(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NumbersBasics)
{
    ui->setupUi(this);
    ui->backButton_4->setIcon(QIcon(":/icons/back.png"));
    ui->backButton_4->setIconSize(QSize(52, 52));
    ui->stackedWidget->setCurrentIndex(0);

    ui->previousSlideButton_4->setIcon(QIcon(":/icons/back.png"));
    ui->previousSlideButton_4->setIconSize(QSize(52, 52));
    ui->previousSlideButton_4->setVisible(false);

    ui->nextSlideButton_4->setIcon(QIcon(QPixmap(":/icons/back.png").transformed(QTransform().scale(-1, 1)))); // Reverses the arrow icon
    ui->nextSlideButton_4->setIconSize(QSize(52, 52));

    ui->completeLessonButton_4->setVisible(false);

    QObject::connect(ui->backButton_4, &QPushButton::clicked, this, &NumbersBasics::onBackButtonClicked);
    QObject::connect(ui->nextSlideButton_4, &QPushButton::clicked, this, &NumbersBasics::onNextSlideClicked);
    QObject::connect(ui->previousSlideButton_4, &QPushButton::clicked, this, &NumbersBasics::onPreviousSlideClicked);
    QObject::connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &NumbersBasics::onStackedWidgetIndexChange);
    QObject::connect(ui->completeLessonButton_4, &QPushButton::clicked, this, &NumbersBasics::completeLesson);

    currentIndex = 0;
    userOnThisPage = false;
}

NumbersBasics::~NumbersBasics()
{
    delete ui;
}

void NumbersBasics::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

void NumbersBasics::onBackButtonClicked() {
    emit goBack();
    userOnThisPage = false;
    currentIndex = 0;
    ui->stackedWidget->setCurrentIndex(currentIndex);
}

void NumbersBasics::onNextSlideClicked() {
    if (currentIndex != 2) {
        currentIndex++;
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void NumbersBasics::onPreviousSlideClicked() {
    if (currentIndex != 0) {
        currentIndex--;
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void NumbersBasics::onStackedWidgetIndexChange(int index) {
    if (index == 0) {
        ui->previousSlideButton_4->setVisible(false);
        ui->nextSlideButton_4->setVisible(true);
        ui->completeLessonButton_4->setVisible(false);
    } else if (index == 1) {
        ui->previousSlideButton_4->setVisible(true);
        ui->nextSlideButton_4->setVisible(true);
        ui->completeLessonButton_4->setVisible(false);
    } else if (index == 2) {
        ui->previousSlideButton_4->setVisible(true);
        ui->nextSlideButton_4->setVisible(false);
        ui->completeLessonButton_4->setVisible(true);
    }
}

void NumbersBasics::completeLesson() {
    QSettings settings("Tigers", "MorseCodeLearner");
    settings.setValue("lesson7Completed", true);

    onBackButtonClicked();
}
