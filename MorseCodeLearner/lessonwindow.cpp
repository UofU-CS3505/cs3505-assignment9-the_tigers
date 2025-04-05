#include "lessonwindow.h"
#include "ui_lessonwindow.h"

lessonwindow::lessonwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::lessonwindow)
{
    ui->setupUi(this);
}

lessonwindow::~lessonwindow()
{
    delete ui;
}
