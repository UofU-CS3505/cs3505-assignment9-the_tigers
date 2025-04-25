#include "translatehandler.h"

TranslateHandler::TranslateHandler(MorseHandler *morseHandler, KeyEventFilter *keyEventFilter, QObject *parent)
    : QObject{parent},
    morseHandler(morseHandler),
    keyEventFilter(keyEventFilter)
{
    // Key Event Connections
    QObject::connect(keyEventFilter, &KeyEventFilter::spacePressed, this, &TranslateHandler::handleSpacePressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::spaceReleased, this, &TranslateHandler::handleSpaceReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowPressed, this, &TranslateHandler::handleLeftArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::leftArrowReleased, this, &TranslateHandler::handleLeftArrowReleased);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowPressed, this, &TranslateHandler::handleRightArrowPressed);
    QObject::connect(keyEventFilter, &KeyEventFilter::rightArrowReleased, this, &TranslateHandler::handleRightArrowReleased);

    // Morse Handler Connections
    QObject::connect(morseHandler, &MorseHandler::decodedInput, this, &TranslateHandler::onMorseReceived);
    QObject::connect(morseHandler, &MorseHandler::playbackEnd, this, [=]() {emit setPlayButtonPaused();});

    // Light indicator handling
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOn, this, [=]() {emit turnLightIndicatorOn();});
    QObject::connect(morseHandler, &MorseHandler::lightIndicatorOff, this, [=]() {emit turnLightIndicatorOff();});
}

TranslateHandler::~TranslateHandler() {}

void TranslateHandler::onUserEnteredTranslate() {
    userUsingTranslator = true;

    currentInputText.clear();
    currentMorsePreview.clear();
    emit updateInputText(currentInputText);
    emit updateMorsePreview(currentMorsePreview);

    if (morseHandler->getDevice() == MorseHandler::STRAIGHT_KEY) {
        emit straightKeySelected();
    } else {
        emit paddleSelected();
    }
}

void TranslateHandler::onUserLeftTranslate() {
    currentInputText.clear();
    currentMorsePreview.clear();
    emit updateInputText(currentInputText);
    emit updateMorsePreview(currentMorsePreview);
    emit setPlayButtonPaused();
    morseHandler->stopPlayback();
    userUsingTranslator = false;
}

void TranslateHandler::onSwapTranslateMode() {
    morseHandler->stopTimers();

    if (mode == MORSE_TO_TEXT) {
        mode = TEXT_TO_MORSE;
        emit modeToEnglishToMorse();
    } else {
        mode = MORSE_TO_TEXT;
        emit modeToMorseToEnglish();
    }

    currentInputText = currentOutputText;
    currentMorsePreview.clear();
    emit updateInputText(currentInputText);
    emit updateMorsePreview(currentMorsePreview);
}

void TranslateHandler::onInputTextChanged(const std::string text) {
    currentInputText = text;
    if (mode == TEXT_TO_MORSE) {
        currentOutputText = morseHandler->encodeText(currentInputText);
    } else {
        currentOutputText = morseHandler->decodeMorse(currentInputText);
    }
    emit updateOutputText(currentOutputText);
}

void TranslateHandler::handleSpacePressed() {
    if (userUsingTranslator == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || mode == TEXT_TO_MORSE)
        return;
    morseHandler->straightKeyDown();
    emit straightKeyPressed();
}

void TranslateHandler::handleSpaceReleased() {
    if (userUsingTranslator == false || morseHandler->getDevice() != MorseHandler::STRAIGHT_KEY || mode == TEXT_TO_MORSE)
        return;
    morseHandler->straightKeyUp();
    emit straightKeyReleased();
}

void TranslateHandler::handleLeftArrowPressed() {
    if (userUsingTranslator == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || mode == TEXT_TO_MORSE)
        return;
    morseHandler->paddleDotDown();
    emit paddleLeftPressed();
}

void TranslateHandler::handleLeftArrowReleased() {
    if (userUsingTranslator == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || mode == TEXT_TO_MORSE)
        return;
    morseHandler->paddleDotUp();
    emit paddleLeftReleased();
}

void TranslateHandler::handleRightArrowPressed() {
    if (userUsingTranslator == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || mode == TEXT_TO_MORSE)
        return;
    morseHandler->paddleDashDown();
    emit paddleRightPressed();
}

void TranslateHandler::handleRightArrowReleased() {
    if (userUsingTranslator == false || morseHandler->getDevice() != MorseHandler::IAMBIC_PADDLE || mode == TEXT_TO_MORSE)
        return;
    morseHandler->paddleDashUp();
    emit paddleRightReleased();
}

void TranslateHandler::onMorseReceived(const std::string morse) {
    if (!userUsingTranslator)
        return;

    if (morse == "/ " || morse == " ") {
        currentInputText += currentMorsePreview + morse;
        emit updateInputText(currentInputText);
        currentMorsePreview.clear();
        emit updateMorsePreview(currentMorsePreview);
    } else {
        currentMorsePreview += morse;
        emit updateMorsePreview(currentMorsePreview);
    }
}

void TranslateHandler::onAudioPlayButtonClicked() {
    if (!morseHandler->getPlayback()) {
        morseHandler->stopPlayback();
        emit setPlayButtonPlaying();

        if (mode == TEXT_TO_MORSE) {
            morseHandler->playMorse(currentOutputText);
        } else {
            morseHandler->playMorse(currentInputText);
        }
    } else {
        morseHandler->stopPlayback();
        emit setPlayButtonPaused();
    }
}

void TranslateHandler::onClearInput() {
    currentInputText.clear();
    emit updateInputText(currentInputText);
    morseHandler->stopPlayback();
}
