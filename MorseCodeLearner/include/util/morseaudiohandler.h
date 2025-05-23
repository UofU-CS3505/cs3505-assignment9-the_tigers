#ifndef MORSEAUDIOHANDLER_H
#define MORSEAUDIOHANDLER_H

#include <QObject>
#include <QTimer>
#include "audiosink.h"
#include "sinewavegenerator.h"

using std::string;

/**
 * Handles all morse code audio using a custom audio sink
 * and a sine wave generator.
 *
 * @name Michael Timothy
 * @date 04/06/2025
 */
class MorseAudioHandler : public QObject
{
    Q_OBJECT
private:
    SineWaveGenerator *sineGenerator;
    AudioSink *audio;
    QTimer stopTimer;
    QTimer gapTimer;

    string outputBuffer;

    bool playingPlayback;

    int frequency; // Could be customized, for now we'll leave at 440 hz
    float unit;

    int sampleRate = 48000; // 48kHz
    const int framesPerBatch = 480; // ~10ms

    QTimer *audioBufferTimer;

public:
    /**
     * Constructor for MorseAudioHandler. Defaults to 10 wpm timings.
     */
    MorseAudioHandler(float unit = 120); // 120 is 10 wpm
    ~MorseAudioHandler();

    /**
     * Sets the unit time. Conversion to unit is WPM / 1200
     */
    void setWpm(float wpm);

    /**
     * Sets the volume of the audio sink.
     */
    void setVolume(signed int volumeValue);

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

    /**
     * Sent to turn on a light indicator when audio has started playing.
     */
    void lightIndicatorOn();

    /**
     * Sent to turn off a light indicator when audio has stopped playing.
     */
    void lightIndicatorOff();
};

#endif // MORSEAUDIOHANDLER_H
