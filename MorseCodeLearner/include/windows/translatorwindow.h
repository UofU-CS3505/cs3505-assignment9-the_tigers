#ifndef TRANSLATORWINDOW_H
#define TRANSLATORWINDOW_H

#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QWidget>
#include <QTimer>
#include "translatehandler.h"

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
private:
    Ui::translatorwindow *ui;
    TranslateHandler *translateHandler;

    enum paddleAnimationStates {
        LEFT, RIGHT, CENTER, BOTH
    };

    paddleAnimationStates paddleState = CENTER;

    // Connection objects for illustrations, must be retained so they can be changed later
    QMetaObject::Connection rightPressedConnection;
    QMetaObject::Connection rightReleasedConnection;
    QMetaObject::Connection leftPressedConnection;
    QMetaObject::Connection leftReleasedConnection;
    QMetaObject::Connection straightPressedConnection;
    QMetaObject::Connection straightReleasedConnection;

public:
    /**
     * Constructor for a translator window. Takes in a MorseHandler, MorseAudioHandler, and KeyEventFilter.
     */
    explicit translatorwindow(QWidget *parent = nullptr,
                              TranslateHandler *translateHandler = nullptr);
    ~translatorwindow();

    /**
     * If this page is navigated to, then this will send
     * a signal to the translate handler letting it know.
     */
    void setUserOnThisPage(bool userOnThisPage);

private slots:
    /**
     * Updates the connections for input device animations so that the paddle is
     * shown/animated.
     */
    void paddleSelected();

    /**
     * Updates the connections for input device animations so that the
     * straight key is shown/animated.
     */
    void straightKeySelected();

    /**
     * Changes ui to show that the mode is English to Morse.
     */
    void modeEnglishToMorse();

    /**
     * Changes ui to show that the mode is English to Morse.
     */
    void modeMorseToEnglish();

    /**
     * Handles view logic when audio playback is started.
     * Disables some elements.
     */
    void handlePlaybackStarted();

    /**
     * Handles view logic when audio playback finishes.
     * Reenables elements.
     */
    void handlePlaybackStopped();

    /**
     * Resets the page's state when the back button is clicked.
     */
    void onBackButtonClicked();

signals:
    /**
     * Signal that gets emitted when the back button is pressed.
     */
    void goHome();

    /**
     * A signal for the translate handler with updated
     * input text from the user.
     */
    void inputTextChanged(const std::string text);

    /**
     * A signal for the translate handler to
     * tell it when the user has started using the translator page.
     */
    void enteredTranslator();

    /**
     * A signal for the translate handler to
     * tell it when the user has stopped using the translator page.
     */
    void leftTranslator();
};

#endif // TRANSLATORWINDOW_H
