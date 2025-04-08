QT       += core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/include/util \
               $$PWD/include/windows \
               $$PWD/src/util \
               $$PWD/src/windows

SOURCES += \
    src/util/keyeventfilter.cpp \
    src/windows/lessonselectwindow.cpp \
    src/windows/lessonwindow.cpp \
    src/main.cpp \
    src/windows/mainwindow.cpp \
    src/windows/menuwindow.cpp \
    src/util/morseaudiohandler.cpp \
    src/util/morsehandler.cpp \
    src/windows/practicewindow.cpp \
    src/windows/settingswindow.cpp \
    src/util/sinewavegenerator.cpp \
    src/windows/translatorwindow.cpp

HEADERS += \
    include/util/keyeventfilter.h \
    include/windows/lessonselectwindow.h \
    include/windows/lessonwindow.h \
    include/windows/mainwindow.h \
    include/windows/menuwindow.h \
    include/util/morseaudiohandler.h \
    include/util/morsehandler.h \
    include/windows/practicewindow.h \
    include/windows/settingswindow.h \
    include/util/sinewavegenerator.h \
    include/windows/translatorwindow.h

FORMS += \
    lessonselectwindow.ui \
    lessonwindow.ui \
    mainwindow.ui \
    menuwindow.ui \
    practicewindow.ui \
    settingswindow.ui \
    translatorwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
