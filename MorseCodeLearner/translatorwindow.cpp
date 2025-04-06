#include "translatorwindow.h"
#include "ui_translatorwindow.h"

translatorwindow::translatorwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::translatorwindow)
{
    ui->setupUi(this);

    QObject::connect(ui->backButton, &QPushButton::clicked, this, &translatorwindow::onBackButtonClicked);
}

translatorwindow::~translatorwindow()
{
    delete ui;
}

void translatorwindow::onBackButtonClicked() {
    emit goHome();
}

