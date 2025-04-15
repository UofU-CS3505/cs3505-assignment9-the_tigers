#ifndef PRACTICEWINDOW_H
#define PRACTICEWINDOW_H

#include <QWidget>
#include "morsehandler.h"
#include "keyeventfilter.h"
#include "difficultyhandler.h"
#include "practicehandler.h"

namespace Ui {
class practicewindow;
}

/**
 * The practice page. Allows the user to infinitely practice morse code decoding and encoding.
 *
 * @name Chandler Eyre and Michael Timothy
 * @date 04/15/2025
 */
class practicewindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor for a practice window. Takes in a MorseHandler, MorseAudioHandler, and KeyEventFilter.
     */
    explicit practicewindow(QWidget *parent = nullptr,
                            KeyEventFilter *keyEventFilter = nullptr,
                            PracticeHandler *practiceHandler = nullptr);
    ~practicewindow();

    void setUserOnThisPage(bool userOnThisPage);

    bool getUserOnThisPage();

signals:
    /**
     * Signal that gets emitted when the back button is pressed.
     */
    void goHome();

    /**
     * Signal that sets the practice difficulty.
     */
    void setDifficulty(QString difficulty);

    /**
     * Gets the correct practice text for the currently selected difficulty.
     */
    void getPracticeText();

public slots:
    /**
     * Updates practice text display.
     */
    void updatePracticeText(QString text);

    /**
     * Updates input text display.
     */
    void updateInputText(QString text);

private slots:

private:
    Ui::practicewindow *ui;

    MorseHandler *morseHandler;
    KeyEventFilter *keyEventFilter;
    PracticeHandler *practiceHandler;
};

#endif // PRACTICEWINDOW_H
