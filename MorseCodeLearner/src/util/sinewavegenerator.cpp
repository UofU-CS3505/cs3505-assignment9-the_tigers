#include "sinewavegenerator.h"
#include "qdebug.h"

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

    QVector<qint16> samples;

    qint64 totalSamples = maxlen / bytesPerSample;

    for (qint64 i = 0; i < totalSamples; ++i) {
        float sample = std::sin(m_phase * 2 * M_PI) * volume;
        std::clamp(sample, -1.0f, 1.0f);

        // Assumes 16 bit format
        qint16 value = static_cast<qint16>(sample * 32767);

        for (int ch = 0; ch < channelCount; ++ch) {
            memcpy(data, &value, sizeof(qint16));
            data += sizeof(qint16);
            samples.append(value);
        }

        m_phase += m_frequency / sampleRate;
        if (m_phase >= 1.0)
            m_phase -= 1.0;
    }

    QMutexLocker locker(&bufferMutex);
    for (int i = 0; i < samples.count(); ++i) {
        sampleBuffer.append(samples[i]);
    }

    if (sampleBuffer.size() > MaxBufferSamples) {
        sampleBuffer = sampleBuffer.mid(sampleBuffer.size() - MaxBufferSamples);
    }


    return totalSamples * bytesPerSample;
}

QVector<qint16> SineWaveGenerator::getMostRecentSamples(int count) const {
    QMutexLocker locker(&bufferMutex);
    if (sampleBuffer.size() < count)
        return sampleBuffer;
    return sampleBuffer.mid(sampleBuffer.size() - count, count);
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
