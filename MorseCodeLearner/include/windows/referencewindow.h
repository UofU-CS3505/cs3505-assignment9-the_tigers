#ifndef REFERENCEWINDOW_H
#define REFERENCEWINDOW_H

#include <QWidget>

namespace Ui {
class referencewindow;
}

class referencewindow : public QWidget
{
    Q_OBJECT

public:
    explicit referencewindow(QWidget *parent = nullptr);
    ~referencewindow();
    void setUserOnThisPage(bool userOnThisPage);
    bool getUserOnThisPage();

private:
    Ui::referencewindow *ui;
    bool userOnThisPage;

signals:
    void goHome();

private slots:
    void on_backButton_clicked();
};

#endif // REFERENCEWINDOW_H
