#ifndef SINEWAVEGENERATOR_H
#define SINEWAVEGENERATOR_H

#include <cmath>

/**
 * A sine wave data generator that can be used to
 * retrieve generated sine data from. Designed to be paired with an audio
 * sink that is able to tell the generator if there were dropped audio frames,
 * which allows for the generator to revert its phase, allowing for a seamless
 * continuous sine wave.
 *
 * @author Michael Timothy
 * @date 4/21/2025
 */
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
