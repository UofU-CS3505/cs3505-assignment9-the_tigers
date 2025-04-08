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
    void setUserOnThisPage(bool userOnThisPage);
    bool getUserOnThisPage();

private slots:
    void on_backButton_clicked();

private:
    Ui::practicewindow *ui;
    bool userOnThisPage;

signals:
    void goHome();
};

#endif // PRACTICEWINDOW_H
