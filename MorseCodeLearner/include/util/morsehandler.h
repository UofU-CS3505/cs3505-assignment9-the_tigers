#ifndef MORSEHANDLER_H
#define MORSEHANDLER_H

#include <QObject>
#include <map>
#include <QTimer>
#include <QElapsedTimer>
#include <morseaudiohandler.h>

using std::map, std::string;

/**
 * The MorseHandler class handles all morse timings from user input.
 * It also plays morse audio with a MorseAudioHandler.
 *
 * Code which uses a MorseHandler object must call the methods related
 * to straight key presses/releases or paddle presses/releases.
 *
 * The MorseHandler object will send a signal when a valid morse input
 * from the straightKeyDown/Up or paddleDown/Up happens. The morse
 * character that was decoded from the input is sent over the signal.
 *
 * There are methods within this class that can return the decoded version of
 * a morse code string, and an encoded version of a passed in text string.
 *
 * Whenever a user navigates away from a page that is currently using the morsehandler,
 * stopTimers() should be called.
 *
 * The standard international morse timings are used, which can be found here:
 * https://morsecode.world/international/timing.html
 *
 * @name Michael Timothy
 * @date 04/04/2025
 */
class MorseHandler : public QObject
{
    Q_OBJECT
private:
    const map<char, string> encodings = {
        {'a', ".-"},    {'b', "-..."},  {'c', "-.-."},  {'d', "-.."},   {'e', "."},
        {'f', "..-."},  {'g', "--."},   {'h', "...."},  {'i', ".."},    {'j', ".---"},
        {'k', "-.-"},   {'l', ".-.."},  {'m', "--"},    {'n', "-."},    {'o', "---"},
        {'p', ".--."},  {'q', "--.-"},  {'r', ".-."},   {'s', "..."},   {'t', "-"},
        {'u', "..-"},   {'v', "...-"},  {'w', ".--"},   {'x', "-..-"},  {'y', "-.--"},
        {'z', "--.."},
        {'0', "-----"}, {'1', ".----"}, {'2', "..---"}, {'3', "...--"}, {'4', "....-"},
        {'5', "....."}, {'6', "-...."}, {'7', "--..."}, {'8', "---.."}, {'9', "----."}
    };

    map<string, char> reverseEncodings;

    enum morseChar { DOT, DASH, EMPTY };

    float unit;

    bool paddleDotIsDown = false;
    bool paddleDashIsDown = false;

    morseChar currentPaddleInput = EMPTY;

    QTimer paddleDotTimer;
    QTimer paddleDashTimer;
    QTimer charGapTimer;
    QTimer wordGapTimer;
    QTimer suspendAudioTimer;
    QElapsedTimer keyDownTimer;

    void signalPaddleDot();
    void signalPaddleDash();

    MorseAudioHandler audioHandler;

public:
    enum device { STRAIGHT_KEY, IAMBIC_PADDLE };

    MorseHandler(device inputDevice, int wpm);

    string encodeText(const string text);

    string decodeMorse(const string morse);

    void setDevice(device input);

    device getDevice();

    /**
     * Used when a straight key is pressed down.
     * Starts a timer that tracks how long the key has been pressed
     * and a char gap timer.
     */
    void straightKeyDown();

    /**
     * Used when a straight key is released.
     * Checks the duration that has elapsed since the straight key was pressed down.
     * Depending on the length, either emits a dot or dash.
     *
     * Also starts a char gap timer and word gap timer.
     */
    void straightKeyUp();

    void paddleDotDown();
    void paddleDotUp();
    void paddleDashDown();
    void paddleDashUp();

    /**
     * Can be used to stop all current timers to ensure
     * that no more signals are sent/no more background
     * timers are running.
     *
     * Useful for when the user clicks the button that takes them back to a menu.
     */
    void stopTimers();

    /**
     * Sets the morse unit duration.
     */
    void setWpm(float wpm);

    /**
     * @return The current morse unit duration.
     */
    float getUnitTime();

    bool getPlayback();

    void stopPlayback();

    void playMorse(string morse);

    void setVolume(int volume);



private slots:
    /**
     * Emits decodedInput() with a space character to indicate the start of a new letter.
     */
    void onCharGapTimeout();

    /**
     * Emits decodedInput() with a slash (/) character to indicate the start of a new word.
     */
    void onWordGapTimeout();

signals:
    /**
     * A signal that sends morse characters generated.
     * @param The morse character generated.
     */
    void decodedInput(const std::string morse);

    void playbackEnd();

    void lightIndicatorOn();
    void lightIndicatorOff();
private:
    device inputDevice;
};



#endif // MORSEHANDLER_H
