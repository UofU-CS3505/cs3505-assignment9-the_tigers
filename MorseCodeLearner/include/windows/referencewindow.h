#ifndef REFERENCEWINDOW_H
#define REFERENCEWINDOW_H

#include <QWidget>

namespace Ui {
class referencewindow;
}

/**
 * The reference window.
 * Displays morse code character references.
 *
 * @name Taylor Marlow, Chandler Eyre
 * @date 04/24/2025
 */
class ReferenceWindow : public QWidget
{
    Q_OBJECT
private:
    Ui::referencewindow *ui;
    bool userOnThisPage;

public:
    /**
     * Constructor for a reference window.
     * @param parent - The QObject parent
     */
    explicit ReferenceWindow(QWidget *parent = nullptr);

    /**
     * Destructor for a reference window.
     */
    ~ReferenceWindow();

    /**
     * Sets whether the user is on this page.
     * @param userOnThisPage - Whether the user is on this page.
     */
    void setUserOnThisPage(bool userOnThisPage);

    /**
     * @return - Whether the user is on this page.
     */
    bool getUserOnThisPage();

private slots:
    /**
     * Resets the page's state when the back button is clicked.
     */
    void onBackButtonClicked();

signals:
    /**
     * Signal that gets emitted when the back button is pressed.
     * Navigates to main menu.
     */
    void goHome();
};

#endif // REFERENCEWINDOW_H
