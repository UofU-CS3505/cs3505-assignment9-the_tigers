#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

namespace Ui {
class settingswindow;
}

class settingswindow : public QWidget
{
    Q_OBJECT

public:
    explicit settingswindow(QWidget *parent = nullptr);
    ~settingswindow();

signals:
    void goHome();

private slots:
    void on_backButton_clicked();

private:
    Ui::settingswindow *ui;
};

#endif // SETTINGSWINDOW_H
