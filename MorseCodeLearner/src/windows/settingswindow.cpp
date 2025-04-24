#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent, MorseHandler *morseHandler)
    : QWidget(parent)
    , ui(new Ui::settingswindow)
    , morseHandler(morseHandler)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.png"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &SettingsWindow::onBackButtonClicked);
    QObject::connect(ui->volumeSlider, &QAbstractSlider::valueChanged, this, &SettingsWindow::volumeChanged);
    QObject::connect(ui->wpmSpinBox, &QSpinBox::valueChanged, this, &SettingsWindow::wpmChanged);
    QObject::connect(ui->inputDevice, &QComboBox::currentIndexChanged, this, &SettingsWindow::onInputDeviceIndexChanged);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool SettingsWindow::getUserOnThisPage() {
    return userOnThisPage;
}

void SettingsWindow::onBackButtonClicked()
{
    saveSettings();
    emit goHome();
    userOnThisPage = false;
}

void SettingsWindow::volumeChanged(int volumeValue){
    morseHandler->setVolume(volumeValue);
}

void SettingsWindow::wpmChanged(int wpm){
    morseHandler->setWpm(wpm);
}


void SettingsWindow::onInputDeviceIndexChanged(int index)
{
    if (index == 0) {
        morseHandler->setDevice(MorseHandler::STRAIGHT_KEY);
    } else if (index == 1) {
        morseHandler->setDevice(MorseHandler::IAMBIC_PADDLE);
    }
}

void SettingsWindow::saveSettings() {
    QSettings settings("Tigers", "MorseCodeLearner");
    settings.setValue("deviceType", ui->inputDevice->currentIndex());
    settings.setValue("volume", ui->volumeSlider->value());
    settings.setValue("wpm", ui->wpmSpinBox->value());
}

void SettingsWindow::loadSettings() {
    QSettings settings("Tigers", "MorseCodeLearner");
    ui->inputDevice->setCurrentIndex(settings.value("deviceType", 0).toInt());
    ui->volumeSlider->setValue(settings.value("volume", 100).toInt());
    ui->wpmSpinBox->setValue(settings.value("wpm", 10).toInt());
}

