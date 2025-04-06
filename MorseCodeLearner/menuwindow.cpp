#include "menuwindow.h"
#include "ui_menuwindow.h"

MenuWindow::MenuWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->translatenav, &QPushButton::clicked, this, [=]() {emit goToTranslatorPage();});
    QObject::connect(ui->practicenav, &QPushButton::clicked, this, [=]() {emit goToPracticePage();});
    QObject::connect(ui->learningnav, &QPushButton::clicked, this, [=]() {emit goToLessonPage();});
    QObject::connect(ui->settingsnav, &QPushButton::clicked, this, [=]() {emit goToSettingsPage();});
}

MenuWindow::~MenuWindow()
{
    delete ui;
}
