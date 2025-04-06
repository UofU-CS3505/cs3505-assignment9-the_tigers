#include "practicewindow.h"
#include "ui_practicewindow.h"

practicewindow::practicewindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::practicewindow)
{
    ui->setupUi(this);

    QObject::connect(ui->backButton, &QPushButton::clicked, this, &practicewindow::on_backButton_clicked);
}

practicewindow::~practicewindow()
{
    delete ui;
}

void practicewindow::on_backButton_clicked()
{
    emit goHome();
}

