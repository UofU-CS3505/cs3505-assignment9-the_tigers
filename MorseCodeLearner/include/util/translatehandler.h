#ifndef TRANSLATEHANDLER_H
#define TRANSLATEHANDLER_H

#include <QObject>
#include "morsehandler.h"
#include "keyeventfilter.h"

/**
 * Handles all translating logic for the translator page.
 *
 * @author Michael Timothy
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

    explicit TranslateHandler(MorseHandler *morseHandler = nullptr, KeyEventFilter *keyEventFilter = nullptr, QObject *parent = nullptr);
    ~TranslateHandler();

private slots:
    void handleSpacePressed();

    void handleSpaceReleased();

    void handleLeftArrowPressed();

    void handleLeftArrowReleased();

    void handleRightArrowPressed();

    void handleRightArrowReleased();

    void onMorseReceived(const std::string morse);

    void onPlaybackStopped();

public slots:
    void onSwapTranslateMode();

    void onClearInput();

    void onAudioPlayButtonClicked();

    void onInputTextChanged(const std::string text);

    void onUserEnteredTranslate();

    void onUserLeftTranslate();

signals:
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
