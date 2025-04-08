#ifndef TRANSLATORWINDOW_H
#define TRANSLATORWINDOW_H


#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QWidget>
#include <QTimer>
#include "morsehandler.h"
#include "morseaudiohandler.h"
#include "keyeventfilter.h"

namespace Ui {
class translatorwindow;
}

class translatorwindow : public QWidget
{
    Q_OBJECT

public:
    explicit translatorwindow(QWidget *parent = nullptr,
                              MorseHandler *morseHandler = nullptr,
                              MorseAudioHandler *audioHandler = nullptr,
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
    void onBackButtonClicked();

    void on_swapButton_clicked();

    void on_inputText_textChanged();

    void onMorseReceived(const std::string morse);

    void on_audioPlayButton_clicked();

    void handleSpacePressed();

    void handleSpaceReleased();

private:
    Ui::translatorwindow *ui;

    enum translateMode {
        MORSE_TO_TEXT, TEXT_TO_MORSE
    };

    MorseHandler *morseHandler;
    MorseAudioHandler *audioHandler;
    KeyEventFilter *keyEventFilter;


    translateMode mode = MORSE_TO_TEXT;

    bool userOnThisPage = false;
};

#endif // TRANSLATORWINDOW_H
