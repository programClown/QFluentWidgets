QT       += core gui
win32: {
    QT += winextras
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

include($$PWD/../baseconfig.pri)

INCLUDEPATH += $$PWD/../../QFramelessWindow
INCLUDEPATH += $$PWD/../../QFramelessWindow/windows
INCLUDEPATH += $$PWD/../../QFramelessWindow/titlebar

CONFIG(debug, release|debug){
    win32:LIBS += -lqframelesswindowd -luser32 -lDwmapi
    else: LIBS += -lqframelesswindow -luser32 -lDwmapi
} else {
    LIBS += -lqframelesswindow
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
#    AWindow.cpp \
#    DWindow.cpp \
#    FWindow.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
#    AWindow.h \
#    DWindow.h \
#    FWindow.h \
    MainWindow.h

FORMS += \
    MainWindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc



