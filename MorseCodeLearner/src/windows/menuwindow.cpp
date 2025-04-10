#include "menuwindow.h"
#include "ui_menuwindow.h"

MenuWindow::MenuWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuWindow), showingHelp(false)
{
    ui->setupUi(this);

    ui->settingsnav->setIcon(QIcon(":/icons/settings.svg"));
    ui->settingsnav->setIconSize(QSize(52, 52));

    ui->settingsHelpLabel->hide();
    ui->learnHelpLabel->hide();
    ui->practiceHelpLabel->hide();
    ui->translateHelpLabel->hide();

    QObject::connect(ui->translatenav, &QPushButton::clicked, this, [=]() {
        emit goToTranslatorPage();
        userOnThisPage = false;
    });
    QObject::connect(ui->practicenav, &QPushButton::clicked, this, [=]() {
        emit goToPracticePage();
        userOnThisPage = false;
    });
    QObject::connect(ui->learningnav, &QPushButton::clicked, this, [=]() {
        emit goToLessonPage();
        userOnThisPage = false;
    });
    QObject::connect(ui->settingsnav, &QPushButton::clicked, this, [=]() {
        emit goToSettingsPage();
        userOnThisPage = false;
    });
    QObject::connect(ui->helpnav, &QPushButton::clicked, this, &MenuWindow::toggleHelp);
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool MenuWindow::getUserOnThisPage() {
    return userOnThisPage;
}

void MenuWindow::toggleHelp() {
    if (!showingHelp) {
        ui->settingsHelpLabel->show();
        ui->learnHelpLabel->show();
        ui->practiceHelpLabel->show();
        ui->translateHelpLabel->show();
        showingHelp = true;
    } else {
        ui->settingsHelpLabel->hide();
        ui->learnHelpLabel->hide();
        ui->practiceHelpLabel->hide();
        ui->translateHelpLabel->hide();
        showingHelp = false;
    }
}
