#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>
#include <QTimer>
#include "Box2D/Box2D.h"

namespace Ui {
class MenuWindow;
}

/**
 * The main menu window.
 * Navigates between modes, settings and encodings window, and shows help.
 *
 * @name Chandler Eyre, Taylor Marlow
 * @date 04/24/2025
 */
class MenuWindow : public QWidget
{
    Q_OBJECT

private:
    Ui::MenuWindow *ui;

    bool userOnThisPage;
    bool showingHelp;

    // Box2D elements
    b2World world;
    b2Body* lessonBody;
    b2Body* translatorBody;
    b2Body* practiceBody;
    b2Body* settingsBody;
    b2Body* helpBody;
    b2Body* referenceBody;

    int practiceButtonY;
    int lessonButtonY;
    int translatorButtonY;
    int settingsButtonY;
    int helpButtonY;
    int referenceButtonY;

    QTimer timer;

    /**
     * Filters QEvents to call jumpButton on lesson select buttons
     * @param object - Lesson button objects
     * @param event - QEvent
     * @return false
     */
    bool eventFilter(QObject *object, QEvent *event);

    /**
     * Makes a button jump.
     * @param jumpBody - Body object for button to jump.
     */
    void jumpButton(b2Body* jumpBody);

public:
    /**
     * Constructor for a menu window.
     * @param parent - The QObject parent
     */
    explicit MenuWindow(QWidget *parent = nullptr);

    /**
     * Destructor for a menu window.
     */
    ~MenuWindow();

    /**
     * Sets whether the user is on this page.
     * @param userOnThisPage - Whether the user is on this page.
     */
    void setUserOnThisPage(bool userOnThisPage);

    /**
     * @return - Whether the user is on this page.
     */
    bool getUserOnThisPage();

    /**
     * Sets up Box2D objects and environment.
     */
    void setupWorld();

    /**
     * Gets updated Box2D positions.
     */
    void updateWorld();

private slots:
    /**
     * Toggles the help overlay.
     */
    void toggleHelp();

signals:
    /**
     * Navigates to the lesson select page.
     */
    void goToLessonPage();

    /**
     * Navigates to the translator page.
     */
    void goToTranslatorPage();

    /**
     * Navigates to the practice page.
     */
    void goToPracticePage();

    /**
     * Navigates to the settings page.
     */
    void goToSettingsPage();

    /**
     * Navigates to the encodings reference page.
     */
    void goToReferencePage();
};

#endif // MENUWINDOW_H
