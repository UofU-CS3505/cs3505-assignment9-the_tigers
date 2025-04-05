#ifndef LESSONSELECTWINDOW_H
#define LESSONSELECTWINDOW_H

#include <QWidget>

namespace Ui {
class lessonselectwindow;
}

class lessonselectwindow : public QWidget
{
    Q_OBJECT

public:
    explicit lessonselectwindow(QWidget *parent = nullptr);
    ~lessonselectwindow();

private:
    Ui::lessonselectwindow *ui;
};

#endif // LESSONSELECTWINDOW_H
