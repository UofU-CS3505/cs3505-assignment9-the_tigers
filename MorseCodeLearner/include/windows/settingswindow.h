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
    void setUserOnThisPage(bool userOnThisPage);
    bool getUserOnThisPage();

signals:
    void goHome();

private slots:
    void on_backButton_clicked();

private:
    Ui::settingswindow *ui;
    bool userOnThisPage;
};

#endif // SETTINGSWINDOW_H
