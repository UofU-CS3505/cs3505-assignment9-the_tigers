#ifndef LESSONSELECTWINDOW_H
#define LESSONSELECTWINDOW_H

#include <QWidget>
#include <QTimer>
#include "Box2D/Box2D.h"

namespace Ui {
class lessonselectwindow;
}

/**
 * The lesson select page. Navigates between lessons and shows lesson completion.
 *
 * @name Aiden Fornalski, Taylor Marlow, Chandler Eyre
 * @date 04/24/2025
 */
class LessonSelectWindow : public QWidget
{
    Q_OBJECT

private:
    Ui::lessonselectwindow *ui;

    bool userOnThisPage = false;

    // Box2D elements
    b2World world;
    b2Body* lessonOneBody;
    b2Body* lessonTwoBody;
    b2Body* lessonThreeBody;
    b2Body* lessonFourBody;
    b2Body* lessonFiveBody;
    b2Body* lessonSixBody;
    b2Body* lessonSevenBody;
    b2Body* lessonEightBody;
    b2Body* lessonNineBody;
    b2Body* lessonTenBody;

    int lessonOneY;
    int lessonTwoY;
    int lessonThreeY;
    int lessonFourY;
    int lessonFiveY;
    int lessonSixY;
    int lessonSevenY;
    int lessonEightY;
    int lessonNineY;
    int lessonTenY;

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
     * Constructor for a lesson select window.
     */
    explicit LessonSelectWindow(QWidget *parent = nullptr);

    /**
     * Destructor for a lesson select window.
     */
    ~LessonSelectWindow();

    /**
     * Updates whether a lesson is completed; generates complete checkmarks.
     */
    void checkAndUpdateLessonComplete();

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
     * Resets the page's state when the back button is clicked.
     */
    void onBackButtonClicked();

signals:
    /**
     * Signal that gets emitted when the back button is pressed.
     * Navigates to main menu.
     */
    void goHome();

    /**
     * Emitted when a lesson button is pressed.
     */
    void selectLesson(int lesson);
};

#endif // LESSONSELECTWINDOW_H
