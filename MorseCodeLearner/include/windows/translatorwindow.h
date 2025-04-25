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

    void setUserOnThisPage(bool userOnThisPage);

private slots:

    void paddleSelected();

    void straightKeySelected();

    void modeEnglishToMorse();

    void modeMorseToEnglish();

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

    void inputTextChanged(const std::string text);

    void enteredTranslator();

    void leftTranslator();
};

#endif // TRANSLATORWINDOW_H
