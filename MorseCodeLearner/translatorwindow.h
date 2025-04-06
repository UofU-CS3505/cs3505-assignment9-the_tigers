#ifndef TRANSLATORWINDOW_H
#define TRANSLATORWINDOW_H


#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QWidget>
#include "morsehandler.h"

namespace Ui {
class translatorwindow;
}

class translatorwindow : public QWidget
{
    Q_OBJECT

public:
    explicit translatorwindow(QWidget *parent = nullptr, MorseHandler *morseHandler = nullptr);
    ~translatorwindow();

    void setupMorse(MorseHandler *handler);

signals:
    /**
     * Signal that gets emitted when the back button is pressed.
     */
    void goHome();

private slots:
    /**
     * Slot that gets called when the back button is pressed; emits the
     * goBack() signal.
     */
    void onBackButtonClicked();

    void on_swapButton_clicked();

    void on_inputText_textChanged();

    void onMorseReceived(const std::string morse);

private:
    Ui::translatorwindow *ui;

    enum translateMode {
        MORSE_TO_TEXT, TEXT_TO_MORSE
    };

    MorseHandler *morseHandler;

    translateMode mode;

    string currentMorseLetter = "";

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // TRANSLATORWINDOW_H
