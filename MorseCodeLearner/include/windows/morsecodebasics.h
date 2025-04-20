#ifndef MORSECODEBASICS_H
#define MORSECODEBASICS_H

#include <QWidget>

namespace Ui {
class MorseCodeBasics;
}

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

#endif // MORSECODEBASICS_H
