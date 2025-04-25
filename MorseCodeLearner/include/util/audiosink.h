#ifndef AUDIOSINK_H
#define AUDIOSINK_H

#include "crossplatformaudiosink.h"
#include "crossplatformaudiosink.cpp"

/**
 * Wrapper class for a CrossPlatformAudioSink
 *
 * @name Michael Timothy
 * @date 4/21/2025
 */
class AudioSink : public CrossPlatformAudioSink
{
private:
    CrossPlatformAudioSink *impl;
public:
    /**
     * Creates either a CoreAudio sink
     * or a Wasapi sink depending on whether
     * the user is using a macOS or Windows computer respectively.
     */
    AudioSink();

    ~AudioSink();

    /**
     * Initializes the sink and starts
     * playing back the ring buffer.
     */
    void start() override;

    /**
     * Stops any audio playback and
     * uninitializes the sink.
     */
    void stop() override;

    /**
     * Suspends all audio playback.
     */
    void suspend() override;

    /**
     * Resumes playback of the buffer.
     */
    void resume() override;

    /**
     * Sets the volume of the sink
     * @param v Float from 0 to 1.
     */
    void setVolume(float v) override;

    /**
     * Writes audio data to the ring buffer.
     * @param data The data to write.
     * @param numFrames The number of frames given to be written.
     * @return Returns the number of frames dropped, that is, any frames not written due to buffer overflow.
     */
    int writeAudioData(const float *data, int numFrames) override;
};

#endif // AUDIOSINK_H
