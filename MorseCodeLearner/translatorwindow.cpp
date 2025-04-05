#include "translatorwindow.h"
#include "ui_translatorwindow.h"

translatorwindow::translatorwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::translatorwindow)
{
    ui->setupUi(this);
}

translatorwindow::~translatorwindow()
{
    delete ui;
}
