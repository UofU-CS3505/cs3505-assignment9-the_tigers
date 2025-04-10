#include "settingswindow.h"
#include "ui_settingswindow.h"

settingswindow::settingswindow(QWidget *parent, MorseHandler *morseHandler)
    : QWidget(parent)
    , ui(new Ui::settingswindow)
    , morseHandler(morseHandler)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.svg"));
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

