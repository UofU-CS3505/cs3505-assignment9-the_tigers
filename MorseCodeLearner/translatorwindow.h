#ifndef TRANSLATORWINDOW_H
#define TRANSLATORWINDOW_H

#include <QWidget>

namespace Ui {
class translatorwindow;
}

class translatorwindow : public QWidget
{
    Q_OBJECT

public:
    explicit translatorwindow(QWidget *parent = nullptr);
    ~translatorwindow();

private:
    Ui::translatorwindow *ui;
};

#endif // TRANSLATORWINDOW_H
