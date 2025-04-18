#include "settingswindow.h"
#include "ui_settingswindow.h"

settingswindow::settingswindow(QWidget *parent, MorseHandler *morseHandler)
    : QWidget(parent)
    , ui(new Ui::settingswindow)
    , morseHandler(morseHandler)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.png"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &settingswindow::onBackButtonClicked);
    QObject::connect(ui->volumeSlider, &QAbstractSlider::valueChanged, this, &settingswindow::volumeChanged);
    QObject::connect(ui->wpmSpinBox, &QSpinBox::valueChanged, this, &settingswindow::wpmChanged);
    QObject::connect(ui->inputDevice, &QComboBox::currentIndexChanged, this, &settingswindow::onInputDeviceIndexChanged);
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

void settingswindow::onBackButtonClicked()
{
    saveSettings();
    emit goHome();
    userOnThisPage = false;
}

void settingswindow::volumeChanged(int volumeValue){
    morseHandler->setVolume(volumeValue);
}

void settingswindow::wpmChanged(int wpm){
    morseHandler->setWpm(wpm);
}


void settingswindow::onInputDeviceIndexChanged(int index)
{
    if (index == 0) {
        morseHandler->setDevice(MorseHandler::STRAIGHT_KEY);
    } else if (index == 1) {
        morseHandler->setDevice(MorseHandler::IAMBIC_PADDLE);
    }
}

void settingswindow::saveSettings() {
    QSettings settings("Tigers", "MorseCodeLearner");
    settings.setValue("deviceType", ui->inputDevice->currentIndex());
    settings.setValue("volume", ui->volumeSlider->value());
    settings.setValue("wpm", ui->wpmSpinBox->value());
}

void settingswindow::loadSettings() {
    QSettings settings("Tigers", "MorseCodeLearner");
    ui->inputDevice->setCurrentIndex(settings.value("deviceType", 0).toInt());
    ui->volumeSlider->setValue(settings.value("volume", 100).toInt());
    ui->wpmSpinBox->setValue(settings.value("wpm", 10).toInt());
}

