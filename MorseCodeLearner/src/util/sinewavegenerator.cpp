#include "sinewavegenerator.h"

SineWaveGenerator::SineWaveGenerator(float freq, int sampleRate) : frequency(freq), rate(sampleRate) {}

void SineWaveGenerator::generate(float *buffer, int numFrames) {
    for (int i = 0; i < numFrames; ++i) {
        float sample = sinf(phase);
        phase += 2.0f * M_PI * frequency / rate;
        if (phase >= 2.0f * M_PI)
            phase -= 2.0f * M_PI;

        buffer[i * 2] = sample; // Left channel
        buffer[i * 2 + 1] = sample; // Right channel
    }
}

void SineWaveGenerator::revertPhase(int frames) {
    float delta = 2.0f * M_PI * frequency / rate;
    phase -= frames * delta;
    while (phase < 0.0f)
        phase += 2.0f * M_PI;
}
