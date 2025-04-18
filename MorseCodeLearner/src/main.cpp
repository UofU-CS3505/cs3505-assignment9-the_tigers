#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFontDatabase::addApplicationFont(":/fonts/Parchment.ttf");
    QFontDatabase::addApplicationFont(":/fonts/OldLondon.ttf");
    QFontDatabase::addApplicationFont(":/fonts/OldNewspaperTypes.ttf");

    KeyEventFilter *keyEventFilter = new KeyEventFilter(&a);
    a.installEventFilter(keyEventFilter);

    MorseHandler morseHandler(MorseHandler::STRAIGHT_KEY, 12);
    morseHandler.setVolume(100);

    PracticeHandler practiceHandler(&morseHandler);
    LessonHandler *lessonHandler = new LessonHandler(&morseHandler);

    MainWindow menu(nullptr, &morseHandler, keyEventFilter, &practiceHandler, lessonHandler);
    menu.setFixedSize(1280, 720);
    menu.setWindowIcon(QIcon(":/icons/app_icon.png"));

    menu.show();
    return a.exec();
}
