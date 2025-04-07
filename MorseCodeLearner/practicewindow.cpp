#include "practicewindow.h"
#include "ui_practicewindow.h"

practicewindow::practicewindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::practicewindow)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.svg"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &practicewindow::on_backButton_clicked);
}

practicewindow::~practicewindow()
{
    delete ui;
}

void practicewindow::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool practicewindow::getUserOnThisPage() {
    return userOnThisPage;
}

void practicewindow::on_backButton_clicked()
{
    emit goHome();
    userOnThisPage = false;
}

