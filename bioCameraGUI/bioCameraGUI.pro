#-------------------------------------------------
#
# Project created by QtCreator 2018-07-22T23:48:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bioCameraGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    celexsensordll.h

FORMS    += mainwindow.ui

linux-g++ {
    contains(QT_ARCH, i386) {
        LIBS += -L$$PWD/lib/linux_x86 -lokFrontPanel -lCelexSensor }
    else {
        LIBS += -L$$PWD/lib/linux_x64 -lokFrontPanel -lCelexSensor }
}
win32 {
    contains(QT_ARCH, i386) {
        LIBS += -L$$PWD/lib/windows_x86/ -lCelexSensor }
    else {
        LIBS += -L$$PWD/lib/windows_x64/ -lCelexSensor }
}

win32 {
    CONFIG(debug) {
        QMAKE_POST_LINK += copy $$shell_path($$PWD)\*.xml $$shell_path($$OUT_PWD)\debug\ &
        QMAKE_POST_LINK += copy $$shell_path($$PWD)\*.dll $$shell_path($$OUT_PWD)\debug\ &
        QMAKE_POST_LINK += copy $$shell_path($$PWD)\FPN.txt $$shell_path($$OUT_PWD)\debug\ &
        QMAKE_POST_LINK += copy $$shell_path($$PWD)\*.bit $$shell_path($$OUT_PWD)\debug\
    }
    else {
        QMAKE_POST_LINK += copy $$shell_path($$PWD)\*.xml $$shell_path($$OUT_PWD)\release\ &
        QMAKE_POST_LINK += copy $$shell_path($$PWD)\*.dll $$shell_path($$OUT_PWD)\release\ &
        QMAKE_POST_LINK += copy $$shell_path($$PWD)\FPN.txt $$shell_path($$OUT_PWD)\release\ &
        QMAKE_POST_LINK += copy $$shell_path($$PWD)\*.bit $$shell_path($$OUT_PWD)\release\
    }
}

linux-g++ {
    QMAKE_POST_LINK += cp $$shell_path($$PWD)/*.xml $$shell_path($$OUT_PWD)/ &
    QMAKE_POST_LINK += cp $$shell_path($$PWD)/FPN.txt $$shell_path($$OUT_PWD)/ &
    QMAKE_POST_LINK += cp $$shell_path($$PWD)/*.bit $$shell_path($$OUT_PWD)/
}
