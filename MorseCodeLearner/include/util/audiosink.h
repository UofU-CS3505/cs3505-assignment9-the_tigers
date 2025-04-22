#ifndef AUDIOSINK_H
#define AUDIOSINK_H

#include "crossplatformaudiosink.h"
#include "crossplatformaudiosink.cpp"

/**
 * Wrapper class for a CrossPlatformAudioSink
 *
 * @author Michael Timothy
 * @date 4/21/2025
 */
class AudioSink : public CrossPlatformAudioSink
{
public:
    AudioSink();

    ~AudioSink();

    void start() override;
    void stop() override;
    void suspend() override;
    void resume() override;
    void setVolume(float v) override;
    int writeAudioData(const float *data, int numFrames) override;

private:
    CrossPlatformAudioSink *impl;

};

#endif // AUDIOSINK_H
