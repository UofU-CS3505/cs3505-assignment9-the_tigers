#ifndef LESSONHANDLER_H
#define LESSONHANDLER_H

#include <QObject>
#include "morsehandler.h"

class LessonHandler : public QObject
{
    Q_OBJECT
private:
    MorseHandler *morseHandler;
    std::vector<std::string> lessonLetters;
    std::vector<std::string> learnedLetters;
    int currentLesson;

    void displayMorse(const std::string text);

    void displayText(const std::string morse);

    std::string getRandomLetter();

    void lessonOne();

    void lessonTwo();

    void lessonThree();

    void letterLearned();

public:
    explicit LessonHandler(MorseHandler *morseHandler = nullptr, QObject* parent = nullptr);

public slots:
    // void startLesson();

    // void nextQuestion();

    // void checkUserGuess();

signals:

    void guessCorrect();

    void guessIncorrect();

    void displayMorseToUI(const std::string morse);

    void displayTextToUI(const std::string text);
};

#endif // LESSONHANDLER_H
