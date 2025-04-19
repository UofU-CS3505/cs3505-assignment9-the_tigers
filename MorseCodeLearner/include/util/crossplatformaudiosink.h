#ifndef CROSSPLATFORMAUDIOSINK_H
#define CROSSPLATFORMAUDIOSINK_H

class CrossPlatformAudioSink
{
public:
    virtual ~CrossPlatformAudioSink() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void suspend() = 0;
    virtual void resume() = 0;
    virtual void setVolume(float v) = 0;
    virtual int writeAudioData(const float* data, int numFrames) = 0;
};


#endif // CROSSPLATFORMAUDIOSINK_H
