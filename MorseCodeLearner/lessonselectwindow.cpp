#include "lessonselectwindow.h"
#include "ui_lessonselectwindow.h"

lessonselectwindow::lessonselectwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::lessonselectwindow)
{
    ui->setupUi(this);
}

lessonselectwindow::~lessonselectwindow()
{
    delete ui;
}

void lessonselectwindow::on_backButton_clicked()
{
    emit goHome();
}

