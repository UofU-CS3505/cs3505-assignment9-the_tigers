#include "lessonhandler.h"
#include <QObject>
#include <QSettings>

LessonHandler::LessonHandler(MorseHandler *morseHandler, QObject* parent) :
    QObject(parent),
    morseHandler(morseHandler),
    acceptingInput(false),
    userOnThisPage(false)
{
    QTimer timer(this);
}

void LessonHandler::displayMorse(const std::string text) {
    std::string morse = morseHandler->encodeText(text);
    emit displayMorseToUI(morse);
}

void LessonHandler::displayText(const std::string morse) {
    std::string text = morseHandler->decodeMorse(morse);
    emit displayTextToUI(QString::fromStdString(text));
}

void LessonHandler::nextQuestion() {
    acceptingInput = true;
    int charactersLearned = 0;

    for (const std::string &character : currentLessonCharacters) {
        if (learnedCharacters[character] >= 3) {
            charactersLearned++;
        }
    }

    if (charactersLearned == (int)currentLessonCharacters.size()) {
        lessonComplete();
        return;
    }

    while (true) {
        currentQuestion = currentLessonCharacters[rand() % currentLessonCharacters.size()];
        if (learnedCharacters[currentQuestion] < 3) {
            break;
        }
    }

    emit displayTextToUI("What is " + QString::fromStdString(currentQuestion) + " in morse?");
}

void LessonHandler::startLesson(int lessonNumber) {
    currentLessonNumber = lessonNumber;

    switch (currentLessonNumber) {
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
    acceptingInput = false;

    std::string correctAnswer = morseHandler->encodeText(currentQuestion);

    if (guess == correctAnswer) {
        learnedCharacters[currentQuestion] += 1;
        emit guessCorrect();
        morseText = "";
        emit updateInputText(morseText);
    } else {
        emit guessIncorrect();
        morseText = "";
        emit updateInputText(morseText);
    }

    timer.singleShot(1500, this, [this](){nextQuestion();});
}

void LessonHandler::lessonComplete() {
    acceptingInput = false;

    QSettings settings("Tigers", "MorseCodeLearner");
    settings.setValue("lesson" + std::to_string(currentLessonNumber) + "Completed", true);

    emit displayTextToUI("You have completed this lesson!");

    timer.singleShot(1500, this, [=](){emit completedLesson();});
}

void LessonHandler::onMorseReceived(const std::string morse) {
    if (acceptingInput) {
        QString qmorse = QString::fromStdString(morse);

        if (qmorse == "/ ") {
            checkUserGuess(morseText.toStdString());
        } else {
            morseText += qmorse;
            emit updateInputText(morseText);
        }
    }
}

void LessonHandler::handleSpacePressed() {
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput)
        return;
    emit lightIndicatorOn();
    morseHandler->straightKeyDown();
}

void LessonHandler::handleSpaceReleased() {
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput)
        return;
    emit lightIndicatorOff();
    morseHandler->straightKeyUp();
}

void LessonHandler::onBackButtonClicked() {
    acceptingInput = false;
    userOnThisPage = false;
}

void LessonHandler::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}
