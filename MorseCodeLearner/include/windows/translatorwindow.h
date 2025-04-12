#ifndef TRANSLATORWINDOW_H
#define TRANSLATORWINDOW_H

#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QWidget>
#include <QTimer>
#include "morsehandler.h"
#include "keyeventfilter.h"

namespace Ui {
class translatorwindow;
}

/**
 * The translator page. Allows for morse or text input which is translated to its counterpart.
 * Morse can be played back audibly with the play/pause button.
 *
 * @name Michael Timothy and Chandler Eyre
 * @date 04/11/2025
 */
class translatorwindow : public QWidget
{
    Q_OBJECT
public:
    /**
     * Constructor for a translator window. Takes in a MorseHandler, MorseAudioHandler, and KeyEventFilter.
     */
    explicit translatorwindow(QWidget *parent = nullptr,
                              MorseHandler *morseHandler = nullptr,
                              KeyEventFilter *keyEventFilter = nullptr);
    ~translatorwindow();

    void setUserOnThisPage(bool userOnThisPage);

    bool getUserOnThisPage();

signals:
    /**
     * Signal that gets emitted when the back button is pressed.
     */
    void goHome();

private slots:
    /**
     * Resets the page's state when the back button is clicked.
     */
    void onBackButtonClicked();

    /**
     * Swaps the input type and puts whatever is currently in the output text box
     * into the input text box.
     */
    void onSwapButtonClicked();

    /**
     * Updates the output text box with the translated version of the input text box.
     */
    void onInputTextTextChanged();

    /**
     * Slot for when the MorseHandler object sends us a morse character.
     * The morse character is appended to our input text box.
     * @param morse The morse character.
     */
    void onMorseReceived(const std::string morse);

    /**
     * Starts playback of the morse that is either in the input box or output box depending on the
     * current translate mode.
     *
     * Disables morse device input while playing.
     */
    void onAudioPlayButtonClicked();

    /**
     * Tells the MorseHandler and MorseAudioHandler that the straight key has been pressed
     * as long as: the user is currently on this page, playback of morse is not currently being played,
     * and the translate mode is morse to text.
     */
    void handleSpacePressed();

    /**
     * Tells the MorseHandler and MorseAudioHandler that the straight key has been released
     * as long as: the user is currently on this page, playback of morse is not currently being played,
     * and the translate mode is morse to text.
     */
    void handleSpaceReleased();

    void handleLeftArrowPressed();

    void handleLeftArrowReleased();

    void handleRightArrowPressed();

    void handleRightArrowReleased();

    /**
     * Clears the current input.
     */
    void clearInput();

    /**
     * Handles view logic when audio playback finishes.
     * Reenables elements.
     */
    void handlePlaybackStopped();

private:
    Ui::translatorwindow *ui;

    enum translateMode {
        MORSE_TO_TEXT, TEXT_TO_MORSE
    };

    MorseHandler *morseHandler;
    KeyEventFilter *keyEventFilter;

    translateMode mode = MORSE_TO_TEXT;

    bool userOnThisPage = false;

    QTimer *displayTimer;
};

#endif // TRANSLATORWINDOW_H
