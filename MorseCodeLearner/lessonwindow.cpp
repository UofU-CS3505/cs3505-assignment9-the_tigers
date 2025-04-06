#include "lessonwindow.h"
#include "ui_lessonwindow.h"

lessonwindow::lessonwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::lessonwindow)
{
    ui->setupUi(this);

    QObject::connect(ui->backButton, &QPushButton::clicked, this, &lessonwindow::on_backButton_clicked);
}

lessonwindow::~lessonwindow()
{
    delete ui;
}

void lessonwindow::on_backButton_clicked()
{
    emit goToLessonSelect();
}

