#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "morsehandler.h"
#include "lessonwindow.h"
#include "lessonselectwindow.h"
#include "practicewindow.h"
#include "settingswindow.h"
#include "translatorwindow.h"
#include "menuwindow.h"
#include "referencewindow.h"
#include "keyeventfilter.h"
#include "lessonhandler.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr,
                        MorseHandler *morseHandler = nullptr,
                        KeyEventFilter *keyEventFilter = nullptr,
                        LessonHandler *lessonHandler = nullptr);
    ~MainWindow();

    MorseHandler *morseHandler;
    KeyEventFilter *keyEventFilter;
    LessonHandler *lessonHandler;

signals:
    /**
     * Emits this signal when a lesson is selected.
     * @param lessonNumber
     */
    void startLesson(int lessonNumber);

private slots:
    /**
     * Called when navigating to the Translator Page.
     */
    void onTranslatorNavClicked();

    /**
     * Called when navigating to the Practice Page.
     */
    void onPracticeNavClicked();

    /**
     * Called when navigating to the Learning Page.
     */
    void onLearningNavClicked();

    /**
     * Called when navigating to a Lesson Page.
     * @param lessonNumber - the number of the lesson selected.
     */
    void onLessonClicked(int lessonNumber);

    /**
     * Called when navigating to the Settings Page.
     */
    void onSettingsNavClicked();

    /**
     * Called when navigating to the Reference Page.
     */
    void onReferenceNavClicked();

    /**
     * Called when a back/home button is pressed from any page.
     */
    void goHome();

private:
    Ui::MainWindow *ui;
    QStackedWidget* stackedWidget;
    lessonwindow* lessonWindow;
    lessonselectwindow* lessonSelectWindow;
    practicewindow* practiceWindow;
    settingswindow* settingsWindow;
    translatorwindow* translatorWindow;
    MenuWindow* menuWindow;
    referencewindow* referenceWindow;

    /**
     * A simple method to set up all the pages with the QStackedWidget.
     */
    void setUpPages();
};
#endif // MAINWINDOW_H
