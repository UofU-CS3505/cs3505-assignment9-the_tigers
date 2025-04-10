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
    if (audioHandler.getPlayback()) return;
    charGapTimer.stop();
    wordGapTimer.stop();
    paddleDotIsDown = true;

    if (!symbolInProgress) {
        currentPaddleInput = DOT;
        playSymbol(DOT);
    } else {
        pendingSymbol = DOT;
    }
}

void MorseHandler::paddleDotUp() {
    paddleDotIsDown = false;
    if (!paddleDashIsDown && !symbolInProgress) {
        charGapTimer.start(3 * unit);
        wordGapTimer.start(7 * unit);
    }
}

void MorseHandler::paddleDashDown() {
    if (audioHandler.getPlayback()) return;
    charGapTimer.stop();
    wordGapTimer.stop();
    paddleDashIsDown = true;

    if (!symbolInProgress) {
        currentPaddleInput = DASH;
        playSymbol(DASH);
    } else {
        pendingSymbol = DASH;
    }
}

void MorseHandler::paddleDashUp() {
    paddleDashIsDown = false;
    if (!paddleDotIsDown && !symbolInProgress) {
        charGapTimer.start(3 * unit);
        wordGapTimer.start(7 * unit);
    }
}

void MorseHandler::playSymbol(morseChar symbol) {
    symbolInProgress = true;
    currentPaddleInput = symbol;

    if (symbol == DOT) {
        audioHandler.start();
        emit decodedInput(".");
        suspendAudioTimer.start(unit);
        QTimer::singleShot(unit * 2, this, &MorseHandler::onSymbolComplete);
    } else {
        audioHandler.start();
        emit decodedInput("-");
        suspendAudioTimer.start(unit * 3);
        QTimer::singleShot(unit * 4, this, &MorseHandler::onSymbolComplete);
    }
}

void MorseHandler::onSymbolComplete() {
    symbolInProgress = false;

    if (paddleDotIsDown && paddleDashIsDown) {
        currentPaddleInput = (currentPaddleInput == DOT) ? DASH : DOT;
        playSymbol(currentPaddleInput);
    } else if (pendingSymbol.has_value()) {
        playSymbol(pendingSymbol.value());
        pendingSymbol.reset();
    } else if (paddleDotIsDown) {
        playSymbol(DOT);
    } else if (paddleDashIsDown) {
        playSymbol(DASH);
    } else {
        charGapTimer.start(3 * unit);
        wordGapTimer.start(7 * unit);
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
