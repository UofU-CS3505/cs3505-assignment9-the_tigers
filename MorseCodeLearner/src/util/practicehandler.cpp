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
    inputText = "";
    morseText = "";
    problemText = difficultyHandler->getPracticeString();

    if (mode == ENCODE_ENGLISH) {
        emit updatePracticeText(problemText);
    } else if (mode == DECODE_MORSE) {
        emit updatePracticeText(QString::fromStdString(morseHandler->encodeText(problemText.toStdString())));
    }

    emit updateInputText("");
    acceptingInput = true;
}

void PracticeHandler::loadPracticeProblem(QString problemText) {
    inputText = "";
    morseText = "";
    if (mode == ENCODE_ENGLISH) {
        emit updatePracticeText(problemText);
    } else if (mode == DECODE_MORSE) {
        emit updatePracticeText(QString::fromStdString(morseHandler->encodeText(problemText.toStdString())));
    }

    emit updateInputText("");
    acceptingInput = true;
}

void PracticeHandler::onMorseReceived(const string morse) {
    if (!userOnThisPage || !(mode == ENCODE_ENGLISH))
        return;

    QString qmorse = QString::fromStdString(morse);

    if (qmorse == "/ ") {
        checkProblem();
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

void PracticeHandler::setMode(QString newMode) {
    newMode = newMode.toLower();
    if (newMode == "encode english") {
        mode = ENCODE_ENGLISH;
        emit hideInputCheck();
        emit isInputReadOnly(true);
    } else if (newMode == "decode morse") {
        mode = DECODE_MORSE;
        emit showInputCheck();
        emit isInputReadOnly(false);
        emit focusInput();
    } else if (newMode == "decode sound") {
        mode = DECODE_SOUND;
        emit showInputCheck();
        emit isInputReadOnly(false);
        emit focusInput();
    }
    loadPracticeProblem();
}

void PracticeHandler::checkProblem() {
    acceptingInput = false;
    if (inputText == problemText) {
        emit updatePracticeText("Correct!");
        timer.singleShot(1500, this, [this](){loadPracticeProblem();});
    }
    else {
        emit updatePracticeText("Try Again!");
        timer.singleShot(1500, this, [this](){loadPracticeProblem(problemText);});
    }
}

void PracticeHandler::receiveInputText(QString text) {
    if (!acceptingInput)
        return;
    if (mode == ENCODE_ENGLISH) {
        inputText = QString::fromStdString(morseHandler->decodeMorse(text.toStdString()));
    } else {
        inputText = text;
    }
}

