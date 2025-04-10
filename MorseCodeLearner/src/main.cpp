#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont(":/fonts/OldLondon.ttf");
    QFontDatabase::addApplicationFont(":/fonts/JMHTypewriter.ttf");

    KeyEventFilter *keyEventFilter = new KeyEventFilter(&a);
    a.installEventFilter(keyEventFilter);

    MorseHandler morseHandler(MorseHandler::STRAIGHT_KEY, 12); // Param should be based on the user's saved settings, default to 10 otherwise

    MainWindow menu(nullptr, &morseHandler, keyEventFilter);
    menu.setFixedSize(1280, 720);
    menu.setWindowIcon(QIcon(":/icons/app_icon.png"));

    menu.show();
    return a.exec();
}
