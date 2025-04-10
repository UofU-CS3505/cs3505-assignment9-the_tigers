#ifndef LESSONHANDLER_H
#define LESSONHANDLER_H

#include <QObject>
#include "morsehandler.h"

class LessonHandler : public QObject
{
    Q_OBJECT
private:
    MorseHandler& morseHandler;
    std::vector<char> lessonLetters;
    std::vector<char> learnedLetters;
    int currentLesson;

    void displayMorse();

    void displayCharacter();

    char getRandomLetter();

    void lessonOne();

    void lessonTwo();

    void lessonThree();

    void letterLearned();

public:
    LessonHandler(MorseHandler& morseHandler, QObject* parent = nullptr);

public slots:
    void startLesson();

    void nextQuestion();

    void checkUserGuess();

signals:

    void guessCorrect();

    void guessIncorrect();

    void displayQuestion();
};

#endif // LESSONHANDLER_H
