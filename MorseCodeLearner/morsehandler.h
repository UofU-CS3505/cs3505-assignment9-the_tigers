#ifndef MORSEHANDLER_H
#define MORSEHANDLER_H

#include <QObject>
#include <map>
#include <QTimer>
#include <QElapsedTimer>

using std::map, std::string;

/**
 * The MorseHandler class handles all morse timings from user input.
 * It receives slots when a straight key is pressed/released or
 * when a paddle is pressed/released. It then sends a signal when a valid
 * morse input happens and the morse character that was decoded from the input
 * is sent over the signal.
 *
 * The class also is able to encode English text with encodeText which
 * returns the translated morse code.
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

    enum morseChar { DOT, DASH, EMPTY };

    float wpm;
    float unit;

    bool paddleDotIsDown = false;
    bool paddleDashIsDown = false;

    morseChar currentPaddleInput = EMPTY;

    QTimer paddleDotTimer;
    QTimer paddleDashTimer;
    QTimer gapTimer;
    QElapsedTimer keyDownTimer;

    void signalPaddleDot();
    void signalPaddleDash();

public:
    MorseHandler(int wpm);

    string encodeText(const string text);

public slots:
    void straightKeyDown();

    void straightKeyUp();

    void paddleDotDown();

    void paddleDotUp();

    void paddleDashDown();

    void paddleDashUp();

    void onGapTimeout();

    /**
     * Can be used to stop all current timers to ensure
     * that no more signals are sent/no more background
     * timers are running.
     *
     * Useful for when the user clicks the button that takes them back to a menu.
     */
    void stopTimers();

    void setWpm(float wpm);

signals:
    void decodedInput(const std::string morse);
};

#endif // MORSEHANDLER_H
