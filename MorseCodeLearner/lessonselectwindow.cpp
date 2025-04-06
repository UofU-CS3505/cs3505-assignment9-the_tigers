#include "lessonselectwindow.h"
#include "ui_lessonselectwindow.h"

lessonselectwindow::lessonselectwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::lessonselectwindow)
{
    ui->setupUi(this);

    QObject::connect(ui->backButton, &QPushButton::clicked, this, &lessonselectwindow::on_backButton_clicked);
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

void lessonselectwindow::on_backButton_clicked()
{
    emit goHome();
    userOnThisPage = false;
}

