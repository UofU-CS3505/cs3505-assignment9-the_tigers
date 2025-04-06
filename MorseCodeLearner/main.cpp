#include "mainwindow.h"
#include "morsehandler.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MorseHandler morseHandler(10); // Param should be based on the user's saved settings, default to 10 otherwise
    MainWindow menu(nullptr, &morseHandler);
    menu.show();
    return a.exec();
}
