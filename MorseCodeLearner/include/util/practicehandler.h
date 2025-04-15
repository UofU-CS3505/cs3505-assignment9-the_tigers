#ifndef PRACTICEHANDLER_H
#define PRACTICEHANDLER_H

#include <QObject>
#include "morsehandler.h"
#include "difficultyhandler.h"

/**
 * Model and backend handler for practice mode.
 * Practice mode has three difficulties and three modes.
 *
 * @name Chandler Eyre
 * @date 4/15/25
 */

class PracticeHandler : public QObject
{
    Q_OBJECT
private:
    MorseHandler *morseHandler;
    DifficultyHandler *difficultyHandler;
    QTimer timer;
    bool userOnThisPage;
    QString problemText;
    QString morseText; // string containing morse input for english problem and morse input
    bool acceptingInput;

    /**
     * Loads a practice problem using state and difficulty info, and sends to the window.
     */
    void loadPracticeProblem();

    /**
     * Loads a practice problem using a seed text, and sends to the window.
     */
    void loadPracticeProblem(QString seedText);

public:
    /**
     * Constructor for the Practice Handler.
     * @param morseHandler - a pointer to the Morse Handler
     * @param parent - The QObject parent
     */
    explicit PracticeHandler(MorseHandler *morseHandler = nullptr, QObject *parent = nullptr);

    /**
     * Sets whether the user is on this page.
     * @param userOnThisPage
     */
    void setUserOnThisPage(bool userOnThisPage);

    /**
     * @return whether the user is on this page.
     */
    bool getUserOnThisPage();

public slots:
    /**
     * Tells the MorseHandler and MorseAudioHandler that the straight key has been pressed
     * as long as: the user is currently on this page, playback of morse is not currently being played,
     * and the translate mode is morse to text.
     */
    void handleSpacePressed();

    /**
     * Tells the MorseHandler and MorseAudioHandler that the straight key has been released
     * as long as: the user is currently on this page, playback of morse is not currently being played,
     * and the translate mode is morse to text.
     */
    void handleSpaceReleased();

    /**
     * Slot for when the MorseHandler object sends us a morse character.
     * The morse character is appended to our input text box.
     * @param morse The morse character.
     */
    void onMorseReceived(const std::string morse);

    /**
     * Sets the practice difficulty in the DifficultyHandler.
     * @param difficulty - new difficulty
     */
    void setDifficulty(QString difficulty);

    /**
     * Resets the page's state when the back button is clicked.
     */
    void onBackButtonClicked();

signals:
    /**
     * Sends a character, word, or sentence to practice to the window.
     * @param displayText - text to display to the user
     */
    void updatePracticeText(QString problemText);

    /**
     * Sends morse representation of the user input to the window.
     * @param morseText - morse code representation
     */
    void updateInputText(QString morseText);

    /**
     * Signal that gets emitted when the back button is pressed.
     */
    void goHome();

    /**
     * Turns on the flashing light indicator.
     */
    void lightIndicatorOn();

    /**
     * Turns off the flashing light indicator.
     */
    void lightIndicatorOff();
};

#endif // PRACTICEHANDLER_H
