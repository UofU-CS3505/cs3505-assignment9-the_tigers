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
    void setUserOnThisPage(bool userOnThisPage);
    bool getUserOnThisPage();

private:
    Ui::lessonwindow *ui;
    bool userOnThisPage;

signals:
    void goToLessonSelect();
private slots:
    void on_backButton_clicked();
};



#endif // LESSONWINDOW_H
