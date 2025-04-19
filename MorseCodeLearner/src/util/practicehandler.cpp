#include "practicehandler.h"
#include <QSettings>

PracticeHandler::PracticeHandler(MorseHandler *morseHandler, QObject *parent)
    : QObject{parent}
    , morseHandler(morseHandler)
{
    loadProblemTimer.setSingleShot(true);
    loadProblemFromTextTimer.setSingleShot(true);
    audioDelayTimer.setSingleShot(true);

    // Timer connects
    QObject::connect(&loadProblemTimer, &QTimer::timeout, this, [this](){loadPracticeProblem();});
    QObject::connect(&loadProblemFromTextTimer, &QTimer::timeout, this, [this](){loadPracticeProblem(problemText);});
    QObject::connect(&audioDelayTimer, &QTimer::timeout, this, [this, morseHandler](){
        morseHandler->playMorse(morseHandler->encodeText(problemText.toStdString()));
        emit soundPlaying();
    });

    difficultyHandler = new DifficultyHandler();

    QObject::connect(morseHandler, &MorseHandler::decodedInput, this, &PracticeHandler::onMorseReceived);
    QObject::connect(morseHandler, &MorseHandler::playbackEnd, this, [this](){emit soundNotPlaying();});
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOn, this, [this](){emit lightIndicatorOn();});
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOff, this, [this](){emit lightIndicatorOff();});
}

void PracticeHandler::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
    morsePreviewText = "";
    emit updateMorsePreviewText(morsePreviewText);

    if (userOnThisPage) {
        loadHighScore();
        loadPracticeProblem();
        firstAudioPlay = true;

        if (morseHandler->getDevice() == MorseHandler::STRAIGHT_KEY) {
            emit straightKeySelected();
        } else {
            emit paddleSelected();
        }
    }
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
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || mode != ENCODE_ENGLISH)
        return;
    morseHandler->straightKeyUp();
}

void PracticeHandler::handleLeftArrowPressed() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || !acceptingInput || mode != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDotDown();
}

void PracticeHandler::handleLeftArrowReleased() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || mode != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDotUp();
}

void PracticeHandler::handleRightArrowPressed() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || !acceptingInput || mode != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDashDown();
}

void PracticeHandler::handleRightArrowReleased() {
    if (userOnThisPage == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || mode != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDashUp();
}


void PracticeHandler::loadPracticeProblem() {
    inputText = "";
    morseText = "";
    hardWordCounter = 0;
    morseHandler->stopPlayback();

    problemText = difficultyHandler->getPracticeString();

    if (mode == ENCODE_ENGLISH) {
        emit updatePracticeText(problemText);
    } else if (mode == DECODE_MORSE) {
        emit updatePracticeText(QString::fromStdString(morseHandler->encodeText(problemText.toStdString())));
    } else if (mode == DECODE_SOUND) {
        emit updatePracticeText("");

        if (firstAudioPlay) {
            audioDelayTimer.start(1000);
            firstAudioPlay = false;
        } else {
            emit soundPlaying();
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
    morseHandler->stopPlayback();

    if (mode == ENCODE_ENGLISH) {
        emit updatePracticeText(problemText);
    } else if (mode == DECODE_MORSE) {
        emit updatePracticeText(QString::fromStdString(morseHandler->encodeText(problemText.toStdString())));
    } else if (mode == DECODE_SOUND) {
        emit updatePracticeText("");

        if (firstAudioPlay) {
            audioDelayTimer.start(1000);
            firstAudioPlay = false;
        } else {
            emit soundPlaying();
            morseHandler->playMorse(morseHandler->encodeText(problemText.toStdString()));
        }
    }

    emit updateInputText("");
    acceptingInput = true;
}

void PracticeHandler::onMorseReceived(const string morse) {
    if (!userOnThisPage || !(mode == ENCODE_ENGLISH) || !acceptingInput)
        return;

    QString qmorse = QString::fromStdString(morse);

    if (qmorse == " " || qmorse == "/ ") {
        morseText += morsePreviewText + qmorse;
        emit updateInputText(morseText);
        morsePreviewText = "";
    } else {
        morsePreviewText += qmorse;
    }
    emit updateMorsePreviewText(morsePreviewText);

    if (difficultyHandler->getDifficulty() == DifficultyHandler::difficulty::HARD) {
        // Hard Difficulty
        if (qmorse == "/ " && hardWordCounter < 4) {
            hardWordCounter++;
        } else if (qmorse == "/ " && hardWordCounter >= 4) {
            checkProblem();
        }
    }
    // checks for other modes
    else if (qmorse == "/ ") {
        checkProblem();
    }

}

void PracticeHandler::setDifficulty(QString difficulty) {
    firstAudioPlay = true;
    difficultyHandler->setDifficulty(difficulty);
    score = 0;
    audioDelayTimer.stop();

    emit updateScore(QString::number(score));
    loadPracticeProblem();
}

void PracticeHandler::setMode(QString newMode) {
    firstAudioPlay = true;
    audioDelayTimer.stop();

    newMode = newMode.toLower();
    if (newMode == "encode english") {
        mode = ENCODE_ENGLISH;
        emit isInputReadOnly(true);
    } else if (newMode == "decode morse") {
        mode = DECODE_MORSE;
        emit isInputReadOnly(false);
        emit focusInput();
    } else if (newMode == "decode sound") {
        mode = DECODE_SOUND;
        emit isInputReadOnly(false);
        emit focusInput();
        emit updateInputText("");
    }

    score = 0;
    streak = 0;
    emit updateScore(QString::number(score));

    loadHighScore();
    loadPracticeProblem();
}

void PracticeHandler::checkProblem() {
    acceptingInput = false;

    if (inputText == problemText || inputText == problemText + " " || inputText == problemText + " / ") {
        emit updatePracticeText("Correct!");
        streak++;
        score += problemText.length() * 100 * (streak * 0.25) * (1200 / morseHandler->getUnitTime());
        emit updateScore(QString::number(score));
        emit correctTextJump();

        if (isHighScore(score)) {
            saveHighScore(score);
            emit updateHighScore(QString::number(score));
        }

        loadProblemTimer.start(1500);
    }
    else {
        emit updatePracticeText("Try Again!");
        streak = 0;
        score = 0;
        emit updateScore(QString::number(0));
        emit incorrectTextShake();
        loadProblemFromTextTimer.start(1000);
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

void PracticeHandler::skipProblem() {
    if (acceptingInput) {
        if (isHighScore(score)) {
            saveHighScore(score);
        }

        score = 0;
        streak = 0;
        emit updateScore(QString::number(score));
        loadPracticeProblem();
    }
}

void PracticeHandler::saveHighScore(int score) {
    QSettings settings("Tigers", "MorseCodeLearner");
    QString key = QString::number(difficultyHandler->getDifficulty()) + QString::number(mode);
    settings.setValue(key, score);
}

void PracticeHandler::loadHighScore() {
    QSettings settings("Tigers", "MorseCodeLearner");
    QString key = QString::number(difficultyHandler->getDifficulty()) + QString::number(mode);
    int highScore = settings.value(key).toInt();
    emit updateHighScore(QString::number(highScore));
}

bool PracticeHandler::isHighScore(int score) {
    QSettings settings("Tigers", "MorseCodeLearner");
    QString key = QString::number(difficultyHandler->getDifficulty()) + QString::number(mode);
    return score > settings.value(key).toInt();
}

