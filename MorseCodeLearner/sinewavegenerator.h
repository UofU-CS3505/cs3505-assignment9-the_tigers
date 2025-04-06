#ifndef SINEWAVEGENERATOR_H
#define SINEWAVEGENERATOR_H

#include <QIODevice>
#include <QObject>

class SineWaveGenerator : public QIODevice
{
    Q_OBJECT
public:
    SineWaveGenerator(QObject *parent = nullptr);

    void start(int freq, int durationMs);

    qint64 readData(char *data, qint64 maxlen) override;

    qint64 writeData(const char *, qint64) override;

    qint64 bytesAvailable() const override;

    void fadeOut(int durationMs);
private:
    int m_sampleRate = 44100;
    int m_duration;
    int m_freq;
    qint64 m_pos;
    QByteArray m_data;
};

#endif // SINEWAVEGENERATOR_H
