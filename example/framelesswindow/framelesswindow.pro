QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 framelesshelper

include($$PWD/../baseconfig.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AcrylicDemo.cpp \
    CustomWidget.cpp \
    Dialog.cpp \
    Widget.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    AcrylicDemo.h \
    CustomWidget.h \
    Dialog.h \
    MainWindow.h \
    Widget.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
