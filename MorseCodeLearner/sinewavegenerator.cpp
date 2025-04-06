#include "sinewavegenerator.h"

SineWaveGenerator::SineWaveGenerator(QObject *parent) :
    QIODevice(parent)
    , m_pos(0)
{}

void SineWaveGenerator::start(int freq, int durationMs) {
    m_duration = durationMs;
    m_freq = freq;
    m_pos = 0;
    m_data.clear();

    int samples = (m_sampleRate * durationMs) / 1000;
    int fadeInSamples = m_sampleRate * 0.005;
    qreal fadeInStep = 1.0 / fadeInSamples;

    qreal phaseShift = M_PI / 2;

    for (int i = 0; i < samples; ++i) {
        qreal t = qreal(i) / m_sampleRate;
        qreal fadeFactor = 1.0;

        if (i < fadeInSamples) {
            fadeFactor = fadeInStep * i;
        }

        qint16 value = 32767 * fadeFactor * std::sin(2 * M_PI * freq * t + phaseShift);
        m_data.append(reinterpret_cast<const char*>(&value), sizeof(qint16));
    }

    open(QIODevice::ReadOnly);
    m_pos = 0;
}


qint64 SineWaveGenerator::readData(char *data, qint64 maxlen) {
    if (m_pos >= m_data.size())
        return 0;
    qint64 bytesToRead = qMin(maxlen, qint64(m_data.size() - m_pos));
    memcpy(data, m_data.constData() + m_pos, bytesToRead);
    m_pos += bytesToRead;
    return bytesToRead;
}

qint64 SineWaveGenerator::writeData(const char *, qint64) {
    return 0;
}

qint64 SineWaveGenerator::bytesAvailable() const {
    return m_data.size() - m_pos + QIODevice::bytesAvailable();
}


void SineWaveGenerator::fadeOut(int durationMs) {
    int fadeOutSamples = (m_sampleRate * durationMs) / 1000;

    for (int i = 0; i < fadeOutSamples; ++i) {
        float fadeFactor = 1.0f - (float(i) / fadeOutSamples);

        qreal t = qreal(m_pos) / m_sampleRate;
        qint16 value = 32767 * std::sin(2 * M_PI * m_freq * t) * fadeFactor;

        m_data.append(reinterpret_cast<const char*>(&value), sizeof(qint16));
        m_pos++;
    }
}
