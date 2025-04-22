#include <condition_variable>
#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#include "crossplatformaudiosink.h"
#include "qdebug.h"
#include "qlogging.h"


#ifdef _WIN32
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <windows.h>
#include <combaseapi.h>

class WasapiAudioSink : public CrossPlatformAudioSink {
public:
    WasapiAudioSink()
        : audioClient(nullptr), renderClient(nullptr), sampleRate(48000), channels(2), running(false) {
        ringBuffer.resize(sampleRate * channels * 1);
    }

    ~WasapiAudioSink() {
        WasapiAudioSink::stop();
        if (renderClient) renderClient->Release();
        if (audioClient) audioClient->Release();
    }

    void start() override {
        HRESULT hr = CoInitialize(nullptr);
        if (FAILED(hr)) {
            qDebug() << "coinitialize failed: " << hr;
        }

        IMMDeviceEnumerator* enumerator = nullptr;
        IMMDevice* device = nullptr;

        CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
                         __uuidof(IMMDeviceEnumerator), (void**)&enumerator);
        enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
        device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (void**)&audioClient);

        WAVEFORMATEX* format = nullptr;
        audioClient->GetMixFormat(&format);

        REFERENCE_TIME defaultPeriod = 0;
        audioClient->GetDevicePeriod(&defaultPeriod, nullptr);


        audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, defaultPeriod, 0, format, nullptr);
        audioClient->GetBufferSize(&bufferSize);
        audioClient->GetService(__uuidof(IAudioRenderClient), (void**)&renderClient);
        running = true;
        thread = std::thread(&WasapiAudioSink::loop, this);
    }

    void suspend() override {
        {
            std::lock_guard<std::mutex> lock(suspendMutex);
            suspended = true;
        }
        if (audioClient) {
            audioClient->Stop();
        }
    }

    void resume() override {
        {
            std::lock_guard<std::mutex> lock(suspendMutex);
            suspended = false;
        }
        cv.notify_all();

        if (audioClient) audioClient->Start();
    }

    void stop() override {
        running = false;
        cv.notify_all();
        if (thread.joinable()) thread.join();
        if (audioClient) audioClient->Stop();
        CoUninitialize();
    }

    void setVolume(float v) override {
        volume = v;
    }

    // assumes buffer lock from caller
    size_t bufferedFrames() {
        if (bufferWritePos >= bufferReadPos)
            return (bufferWritePos - bufferReadPos) / channels;
        else
            return (ringBuffer.size() - bufferReadPos + bufferWritePos) / channels;
    }

    int writeAudioData(const float* data, int numFrames) override {
        std::lock_guard<std::mutex> lock(bufferMutex);

        size_t framesAvailable = (ringBuffer.size() / channels) - bufferedFrameCount;
        size_t framesToWrite = std::min((size_t)numFrames, framesAvailable);

        for (size_t i = 0; i < framesToWrite * channels; ++i) {
            ringBuffer[bufferWritePos] = data[i];
            bufferWritePos = (bufferWritePos + 1) % ringBuffer.size();
        }

        bufferedFrameCount += framesToWrite;
        return static_cast<int>(framesToWrite);
    }

