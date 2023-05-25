QT += widgets core gui xml svg

win32: {
    QT += winextras
}

TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#-------------------------------------------------------------------------------
# Compiler options
#-------------------------------------------------------------------------------

*g++*: {
    QMAKE_CXXFLAGS_RELEASE -= -O
    QMAKE_CXXFLAGS_RELEASE *= -O3
}

*msvc*: {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
    QMAKE_CXXFLAGS_RELEASE -= /O
    QMAKE_CXXFLAGS_RELEASE *= /O2
}

CONFIG(debug, release|debug){
  win32:TARGET = qframelesswindowd
  else:TARGET = qframelesswindow
} else {
  TARGET = qframelesswindow
}

SOURCES += \
    titlebar/TitleBar.cpp \
    titlebar/TitleBarButton.cpp \
    utils/Win32Utils.cpp \
    widgetResizeHandler/WidgetResizeHandler.cpp \
    windows/WindowsFramelessHelper.cpp

HEADERS += \
    QFramelessWindow.h \
    titlebar/TitleBar.h \
    titlebar/TitleBarButton.h \
    utils/Win32Utils.h \
    widgetResizeHandler/WidgetResizeHandler.h \
    windows/WindowsFramelessHelper.h

#-------------------------------------------------------------------------------
# Build options
#-------------------------------------------------------------------------------
DESTDIR = $$PWD/../bin      #指定生成的应用程序放置的路径
LIBS += -L$$PWD/../bin      #指定生成的库文件放置的路径，与应用程序放在一起

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qframelesswindow.qrc
