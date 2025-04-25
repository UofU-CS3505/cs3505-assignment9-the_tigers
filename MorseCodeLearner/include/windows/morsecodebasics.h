#ifndef MORSECODEBASICS_H
#define MORSECODEBASICS_H

#include <QWidget>

namespace Ui {
class MorseCodeBasics;
}

/**
 * Window for morse code basics.
 * Displays a series of pages teaching the history and basics of morse code.
 *
 * @name Aiden Fornalski
 * @date 04/24/2025
 */
class MorseCodeBasics : public QWidget
{
    Q_OBJECT

private:
    Ui::MorseCodeBasics *ui;
    int currentIndex;
    bool userOnThisPage;

public:
    explicit MorseCodeBasics(QWidget *parent = nullptr);

    ~MorseCodeBasics();

private slots:
    /**
     * Resets the page's state when the back button is clicked.
     */
    void onBackButtonClicked();

    /**
     * Advances the slide forward when the next page button is clicked.
     */
    void onNextSlideClicked();

    /**
     * Goes to the previous slide when the previous page button is clicked.
     */
    void onPreviousSlideClicked();

    /**
     * Changes the current page in the stacked widget with the indicated index.
     * @param index - The current page index to go to in the stacked widget
     */
    void onStackedWidgetIndexChange(int index);

    /**
     * Marks the current lesson as completed.
     */
    void completeLesson();

public slots:
    void setUserOnThisPage(bool userOnThisPage);

signals:
    void goBack();
};

#endif // MORSECODEBASICS_H
