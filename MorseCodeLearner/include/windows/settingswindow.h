#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include "morsehandler.h"
#include "morseaudiohandler.h"

namespace Ui {
class settingswindow;
}

class settingswindow : public QWidget
{
    Q_OBJECT

public:
    explicit settingswindow(QWidget *parent = nullptr,
                            MorseHandler *morseHandler = nullptr,
                            MorseAudioHandler *audioHandler = nullptr);
    ~settingswindow();
    void setUserOnThisPage(bool userOnThisPage);
    bool getUserOnThisPage();

signals:
    void goHome();

private slots:
    void on_backButton_clicked();
    void volumeChanged(int volume);

private:
    Ui::settingswindow *ui;

    MorseHandler *morseHandler;
    MorseAudioHandler *audioHandler;
    bool userOnThisPage;
};

#endif // SETTINGSWINDOW_H
