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

private slots:
    void on_backButton_clicked();

private:
    Ui::practicewindow *ui;

signals:
    void goHome();
};

#endif // PRACTICEWINDOW_H