private:
    IAudioClient* audioClient;
    IAudioRenderClient* renderClient;
    UINT32 bufferSize;
    UINT32 sampleRate;
    UINT32 channels;
    std::atomic<bool> running;
    std::atomic<bool> suspended = false;
    std::thread thread;

    std::vector<float> ringBuffer;
    std::mutex bufferMutex;
    std::mutex suspendMutex;
    std::condition_variable cv;
    size_t bufferWritePos = 0;
    size_t bufferReadPos = 0;
    size_t bufferedFrameCount = 0;
    float volume = 1.0f;
    float prevSample = 0.0f;

    void loop() {
        while (running) {
            {
                std::unique_lock<std::mutex> lock(suspendMutex);
                cv.wait(lock, [this] { return !suspended || !running; });
            }

            if (!running)
                break;

            UINT32 padding = 0;
            HRESULT hr = audioClient->GetCurrentPadding(&padding);
            if (FAILED(hr)) {
                Sleep(5);
                continue;
            }

            UINT32 availableFrames = bufferSize - padding;
            if (availableFrames == 0) {
                Sleep(2);
                continue;
            }

            BYTE* data = nullptr;
            hr = renderClient->GetBuffer(availableFrames, &data);
            if (FAILED(hr)) {
                Sleep(2);
                continue;
            }

            float* floatData = reinterpret_cast<float*>(data);

            {
                std::lock_guard<std::mutex> lock(bufferMutex);

                if (bufferedFrameCount >= availableFrames) {
                    for (UINT32 i = 0; i < availableFrames * channels; ++i) {
                        float sample = ringBuffer[bufferReadPos];
                        bufferReadPos = (bufferReadPos + 1) % ringBuffer.size();
                        floatData[i] = sample * volume;

                        // if (abs(sample - prevSample) > 0.0576) {
                        //     qDebug() << "sample: " << sample << " prev: " << prevSample;
                        //     qDebug() << "read pos: " << bufferReadPos << " write pos: " << bufferWritePos;
                        // }
                        prevSample = sample;
                    }

                    bufferedFrameCount -= availableFrames;
                } else {
                    renderClient->ReleaseBuffer(availableFrames, AUDCLNT_BUFFERFLAGS_SILENT);
                    continue;
                }
            }

            hr = renderClient->ReleaseBuffer(availableFrames, 0);
            if (FAILED(hr)) {
                Sleep(2);
            }

            Sleep(2);
        }
    }


};
#endif

#ifdef __APPLE__
#import <AudioToolbox/AudioToolbox.h>
#import <CoreAudio/CoreAudio.h>

class CoreAudioSink : public CrossPlatformAudioSink {
public:
    CoreAudioSink()
        : audioQueue(nullptr), bufferByteSize(0), sampleRate(48000),
        channels(2), bufferWritePos(0), bufferReadPos(0), bufferedFrameCount(0),
        running(false), suspended(false), volume(1.0f) {
        ringBuffer.resize(sampleRate * channels * 5); // 5 seconds
    }

    ~CoreAudioSink() {
        stop();
    }

    void start() override {
        AudioStreamBasicDescription format = {};
        format.mSampleRate = sampleRate;
        format.mFormatID = kAudioFormatLinearPCM;
        format.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked;
        format.mFramesPerPacket = 1;
        format.mChannelsPerFrame = channels;
        format.mBytesPerFrame = sizeof(float) * channels;
        format.mBytesPerPacket = format.mBytesPerFrame * format.mFramesPerPacket;
        format.mBitsPerChannel = 32;

        bufferByteSize = format.mBytesPerFrame * sampleRate / 132;

        AudioQueueNewOutput(&format, audioCallback, this, nullptr, nullptr, 0, &audioQueue);

        for (int i = 0; i < kNumBuffers; ++i) {
            AudioQueueAllocateBuffer(audioQueue, bufferByteSize, &buffers[i]);
            audioCallback(this, audioQueue, buffers[i]);
        }

        AudioQueueStart(audioQueue, nullptr);
        running = true;
        suspend();
    }

    void stop() override {
        running = false;
        if (audioQueue) {
            AudioQueueStop(audioQueue, true);
            AudioQueueDispose(audioQueue, true);
            audioQueue = nullptr;
        }
    }

    void suspend() override {
        {
            std::lock_guard<std::mutex> lock(suspendMutex);
            suspended = true;
        }
        // AudioQueuePause(audioQueue);
    }

    void resume() override {
        {
            std::lock_guard<std::mutex> lock(suspendMutex);
            suspended = false;
        }

        {
            std::lock_guard<std::mutex> lock(bufferMutex);
            while (true) {
                if (abs(ringBuffer[bufferReadPos]) > 0.009) {
                    bufferReadPos = (bufferReadPos + 1) % ringBuffer.size();
                } else {
                    break;
                }
            }
        }


        fadeOut = 1;

        AudioQueueStart(audioQueue, nullptr);
    }




