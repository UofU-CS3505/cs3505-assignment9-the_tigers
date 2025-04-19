#ifndef LESSONWINDOW_H
#define LESSONWINDOW_H

#include <QWidget>
#include "keyeventfilter.h"
#include "morsehandler.h"
#include "lessonhandler.h"
#include "Box2D/Box2D.h"

namespace Ui {
class lessonwindow;
}

class lessonwindow : public QWidget
{
    Q_OBJECT

public:
    explicit lessonwindow(LessonHandler *lessonHandler = nullptr, MorseHandler *morseHandler = nullptr, KeyEventFilter *keyEventFilter = nullptr, QWidget *parent = nullptr);
    ~lessonwindow();

private:
    Ui::lessonwindow *ui;
    LessonHandler *lessonHandler;
    MorseHandler *morseHandler;
    KeyEventFilter *keyEventFilter;

    QTimer timer;

    b2World world;
    b2Body* textShakeBody;
    b2Body* textJumpBody;
    b2Body* shakeAnchor;
    b2RopeJoint* ropeJoint;

    int problemTextX;
    int problemTextY;

    int shakeFrameCount;
    bool currentlyShaking;

    /**
     * Sets up Box2D objects and environment.
     */
    void setupWorld();

    /**
     * Gets updated Box2D positions.
     */
    void updateWorld();

    // Connection objects for illustrations, must be retained so they can be changed later
    QMetaObject::Connection rightPressedConnection;
    QMetaObject::Connection rightReleasedConnection;
    QMetaObject::Connection leftPressedConnection;
    QMetaObject::Connection leftReleasedConnection;
    QMetaObject::Connection straightPressedConnection;
    QMetaObject::Connection straightReleasedConnection;

signals:
    /**
     * A signal that tells the stackedWidget to send the user back
     * to the lesson select page.
     */
    void goToLessonSelect();

    void backButtonClicked();

private slots:
    void onBackButtonClicked();

    /**
     * A slot that runs when a guess is correct.
     */
    void guessCorrect();

    /**
     * A slot that runs when a guess is incorrect.
     */
    void guessIncorrect();

    /**
     * Displays the current question to the view.
     * @param text - the current question.
     */
    void displayTextQuestion(QString text);

    void updateInputText(QString inputText);
};



#endif // LESSONWINDOW_H
