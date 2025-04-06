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

signals:
    /**
     * Signal that gets emitted when the back button is pressed.
     */
    void goHome();

private slots:
    /**
     * Slot that gets called when the back button is pressed; emits the
     * goBack() signal.
     */
    void onBackButtonClicked();

private:
    Ui::translatorwindow *ui;
};

#endif // TRANSLATORWINDOW_H
