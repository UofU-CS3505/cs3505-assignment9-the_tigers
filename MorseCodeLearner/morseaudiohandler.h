#ifndef MORSEAUDIOHANDLER_H
#define MORSEAUDIOHANDLER_H

#include <QObject>
#include <QTimer>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QAudioDevice>
#include <QAudioSink>
#include "qwidget.h"
#include "sinewavegenerator.h"

using std::string;

class MorseAudioHandler : public QWidget
{
    Q_OBJECT
private:
    SineWaveGenerator *sineGenerator;
    QAudioSink *audio;
    QAudioFormat format;

    QTimer stopTimer;
    QTimer gapTimer;

    string outputBuffer;

    bool playingPlayback;

    int frequency;
    float unit;

public:
    MorseAudioHandler(QWidget *parent = nullptr, float unit = 120); // 120 is 10 wpm
    ~MorseAudioHandler();

    void setMorseUnit(float unit);

    void start();
    void stop();
    void suspend();

    bool getPlayback();

    void playMorse(string morse);

private slots:
    void onAudioStateChanged();

protected:

    void playBuffer();

signals:
    void playbackEnd();
};

#endif // MORSEAUDIOHANDLER_H
