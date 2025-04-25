#ifndef PRACTICEHANDLER_H
#define PRACTICEHANDLER_H

#include <QObject>
#include "morsehandler.h"
#include "difficultyhandler.h"
#include "keyeventfilter.h"

/**
 * Model and backend handler for practice mode.
 * Practice mode has three difficulties and three modes.
 * Difficulties: Easy, Medium, Hard
 * Modes: Encode English, Decode Morse, Decode Audio
 *
 * @name Chandler Eyre
 * @date 4/24/25
 */

class PracticeHandler : public QObject
{
    Q_OBJECT
private:
    MorseHandler *morseHandler;
    KeyEventFilter *keyEventFilter;
    DifficultyHandler *difficultyHandler;

    // Timers
    QTimer loadProblemTimer;
    QTimer loadProblemFromTextTimer;
    QTimer audioDelayTimer;

    bool userOnThisPage = false;

    QString problemText; // contains the current problem in english
    QString inputText; // contains the user's input
    QString morseText; // contains morse input for english problem and morse input
    QString morsePreviewText; // currently typed morse character

    int score = 0;
    int streak = 0; // number of correct answers in a row

    bool acceptingInput = false;
    bool firstAudioPlay = true; // whether this is the first question in decode audio mode
    int hardWordCounter = 0; // the amount of words input thus far; used to track when to check the problem in hard mode

    enum practiceMode { ENCODE_ENGLISH, DECODE_MORSE, DECODE_SOUND };
    practiceMode mode = ENCODE_ENGLISH;

    /**
     * Loads a practice problem using state and difficulty info, and sends to the window.
     */
    void loadPracticeProblem();

    /**
     * Loads a practice problem using a seed text, and sends to the window.
     */
    void loadPracticeProblem(QString seedText);

    /**
     * Saves a high score for a given difficulty and mode.
     *
     * @param difficulty - Current difficulty
     * @param mode - Current mode
     * @param score - High score to save
     */
    void saveHighScore(int score);

    /**
     * Loads a high score for a given difficulty and mode.
     *
     * @param difficulty - Current difficulty
     * @param mode - Current mode
     */
    void loadHighScore();

    /**
     * Checks if a given score is greater than the new high score for that mode.
     *
     * @param score - Score to check
     * @return - Whether given score is greater than high score
     */
    bool isHighScore(int score);

public:
    /**
     * Constructor for the Practice Handler.
     * @param morseHandler - A pointer to the Morse Handler
     * @param eventFilter - A pointer to the KeyEventFilter
     * @param parent - The QObject parent
     */
    explicit PracticeHandler(MorseHandler *morseHandler = nullptr, KeyEventFilter *keyEventFilter = nullptr, QObject *parent = nullptr);

    /**
     * Sets whether the user is on this page.
     * @param userOnThisPage
     */
    void setUserOnThisPage(bool userOnThisPage);

    /**
     * @return whether the user is on this page.
     */
    bool getUserOnThisPage();

    /**
     * Sets the practice mode.
     * Mode can be "Encode English", "Decode Morse", "Decode Sound"
     * @param newMode - The new mode
     */
    void setMode(QString newMode);

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
     * Handles logic for morse input with left arrow.
     */
    void handleLeftArrowPressed();

    /**
     * Handles logic for morse input with left arrow.
     */
    void handleLeftArrowReleased();

    /**
     * Handles logic for morse input with right arrow.
     */
    void handleRightArrowPressed();

    /**
     * Handles logic for morse input with right arrow.
     */
    void handleRightArrowReleased();

    /**
     * Handles logic for pressing the enter key, i.e. checking a problem
     */
    void handleEnterPressed();

    /**
     * Slot for when the MorseHandler object sends us a morse character.
     * The morse character is appended to our input text box.
     * @param morse The morse character.
     */
    void onMorseReceived(const std::string morse);

    /**
     * Sets the practice difficulty in the DifficultyHandler.
     * @param difficulty - New difficulty
     */
    void setDifficulty(QString difficulty);

    /**
     * Resets the page's state when the back button is clicked.
     */
    void onBackButtonClicked();

    /**
     * Checks if the input is correct and sets up the next problem.
     */
    void checkProblem();

    /**
     * Updates the stored input text.
     * If mode is encoding english, will convert morse to english for easier comparison.
     * @param text - New input text
     */
    void receiveInputText(QString text);

    /**
     * Skips the current problem.
     */
    void skipProblem();

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
     * Emits the the current morse character for preview.
     * @param morse - morse code representation of current character
     */
    void updateMorsePreviewText(QString morse);

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

    /**
     * Emits the current score
     * @param score - New score
     */
    void updateScore(QString score);

    /**
     * Emitted when morse code starts playing in decode audio mode.
     */
    void soundPlaying();

    /**
     * Emitted when morse code stops playing in decode audio mode.
     */
    void soundNotPlaying();

    /**
     * Sends a high score
     * @param highScore - high score to send
     */
    void updateHighScore(QString highScore);

    /**
     * Emits a signal for Box2D to make the text jump
     * when a correct answer is inputted.
     */
    void correctTextJump();

    /**
     * Emits a signal for Box2D to make the text shake
     * when an incorrect answer is inputted.
     */
    void incorrectTextShake();

    /**
     * Emitted when the straight key is pressed.
     */
    void straightKeyPressed();

    /**
     * Emitted when the straight key is released.
     */
    void straightKeyReleased();

    /**
     * Emitted when the left paddle is pressed.
     */
    void leftPaddlePressed();

    /**
     * Emitted when the left paddle is released.
     */
    void leftPaddleReleased();

    /**
     * Emitted when the right paddle is pressed.
     */
    void rightPaddlePressed();

    /**
     * Emitted when the right paddle is released.
     */
    void rightPaddleReleased();

    /**
     * Emitted when the paddle is selected as input device.
     */
    void paddleSelected();

    /**
     * Emitted when the straight key is selected as input device.
     */
    void straightKeySelected();
};

#endif // PRACTICEHANDLER_H
