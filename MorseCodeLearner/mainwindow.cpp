#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0); // Ensures main menu is first page on app launch

    QObject::connect(ui->translatenav, &QPushButton::clicked, this, &MainWindow::onTranslatorNavClicked);
    QObject::connect(ui->practicenav, &QPushButton::clicked, this, &MainWindow::onPracticeNavClicked);
    QObject::connect(ui->learningnav, &QPushButton::clicked, this, &MainWindow::onLearningNavClicked);
    QObject::connect(ui->settingsnav, &QPushButton::clicked, this, &MainWindow::onSettingsNavClicked);

    QObject::connect(ui->translatorPage, &translatorwindow::goHome, this, &MainWindow::goHome);
    QObject::connect(ui->practicePage, &practicewindow::goHome, this, &MainWindow::goHome);
    QObject::connect(ui->lessonPage, &lessonwindow::goToLessonSelect, this, &MainWindow::onLearningNavClicked);
    QObject::connect(ui->lessonSelectPage, &lessonselectwindow::goHome, this, &MainWindow::goHome);
    QObject::connect(ui->settingsPage, &settingswindow::goHome, this, &MainWindow::goHome);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTranslatorNavClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onPracticeNavClicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::onLearningNavClicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::onSettingsNavClicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::goHome() {
    ui->stackedWidget->setCurrentIndex(0);
}

