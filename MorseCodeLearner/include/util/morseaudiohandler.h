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

/**
 * Handles all morse code audio using QAudioSink
 * and a sine wave generator.
 *
 * @name Michael Timothy
 * @date 04/06/2025
 */
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
    /**
     * Constructor for MorseAudioHandler. Defaults to 10 wpm timings.
     */
    MorseAudioHandler(QWidget *parent = nullptr, float unit = 120); // 120 is 10 wpm
    ~MorseAudioHandler();

    void setWpm(float wpm);

    /**
     * @brief Starts playing an unending sine wave.
     *
     * If a sine wave is ready to be read from, this resumes the audio sink.
     * If not, it generates the sine wave and starts the audio sink.
     */
    void start();

    /**
     * Stops any active audio as well as erases the morse output buffer.
     */
    void stop();

    /**
     * Pauses active audio.
     */
    void suspend();

    /**
     * @return Whether or not playback of a morse buffer is currently being played.
     */
    bool getPlayback();

    /**
     * Plays morse audio at the speed of the current wpm as long as wpm has been correctly updated.
     * If a character in the provided string is not a morse character (".- /"), it skips it.
     *
     * A playbackEnd() signal is sent when the playback finishes.
     *
     * @param morse The string to play.
     */
    void playMorse(string morse);

private slots:
    /**
     * Handler for when the audio object goes into an idle state, which means the
     * SineWaveGenerator's buffer has been fully read.
     * Regenerates the sine wave and starts playing the audio again.
     * As of right now, this causes some audio clipping, but only once every 30 seconds.
     */
    void onAudioStateChanged();

protected:
    /**
     * Helper method to play out the current morse buffer from playMorse.
     */
    void playBuffer();

signals:
    /**
     * Sent when a buffer has no more morse to read/play.
     */
    void playbackEnd();
};

#endif // MORSEAUDIOHANDLER_H
