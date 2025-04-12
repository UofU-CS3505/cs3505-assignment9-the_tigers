#include "lessonselectwindow.h"
#include "ui_lessonselectwindow.h"

lessonselectwindow::lessonselectwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::lessonselectwindow)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.svg"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &lessonselectwindow::onBackButtonClicked);

    // Connections for lesson buttons
    QObject::connect(ui->lessonButton_1, &QPushButton::clicked, this, [this](){emit selectLesson(1);});
    QObject::connect(ui->lessonButton_2, &QPushButton::clicked, this, [this](){emit selectLesson(2);});
    QObject::connect(ui->lessonButton_3, &QPushButton::clicked, this, [this](){emit selectLesson(3);});
    QObject::connect(ui->lessonButton_4, &QPushButton::clicked, this, [this](){emit selectLesson(4);});
    QObject::connect(ui->lessonButton_5, &QPushButton::clicked, this, [this](){emit selectLesson(5);});
    QObject::connect(ui->lessonButton_6, &QPushButton::clicked, this, [this](){emit selectLesson(6);});
    QObject::connect(ui->lessonButton_7, &QPushButton::clicked, this, [this](){emit selectLesson(7);});
    QObject::connect(ui->lessonButton_8, &QPushButton::clicked, this, [this](){emit selectLesson(8);});
    QObject::connect(ui->lessonButton_9, &QPushButton::clicked, this, [this](){emit selectLesson(9);});
    QObject::connect(ui->lessonButton_10, &QPushButton::clicked, this, [this](){emit selectLesson(10);});
}

lessonselectwindow::~lessonselectwindow()
{
    delete ui;
}

void lessonselectwindow::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool lessonselectwindow::getUserOnThisPage() {
    return userOnThisPage;
}

void lessonselectwindow::onBackButtonClicked()
{
    emit goHome();
    userOnThisPage = false;
}

