#ifndef PRACTICEWINDOW_H
#define PRACTICEWINDOW_H

#include <QWidget>
#include "practicehandler.h"
#include "Box2D/Box2D.h"

namespace Ui {
class practicewindow;
}

/**
 * The practice page. Allows the user to infinitely practice morse code decoding and encoding.
 *
 * @name Chandler Eyre, Michael Timothy, Taylor Marlow
 * @date 04/24/2025
 */
class PracticeWindow : public QWidget
{
    Q_OBJECT

private:
    Ui::practicewindow *ui;
    PracticeHandler *practiceHandler;

    QTimer timer;

    enum paddleAnimationStates {
        LEFT, RIGHT, CENTER, BOTH
    };
    paddleAnimationStates paddleState = CENTER;

    // Box2D elements
    b2World world;
    b2Body* textShakeBody;
    b2Body* textJumpBody;
    b2Body* shakeAnchor;
    b2RopeJoint* ropeJoint;

    int problemTextX;
    int problemTextY;

    int shakeFrameCount;
    bool currentlyShaking;

    // Connection objects for illustrations, must be retained so they can be changed later
    QMetaObject::Connection rightPressedConnection;
    QMetaObject::Connection rightReleasedConnection;
    QMetaObject::Connection leftPressedConnection;
    QMetaObject::Connection leftReleasedConnection;
    QMetaObject::Connection straightPressedConnection;
    QMetaObject::Connection straightReleasedConnection;

    /**
     * Sets up Box2D objects and environment.
     */
    void setupWorld();

    /**
     * Gets updated Box2D positions.
     */
    void updateWorld();

public:
    /**
     * Constructor for a practice window.
     * @param parent - The QObject parent
     * @param practiceHandler - A pointer to the Practice Handler
     */
    explicit PracticeWindow(QWidget *parent = nullptr, PracticeHandler *practiceHandler = nullptr);

    /**
     * Destructor for a practice window.
     */
    ~PracticeWindow();

    /**
     * Sets whether the user is on this page.
     * @param userOnThisPage - Whether the user is on this page.
     */
    void setUserOnThisPage(bool userOnThisPage);

    /**
     * @return - Whether the user is on this page.
     */
    bool getUserOnThisPage();

private slots:
    /**
     * Changes the illustration to the paddle.
     */
    void paddleSelected();

    /**
     * Changes the illustration to the straight key.
     */
    void straightKeySelected();

    /**
     * Visual changes for each mode
     * @param newMode - The new mode
     */
    void changeMode(QString newMode);

public slots:
    /**
     * Updates practice text display.
     */
    void updatePracticeText(QString text);

    /**
     * Updates input text display.
     */
    void updateInputText(QString text);

    /**
     * Causes the text to jump before falling back into place.
     */
    void textJump();

    /**
     * Causes the text to shake before going back into place.
     */
    void textShake();

signals:
    /**
     * Signal that gets emitted when the back button is pressed.
     */
    void goHome();

    /**
     * Signal that sets the practice difficulty.
     */
    void setDifficulty(QString difficulty);

    /**
     * Gets the correct practice text for the currently selected difficulty.
     */
    void getPracticeText();
};

#endif // PRACTICEWINDOW_H
