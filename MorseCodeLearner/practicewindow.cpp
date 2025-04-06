#include "practicewindow.h"
#include "ui_practicewindow.h"

practicewindow::practicewindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::practicewindow)
{
    ui->setupUi(this);
}

practicewindow::~practicewindow()
{
    delete ui;
}

void practicewindow::on_backButton_clicked()
{
    emit goHome();
}

