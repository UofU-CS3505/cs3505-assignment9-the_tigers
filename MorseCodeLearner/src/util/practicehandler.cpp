#include "practicehandler.h"

PracticeHandler::PracticeHandler(MorseHandler *morseHandler, QObject *parent)
    : QObject{parent}
    , morseHandler(morseHandler)
{
    QTimer timer(this);

    DifficultyHandler difficultyHandler;

    QObject::connect(morseHandler, &MorseHandler::decodedInput, this, &PracticeHandler::onMorseReceived);

    loadPracticeProblem();
}

void PracticeHandler::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
}

bool PracticeHandler::getUserOnThisPage() {
    return userOnThisPage;
}

void PracticeHandler::onBackButtonClicked() {
    emit updateInputText("");

    emit goHome();
    userOnThisPage = false;
}


void PracticeHandler::handleSpacePressed() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput)
        return;
    morseHandler->straightKeyDown();
}

void PracticeHandler::handleSpaceReleased() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput)
        return;
    morseHandler->straightKeyUp();
}


void PracticeHandler::loadPracticeProblem() {
    // clear input text and send new practice problem
    emit updateInputText("");
    emit updatePracticeText(QString::fromStdString(difficultyHandler->getPracticeString()));

    acceptingInput = true;
}

void PracticeHandler::loadPracticeProblem(QString problemText) {
    // clear input text and send new practice problem
    emit updateInputText("");
    emit updatePracticeText(problemText);

    acceptingInput = true;
}

void PracticeHandler::onMorseReceived(const string morse) {
    if (!userOnThisPage)
        return;

    QString qmorse = QString::fromStdString(morse);

    // TODO implement different checks depending on mode.

    if (qmorse == "/ ")
        return;

    if (qmorse == " ") {
        // check vs problem character
        if (morseHandler->decodeMorse(morseText.toStdString()) == problemText) {
            acceptingInput = false;
            emit updatePracticeText("Correct!");
            timer.singleShot(1500, this, [this](){loadPracticeProblem();});
        }

        // if incorrect clear and try again
        if (morseHandler->decodeMorse(morseText.toStdString()) != problemText) {
            acceptingInput = false;
            emit updatePracticeText("Try Again!");
            timer.singleShot(1500, this, [this](){loadPracticeProblem(QString::fromStdString(problemText));});
        }
        return;
    }

    emit updateInputText(morseText + qmorse);
}

void PracticeHandler::setDifficulty(QString difficulty) {
    // TODO implement
}



