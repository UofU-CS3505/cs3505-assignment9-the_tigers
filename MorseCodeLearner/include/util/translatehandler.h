#ifndef TRANSLATEHANDLER_H
#define TRANSLATEHANDLER_H

#include <QObject>
#include "morsehandler.h"
#include "keyeventfilter.h"

/**
 * Handles all translating logic for the translator page.
 *
 * @name Michael Timothy
 * @date 4/24/2025
 */
class TranslateHandler : public QObject
{
    Q_OBJECT
private:
    enum translateMode {
        MORSE_TO_TEXT, TEXT_TO_MORSE
    };

    translateMode mode = MORSE_TO_TEXT;

    bool userUsingTranslator = false;

    string currentInputText = "";
    string currentOutputText = "";
    string currentMorsePreview = "";

    MorseHandler *morseHandler;
    KeyEventFilter *keyEventFilter;

public:
    /**
     * Constructor for the translate handler. Needs a MorseHandler and a KeyEventFilter
     */
    explicit TranslateHandler(MorseHandler *morseHandler = nullptr, KeyEventFilter *keyEventFilter = nullptr, QObject *parent = nullptr);
    ~TranslateHandler();

private slots:
    /**
     * Sends a signal to the morse handler telling it that the straight key
     * has been pressed down. Only does this if the straight key is the
     * currently selected input device and the user is in morse to text mode.
     */
    void handleSpacePressed();

    /**
     * Sends a signal to the morse handler telling it that the straight key
     * has been released. Only does this if the straight key is the
     * currently selected input device and the user is in morse to text mode.
     */
    void handleSpaceReleased();

    /**
     * Sends a signal to the morse handler telling it that the left paddle
     * has been pressed. Only does this if the paddle is the
     * currently selected input device and the user is in morse to text mode.
     */
    void handleLeftArrowPressed();

    /**
     * Sends a signal to the morse handler telling it that the left paddle
     * has been released. Only does this if the paddle is the
     * currently selected input device and the user is in morse to text mode.
     */
    void handleLeftArrowReleased();

    /**
     * Sends a signal to the morse handler telling it that the right paddle
     * has been pressed. Only does this if the paddle is the
     * currently selected input device and the user is in morse to text mode.
     */
    void handleRightArrowPressed();

    /**
     * Sends a signal to the morse handler telling it that the right paddle
     * has been released. Only does this if the paddle is the
     * currently selected input device and the user is in morse to text mode.
     */
    void handleRightArrowReleased();

    /**
     * Updates the morse preview and current morse input strings and
     * sends this new information off to the translator window.
     */
    void onMorseReceived(const std::string morse);

public slots:
    /**
     * Swaps the current translate mode.
     */
    void onSwapTranslateMode();

    /**
     * Clears the current input.
     */
    void onClearInput();

    /**
     * Either pauses or starts audio playback for any morse that is currently
     * in the input or output strings.
     */
    void onAudioPlayButtonClicked();

    /**
     * Update the current input and output strings based on
     * the new input text.
     */
    void onInputTextChanged(const std::string text);

    /**
     * Instantiates the translator and sets userUsingTranslator to true.
     */
    void onUserEnteredTranslate();

    /**
     * Resets the translator and sets userUsingTranslator to false.
     */
    void onUserLeftTranslate();

signals:
    // All of these signals are for updating
    // ui elements on the translator page.
    void turnLightIndicatorOn();

    void turnLightIndicatorOff();

    void modeToEnglishToMorse();

    void modeToMorseToEnglish();

    void setPlayButtonPaused();

    void setPlayButtonPlaying();

    void updateOutputText(const std::string text);

    void updateInputText(const std::string text);

    void updateMorsePreview(const std::string morse);

    void paddleSelected();

    void straightKeySelected();

    void straightKeyPressed();

    void straightKeyReleased();

    void paddleLeftPressed();

    void paddleLeftReleased();

    void paddleRightPressed();

    void paddleRightReleased();
};

#endif // TRANSLATEHANDLER_H
