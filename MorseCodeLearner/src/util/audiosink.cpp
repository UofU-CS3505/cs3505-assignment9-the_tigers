#include "audiosink.h"

AudioSink::AudioSink() {
#ifdef _WIN32
    impl = new WasapiAudioSink();
#elif defined(__APPLE__)
    impl = new CoreAudioSink();
#endif
}

AudioSink::~AudioSink() {
    delete impl;
}

void AudioSink::start() {
    impl->start();
}

void AudioSink::stop() {
    impl->stop();
}

void AudioSink::suspend() {
    impl->suspend();
}

void AudioSink::resume() {
    impl->resume();
}

void AudioSink::setVolume(float v) {
    impl->setVolume(v);
}

void AudioSink::writeAudioData(const float *data, int numFrames) {
    impl->writeAudioData(data, numFrames);
}
