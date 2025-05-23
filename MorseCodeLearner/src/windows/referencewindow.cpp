#include "referencewindow.h"
#include "ui_referencewindow.h"

ReferenceWindow::ReferenceWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::referencewindow)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.png"));
    ui->backButton->setIconSize(QSize(52, 52));

    QObject::connect(ui->backButton, &QPushButton::clicked, this, &ReferenceWindow::onBackButtonClicked);
}

ReferenceWindow::~ReferenceWindow()
{
    delete ui;
}

void ReferenceWindow::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool ReferenceWindow::getUserOnThisPage() {
    return userOnThisPage;
}

void ReferenceWindow::onBackButtonClicked()
{
    emit goHome();
    userOnThisPage = false;
}
