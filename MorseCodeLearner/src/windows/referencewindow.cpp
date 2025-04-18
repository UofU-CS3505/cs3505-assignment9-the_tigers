#include "referencewindow.h"
#include "ui_referencewindow.h"

referencewindow::referencewindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::referencewindow)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.png"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &referencewindow::on_backButton_clicked);
}

referencewindow::~referencewindow()
{
    delete ui;
}

void referencewindow::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool referencewindow::getUserOnThisPage() {
    return userOnThisPage;
}

void referencewindow::on_backButton_clicked()
{
    emit goHome();
    userOnThisPage = false;
}
