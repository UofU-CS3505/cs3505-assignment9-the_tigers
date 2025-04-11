#ifndef SINEWAVEGENERATOR_H
#define SINEWAVEGENERATOR_H

#include <QIODevice>
#include <QObject>
#include <QAudioFormat>

/**
 * Creates a QIODevice that can be read from to get sine wave audio.
 *
 * @name Michael Timothy
 * @date 04/06/2025
 */
class SineWaveGenerator : public QIODevice
{
    Q_OBJECT
public:
    /**
     * Constructor for a sine wave generator.
     * Sets up a default QIODevice.
     */
    SineWaveGenerator(QAudioFormat format, QObject *parent = nullptr, float freq = 440, float volume = 1.0f);

    void start();

    void stop();

    void setFormat(const QAudioFormat &format);

    void setFrequency(float freq);

    void setVolume(float volumeValue);

    qint64 readData(char *data, qint64 maxlen) override;

    /**
     * Does nothing because the generator is read only.
     * Still needed for QAudioSink to recognize the SineWaveGenerator
     * as a valid QIODevice.
     */
    qint64 writeData(const char *, qint64) override;

    qint64 bytesAvailable() const override;
private:
    float volume = 1.0;

    QAudioFormat m_format;
    int m_sampleRate = 44100;
    float m_phase;
    float m_frequency;
    int m_bytesPerSample;
};

#endif // SINEWAVEGENERATOR_H
