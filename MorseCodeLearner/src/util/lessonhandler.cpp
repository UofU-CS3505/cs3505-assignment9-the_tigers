#include "lessonhandler.h"
#include <QObject>

LessonHandler::LessonHandler(MorseHandler *morseHandler, QObject* parent) :
    QObject(parent),
    morseHandler(morseHandler)
{
    //QObject::connect();
}

void LessonHandler::displayMorse(const std::string text) {
    std::string morse = morseHandler->encodeText(text);
    emit displayMorseToUI(morse);
}

void LessonHandler::displayText(const std::string morse) {
    std::string text = morseHandler->decodeMorse(morse);
    emit displayTextToUI(text);
}

void LessonHandler::nextQuestion() {
    while (true) {
        currentQuestion = currentLessonCharacters[rand() % currentLessonCharacters.size()];
        if (learnedCharacters[currentQuestion] < 3) {
            break;
        }
    }

    // logic for sending the question to view
}

void LessonHandler::startLesson(int lessonNumber) {
    switch (lessonNumber) {
        case 2:
            currentLessonCharacters = lessonTwoLetters;
            break;
        case 3:
            currentLessonCharacters = lessonThreeLetters;
            break;
        case 4:
            currentLessonCharacters = lessonFourLetters;
            break;
        case 5:
            currentLessonCharacters = lessonFiveLetters;
            break;
        case 6:
            currentLessonCharacters = lessonSixLetters;
            break;
        case 8:
            currentLessonCharacters = lessonEightNumbers;
            break;
        default:
            return;
    }

    for (const std::string &character : currentLessonCharacters) {
        learnedCharacters[character] = 0;
    }
    nextQuestion();
}

void LessonHandler::checkUserGuess(std::string guess) {
    std::string correctAnswer = morseHandler->encodeText(currentQuestion);

    if (guess == correctAnswer) {
        learnedCharacters[currentQuestion] += 1;
        emit guessCorrect();
    } else {
        emit guessIncorrect();
    }

    nextQuestion();
}
