#include "settingswindow.h"
#include "ui_settingswindow.h"

settingswindow::settingswindow(QWidget *parent,
                               MorseHandler *morseHandler,
                               MorseAudioHandler *audioHandler)
    : QWidget(parent)
    , ui(new Ui::settingswindow)
    , morseHandler(morseHandler)
    , audioHandler(audioHandler)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.svg"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, this, &settingswindow::on_backButton_clicked);
    QObject::connect(ui->volumeSlider, &QAbstractSlider::valueChanged, this, &settingswindow::volumeChanged);
    QObject::connect(ui->wpmSpinBox, &QSpinBox::valueChanged, this, &settingswindow::wpmChanged);
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

void settingswindow::on_backButton_clicked()
{
    emit goHome();
    userOnThisPage = false;
}

void settingswindow::volumeChanged(signed int volumeValue){
    audioHandler->setVolume(volumeValue);
}

void settingswindow::wpmChanged(int wpm){
    morseHandler->setWpm(wpm);
    audioHandler->setWpm(wpm);
}

