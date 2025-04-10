#include "lessonhandler.h"

LessonHandler::LessonHandler(MorseHandler *morseHandler, QObject* parent) : QObject(parent), morseHandler(morseHandler) {

}
