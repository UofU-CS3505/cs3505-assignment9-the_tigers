#include "settingswindow.h"
#include "ui_settingswindow.h"

settingswindow::settingswindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::settingswindow)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.svg"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &settingswindow::on_backButton_clicked);
}

settingswindow::~settingswindow()
{
    delete ui;
}

void settingswindow::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool settingswindow::getUserOnThisPage() {
    return userOnThisPage;
}

void settingswindow::on_backButton_clicked()
{
    emit goHome();
    userOnThisPage = false;
}

