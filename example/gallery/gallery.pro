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
    common/Translator.cpp \
    common/Trie.cpp \
    components/AvatarWidget.cpp \
    components/LinkCard.cpp \
    components/SampleCard.cpp \
    main.cpp \
    view/BasicInputInterface.cpp \
    view/DateTimeInterface.cpp \
    view/DialogInterface.cpp \
    view/GalleryInterface.cpp \
    view/GalleryTitleBar.cpp \
    view/HomeInterface.cpp \
    view/IconInterface.cpp \
    view/LayoutInterface.cpp \
    view/MainWindow.cpp \
    view/MaterialInterface.cpp \
    view/MenuInterface.cpp \
    view/ScrollInterface.cpp

HEADERS += \
    common/GalleryConfig.h \
    common/GalleryStyleSheet.h \
    common/SignalBus.h \
    common/Translator.h \
    common/Trie.h \
    components/AvatarWidget.h \
    components/LinkCard.h \
    components/SampleCard.h \
    view/BasicInputInterface.h \
    view/DateTimeInterface.h \
    view/DialogInterface.h \
    view/GalleryInterface.h \
    view/GalleryTitleBar.h \
    view/HomeInterface.h \
    view/IconInterface.h \
    view/LayoutInterface.h \
    view/MainWindow.h \
    view/MaterialInterface.h \
    view/MenuInterface.h \
    view/ScrollInterface.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