    void setVolume(float v) override {
        volume = v;
        if (audioQueue) {
            AudioQueueSetParameter(audioQueue, kAudioQueueParam_Volume, volume);
        }
    }

    int writeAudioData(const float* data, int numFrames) override {

        std::lock_guard<std::mutex> lock(bufferMutex);
        size_t framesAvailable = (ringBuffer.size() / channels) - bufferedFrameCount;

        if (framesAvailable < static_cast<size_t>(numFrames) + 480)
            return 0;

        for (int i = 0; i < numFrames * (int)channels; ++i) {
            ringBuffer[bufferWritePos] = data[i];
            bufferWritePos = (bufferWritePos + 1) % ringBuffer.size();
        }

        bufferedFrameCount += numFrames;
        return numFrames;
    }

    size_t bufferedFrames() {
        if (bufferWritePos >= bufferReadPos)
            return (bufferWritePos - bufferReadPos) / channels;
        else
            return (ringBuffer.size() - bufferReadPos + bufferWritePos) / channels;
    }

private:
    static constexpr int kNumBuffers = 3;

    AudioQueueRef audioQueue;
    AudioQueueBufferRef buffers[kNumBuffers];
    size_t bufferByteSize;
    const uint32_t sampleRate;
    const uint32_t channels;

    std::vector<float> ringBuffer;
    size_t bufferWritePos;
    size_t bufferReadPos;
    size_t bufferedFrameCount;

    std::mutex bufferMutex;
    std::mutex suspendMutex;
    std::condition_variable cv;

    std::atomic<bool> running;
    std::atomic<bool> suspended;
    float volume;

    double fadeOut = 0.0f;
    // float lastOutputSample = 0.0f;

    static void audioCallback(void* userData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer) {
        auto* self = static_cast<CoreAudioSink*>(userData);
        size_t frames = inBuffer->mAudioDataBytesCapacity / (sizeof(float) * self->channels);
        float* output = reinterpret_cast<float*>(inBuffer->mAudioData);

        std::unique_lock<std::mutex> lock(self->suspendMutex);

        if (self->suspended || !self->running) {
            std::lock_guard<std::mutex> bufferLock(self->bufferMutex);

            // Fade out while suspended, lasts approx 44 ms
            if (self->bufferedFrameCount >= frames && self->fadeOut > 0) {
                // qDebug() << "number of times: " << frames * self->channels;
                for (size_t i = 0; i < frames * self->channels; ++i) {
                    output[i] = self->ringBuffer[self->bufferReadPos] * self->volume * self->fadeOut;
                    self->bufferReadPos = (self->bufferReadPos + 1) % self->ringBuffer.size();
                    self->fadeOut -= 1 / (double)(frames * self->channels * 3);
                    if(self->fadeOut < 0)
                        self->fadeOut = 0;
                }
                self->bufferedFrameCount -= frames;
            } else {
                std::memset(output, 0, inBuffer->mAudioDataBytesCapacity);
            }
            inBuffer->mAudioDataByteSize = inBuffer->mAudioDataBytesCapacity;
            AudioQueueEnqueueBuffer(inAQ, inBuffer, 0, nullptr);
            if (self->fadeOut == 0) {
                AudioQueuePause(inAQ);
            }
            return;
        }

        lock.unlock();

        std::lock_guard<std::mutex> bufferLock(self->bufferMutex);
        if (self->bufferedFrameCount >= frames) {
            for (size_t i = 0; i < frames * self->channels; ++i) {
                output[i] = self->ringBuffer[self->bufferReadPos] * self->volume;
                self->bufferReadPos = (self->bufferReadPos + 1) % self->ringBuffer.size();
            }
            self->bufferedFrameCount -= frames;
        } else {
            std::memset(output, 0, inBuffer->mAudioDataBytesCapacity);
        }

        inBuffer->mAudioDataByteSize = inBuffer->mAudioDataBytesCapacity;
        AudioQueueEnqueueBuffer(inAQ, inBuffer, 0, nullptr);
    }
};
#endif
