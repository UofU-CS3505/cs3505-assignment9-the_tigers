#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent,
                       MorseHandler *morseHandler,
                       MorseAudioHandler *audioHandler,
                       KeyEventFilter *keyEventFilter)
    : QMainWindow(parent)
    , morseHandler(morseHandler)
    , audioHandler(audioHandler)
    , keyEventFilter(keyEventFilter)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    setUpPages();

    QObject::connect(menuWindow, &MenuWindow::goToTranslatorPage, this, &MainWindow::onTranslatorNavClicked);
    QObject::connect(menuWindow, &MenuWindow::goToPracticePage, this, &MainWindow::onPracticeNavClicked);
    QObject::connect(menuWindow, &MenuWindow::goToLessonPage, this, &MainWindow::onLearningNavClicked);
    QObject::connect(menuWindow, &MenuWindow::goToSettingsPage, this, &MainWindow::onSettingsNavClicked);

    QObject::connect(translatorWindow, &translatorwindow::goHome, this, &MainWindow::goHome);
    QObject::connect(practiceWindow, &practicewindow::goHome, this, &MainWindow::goHome);
    QObject::connect(lessonWindow, &lessonwindow::goToLessonSelect, this, &MainWindow::onLearningNavClicked);
    QObject::connect(lessonSelectWindow, &lessonselectwindow::goHome, this, &MainWindow::goHome);
    QObject::connect(settingsWindow, &settingswindow::goHome, this, &MainWindow::goHome);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpPages() {
    lessonWindow = new lessonwindow();
    lessonSelectWindow = new lessonselectwindow();
    practiceWindow = new practicewindow();
    settingsWindow = new settingswindow();
    translatorWindow = new translatorwindow(this, morseHandler, audioHandler, keyEventFilter);
    menuWindow = new MenuWindow();

    stackedWidget->addWidget(lessonWindow);
    stackedWidget->addWidget(lessonSelectWindow);
    stackedWidget->addWidget(practiceWindow);
    stackedWidget->addWidget(settingsWindow);
    stackedWidget->addWidget(translatorWindow);
    stackedWidget->addWidget(menuWindow);

    stackedWidget->setCurrentWidget(menuWindow);
    menuWindow->setUserOnThisPage(true);
}

void MainWindow::onTranslatorNavClicked()
{
    stackedWidget->setCurrentWidget(translatorWindow);
    translatorWindow->setUserOnThisPage(true);
}

void MainWindow::onPracticeNavClicked()
{
    stackedWidget->setCurrentWidget(practiceWindow);
    practiceWindow->setUserOnThisPage(true);
}

void MainWindow::onLearningNavClicked()
{
    stackedWidget->setCurrentWidget(lessonSelectWindow);
    lessonSelectWindow->setUserOnThisPage(true);
}

void MainWindow::onSettingsNavClicked()
{
    stackedWidget->setCurrentWidget(settingsWindow);
    settingsWindow->setUserOnThisPage(true);
}

void MainWindow::goHome() {
    morseHandler->stopTimers();
    stackedWidget->setCurrentWidget(menuWindow);
    menuWindow->setUserOnThisPage(true);
}

