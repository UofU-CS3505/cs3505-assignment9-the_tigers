#ifndef LESSONHANDLER_H
#define LESSONHANDLER_H

#include <QObject>
#include "morsehandler.h"
#include "keyeventfilter.h"

/**
 * The model and handler for interactive lessons (i.e. lessons that have practice questions).
 * Built to handle all the logic and backend for each of the different lessons; signals will
 * call the slot to start a lesson, then the startLesson method calls the appropriate method
 * according to the lesson number.
 *
 * @name Aiden Fornalski
 * @date 04/12/25
 */
class LessonHandler : public QObject
{
    Q_OBJECT
private:
    MorseHandler *morseHandler;
    KeyEventFilter *keyEventFilter;

    std::vector<std::string> lessonTwoLetters = {"e", "t", "a", "n", "m", "i"};
    std::vector<std::string> lessonThreeLetters = {"k", "r", "s", "o"};
    std::vector<std::string> lessonFourLetters = {"g", "w", "d", "u"};
    std::vector<std::string> lessonFiveLetters = {"b", "j", "f", "l", "c", "h"};
    std::vector<std::string> lessonSixLetters = {"z", "v", "y", "q", "p", "x"};
    std::vector<std::string> lessonEightNumbers = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    std::vector<std::string> lessonNineWords = {"hello world", "thank you", "repeat that"};

    std::unordered_map<std::string, int> learnedCharacters;
    int encodeQuestionTimes;
    int decodeQuestionTimes;
    int soundQuestionTimes;

    int currentLessonNumber;
    std::vector<std::string> currentLessonCharacters;
    std::string currentQuestion;
    QString inputText;
    int wordCounter;
    float lessonProgress;

    QTimer timer;

    bool acceptingInput;
    bool userOnThisPage = false;
    int currentIndex;

    enum questionType { ENCODE_ENGLISH, DECODE_MORSE, DECODE_SOUND };
    questionType type = ENCODE_ENGLISH;

    /**
     * A helper method that displays each of the questions in a lesson.
     */
    void nextQuestion();

    /**
     * A helper method that runs when the lesson in completed.
     */
    void lessonComplete();

    /**
     * A helper method that randomly picks the question type.
     */
    void pickQuestionType();

public:
    /**
     * The constructor for the Lesson Handler.
     * @param morseHandler - a pointer to the Morse Handler.
     * @param keyEventFilter - a pointer to the Key Event Filter.
     * @param parent - the QObject (nullptr by default).
     */
    explicit LessonHandler(MorseHandler *morseHandler = nullptr, KeyEventFilter *keyEventFilter = nullptr, QObject* parent = nullptr);

public slots:
    /**
     * A slot that starts a lesson; starts the correct lesson
     * according to the lessonNumber.
     * @param lessonNumber - the number of the lesson to start.
     */
    void startLesson(int lessonNumber);

    /**
     * A slot that checks the user's guess.
     */
    void checkUserGuess();

    /**
     * Slot that runs when a morse input is received.
     * @param input - the morse input in a string.
     */
    void onInputReceived(const std::string input);

    /**
     * Runs when the space bar is pressed down.
     */
    void handleSpacePressed();

    /**
     * Runs when the space bar is released.
     */
    void handleSpaceReleased();

    /**
     * Runs when the left arrow key is pressed down.
     */
    void handleLeftArrowPressed();

    /**
     * Runs when the left arrow key is released.
     */
    void handleLeftArrowReleased();

    /**
     * Runs when the right arrow key is pressed down.
     */
    void handleRightArrowPressed();

    /**
     * Runs when the right arrow key is released.
     */
    void handleRightArrowReleased();

    /**
     * Runs when the enter key is used.
     */
    void handleEnterPressed();

    /**
     * Runs when the back button is clicked from the UI.
     */
    void onBackButtonClicked();

    /**
     * Runs when the user is on the page or leaves the
     * page.
     * @param userOnThisPage - whether the user is on the
     * page or not.
     */
    void setUserOnThisPage(bool userOnThisPage);

    /**
     * Runs when the current index of the stackedWidget
     * from the UI is changed.
     * @param currentIndex - the current index in the
     * stackedWidget.
     */
    void setCurrentIndex(int currentIndex);

    /**
     * Gets what is currently in the input box from the
     * UI.
     * @param inputText - the input from the input box.
     */
    void getInputText(QString inputText);

signals:
    /**
     * A signal that gets emitted when the user is correct.
     */
    void guessCorrect();

    /**
     * A signal that gets emitted when the user is incorrect.
     */
    void guessIncorrect();

    /**
     * A signal that gets emitted to send the text to a view.
     * @param text - the text to send.
     */
    void displayTextToUI(QString text);

    /**
     * Updates what the text should be in the input box from
     * the UI.
     * @param inputText - the new text to display in the
     * input box.
     */
    void updateInputText(QString inputText);

    /**
     * Sets the light indicator in the UI.
     * @param lightIndicator - true if on, false if off.
     */
    void setLightIndicator(bool lightIndicator);

    /**
     * A signal to let the UI know that the lesson has been
     * completed.
     */
    void completedLesson();

    /**
     * For illustration, emitted when navigating to this page.
     */
    void paddleSelected();

    /**
     * For illustration, emitted when navigating to this page.
     */
    void straightKeySelected();

    /**
     * Sets and updates the lesson title in the lesson UI.
     * @param lessonNumber - the current lesson number.
     */
    void updateLessonTitle(int lessonNumber);

    /**
     * Updates the lesson progress bar in the lesson UI.
     * @param progress - what the current progress through the
     * lesson is.
     */
    void updateLessonProgressBar(float progress);

    /**
     * Displays what the correct answer of a question was to
     * the UI.
     * @param correctAnswer - the correct answer of the question.
     */
    void displayCorrectAnswer(QString correctAnswer);

    /**
     * Sets the reference text in the UI (the slide before the
     * actual lesson practice).
     * @param referenceText - the reference text for the
     * specified question.
     */
    void setReferenceText(QString referenceText);

    /**
     * Sets the input box to be read only mode or not read
     * only mode.
     * @param readOnly - true for read only mode, false to not be
     * read only mode.
     */
    void isInputReadOnly(bool readOnly);

    /**
     * Emitted when morse code starts playing in decode audio mode.
     */
    void soundPlaying();

    /**
     * Emitted when morse code is no longer playing in decode audio mode.
     */
    void soundNotPlaying();

    /**
     * Emitted so the window can change view stuff in decode audio mode.
     */
    void isAudioDecodeMode(bool mode);

    /**
     * Emitted to focus input for decode morse or decode audio problems.
     */
    void focusInput();

    /**
     * Asks the UI for the text in the input box.
     */
    void getInputTextSignal();

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
    void paddleLeftPressed();

    /**
     * Emitted when the left paddle is released.
     */
    void paddleLeftReleased();

    /**
     * Emitted when the right paddle is pressed.
     */
    void paddleRightPressed();

    /**
     * Emitted when the right paddle is released.
     */
    void paddleRightReleased();
};
#endif // LESSONHANDLER_H
