#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QSettings>
#include "morsehandler.h"

namespace Ui {
class settingswindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr, MorseHandler *morseHandler = nullptr);
    ~SettingsWindow();
    void setUserOnThisPage(bool userOnThisPage);
    bool getUserOnThisPage();
    void loadSettings();

signals:
    void goHome();

private slots:
    void onBackButtonClicked();
    void volumeChanged(signed int volumeValue);
    void wpmChanged(int wpm);
    void onInputDeviceIndexChanged(int index);

private:
    Ui::settingswindow *ui;

    MorseHandler *morseHandler;
    bool userOnThisPage;


    void saveSettings();
};

#endif // SETTINGSWINDOW_H
