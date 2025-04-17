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
 * @name Aiden Fornalski and Chandler Eyre
 * @date 04/11/2025
 */
class lessonselectwindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor for a lesson select window.
     */
    explicit lessonselectwindow(QWidget *parent = nullptr);

    /**
     * Destructor for a lesson select window.
     */
    ~lessonselectwindow();

    void setUserOnThisPage(bool userOnThisPage);

    bool getUserOnThisPage();

    void setupWorld();

    void updateWorld();

signals:
    /**
     * Signal that gets emitted when the back button is pressed.
     */
    void goHome();

    /**
     * Emitted when a lesson button is pressed.
     */
    void selectLesson(int lesson);

private slots:
    /**
     * Resets the page's state when the back button is clicked.
     */
    void onBackButtonClicked();

private:
    Ui::lessonselectwindow *ui;

    bool userOnThisPage = false;

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

    bool eventFilter(QObject *object, QEvent *event);
    void jumpLessonOne();
    void jumpLessonTwo();
    void jumpLessonThree();
    void jumpLessonFour();
    void jumpLessonFive();
    void jumpLessonSix();
    void jumpLessonSeven();
    void jumpLessonEight();
    void jumpLessonNine();
    void jumpLessonTen();
};

#endif // LESSONSELECTWINDOW_H
