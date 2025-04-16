#ifndef LESSONWINDOW_H
#define LESSONWINDOW_H

#include <QWidget>
#include "keyeventfilter.h"
#include "morsehandler.h"
#include "lessonhandler.h"

namespace Ui {
class lessonwindow;
}

class lessonwindow : public QWidget
{
    Q_OBJECT

public:
    explicit lessonwindow(LessonHandler *lessonHandler = nullptr, MorseHandler *morseHandler = nullptr, KeyEventFilter *keyEventFilter = nullptr, QWidget *parent = nullptr);
    ~lessonwindow();
    void setUserOnThisPage(bool userOnThisPage);
    bool getUserOnThisPage();

private:
    Ui::lessonwindow *ui;
    LessonHandler *lessonHandler;
    MorseHandler *morseHandler;
    KeyEventFilter *keyEventFilter;
    bool userOnThisPage;
    bool acceptingInput;

signals:
    /**
     * A signal that tells the stackedWidget to send the user back
     * to the lesson select page.
     */
    void goToLessonSelect();
private slots:
    /**
     * A slot that triggers when the back button is pressed.
     */
    void on_backButton_clicked();

    /**
     * A slot that handles when the space key is pressed down; used
     * for morse input.
     */
    void handleSpacePressed();

    /**
     * A slot that handles when the space key is released; used for
     * morse input.
     */
    void handleSpaceReleased();

    /**
     * A slot that runs when a guess is correct.
     */
    void guessCorrect();

    /**
     * A slot that runs when a guess is incorrect.
     */
    void guessIncorrect();

    /**
     * Displays the current question to the view.
     * @param text - the current question.
     */
    void displayTextQuestion(QString text);

    void updateInputText(QString inputText);
};



#endif // LESSONWINDOW_H
