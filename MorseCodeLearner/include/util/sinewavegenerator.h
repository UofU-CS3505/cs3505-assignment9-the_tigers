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

    /**
     * Generates a specified number of sine wave frames frames
     * and puts them in side of the buffer at
     * the buffer pointer given.
     */
    void generate(float *buffer, int numFrames);

    /**
     * Sets the phase back a number of frames to preserve continous frames
     * in the case that frames were dropped by an audio sink.
     */
    void revertPhase(int frames);

private:
    float frequency;
    float rate;
    float phase = 0.0f;
};

#endif // SINEWAVEGENERATOR_H
