#ifndef SINEWAVEGENERATOR_H
#define SINEWAVEGENERATOR_H

#include <cmath>

class SineWaveGenerator {
public:
    SineWaveGenerator(float freq, int sampleRate);

    void generate(float *buffer, int numFrames);

    void revertPhase(int frames);

private:
    float frequency;
    float rate;
    float phase = 0.0f;
};

#endif // SINEWAVEGENERATOR_H
