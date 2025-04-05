#include "morsehandler.h"

MorseHandler::MorseHandler(int wpm) : wpm(wpm), unit(1200 / wpm) {
    gapTimer.setSingleShot(true);
    connect(&gapTimer, &QTimer::timeout, this, &MorseHandler::onGapTimeout);
}

void MorseHandler::straightKeyDown() {
    keyDownTimer.start();
    gapTimer.stop();
}

void MorseHandler::straightKeyUp() {
    qint64 duration = keyDownTimer.elapsed();

    if (duration < 1.5 * unit) {
        emit decodedInput(".");
    } else {
        emit decodedInput("-");
    }

    gapTimer.start(3 * unit);
}

void MorseHandler::paddleDotDown() {
    paddleDotIsDown = true;
    currentPaddleInput = DOT;
    signalPaddleDot();
}

void MorseHandler::paddleDotUp() {
    paddleDotIsDown = false;
    if (!paddleDashIsDown) {
        gapTimer.start(3 * unit);
    } else {
        currentPaddleInput = DASH;
    }
}

void MorseHandler::paddleDashDown() {
    paddleDashIsDown = true;
    currentPaddleInput = DASH;
    signalPaddleDash();
}

void MorseHandler::paddleDashUp() {
    paddleDashIsDown = false;
    if (!paddleDotIsDown) {
        gapTimer.start(3 * unit);
    } else {
        currentPaddleInput = DOT;
    }
}

void MorseHandler::signalPaddleDot() {
    if (!paddleDotIsDown || currentPaddleInput != DOT)
        return;
    emit decodedInput(".");
    paddleDotTimer.singleShot(unit, this, &MorseHandler::signalPaddleDot);
}

void MorseHandler::signalPaddleDash() {
    if (!paddleDashIsDown || currentPaddleInput != DASH)
        return;
    emit decodedInput("-");
    paddleDashTimer.singleShot(unit, this, &MorseHandler::signalPaddleDash);
}

void MorseHandler::onGapTimeout() {
    emit decodedInput(" ");
}

void MorseHandler::stopTimers() {
    paddleDotIsDown = false;
    paddleDashIsDown = false;
    currentPaddleInput = EMPTY;
    gapTimer.stop();
}

void MorseHandler::setWpm(float newWpm) {
    wpm = newWpm;
    unit = 1200 / wpm;
}

string MorseHandler::encodeText(string text) {
    string encodedText = "";
    for (char c : text) {
        if (c == ' ') {
            encodedText += " ";
        } else {
            if (encodings.count(c) != 0) {
                // Possibly send a signal to display an error message? Or we could check
                // if the message is valid in the view but that'd prolly break MVC.
            } else {
                encodedText += encodings.at(c);
            }
        }
    }
    return encodedText;
}


