#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>
#include <QTimer>
#include "Box2D/Box2D.h"

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
    void setupWorld();
    void updateWorld();

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

    b2World world;
    b2Body* lessonBody;
    b2Body* translatorBody;
    b2Body* practiceBody;
    b2Body* settingsBody;
    b2Body* helpBody;
    b2Body* referenceBody;

    int practiceButtonY;
    int lessonButtonY;
    int translatorButtonY;
    int settingsButtonY;
    int helpButtonY;
    int referenceButtonY;

    QTimer timer;

    bool eventFilter(QObject *object, QEvent *event);
    void jumpPractice();
    void jumpLesson();
    void jumpTranslator();
    void jumpSettings();
    void jumpHelp();
    void jumpReference();
};

#endif // MENUWINDOW_H
