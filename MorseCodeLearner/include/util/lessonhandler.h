#ifndef LESSONHANDLER_H
#define LESSONHANDLER_H

#include <QObject>
#include "morsehandler.h"

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

    std::vector<std::string> lessonTwoLetters = {"e", "t", "a", "n", "m", "i"};
    std::vector<std::string> lessonThreeLetters = {"k", "r", "s", "o"};
    std::vector<std::string> lessonFourLetters = {"g", "w", "d", "u"};
    std::vector<std::string> lessonFiveLetters = {"b", "j", "f", "l", "c", "h"};
    std::vector<std::string> lessonSixLetters = {"z", "v", "y", "q", "p", "x"};
    std::vector<std::string> lessonEightNumbers = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    std::vector<std::string> lessonNineWords = {"hello world", "thank you", "repeat that"};

    std::unordered_map<std::string, int> learnedCharacters;

    int currentLessonNumber;
    std::vector<std::string> currentLessonCharacters;
    std::string currentQuestion;
    QString inputText;
    int wordCounter;
    float lessonProgress;

    QTimer timer;

    bool acceptingInput;
    bool userOnThisPage;
    int currentIndex;

    enum questionType { ENCODE_ENGLISH, DECODE_MORSE, DECODE_SOUND };
    questionType type = ENCODE_ENGLISH;

    /**
     * A helper method that displays each of the questions in a lesson.
     */
    void nextQuestion();

    void lessonComplete();

    void pickQuestionType();

public:
    /**
     * The constructor for the Lesson Handler.
     * @param morseHandler - a pointer to the Morse Handler.
     * @param keyEventFilter - a pointer to the Key Event Filter.
     * @param parent - the QObject (nullptr by default).
     */
    explicit LessonHandler(MorseHandler *morseHandler = nullptr, QObject* parent = nullptr);

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

    void onInputReceived(const std::string input);

    void handleSpacePressed();

    void handleSpaceReleased();

    void handleLeftArrowPressed();

    void handleLeftArrowReleased();

    void handleRightArrowPressed();

    void handleRightArrowReleased();

    void handleEnterPressed();

    void onBackButtonClicked();

    void setUserOnThisPage(bool userOnThisPage);

    void setCurrentIndex(int currentIndex);

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
     * A signal that gets emitted to send the morse to a view.
     * @param morse - the morse to send.
     */
    void displayMorseToUI(std::string morse);

    /**
     * A signal that gets emitted to send the text to a view.
     * @param text - the text to send.
     */
    void displayTextToUI(QString text);

    void updateInputText(QString inputText);

    void lightIndicatorOn();

    void lightIndicatorOff();

    void completedLesson();

    /**
     * For illustration, emitted when navigating to this page.
     */
    void paddleSelected();

    /**
     * For illustration, emitted when navigating to this page.
     */
    void straightKeySelected();

    void updateLessonTitle(int lessonNumber);

    void updateLessonProgressBar(float progress);

    void displayCorrectAnswer(QString correctAnswer);

    void setReferenceText(QString referenceText);

    void isInputReadOnly(bool readOnly);

    /**
     * Emitted when morse code starts playing in decode audio mode.
     */
    void soundPlaying();
};

#endif // LESSONHANDLER_H
