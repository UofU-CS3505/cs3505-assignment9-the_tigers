#ifndef NUMBERSBASICS_H
#define NUMBERSBASICS_H

#include <QWidget>

namespace Ui {
class NumbersBasics;
}

class NumbersBasics : public QWidget
{
    Q_OBJECT

private:
    Ui::NumbersBasics *ui;
    int currentIndex;
    bool userOnThisPage;

public:
    explicit NumbersBasics(QWidget *parent = nullptr);
    ~NumbersBasics();

signals:
    void goBack();

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

    void completeLesson();
};

#endif // NUMBERSBASICS_H
