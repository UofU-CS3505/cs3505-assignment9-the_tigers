#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>

namespace Ui {
class MenuWindow;
}

class MenuWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();
    void setUserOnThisPage(bool userOnThisPage);
    bool getUserOnThisPage();

signals:
    void goToLessonPage();
    void goToTranslatorPage();
    void goToPracticePage();
    void goToSettingsPage();

private slots:
    void toggleHelp();

private:
    Ui::MenuWindow *ui;
    bool userOnThisPage;
    bool showingHelp;
};

#endif // MENUWINDOW_H
