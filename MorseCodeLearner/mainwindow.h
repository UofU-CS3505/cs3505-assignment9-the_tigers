#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /**
     * Called when navigating to the Translator Page.
     */
    void onTranslatorNavClicked();

    /**
     * Called when navigating to the Practice Page.
     */
    void onPracticeNavClicked();

    /**
     * Called when navigating to the Learning Page.
     */
    void onLearningNavClicked();

    /**
     * Called when a back/home button is pressed from any page.
     */
    void goHome();

private:
    Ui::MainWindow *ui;

    /**
     * A simple method to set up all the pages with the QStackedWidget.
     */
    void setUpPages();
};
#endif // MAINWINDOW_H
