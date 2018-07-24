QT += core
QT -= gui

TARGET = storeData_CML
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    storedata.cpp

HEADERS += celexsensordll.h \
    storedata.h

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
