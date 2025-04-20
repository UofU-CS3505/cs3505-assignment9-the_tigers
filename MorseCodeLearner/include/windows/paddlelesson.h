#ifndef PADDLELESSON_H
#define PADDLELESSON_H

#include <QWidget>

namespace Ui {
class PaddleLesson;
}

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

#endif // PADDLELESSON_H
