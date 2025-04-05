#ifndef LESSONWINDOW_H
#define LESSONWINDOW_H

#include <QWidget>

namespace Ui {
class lessonwindow;
}

class lessonwindow : public QWidget
{
    Q_OBJECT

public:
    explicit lessonwindow(QWidget *parent = nullptr);
    ~lessonwindow();

private:
    Ui::lessonwindow *ui;
};

#endif // LESSONWINDOW_H
