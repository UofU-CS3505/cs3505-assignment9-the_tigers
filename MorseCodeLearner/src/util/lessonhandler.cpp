#include "lessonhandler.h"

LessonHandler::LessonHandler(MorseHandler *morseHandler, QObject* parent) : QObject(parent), morseHandler(morseHandler) {}

void LessonHandler::displayMorse(const std::string text) {
    std::string morse = morseHandler->encodeText(text);
    emit displayMorseToUI(morse);
}

void LessonHandler::displayText(const std::string morse) {
    std::string text = morseHandler->decodeMorse(morse);
    emit displayTextToUI(text);
}

std::string LessonHandler::getRandomLetter() {
    return lessonLetters[rand() % lessonLetters.size()];
}

void LessonHandler::lessonOne() {

}

void LessonHandler::lessonTwo() {

}

void LessonHandler::lessonThree() {

}
