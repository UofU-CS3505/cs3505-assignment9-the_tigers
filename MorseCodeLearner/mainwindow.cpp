#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->translatenav, &QPushButton::clicked, this, &MainWindow::onTranslatorNavClicked);
    QObject::connect(ui->practicenav, &QPushButton::clicked, this, &MainWindow::onPracticeNavClicked);
    QObject::connect(ui->learningnav, &QPushButton::clicked, this, &MainWindow::onLearningNavClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpPages() {

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
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::goHome() {
    ui->stackedWidget->setCurrentIndex(0);
}

