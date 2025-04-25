#ifndef LESSONWINDOW_H
#define LESSONWINDOW_H

#include "lessonhandler.h"
#include <QWidget>
#include <QtCore/qtimer.h>
#include "Box2D/Box2D.h"

namespace Ui {
class lessonwindow;
}

/**
 * The lesson window, it displays the current lesson's characters and words
 * that the user needs to learn.
 *
 * @name Aiden Fornalski, Taylor Marlow, Chandler Eyre
 * @date 04/24/2025
 */
class LessonWindow : public QWidget
{
    Q_OBJECT
public:
    explicit LessonWindow(LessonHandler *lessonHandler = nullptr, QWidget *parent = nullptr);
    ~LessonWindow();

private:
    Ui::lessonwindow *ui;
    LessonHandler *lessonHandler;

    QTimer timer;
    int currentIndex;

    enum paddleAnimationStates {
        LEFT, RIGHT, CENTER, BOTH
    };

    paddleAnimationStates paddleState = CENTER;

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

    /**
     * A signal that tells the Lesson Handler that the back button
     * has been clicked.
     */
    void backButtonClicked();

    /**
     * A signal that tells the Lesson Handler what index the
     * stackedWidget is on.
     * @param currentIndex - the current index of the
     * stackedWidget.
     */
    void setCurrentIndex(int currentIndex);

    /**
     * A signal that tells the Lesson Handler what is currently
     * in the input box.
     * @param inputText - the text in the input box.
     */
    void sendInputTextSignal(QString inputText);

public slots:
    /**
     * Runs when a lesson is selected and started.
     * @param lessonNumber - the number of the lesson selected.
     */
    void startLesson(int lessonNumber);

    /**
     * Runs when the back button is clicked.
     */
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

    /**
     * Updates the text in the input box.
     * @param inputText - the new text in the input box.
     */
    void updateInputText(QString inputText);

    /**
     * Updates the title of the lesson.
     * @param lessonNumber - the number of the current lesson.
     */
    void updateLessonTitle(int lessonNumber);

    /**
     * Visually updates the lesson progress bar.
     * @param progress - the current progress in the lesson.
     */
    void updateLessonProgressBar(float progress);

    /**
     * Displays what the correct answer was to a question.
     * @param correctAnswer - the correct answer of a question.
     */
    void displayCorrectAnswer(QString correctAnswer);

    /**
     * Displays what the reference text is for this lesson (the
     * first slide in the lesson page).
     * @param referenceText - the text that the reference page
     * should hold.
     */
    void setReferenceText(QString referenceText);

    /**
     * Runs when the next slide button is clicked; goes to the
     * next page in the stackedWidget.
     */
    void onNextSlideClicked();

    /**
     * Runs when the previous slide button is clicked; goes to
     * the previous page in the stackedWidget.
     */
    void onPreviousSlideClicked();

    /**
     * Runs when the index is changed in the stackedWidget.
     * @param index - what the new index is.
     */
    void onStackedWidgetIndexChange(int index);

    /**
     * Sets the lesson to "audio decode mode".
     * @param mode - true if in "audio decode mode", false if
     * not in "audio decode mode".
     */
    void setAudioDecodeMode(bool mode);

    /**
     * Sets the light indicator to be on and off.
     * @param lightIndicator - true if on, false if off.
     */
    void setLightIndicator(bool lightIndicator);

    /**
     * Sets the input box to either read only mode or not
     * read only mode,
     * @param readOnly - true if in read only mode, false if
     * not in read only mode.
     */
    void setInputReadOnly(bool readOnly);

    /**
     * Sets the sound indicator to playing.
     */
    void setSoundPlaying();

    /**
     * Sets the sound indicator to not playing.
     */
    void setSoundNotPlaying();

    /**
     * Sets the focus to the input box.
     */
    void setFocusInput();

    /**
     * Runs when the text from the input box needs to be sent
     * to the Lesson Handler.
     */
    void sendInputText();

    /**
     * Runs when the user selects the straight key to be used
     * from the settings.
     */
    void straightKeySelected();

    /**
     * Runs when the user selects the paddle to be used from
     * the settings.
     */
    void paddleSelected();
};



#endif // LESSONWINDOW_H
