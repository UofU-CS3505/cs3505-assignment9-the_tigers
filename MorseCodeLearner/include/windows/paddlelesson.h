#ifndef PADDLELESSON_H
#define PADDLELESSON_H

#include <QWidget>

namespace Ui {
class PaddleLesson;
}

/**
 * Window for the paddle lesson.
 * Displays a series of pages teaching the history of the iambic paddle
 * and how it works.
 *
 * @name Aiden Fornalski
 * @date 04/24/2025
 */
class PaddleLesson : public QWidget
{
    Q_OBJECT

private:
    Ui::PaddleLesson *ui;
    int currentIndex;
    bool userOnThisPage;

public:
    explicit PaddleLesson(QWidget *parent = nullptr);

    ~PaddleLesson();

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
    /**
     * Changes the current page in the stacked widget with the indicated index.
     * @param index - The current page index to go to in the stacked widget
     */
    void setUserOnThisPage(bool userOnThisPage);

signals:
    /**
     * Emits a signal to go back to the main menu.
     */
    void goBack();
};

#endif // PADDLELESSON_H
