#ifndef LESSONHANDLER_H
#define LESSONHANDLER_H

#include <QObject>
#include "morsehandler.h"

class LessonHandler : public QObject
{
    Q_OBJECT
private:
    MorseHandler *morseHandler;
    std::vector<std::string> lessonTwoLetters;
    std::vector<std::string> lessonThreeLetters;
    std::vector<std::string> lessonFourLetters;
    std::vector<std::string> lessonFiveLetters;
    std::vector<std::string> lessonSixLetters;
    std::vector<std::string> lessonEightNumbers;
    std::unordered_map<std::string, int> learnedCharacters;
    int currentLesson;

    void displayMorse(const std::string text);

    void displayText(const std::string morse);

    void lessonTwo();

    void lessonThree();

    void lessonFour();
    
    void lessonFive();

    void lessonSix();

    void lessonEight();

    void letterLearned();

public:
    explicit LessonHandler(MorseHandler *morseHandler = nullptr, QObject* parent = nullptr);

public slots:
    void startLesson();

    // void nextQuestion();

    void checkUserGuess();

signals:

    void guessCorrect();

    void guessIncorrect();

    void displayMorseToUI(const std::string morse);

    void displayTextToUI(const std::string text);
};

#endif // LESSONHANDLER_H
