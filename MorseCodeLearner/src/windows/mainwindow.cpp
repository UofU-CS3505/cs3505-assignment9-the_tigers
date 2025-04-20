#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent,
                       MorseHandler *morseHandler,
                       KeyEventFilter *keyEventFilter,
                       PracticeHandler *practiceHandler,
                       LessonHandler *lessonHandler)
    : QMainWindow(parent)
    , morseHandler(morseHandler)
    , keyEventFilter(keyEventFilter)
    , lessonHandler(lessonHandler)
    , ui(new Ui::MainWindow)
    , practiceHandler(practiceHandler)
{
    ui->setupUi(this);

    this->setStyleSheet("QMainWindow { background-image: url(:/images/illustrated_background.jpg); background-position: center; width: 100%; height: 100%;}");

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    setUpPages();

    QObject::connect(menuWindow, &MenuWindow::goToTranslatorPage, this, &MainWindow::onTranslatorNavClicked);
    QObject::connect(menuWindow, &MenuWindow::goToPracticePage, this, &MainWindow::onPracticeNavClicked);
    QObject::connect(menuWindow, &MenuWindow::goToLessonPage, this, &MainWindow::onLearningNavClicked);
    QObject::connect(menuWindow, &MenuWindow::goToSettingsPage, this, &MainWindow::onSettingsNavClicked);
    QObject::connect(menuWindow, &MenuWindow::goToReferencePage, this, &MainWindow::onReferenceNavClicked);

    QObject::connect(lessonSelectWindow, &lessonselectwindow::selectLesson, this, &MainWindow::onLessonClicked);

    QObject::connect(translatorWindow, &translatorwindow::goHome, this, &MainWindow::goHome);
    QObject::connect(practiceHandler, &PracticeHandler::goHome, this, &MainWindow::goHome);
    QObject::connect(lessonWindow, &lessonwindow::goToLessonSelect, this, &MainWindow::onLearningNavClicked);
    QObject::connect(lessonSelectWindow, &lessonselectwindow::goHome, this, &MainWindow::goHome);
    QObject::connect(settingsWindow, &settingswindow::goHome, this, &MainWindow::goHome);
    QObject::connect(referenceWindow, &referencewindow::goHome, this, &MainWindow::goHome);
    QObject::connect(morseCodeBasicsWindow, &MorseCodeBasics::goBack, this, &MainWindow::onLearningNavClicked);
    QObject::connect(paddleLessonWindow, &PaddleLesson::goBack, this, &MainWindow::onLearningNavClicked);

    QObject::connect(this, &MainWindow::startLesson, lessonHandler, &LessonHandler::startLesson);
    QObject::connect(this, &MainWindow::userOnLessonPage, lessonHandler, &LessonHandler::setUserOnThisPage);
    QObject::connect(this, &MainWindow::userOnMorseCodeBasics, morseCodeBasicsWindow, &MorseCodeBasics::setUserOnThisPage);
    QObject::connect(this, &MainWindow::userOnNumbersBasics, numbersBasicsWindow, &NumbersBasics::setUserOnThisPage);
    QObject::connect(this, &MainWindow::userOnPaddleLesson, paddleLessonWindow, &PaddleLesson::setUserOnThisPage);

    // This needs to be called last
    settingsWindow->loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpPages() {
    lessonWindow = new lessonwindow(lessonHandler, morseHandler, keyEventFilter);
    lessonSelectWindow = new lessonselectwindow();
    practiceWindow = new practicewindow(this, keyEventFilter, practiceHandler);
    settingsWindow = new settingswindow(this, morseHandler);
    translatorWindow = new translatorwindow(this, morseHandler, keyEventFilter);
    menuWindow = new MenuWindow();
    referenceWindow = new referencewindow(this);
    morseCodeBasicsWindow = new MorseCodeBasics(this);
    numbersBasicsWindow = new NumbersBasics(this);
    paddleLessonWindow = new PaddleLesson(this);

    stackedWidget->addWidget(lessonWindow);
    stackedWidget->addWidget(lessonSelectWindow);
    stackedWidget->addWidget(practiceWindow);
    stackedWidget->addWidget(settingsWindow);
    stackedWidget->addWidget(translatorWindow);
    stackedWidget->addWidget(menuWindow);
    stackedWidget->addWidget(referenceWindow);
    stackedWidget->addWidget(morseCodeBasicsWindow);
    stackedWidget->addWidget(numbersBasicsWindow);
    stackedWidget->addWidget(paddleLessonWindow);

    stackedWidget->setCurrentWidget(menuWindow);
    menuWindow->setUserOnThisPage(true);
}

void MainWindow::onTranslatorNavClicked()
{
    stackedWidget->setCurrentWidget(translatorWindow);
    translatorWindow->setUserOnThisPage(true);
    this->setStyleSheet("QMainWindow { background-image: url(:/images/background.jpg); background-position: center; width: 100%; height: 100%;}");
}

void MainWindow::onPracticeNavClicked()
{
    stackedWidget->setCurrentWidget(practiceWindow);
    practiceWindow->setUserOnThisPage(true);
    this->setStyleSheet("QMainWindow { background-image: url(:/images/background.jpg); background-position: center; width: 100%; height: 100%;}");
}

void MainWindow::onLearningNavClicked()
{
    stackedWidget->setCurrentWidget(lessonSelectWindow);
    lessonSelectWindow->setUserOnThisPage(true);
    this->setStyleSheet("QMainWindow { background-image: url(:/images/background.jpg); background-position: center; width: 100%; height: 100%;}");
}

void MainWindow::onLessonClicked(int lessonNumber) {
    if (lessonNumber == 1) {
        stackedWidget->setCurrentWidget(morseCodeBasicsWindow);
        emit userOnMorseCodeBasics(true);
        this->setStyleSheet("QMainWindow { background-image: url(:/images/background.jpg); background-position: center; width: 100%; height: 100%;}");
    } else if (lessonNumber == 7) {
        stackedWidget->setCurrentWidget(numbersBasicsWindow);
        emit userOnNumbersBasics(true);
        this->setStyleSheet("QMainWindow { background-image: url(:/images/background.jpg); background-position: center; width: 100%; height: 100%;}");
    } else if (lessonNumber == 9) {

    } else if (lessonNumber == 10) {
        stackedWidget->setCurrentWidget(paddleLessonWindow);
        emit userOnPaddleLesson(true);
        this->setStyleSheet("QMainWindow { background-image: url(:/images/background.jpg); background-position: center; width: 100%; height: 100%;}");
    } else {
        stackedWidget->setCurrentWidget(lessonWindow);
        emit userOnLessonPage(true);
        this->setStyleSheet("QMainWindow { background-image: url(:/images/background.jpg); background-position: center; width: 100%; height: 100%;}");
        emit startLesson(lessonNumber);
    }
}

void MainWindow::onSettingsNavClicked()
{
    stackedWidget->setCurrentWidget(settingsWindow);
    settingsWindow->setUserOnThisPage(true);
    this->setStyleSheet("QMainWindow { background-image: url(:/images/background.jpg); background-position: center; width: 100%; height: 100%;}");
}

void MainWindow::onReferenceNavClicked()
{
    stackedWidget->setCurrentWidget(referenceWindow);
    referenceWindow->setUserOnThisPage(true);
    this->setStyleSheet("QMainWindow { background-image: url(:/images/background.jpg); background-position: center; width: 100%; height: 100%;}");
}

void MainWindow::goHome() {
    morseHandler->stopTimers();
    stackedWidget->setCurrentWidget(menuWindow);
    menuWindow->setUserOnThisPage(true);
    this->setStyleSheet("QMainWindow { background-image: url(:/images/illustrated_background.jpg); background-position: center; width: 100%; height: 100%;}");

}

