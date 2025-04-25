#include "lessonhandler.h"
#include <QObject>
#include <QSettings>

LessonHandler::LessonHandler(MorseHandler *morseHandler, KeyEventFilter *keyEventFilter, QObject* parent) :
    QObject(parent),
    morseHandler(morseHandler),
    keyEventFilter(keyEventFilter),
    acceptingInput(false),
    userOnThisPage(false)
{
    QTimer timer(this);
    wordCounter = 0;
    lessonProgress = 0;
    currentIndex = 0;

    // Key Event Filters
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, this, &LessonHandler::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, this, &LessonHandler::handleSpaceReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowPressed, this, &LessonHandler::handleLeftArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowReleased, this, &LessonHandler::handleLeftArrowReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowPressed, this, &LessonHandler::handleRightArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowReleased, this, &LessonHandler::handleRightArrowReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::enterPressed, this, [=]() {emit getInputTextSignal();});

    // Morse Handler
    QObject::connect(morseHandler, &MorseHandler::decodedInput, this, &LessonHandler::onInputReceived);
    QObject::connect(morseHandler, &MorseHandler::playbackEnd, this, [=]() {emit soundNotPlaying();});
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOn, this, [=]() {emit setLightIndicator(true);});
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOff, this, [=]() {emit setLightIndicator(false);});
}

void LessonHandler::nextQuestion() {
    acceptingInput = true;
    int charactersLearned = 0;
    wordCounter = 0;

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

    pickQuestionType();
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
        case 9:
            currentLessonCharacters = lessonNineWords;
            break;
        default:
            return;
    }

    currentIndex = 0;

    encodeQuestionTimes = currentLessonCharacters.size();
    decodeQuestionTimes = currentLessonCharacters.size();
    soundQuestionTimes = currentLessonCharacters.size();

    emit updateLessonTitle(currentLessonNumber);

    std::string referenceText = "";
    for (const std::string &character : currentLessonCharacters) {
        learnedCharacters[character] = 0;
        referenceText.append(character + ": " + morseHandler->encodeText(character) + "\n");
    }

    emit setReferenceText(QString::fromStdString(referenceText));
    nextQuestion();
}

void LessonHandler::checkUserGuess() {
    acceptingInput = false;
    std::string guess = inputText.toStdString();
    std::string correctAnswer = currentQuestion;

    if (type == ENCODE_ENGLISH) {
        guess = morseHandler->decodeMorse(guess);
    }

    if (guess == correctAnswer || guess == correctAnswer + " ") {
        learnedCharacters[currentQuestion] += 1;
        emit guessCorrect();
        inputText = "";
        emit updateInputText(inputText);
    } else {
        emit guessIncorrect();
        inputText = "";
        emit updateInputText(inputText);

        if (type == ENCODE_ENGLISH) {
            timer.singleShot(1500, this, [this, correctAnswer](){emit displayCorrectAnswer(QString::fromStdString(morseHandler->encodeText(correctAnswer)));});
        } else {
            timer.singleShot(1500, this, [this, correctAnswer](){emit displayCorrectAnswer(QString::fromStdString(correctAnswer));});
        }
    }

    int correctCount = 0;
    for (const std::string &character : currentLessonCharacters) {
        correctCount += learnedCharacters[character];
    }

    lessonProgress = ((float)correctCount / (float)(currentLessonCharacters.size() * 3)) * 100;
    emit updateLessonProgressBar(lessonProgress);

    timer.singleShot(3000, this, [=]() {nextQuestion();});
}

void LessonHandler::lessonComplete() {
    acceptingInput = false;

    QSettings settings("Tigers", "MorseCodeLearner");
    settings.setValue("lesson" + std::to_string(currentLessonNumber) + "Completed", true);

    emit displayTextToUI("You have completed this lesson!");

    timer.singleShot(1500, this, [=](){emit completedLesson();});
}

