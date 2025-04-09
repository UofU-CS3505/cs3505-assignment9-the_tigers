#include "morseaudiohandler.h"
#include <QMediaDevices>

MorseAudioHandler::MorseAudioHandler(QWidget *parent, float unit) : QWidget(parent), unit(unit) {

    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);
    format.setChannelConfig(QAudioFormat::ChannelConfigMono);

    outputDevice = QMediaDevices::defaultAudioOutput();
    audio = new QAudioSink(outputDevice, format, this);
    sineGenerator = new SineWaveGenerator(this);

    playingPlayback = false;
    outputBuffer = "";

    frequency = 440;

    QObject::connect(audio, &QAudioSink::stateChanged, this, &MorseAudioHandler::onAudioStateChanged);

}

MorseAudioHandler::~MorseAudioHandler() {
    delete audio;
    delete sineGenerator;
}

void MorseAudioHandler::setWpm(float wpm) {
    unit = 1200 / wpm;
}

void MorseAudioHandler::setVolume(int volumeValue) {
    volume = volumeValue / qreal(100);
    if (sineGenerator->openMode()) {
        delete sineGenerator;
        sineGenerator = new SineWaveGenerator();
        delete audio;
        audio = new QAudioSink(format);
    }
}

void MorseAudioHandler::onAudioStateChanged() {
    if (audio->state() == QAudio::IdleState) {
        sineGenerator->start(frequency, 30000, volume);
        audio->start(sineGenerator);
    }
}

void MorseAudioHandler::start() {
    if (sineGenerator->bytesAvailable() == 0) {
        sineGenerator->start(frequency, 30000, volume);
        audio->start(sineGenerator);
    } else {
        audio->resume();
    }
}

void MorseAudioHandler::stop() {
    playingPlayback = false;
    outputBuffer = "";
    audio->suspend();
}

void MorseAudioHandler::playMorse(string morse) {
    outputBuffer = morse;
    playingPlayback = true;
    playBuffer();
}

void MorseAudioHandler::suspend() {
    audio->suspend();
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
