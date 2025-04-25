#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QSettings>
#include "morsehandler.h"

namespace Ui {
class settingswindow;
}

/**
 * The settings window.
 * Allows the user to change their input device, words per minute, and audio volume.
 * Saves and loads persistent settings.
 *
 * @name Chandler Eyre, Taylor Marlow
 * @date 04/24/2025
 */
class SettingsWindow : public QWidget
{
    Q_OBJECT
private:
    Ui::settingswindow *ui;
    MorseHandler *morseHandler;

    bool userOnThisPage;

    /**
     * Saves the users settings to a QSettings.
     */
    void saveSettings();

public:
    /**
     * Constructor for a settings window.
     * @param parent - The QObject parent
     * @param morseHandler - pointer to the Morse Handler
     */
    explicit SettingsWindow(QWidget *parent = nullptr, MorseHandler *morseHandler = nullptr);

    /**
     * Destructor for a settings window.
     */
    ~SettingsWindow();

    /**
     * Sets whether the user is on this page.
     * @param userOnThisPage - Whether the user is on this page.
     */
    void setUserOnThisPage(bool userOnThisPage);

    /**
     * @return - Whether the user is on this page.
     */
    bool getUserOnThisPage();

    /**
     * Loads the users settings.
     */
    void loadSettings();

private slots:
    /**
     * Resets the page's state when the back button is clicked.
     */
    void onBackButtonClicked();

    /**
     * Handles volume changes.
     * @param volumeValue - New volume value
     */
    void volumeChanged(signed int volumeValue);

    /**
     * Handles words per minute changes.
     * @param wpm - New words per minute value
     */
    void wpmChanged(int wpm);

    /**
     * Handles input device changes.
     * Inputs can be 0 (straight key) or 1 (iambic paddle)
     * @param index - 0 or 1
     */
    void onInputDeviceIndexChanged(int index);

signals:
    /**
     * Signal that gets emitted when the back button is pressed.
     * Navigates to main menu.
     */
    void goHome();
};

#endif // SETTINGSWINDOW_H