void LessonHandler::pickQuestionType() {
    int sw = learnedCharacters[currentQuestion];

    switch (sw) {
        case 0:
            type = ENCODE_ENGLISH;
            emit isAudioDecodeMode(false);
            emit isInputReadOnly(true);
            emit displayTextToUI("What is '" + QString::fromStdString(currentQuestion) + "' in morse?");
            break;
        case 1:
            type = DECODE_MORSE;
            emit isAudioDecodeMode(false);
            emit isInputReadOnly(false);
            emit focusInput();
            emit displayTextToUI("What is '" + QString::fromStdString(morseHandler->encodeText(currentQuestion)).chopped(1) + "' in English?");
            break;
        case 2:
            type = DECODE_SOUND;
            emit isAudioDecodeMode(true);
            emit displayTextToUI("");
            emit isInputReadOnly(false);
            emit focusInput();
            emit soundPlaying();
            morseHandler->playMorse(morseHandler->encodeText(currentQuestion));
            break;
    }
}

void LessonHandler::onInputReceived(const std::string input) {
    if (!userOnThisPage)
        return;
    if (acceptingInput && type == ENCODE_ENGLISH) {
        QString qmorse = QString::fromStdString(input);

        if (currentLessonNumber == 9) {
            if (qmorse == "/ " && wordCounter < 2) {
                wordCounter++;
                inputText += qmorse;
                emit updateInputText(inputText);
            } else {
                inputText += qmorse;
                emit updateInputText(inputText);
            }

            if (qmorse == "/ " && wordCounter >= 2) {
                checkUserGuess();
            }
        } else {
            if (qmorse == "/ ") {
                checkUserGuess();
            } else {
                inputText += qmorse;
                emit updateInputText(inputText);
            }
        }
    } else {
        inputText = QString::fromStdString(input);
    }
}

void LessonHandler::handleSpacePressed() {
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput || currentIndex == 0 || type != ENCODE_ENGLISH)
        return;
    morseHandler->straightKeyDown();
    emit straightKeyPressed();
}

void LessonHandler::handleSpaceReleased() {
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || !acceptingInput || currentIndex == 0  || type != ENCODE_ENGLISH)
        return;
    morseHandler->straightKeyUp();
    emit straightKeyReleased();
}

void LessonHandler::handleLeftArrowPressed() {
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || !acceptingInput || currentIndex == 0  || type != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDotDown();
    emit paddleLeftPressed();
}

void LessonHandler::handleLeftArrowReleased() {
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || !acceptingInput || currentIndex == 0  || type != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDotUp();
    emit paddleLeftReleased();
}

void LessonHandler::handleRightArrowPressed() {
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || !acceptingInput || currentIndex == 0  || type != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDashDown();
    emit paddleRightPressed();
}

void LessonHandler::handleRightArrowReleased() {
    if (!userOnThisPage || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || !acceptingInput || currentIndex == 0  || type != ENCODE_ENGLISH)
        return;
    morseHandler->paddleDashUp();
    emit paddleRightReleased();
}

void LessonHandler::handleEnterPressed() {
    if (!userOnThisPage || !acceptingInput || currentIndex == 0  || type == ENCODE_ENGLISH)
        return;
    checkUserGuess();
}

void LessonHandler::onBackButtonClicked() {
    acceptingInput = false;
    userOnThisPage = false;
    inputText = "";
    lessonProgress = 0;
    emit updateLessonProgressBar(lessonProgress);
}

void LessonHandler::setUserOnThisPage(bool userOnThisPage) {
    this->userOnThisPage = userOnThisPage;

    if (userOnThisPage) {
        if (morseHandler->getDevice() == MorseHandler::STRAIGHT_KEY) {
            emit straightKeySelected();
        } else {
            emit paddleSelected();
        }
    }
}

void LessonHandler::setCurrentIndex(int currentIndex) {
    this->currentIndex = currentIndex;
}

void LessonHandler::getInputText(QString inputText) {
    onInputReceived(inputText.toStdString());
    handleEnterPressed();
}
