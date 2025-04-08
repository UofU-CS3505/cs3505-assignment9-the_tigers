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
    void setUserOnThisPage(bool userOnThisPage);
    bool getUserOnThisPage();

private:
    Ui::lessonselectwindow *ui;
    bool userOnThisPage;

signals:
    void goHome();
private slots:
    void on_backButton_clicked();
};

#endif // LESSONSELECTWINDOW_H
