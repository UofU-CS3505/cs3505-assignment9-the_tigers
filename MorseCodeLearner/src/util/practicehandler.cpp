#include "practicehandler.h"
#include <QSettings>

PracticeHandler::PracticeHandler(MorseHandler *morseHandler, KeyEventFilter *keyEventFilter, QObject *parent)
    : QObject{parent}
    , morseHandler(morseHandler)
    , keyEventFilter(keyEventFilter)
{
    difficultyHandler = new DifficultyHandler();

    // Timer setup and connects
    loadProblemTimer.setSingleShot(true);
    loadProblemFromTextTimer.setSingleShot(true);
    audioDelayTimer.setSingleShot(true);

    QObject::connect(&loadProblemTimer, &QTimer::timeout, this, [=]() {loadPracticeProblem();});
    QObject::connect(&loadProblemFromTextTimer, &QTimer::timeout, this, [=]() {loadPracticeProblem(problemText);});
    QObject::connect(&audioDelayTimer, &QTimer::timeout, this, [this, morseHandler](){
        morseHandler->playMorse(morseHandler->encodeText(problemText.toStdString()));
        emit soundPlaying();
    });

    QObject::connect(morseHandler, &MorseHandler::decodedInput, this, &PracticeHandler::onMorseReceived);
    QObject::connect(morseHandler, &MorseHandler::playbackEnd, this, [=]() {emit soundNotPlaying();});

    // Pass light indicator to window
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOn, this, [=]() {emit lightIndicatorOn();});
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOff, this, [=]() {emit lightIndicatorOff();});

    // Key Event Filters
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, this, &PracticeHandler::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, this, &PracticeHandler::handleSpaceReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::enterPressed, this, &PracticeHandler::handleEnterPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowPressed, this, &PracticeHandler::handleLeftArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowReleased, this, &PracticeHandler::handleLeftArrowReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowPressed, this, &PracticeHandler::handleRightArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowReleased, this, &PracticeHandler::handleRightArrowReleased);
}

void PracticeHandler::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;
    morsePreviewText = "";
    emit updateMorsePreviewText(morsePreviewText);

    if (userOnThisPage) {
        loadHighScore();
        loadPracticeProblem();
        firstAudioPlay = true;

        // Default illustrations
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
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput || mode != ENCODE_ENGLISH)
        return;
    morseHandler->straightKeyDown();
    emit straightKeyPressed();
}

void PracticeHandler::handleSpaceReleased() {
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || mode != ENCODE_ENGLISH)
        return;
    morseHandler->straightKeyUp();
    emit straightKeyReleased();
}

void PracticeHandler::handleLeftArrowPressed() {
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || !acceptingInput || mode != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDotDown();
    emit leftPaddlePressed();
}

void PracticeHandler::handleLeftArrowReleased() {
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || mode != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDotUp();
    emit leftPaddleReleased();
}

void PracticeHandler::handleRightArrowPressed() {
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || !acceptingInput || mode != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDashDown();
    emit rightPaddlePressed();
}

void PracticeHandler::handleRightArrowReleased() {
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || mode != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDashUp();
    emit rightPaddleReleased();
}

void PracticeHandler::handleEnterPressed() {
    if (!userOnThisPage || mode == ENCODE_ENGLISH)
        return;
    checkProblem();
}

void PracticeHandler::loadPracticeProblem() {
    // Reset state
    inputText = "";
    morseText = "";
    hardWordCounter = 0;
    morseHandler->stopPlayback();

    problemText = difficultyHandler->getPracticeString(); // Load new problem

    if (mode == ENCODE_ENGLISH) {
        emit updatePracticeText(problemText);
    } else if (mode == DECODE_MORSE) {
        emit updatePracticeText(QString::fromStdString(morseHandler->encodeText(problemText.toStdString())));
    } else if (mode == DECODE_SOUND) {
        emit updatePracticeText("");

        // Logic to delay sound playing if newly navigated to page or switched to audio mode
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
    // Reset state
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

        // Logic to delay sound playing if newly navigated to page or switched to audio mode
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

    // Update the morse preview, or the full input if the character is complete
    QString qmorse = QString::fromStdString(morse);
    if (qmorse == " " || qmorse == "/ ") {
        morseText += morsePreviewText + qmorse;
        emit updateInputText(morseText);
        morsePreviewText = "";
    } else {
        morsePreviewText += qmorse;
    }
    emit updateMorsePreviewText(morsePreviewText);

    // Logic for hard difficulty (handling multiple words)
    if (difficultyHandler->getDifficulty() == DifficultyHandler::difficulty::HARD) {
        if (qmorse == "/ " && hardWordCounter < 4) {
            hardWordCounter++;
        } else if (qmorse == "/ " && hardWordCounter >= 4) {
            checkProblem();
        }
    }
    // Medium and easy difficulty
    else if (qmorse == "/ ") {
        checkProblem();
    }

}

void PracticeHandler::setDifficulty(QString difficulty) {
    firstAudioPlay = true;
    difficultyHandler->setDifficulty(difficulty);
    score = 0;
    streak = 0;
    audioDelayTimer.stop();

    emit updateScore(QString::number(score));
    loadHighScore();
    loadPracticeProblem();
}

void PracticeHandler::setMode(QString newMode) {
    // State reset
    firstAudioPlay = true;
    audioDelayTimer.stop();
    score = 0;
    streak = 0;
    emit updateScore(QString::number(score));

    // Mode switching logic
    newMode = newMode.toLower();
    if (newMode == "encode english") {
        mode = ENCODE_ENGLISH;
    } else if (newMode == "decode morse") {
        mode = DECODE_MORSE;
    } else if (newMode == "decode sound") {
        mode = DECODE_SOUND;
        emit updateInputText("");
    }

    loadHighScore();
    loadPracticeProblem();
}

void PracticeHandler::checkProblem() {
    acceptingInput = false;

    // Correct answer
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
    // Incorrect answer
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
    int highScore = settings.value(key, 0).toInt();
    emit updateHighScore(QString::number(highScore));
}

bool PracticeHandler::isHighScore(int score) {
    QSettings settings("Tigers", "MorseCodeLearner");
    QString key = QString::number(difficultyHandler->getDifficulty()) + QString::number(mode);

    return score > settings.value(key, 0).toInt();
}

