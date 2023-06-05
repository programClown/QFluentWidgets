QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 framelesshelper

include($$PWD/../baseconfig.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    common/GalleryConfig.cpp \
    common/GalleryStyleSheet.cpp \
    common/SignalBus.cpp \
    components/AvatarWidget.cpp \
    components/LinkCard.cpp \
    main.cpp \
    view/GalleryTitleBar.cpp \
    view/HomeInterface.cpp \
    view/MainWindow.cpp

HEADERS += \
    common/GalleryConfig.h \
    common/GalleryStyleSheet.h \
    common/SignalBus.h \
    components/AvatarWidget.h \
    components/LinkCard.h \
    view/GalleryTitleBar.h \
    view/HomeInterface.h \
    view/MainWindow.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
