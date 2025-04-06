#include "morsehandler.h"
#include <QDebug>

MorseHandler::MorseHandler(int wpm) : wpm(wpm), unit(1200 / wpm) {
    charGapTimer.setSingleShot(true);
    wordGapTimer.setSingleShot(true);
    connect(&charGapTimer, &QTimer::timeout, this, &MorseHandler::onCharGapTimeout);
    connect(&wordGapTimer, &QTimer::timeout, this, &MorseHandler::onWordGapTimeout);
    for (const auto& pair : encodings) {
        reverseEncodings[pair.second] = pair.first;
    };
}

void MorseHandler::straightKeyDown() {
    keyDownTimer.start();
    charGapTimer.stop();
}

void MorseHandler::straightKeyUp() {
    qint64 duration = keyDownTimer.elapsed();

    if (duration < 1.5 * unit) {
        emit decodedInput(".");
    } else {
        emit decodedInput("-");
    }

    charGapTimer.start(3 * unit);
    wordGapTimer.start(7 * unit);

}

void MorseHandler::paddleDotDown() {
    charGapTimer.stop();
    wordGapTimer.stop();
    paddleDotIsDown = true;
    currentPaddleInput = DOT;
    signalPaddleDot();
}

void MorseHandler::paddleDotUp() {
    paddleDotIsDown = false;
    if (!paddleDashIsDown) {
        charGapTimer.start(3 * unit);
        wordGapTimer.start(7 * unit);
    } else {
        currentPaddleInput = DASH;
    }
}

void MorseHandler::paddleDashDown() {
    charGapTimer.stop();
    wordGapTimer.stop();
    paddleDashIsDown = true;
    currentPaddleInput = DASH;
    signalPaddleDash();
}

void MorseHandler::paddleDashUp() {
    paddleDashIsDown = false;
    if (!paddleDotIsDown) {
        charGapTimer.start(3 * unit);
        wordGapTimer.start(7 * unit);
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

void MorseHandler::onCharGapTimeout() {
    emit decodedInput(" ");
}

void MorseHandler::onWordGapTimeout() {
    emit decodedInput("/ ");
}

void MorseHandler::stopTimers() {
    paddleDotIsDown = false;
    paddleDashIsDown = false;
    currentPaddleInput = EMPTY;
    wordGapTimer.stop();
    charGapTimer.stop();
}

void MorseHandler::setWpm(float newWpm) {
    wpm = newWpm;
    unit = 1200 / wpm;
}

string MorseHandler::encodeText(const string text) {
    string encodedText = "";
    for (char c : text) {
        if (c == ' ') {
            encodedText += " / ";
        } else {
            if (encodings.count(c) == 0) {
                encodedText += "?";
            } else {
                encodedText += encodings.at(c) + " ";
            }
        }
    }
    return encodedText;
}

string MorseHandler::decodeMorse(const string morse) {
    std::stringstream ss(morse);
    string token;
    string decoded;
    while (ss >> token) {
        if (token == "/") {
            decoded += " ";
        } else {
            if (reverseEncodings.count(token) != 0) {
                decoded += reverseEncodings.at(token);
            }
        }
    }
    return decoded;
}

float MorseHandler::getUnitTime() {
    return unit;
}


