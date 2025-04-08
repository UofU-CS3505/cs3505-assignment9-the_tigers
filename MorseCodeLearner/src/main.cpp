#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    KeyEventFilter *keyEventFilter = new KeyEventFilter(&a);
    a.installEventFilter(keyEventFilter);

    MorseHandler morseHandler(10); // Param should be based on the user's saved settings, default to 10 otherwise
    MorseAudioHandler audioHandler;

    MainWindow menu(nullptr, &morseHandler, &audioHandler, keyEventFilter);
    menu.setFixedSize(1280, 720);
    menu.setWindowIcon(QIcon(":/icons/app_icon.png"));

    menu.show();
    return a.exec();
}
