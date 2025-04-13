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

void LessonHandler::lessonTwo() {

}

void LessonHandler::lessonThree() {

}

void LessonHandler::lessonFour() {

}

void LessonHandler::lessonFive() {

}

void LessonHandler::lessonSix() {

}

void LessonHandler::lessonEight() {

}

void LessonHandler::startLesson(int lessonNumber) {
    switch (lessonNumber) {
        case 2:
            lessonTwo();
            break;
        case 3:
            lessonThree();
            break;
        case 4:
            lessonFour();
            break;
        case 5:
            lessonFive();
            break;
        case 6:
            lessonSix();
            break;
        case 8:
            lessonEight();
            break;
        default:
            break;
    }
}

void LessonHandler::checkUserGuess(std::string guess) {
    std::string correctAnswer = morseHandler->encodeText(currentQuestion);

}
