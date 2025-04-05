#ifndef PRACTICEWINDOW_H
#define PRACTICEWINDOW_H

#include <QWidget>

namespace Ui {
class practicewindow;
}

class practicewindow : public QWidget
{
    Q_OBJECT

public:
    explicit practicewindow(QWidget *parent = nullptr);
    ~practicewindow();

private:
    Ui::practicewindow *ui;
};

#endif // PRACTICEWINDOW_H
