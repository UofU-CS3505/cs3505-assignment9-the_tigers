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

    std::vector<std::string> lessonTwoLetters;
    std::vector<std::string> lessonThreeLetters;
    std::vector<std::string> lessonFourLetters;
    std::vector<std::string> lessonFiveLetters;
    std::vector<std::string> lessonSixLetters;
    std::vector<std::string> lessonEightNumbers;

    std::unordered_map<std::string, int> learnedCharacters;

    int currentLesson;

    /**
     * Sends a signal to the view of what morse to display.
     * @param text - the text to translate to morse.
     */
    void displayMorse(const std::string text);

    /**
     * Sends a signal to the view of what text to display.
     * @param morse - the morse to translate to text.
     */
    void displayText(const std::string morse);

    /**
     * A method to handle all questions and logic for lesson two.
     */
    void lessonTwo();

    /**
     * A method to handle all questions and logic for lesson three.
     */
    void lessonThree();

    /**
     * A method to handle all questions and logic for lesson four.
     */
    void lessonFour();
    
    /**
     * A method to handle all questions and logic for lesson five.
     */
    void lessonFive();

    /**
     * A method to handle all questions and logic for lesson six.
     */
    void lessonSix();

    /**
     * A method to handle all questions and logic for lesson eight.
     */
    void lessonEight();

    /**
     * A helper method to update the map of letters/numbers learned by
     * the user.
     */
    void letterCharacter();

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

    // void nextQuestion();

    /**
     * A slot that checks the user's guess.
     */
    void checkUserGuess();

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
    void displayMorseToUI(const std::string morse);

    /**
     * A signal that gets emitted to send the text to a view.
     * @param text - the text to send.
     */
    void displayTextToUI(const std::string text);
};

#endif // LESSONHANDLER_H
