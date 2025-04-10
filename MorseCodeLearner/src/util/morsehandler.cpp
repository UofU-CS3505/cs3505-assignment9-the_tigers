#include "morsehandler.h"
#include <QDebug>
#include <sstream>

MorseHandler::MorseHandler(device input, int wpm) : unit(1200 / wpm), inputDevice(input) {
    charGapTimer.setSingleShot(true);
    wordGapTimer.setSingleShot(true);
    suspendAudioTimer.setSingleShot(true);

    connect(&charGapTimer, &QTimer::timeout, this, &MorseHandler::onCharGapTimeout);
    connect(&wordGapTimer, &QTimer::timeout, this, &MorseHandler::onWordGapTimeout);
    connect(&suspendAudioTimer, &QTimer::timeout, &audioHandler, &MorseAudioHandler::suspend);

    connect(&audioHandler, &MorseAudioHandler::playbackEnd, this, [=]() {emit playbackEnd();});
    connect(&audioHandler, &MorseAudioHandler::lightIndicatorOn, this, [=]() {emit lightIndicatorOn();});
    connect(&audioHandler, &MorseAudioHandler::lightIndicatorOff, this, [=](){emit lightIndicatorOff();});

    audioHandler.setWpm(wpm);

    for (const auto& pair : encodings) {
        reverseEncodings[pair.second] = pair.first;
    };
}

void MorseHandler::setDevice(device input) {
    inputDevice = input;
}

MorseHandler::device MorseHandler::getDevice() {
    return inputDevice;
}

void MorseHandler::straightKeyDown() {
    if (audioHandler.getPlayback())
        return;
    audioHandler.start();
    keyDownTimer.start();
    charGapTimer.stop();
    wordGapTimer.stop();
}

void MorseHandler::straightKeyUp() {
    if (audioHandler.getPlayback())
        return;
    audioHandler.suspend();
    qint64 duration = keyDownTimer.elapsed();

    if (duration < 1.5 * unit) { // User needs to be close enough to the dot timing
        emit decodedInput(".");
    } else {
        emit decodedInput("-");
    }

    charGapTimer.start(3 * unit);
    wordGapTimer.start(7 * unit);

}

void MorseHandler::paddleDotDown() {
    if (audioHandler.getPlayback())
        return;
    charGapTimer.stop();
    wordGapTimer.stop();
    paddleDotIsDown = true;
    currentPaddleInput = DOT;
    signalPaddleDot();
}

void MorseHandler::paddleDotUp() {
    if (audioHandler.getPlayback())
        return;
    paddleDotTimer.stop();
    paddleDotIsDown = false;
    if (!paddleDashIsDown) {
        charGapTimer.start(3 * unit);
        wordGapTimer.start(7 * unit);
    } else {
        currentPaddleInput = DASH;
        signalPaddleDash();
    }
}

void MorseHandler::paddleDashDown() {
    if (audioHandler.getPlayback())
        return;
    charGapTimer.stop();
    wordGapTimer.stop();
    paddleDashIsDown = true;
    currentPaddleInput = DASH;
    signalPaddleDash();
}

void MorseHandler::paddleDashUp() {
    if (audioHandler.getPlayback())
        return;
    paddleDashTimer.stop();
    paddleDashIsDown = false;
    if (!paddleDotIsDown) {
        charGapTimer.start(3 * unit);
        wordGapTimer.start(7 * unit);
    } else {
        currentPaddleInput = DOT;
        signalPaddleDot();
    }
}

void MorseHandler::signalPaddleDot() {
    if (!paddleDotIsDown || currentPaddleInput != DOT)
        return;
    audioHandler.start();
    emit decodedInput(".");
    suspendAudioTimer.start(unit);
    paddleDotTimer.singleShot(unit * 2, this, &MorseHandler::signalPaddleDot);
}

void MorseHandler::signalPaddleDash() {
    if (!paddleDashIsDown || currentPaddleInput != DASH)
        return;
    audioHandler.start();
    emit decodedInput("-");
    suspendAudioTimer.start(unit * 3);
    paddleDashTimer.singleShot(unit * 4, this, &MorseHandler::signalPaddleDash);
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

void MorseHandler::setWpm(float wpm) {
    unit = 1200 / wpm;
    audioHandler.setWpm(wpm);
}

string MorseHandler::encodeText(const string text) {
    string encodedText = "";
    for (char c : text) {
        c = std::tolower(c);
        if (c == ' ') {
            encodedText += " / ";
        } else {
            if (encodings.count(c) == 0) {  // Unknown chars are encoded as "?"
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

bool MorseHandler::getPlayback() {
    return audioHandler.getPlayback();
}

void MorseHandler::stopPlayback() {
    audioHandler.stop();
}

void MorseHandler::playMorse(string morse) {
    audioHandler.playMorse(morse);
}

void MorseHandler::setVolume(int volumeValue) {
    audioHandler.setVolume(volumeValue);
}
