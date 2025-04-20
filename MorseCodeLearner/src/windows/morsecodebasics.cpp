#include "morsecodebasics.h"
#include "ui_morsecodebasics.h"
#include <QSettings>

MorseCodeBasics::MorseCodeBasics(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MorseCodeBasics)
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

    ui->completeLessonButton->setVisible(false);

    QObject::connect(ui->backButton, &QPushButton::clicked, this, &MorseCodeBasics::onBackButtonClicked);
    QObject::connect(ui->nextSlideButton, &QPushButton::clicked, this, &MorseCodeBasics::onNextSlideClicked);
    QObject::connect(ui->previousSlideButton, &QPushButton::clicked, this, &MorseCodeBasics::onPreviousSlideClicked);
    QObject::connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &MorseCodeBasics::onStackedWidgetIndexChange);
    QObject::connect(ui->completeLessonButton, &QPushButton::clicked, this, &MorseCodeBasics::completeLesson);

    currentIndex = 0;
    userOnThisPage = false;
}

MorseCodeBasics::~MorseCodeBasics()
{
    delete ui;
}

void MorseCodeBasics::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

void MorseCodeBasics::onBackButtonClicked() {
    emit goBack();
    userOnThisPage = false;
    currentIndex = 0;
    ui->stackedWidget->setCurrentIndex(currentIndex);
}

void MorseCodeBasics::onNextSlideClicked() {
    if (currentIndex != 2) {
        currentIndex++;
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void MorseCodeBasics::onPreviousSlideClicked() {
    if (currentIndex != 0) {
        currentIndex--;
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void MorseCodeBasics::onStackedWidgetIndexChange(int index) {
    if (index == 0) {
        ui->previousSlideButton->setVisible(false);
        ui->nextSlideButton->setVisible(true);
        ui->completeLessonButton->setVisible(false);
    } else if (index == 1) {
        ui->previousSlideButton->setVisible(true);
        ui->nextSlideButton->setVisible(true);
        ui->completeLessonButton->setVisible(false);
    } else if (index == 2) {
        ui->previousSlideButton->setVisible(true);
        ui->nextSlideButton->setVisible(false);
        ui->completeLessonButton->setVisible(true);
    }
}

void MorseCodeBasics::completeLesson() {
    QSettings settings("Tigers", "MorseCodeLearner");
    settings.setValue("lesson1Completed", true);

    onBackButtonClicked();
}
