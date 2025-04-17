#include "practicehandler.h"

PracticeHandler::PracticeHandler(MorseHandler *morseHandler, QObject *parent)
    : QObject{parent}
    , morseHandler(morseHandler)
{
    QTimer timer(this);

    difficultyHandler = new DifficultyHandler();

    QObject::connect(morseHandler, &MorseHandler::decodedInput, this, &PracticeHandler::onMorseReceived);
    QObject::connect(morseHandler, &MorseHandler::playbackEnd, this, [this](){emit soundNotPlaying();});
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOn, this, [this](){emit lightIndicatorOn();});
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOff, this, [this](){emit lightIndicatorOff();});
}

void PracticeHandler::setUserOnThisPage(bool userOnThisPage) {
    firstAudioPlay = true;
    this->userOnThisPage = userOnThisPage;
    loadPracticeProblem();
}

bool PracticeHandler::getUserOnThisPage() {
    return userOnThisPage;
}

void PracticeHandler::onBackButtonClicked() {
    emit updateInputText("");
    inputText = "";
    morseText = "";
    problemText = "";
    score = 0;
    streak = 0;
    emit updateScore(QString::number(0));
    morseHandler->stopPlayback();

    emit goHome();
    userOnThisPage = false;
}

void PracticeHandler::handleSpacePressed() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput || mode != ENCODE_ENGLISH)
        return;
    morseHandler->straightKeyDown();
}

void PracticeHandler::handleSpaceReleased() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput || mode != ENCODE_ENGLISH)
        return;
    morseHandler->straightKeyUp();
}

void PracticeHandler::handleLeftArrowPressed() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || !acceptingInput || mode != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDotDown();
}

void PracticeHandler::handleLeftArrowReleased() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || !acceptingInput || mode != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDotUp();
}

void PracticeHandler::handleRightArrowPressed() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || !acceptingInput || mode != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDashDown();
}

void PracticeHandler::handleRightArrowReleased() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || !acceptingInput || mode != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDashUp();
}


void PracticeHandler::loadPracticeProblem() {
    inputText = "";
    morseText = "";
    hardWordCounter = 0;
    problemText = difficultyHandler->getPracticeString();

    if (mode == ENCODE_ENGLISH) {
        emit updatePracticeText(problemText);
    } else if (mode == DECODE_MORSE) {
        emit updatePracticeText(QString::fromStdString(morseHandler->encodeText(problemText.toStdString())));
    } else if (mode == DECODE_SOUND) {
        emit updatePracticeText("");


        if (firstAudioPlay) {
            timer.singleShot(1000, [this](){
                morseHandler->playMorse(morseHandler->encodeText(problemText.toStdString()));
                emit soundPlaying();
            });
            firstAudioPlay = false;
        } else {
            morseHandler->playMorse(morseHandler->encodeText(problemText.toStdString()));
        }
    }

    emit updateInputText("");
    acceptingInput = true;
}

void PracticeHandler::loadPracticeProblem(QString problemText) {
    inputText = "";
    morseText = "";
    hardWordCounter = 0;

    if (mode == ENCODE_ENGLISH) {
        emit updatePracticeText(problemText);
    } else if (mode == DECODE_MORSE) {
        emit updatePracticeText(QString::fromStdString(morseHandler->encodeText(problemText.toStdString())));
    } else if (mode == DECODE_SOUND) {
        emit updatePracticeText("");
        emit soundPlaying();
        if (firstAudioPlay) {
            timer.singleShot(1000, [this, problemText](){
                morseHandler->playMorse(morseHandler->encodeText(problemText.toStdString()));
                emit soundPlaying();
            });
            firstAudioPlay = false;
        } else {
            morseHandler->playMorse(morseHandler->encodeText(problemText.toStdString()));
        }
    }

    emit updateInputText("");
    acceptingInput = true;
}

void PracticeHandler::onMorseReceived(const string morse) {
    if (!userOnThisPage || !(mode == ENCODE_ENGLISH))
        return;

    QString qmorse = QString::fromStdString(morse);

    if (difficultyHandler->getDifficulty() == 2) {
        // Hard Difficulty
        if (qmorse == "/ " && hardWordCounter < 4) {
            hardWordCounter++;
            morseText += qmorse;
            emit updateInputText(morseText);
        } else if (qmorse == "/ " && hardWordCounter >= 4) {
            checkProblem();
        } else {
            morseText += qmorse;
            emit updateInputText(morseText);
        }
    }
    // checks for other modes
    else if (qmorse == "/ ") {
        checkProblem();
    }
    else {
        morseText += qmorse;
        emit updateInputText(morseText);
    }
}

void PracticeHandler::setDifficulty(QString difficulty) {
    firstAudioPlay = true;
    difficultyHandler->setDifficulty(difficulty);
    score = 0;
    morseHandler->stopPlayback();
    emit updateScore(QString::number(score));
    loadPracticeProblem();
    loadPracticeProblem();
}

void PracticeHandler::setMode(QString newMode) {
    firstAudioPlay = true;
    morseHandler->stopPlayback();

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
        emit updateInputText("");
    }
    score = 0;
    streak = 0;
    emit updateScore(QString::number(score));
    loadPracticeProblem();
}

void PracticeHandler::checkProblem() {
    acceptingInput = false;
    if (inputText == problemText) {
        emit updatePracticeText("Correct!");
        streak++;
        score += problemText.length() * 100 * (streak * 0.25);
        emit updateScore(QString::number(score));
        timer.singleShot(1500, this, [this](){loadPracticeProblem();});
    }
    else {
        emit updatePracticeText("Try Again!");
        streak = 0;
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

