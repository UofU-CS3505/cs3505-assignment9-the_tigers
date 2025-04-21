#ifndef WORDSLESSON_H
#define WORDSLESSON_H

#include <QWidget>

namespace Ui {
class WordsLesson;
}

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

signals:
    void goBack();

    void goToPracticeLesson();

public slots:
    void setUserOnThisPage(bool userOnThisPage);

private slots:
    /**
     * Resets the page's state when the back button is clicked.
     */
    void onBackButtonClicked();

    void onNextSlideClicked();

    void onPreviousSlideClicked();

    void onStackedWidgetIndexChange(int index);

    void startPracticeLesson();
};

#endif // WORDSLESSON_H
