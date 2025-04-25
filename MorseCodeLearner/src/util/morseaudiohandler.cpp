#include "morseaudiohandler.h"

MorseAudioHandler::MorseAudioHandler(float unit) : unit(unit) {
    frequency = 440;

    audio = new AudioSink();
    sineGenerator = new SineWaveGenerator(frequency, sampleRate);
    audio->start();

    playingPlayback = false;
    outputBuffer = "";

    audioBufferTimer = new QTimer(this);

    QObject::connect(audioBufferTimer, &QTimer::timeout, this, [=]() {
        float buffer[960];
        sineGenerator->generate(buffer, 480);
        int written = audio->writeAudioData(buffer, 480);

        sineGenerator->revertPhase(480 - written);
    });

    audioBufferTimer->start(10);
}

MorseAudioHandler::~MorseAudioHandler() {
    delete audio;
    delete sineGenerator;
}

void MorseAudioHandler::setWpm(float wpm) {
    unit = 1200 / wpm;
}

void MorseAudioHandler::setVolume(int volumeValue) {
    audio->setVolume((float)volumeValue / 100);
}

void MorseAudioHandler::start() {

    audio->resume();
    emit lightIndicatorOn();
}

void MorseAudioHandler::stop() {
    playingPlayback = false;
    outputBuffer = "";
    audio->suspend();
    emit lightIndicatorOff();
}

void MorseAudioHandler::playMorse(string morse) {
    outputBuffer = morse;
    playingPlayback = true;
    playBuffer();
}

void MorseAudioHandler::suspend() {
    audio->suspend();

    emit lightIndicatorOff();
}

bool MorseAudioHandler::getPlayback() {
    return playingPlayback;
}

void MorseAudioHandler::playBuffer() {
    if (outputBuffer.empty()) {
        playingPlayback = false;
        emit playbackEnd();
        return;
    }

    if (!playingPlayback) {
        outputBuffer = "";
        return;
    }
    if (outputBuffer.at(0) == '-') {
        start();
        stopTimer.singleShot(unit * 3, this, &MorseAudioHandler::suspend);
        gapTimer.singleShot(unit * 4, this, &MorseAudioHandler::playBuffer);
    } else if (outputBuffer.at(0) == '.') {
        start();
        stopTimer.singleShot(unit, this, &MorseAudioHandler::suspend);
        gapTimer.singleShot(unit * 2, this, &MorseAudioHandler::playBuffer);
    } else if (outputBuffer.at(0) == ' ' || outputBuffer.at(0) == '/') {
        gapTimer.singleShot(unit * 2, this, &MorseAudioHandler::playBuffer); // A slash is between 2 spaces so 1(char gap) + 2(space gap) + 2(slash gap) + 2(space gap) = 7 units
    } else {
        outputBuffer = outputBuffer.substr(1);
        playBuffer();
        return;
    }
    outputBuffer = outputBuffer.substr(1);
}
