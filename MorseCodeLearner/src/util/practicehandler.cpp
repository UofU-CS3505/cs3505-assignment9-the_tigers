#include "practicehandler.h"

PracticeHandler::PracticeHandler(MorseHandler *morseHandler, QObject *parent)
    : QObject{parent}
    , morseHandler(morseHandler)
{
    QTimer timer(this);

    difficultyHandler = new DifficultyHandler();

    QObject::connect(morseHandler, &MorseHandler::decodedInput, this, &PracticeHandler::onMorseReceived);
}

void PracticeHandler::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
    loadPracticeProblem();
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
    emit lightIndicatorOn();
    morseHandler->straightKeyDown();
}

void PracticeHandler::handleSpaceReleased() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput)
        return;
    emit lightIndicatorOff();
    morseHandler->straightKeyUp();
}


void PracticeHandler::loadPracticeProblem() {
    // clear input text and send new practice problem
    emit updateInputText("");
    problemText = difficultyHandler->getPracticeString();
    emit updatePracticeText(problemText);

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

    string inputAsText = morseHandler->decodeMorse(morseText.toStdString());

     if (qmorse == "/ ") {
        if (inputAsText == problemText.toStdString()) {
            acceptingInput = false;
            emit updatePracticeText("Correct!");
            morseText = "";
            timer.singleShot(1500, this, [this](){loadPracticeProblem();});
        }

        // if incorrect clear and try again
        else if (inputAsText != problemText.toStdString()) {
            acceptingInput = false;
            morseText = "";
            emit updatePracticeText("Try Again!");
            timer.singleShot(1500, this, [this](){loadPracticeProblem(problemText);});
        }
    return;
    }
    else {
        morseText += qmorse;
        emit updateInputText(morseText);
    }
}

void PracticeHandler::setDifficulty(QString difficulty) {
    difficultyHandler->setDifficulty(difficulty);

    loadPracticeProblem();
}



