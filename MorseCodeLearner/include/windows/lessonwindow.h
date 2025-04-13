#ifndef LESSONWINDOW_H
#define LESSONWINDOW_H

#include <QWidget>
#include "keyeventfilter.h"

namespace Ui {
class lessonwindow;
}

class lessonwindow : public QWidget
{
    Q_OBJECT

public:
    explicit lessonwindow(KeyEventFilter *keyEventFilter = nullptr, QWidget *parent = nullptr);
    ~lessonwindow();
    void setUserOnThisPage(bool userOnThisPage);
    bool getUserOnThisPage();

private:
    Ui::lessonwindow *ui;
    KeyEventFilter *keyEventFilter;
    bool userOnThisPage;

signals:
    void goToLessonSelect();
private slots:
    void on_backButton_clicked();
};



#endif // LESSONWINDOW_H
