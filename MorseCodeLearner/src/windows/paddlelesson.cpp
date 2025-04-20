#include "paddlelesson.h"
#include "ui_paddlelesson.h"
#include <QSettings>

PaddleLesson::PaddleLesson(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PaddleLesson)
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

    QObject::connect(ui->backButton, &QPushButton::clicked, this, &PaddleLesson::onBackButtonClicked);
    QObject::connect(ui->nextSlideButton, &QPushButton::clicked, this, &PaddleLesson::onNextSlideClicked);
    QObject::connect(ui->previousSlideButton, &QPushButton::clicked, this, &PaddleLesson::onPreviousSlideClicked);
    QObject::connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &PaddleLesson::onStackedWidgetIndexChange);
    QObject::connect(ui->completeLessonButton, &QPushButton::clicked, this, &PaddleLesson::completeLesson);

    currentIndex = 0;
    userOnThisPage = false;
}

PaddleLesson::~PaddleLesson()
{
    delete ui;
}

void PaddleLesson::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

void PaddleLesson::onBackButtonClicked() {
    emit goBack();
    userOnThisPage = false;
    currentIndex = 0;
    ui->stackedWidget->setCurrentIndex(currentIndex);
}

void PaddleLesson::onNextSlideClicked() {
    if (currentIndex != 3) {
        currentIndex++;
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void PaddleLesson::onPreviousSlideClicked() {
    if (currentIndex != 0) {
        currentIndex--;
        ui->stackedWidget->setCurrentIndex(currentIndex);
    }
}

void PaddleLesson::onStackedWidgetIndexChange(int index) {
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
        ui->nextSlideButton->setVisible(true);
        ui->completeLessonButton->setVisible(false);
    } else if (index == 3) {
        ui->previousSlideButton->setVisible(true);
        ui->nextSlideButton->setVisible(false);
        ui->completeLessonButton->setVisible(true);
    }
}

void PaddleLesson::completeLesson() {
    QSettings settings("Tigers", "MorseCodeLearner");
    settings.setValue("lesson10Completed", true);

    onBackButtonClicked();
}
