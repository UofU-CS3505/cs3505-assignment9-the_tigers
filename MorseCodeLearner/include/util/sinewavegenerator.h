#ifndef SINEWAVEGENERATOR_H
#define SINEWAVEGENERATOR_H

#include <QIODevice>
#include <QObject>

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
    SineWaveGenerator(QObject *parent = nullptr);

    /**
     * Generates the sine wave and opens the QIODevice for reading.
     * @param freq The frequency of the wave.
     * @param durationMs The duration of the sine wave in milliseconds.
     * @param volume The scalar for volume, 0.0 to 1.0
     */
    void start(int freq, int durationMs, float volume);

    qint64 readData(char *data, qint64 maxlen) override;

    /**
     * Does nothing because the generator is read only.
     * Still needed for QAudioSink to recognize the SineWaveGenerator
     * as a valid QIODevice.
     */
    qint64 writeData(const char *, qint64) override;

    /**
     * @return Returns the number of bytes left to read in the buffer.
     */
    qint64 bytesAvailable() const override;
private:
    int m_sampleRate = 44100;
    int m_duration;
    int m_freq;
    qint64 m_pos;
    QByteArray m_data;
};

#endif // SINEWAVEGENERATOR_H
