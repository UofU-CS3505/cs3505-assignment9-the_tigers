#ifndef SINEWAVEGENERATOR_H
#define SINEWAVEGENERATOR_H

#include <QIODevice>
#include <QObject>

/**
 * Creates a QIODevice that can be read from to get sin wave audio.
 *
 * Uses code from two different projects:
 * https://forum.qt.io/topic/154815/creating-qiodevice-with-infinite-data
 * https://github.com/pbohun/sound-gen
 *
 * @name Michael Timothy
 * @date 04/06/2025
 */
class SineWaveGenerator : public QIODevice
{
    Q_OBJECT
public:
    SineWaveGenerator(QObject *parent = nullptr);

    void start(int freq, int durationMs);

    qint64 readData(char *data, qint64 maxlen) override;

    qint64 writeData(const char *, qint64) override;

    qint64 bytesAvailable() const override;
private:
    int m_sampleRate = 44100;
    int m_duration;
    int m_freq;
    qint64 m_pos;
    QByteArray m_data;
};

#endif // SINEWAVEGENERATOR_H
