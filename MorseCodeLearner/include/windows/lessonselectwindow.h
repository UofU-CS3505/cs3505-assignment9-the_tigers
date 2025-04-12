#ifndef LESSONSELECTWINDOW_H
#define LESSONSELECTWINDOW_H

#include <QWidget>

namespace Ui {
class lessonselectwindow;
}

/**
 * The lesson select page. Navigates between lessons and shows lesson completion.
 *
 * @name Aiden Fornalski and Chandler Eyre
 * @date 04/11/2025
 */
class lessonselectwindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor for a lesson select window.
     */
    explicit lessonselectwindow(QWidget *parent = nullptr);

    /**
     * Destructor for a lesson select window.
     */
    ~lessonselectwindow();

    void setUserOnThisPage(bool userOnThisPage);

    bool getUserOnThisPage();

signals:
    /**
     * Signal that gets emitted when the back button is pressed.
     */
    void goHome();

    /**
     * Emitted when a lesson button is pressed.
     */
    void selectLesson(int lesson);

private slots:
    /**
     * Resets the page's state when the back button is clicked.
     */
    void onBackButtonClicked();

private:
    Ui::lessonselectwindow *ui;

    bool userOnThisPage = false;
};

#endif // LESSONSELECTWINDOW_H
