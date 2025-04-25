#ifndef CROSSPLATFORMAUDIOSINK_H
#define CROSSPLATFORMAUDIOSINK_H

/**
 * An audio sink implementation similar to QAudioSink but specifically designed
 * for the playback of generated sine wave data. Designed to minimize the number of 'pops'
 * heard on sine wave playback. Current version has no audible pops on macOS but still
 * some on Windows machines.
 *
 * Implementation works for macOS and Windows using CoreAudio and Wasapi accordingly.
 *
 * @name Michael Timothy
 * @date 4/21/2025
 */
class CrossPlatformAudioSink
{
public:
    /// See audiosink.h for method header comments.
    virtual ~CrossPlatformAudioSink() = default;
    virtual void start() = 0;
    virtual void stop() = 0;

    /**
     * Note for macOS vs Windows:
     * mac has a fade out on suspension of audio; windows purely stops playback.
     */
    virtual void suspend() = 0;
    virtual void resume() = 0;
    virtual void setVolume(float v) = 0;
    virtual int writeAudioData(const float* data, int numFrames) = 0;
};

#endif // CROSSPLATFORMAUDIOSINK_H
