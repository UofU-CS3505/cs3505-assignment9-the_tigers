#ifndef PRACTICEWINDOW_H
#define PRACTICEWINDOW_H

#include <QWidget>
#include "morsehandler.h"
#include "keyeventfilter.h"
#include "difficultyhandler.h"

namespace Ui {
class practicewindow;
}

/**
 * The practice page. Allows the user to infinitely practice morse code decoding and encoding.
 *
 * @name Chandler Eyre and Michael Timothy
 * @date 04/12/2025
 */
class practicewindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor for a practice window. Takes in a MorseHandler, MorseAudioHandler, and KeyEventFilter.
     */
    explicit practicewindow(QWidget *parent = nullptr,
                            MorseHandler *morseHandler = nullptr,
                            KeyEventFilter *keyEventFilter = nullptr);
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
    void setDifficulty(string difficulty);

    /**
     * Gets the correct practice text for the currently selected difficulty.
     */
    void getPracticeText();

public slots:
    /**
     * Loads a character, word, or sentence to practice from a set problem text.
     */
    void loadPracticeProblem(std::string seedText);

private slots:
    /**
     * Resets the page's state when the back button is clicked.
     */
    void onBackButtonClicked();

    /**
     * Slot for when the MorseHandler object sends us a morse character.
     * The morse character is appended to our input text box.
     * @param morse The morse character.
     */
    void onMorseReceived(const std::string morse);

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

private:
    Ui::practicewindow *ui;

    MorseHandler *morseHandler;
    KeyEventFilter *keyEventFilter;
    DifficultyHandler *difficultyHandler;

    bool userOnThisPage = false;

    string problemText;

    QTimer timer;

    bool acceptingInput;

    /**
     * Loads a character, word, or sentence to practice.
     */
    void loadPracticeProblem();

    /**
     * Gets a single character, a-z.
     */
    std::string getCharacter();
};

#endif // PRACTICEWINDOW_H
