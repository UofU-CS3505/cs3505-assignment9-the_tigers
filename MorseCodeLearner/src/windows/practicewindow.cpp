#include "practicewindow.h"
#include "ui_practicewindow.h"
#include <ctime>

practicewindow::practicewindow(QWidget *parent, KeyEventFilter *keyEventFilter, PracticeHandler *practiceHandler)
    : QWidget(parent)
    , ui(new Ui::practicewindow)
    , keyEventFilter(keyEventFilter)
    , practiceHandler(practiceHandler)
{
    ui->setupUi(this);

    ui->backButton->setIcon(QIcon(":/icons/back.svg"));
    ui->backButton->setIconSize(QSize(52, 52));
    QObject::connect(ui->backButton, &QPushButton::clicked, practiceHandler, &PracticeHandler::onBackButtonClicked);

    QObject::connect(practiceHandler, &PracticeHandler::lightIndicatorOn, this, [=]() {ui->flashIndicator->setStyleSheet("QLabel { background-color : white; border: 1px solid black;}");});
    QObject::connect(practiceHandler, &PracticeHandler::lightIndicatorOff, this, [=]() {ui->flashIndicator->setStyleSheet("QLabel { background-color : gray; border: 1px solid black;}");});

    // Key Event Filters
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, practiceHandler, &PracticeHandler::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, practiceHandler, &PracticeHandler::handleSpaceReleased);

    QObject::connect(practiceHandler, &PracticeHandler::updateInputText, this, &practicewindow::updateInputText);
    QObject::connect(practiceHandler, &PracticeHandler::updatePracticeText, this, &practicewindow::updatePracticeText);
    QObject::connect(ui->difficultySelectBox, &QComboBox::currentTextChanged, practiceHandler, &PracticeHandler::setDifficulty);
}

practicewindow::~practicewindow() {
    delete ui;
}

void practicewindow::setUserOnThisPage(bool userOnThisPage) {
    practiceHandler->setUserOnThisPage(userOnThisPage);
}

bool practicewindow::getUserOnThisPage() {
    return practiceHandler->getUserOnThisPage();
}

void practicewindow::updateInputText(QString text) {
    ui->inputText->setText(text);
}

void practicewindow::updatePracticeText(QString text) {
    ui->problemText->setText(text);
}

