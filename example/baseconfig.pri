#-------------------------------------------------------------------------------
# Compiler options
#-------------------------------------------------------------------------------
QT += widgets core gui xml svg

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

contains(CONFIG, framelesshelper) {
include($$PWD/share.pri)
include($$PWD/../3rdparty/framelesshelper/qmake/core.pri)
include($$PWD/../3rdparty/framelesshelper/qmake/widgets.pri)
}

INCLUDEPATH += $$PWD/../QFluentWidgets

CONFIG(debug, release|debug){
    win32:LIBS += -lqfluentwidgetsd
    else: LIBS += -lqfluentwidgets
} else {
    LIBS += -lqfluentwidgets
}

#-------------------------------------------------------------------------------
# Build options
#-------------------------------------------------------------------------------
DESTDIR = $$PWD/../bin      #指定生成的应用程序放置的路径
LIBS += -L$$PWD/../bin      #指定生成的库文件放置的路径，与应用程序放在一起
