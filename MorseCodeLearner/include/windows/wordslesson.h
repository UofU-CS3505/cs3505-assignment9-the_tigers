#ifndef WORDSLESSON_H
#define WORDSLESSON_H

#include <QWidget>

namespace Ui {
class WordsLesson;
}

/**
 * Window for the words lesson.
 * Displays a series of pages teaching how words and their timings work
 * in morse code before sending the user to a practice lesson.
 *
 * @name Aiden Fornalski
 * @date 04/24/2025
 */
class WordsLesson : public QWidget
{
    Q_OBJECT

private:
    Ui::WordsLesson *ui;
    int currentIndex;
    bool userOnThisPage;

public:
    explicit WordsLesson(QWidget *parent = nullptr);

    ~WordsLesson();

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
     * Starts a lesson to learn common words/phrases.
     */
    void startPracticeLesson();

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

    /**
     * Emits a signal to go to the words practice lesson.
     */
    void goToPracticeLesson();
};

#endif // WORDSLESSON_H
