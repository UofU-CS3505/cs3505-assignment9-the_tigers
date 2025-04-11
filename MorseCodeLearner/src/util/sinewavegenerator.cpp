#include "sinewavegenerator.h"

SineWaveGenerator::SineWaveGenerator(QAudioFormat format, QObject *parent, float frequency, float volume) :
    QIODevice(parent),
    volume(volume),
    m_format(format),
    m_phase(0.0f),
    m_frequency(frequency)
{
    m_sampleRate = format.sampleRate();
    m_bytesPerSample = format.bytesPerFrame() / format.channelCount();
}

void SineWaveGenerator::start() {
    open(QIODevice::ReadOnly);
}

void SineWaveGenerator::stop() {
    close();
}

void SineWaveGenerator::setFormat(const QAudioFormat &format) {
    m_format = format;
    m_sampleRate = format.sampleRate();
    m_bytesPerSample = format.bytesPerFrame() / format.channelCount();
}

qint64 SineWaveGenerator::readData(char *data, qint64 maxlen) {
    const int bytesPerSample = m_bytesPerSample;
    const int channelCount = m_format.channelCount();
    const int sampleRate = m_sampleRate;

    qint64 totalSamples = maxlen / bytesPerSample;

    for (qint64 i = 0; i < totalSamples; ++i) {
        float sample = std::sin(m_phase * 2 * M_PI) * volume;

        // Assumes 16 bit format
        qint16 value = static_cast<qint16>(sample * 32767);

        for (int ch = 0; ch < channelCount; ++ch) {
            memcpy(data, &value, sizeof(qint16));
            data += sizeof(qint16);
        }

        m_phase += m_frequency / sampleRate;
        if (m_phase >= 1.0)
            m_phase -= 1.0;
    }

    return totalSamples * bytesPerSample;
}

void SineWaveGenerator::setFrequency(float freq) {
    m_frequency = freq;
}

void SineWaveGenerator::setVolume(float volumeValue) {
    volume = volumeValue;
}

qint64 SineWaveGenerator::writeData(const char *, qint64) {
    return 0;
}

qint64 SineWaveGenerator::bytesAvailable() const {
    return 4096 + QIODevice::bytesAvailable();
}
