QT += widgets svg xml

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
    win32:TARGET = qfluentwidgetsd
    else:TARGET = qfluentwidgets
} else {
    TARGET = qfluentwidgets
}

SOURCES += \
    Common/Config.cpp \
    Common/Icon.cpp \
    Common/SmoothScroll.cpp \
    DateTime/DatePicker.cpp \
    DateTime/PickerBase.cpp \
    DateTime/TimePicker.cpp \
    DialogBox/ColorDialog.cpp \
    DialogBox/Dialog.cpp \
    DialogBox/FolderListDialog.cpp \
    DialogBox/HuePanel.cpp \
    DialogBox/MaskDialogBase.cpp \
    DialogBox/MessageDialog.cpp \
    Layout/ExpandLayout.cpp \
    Layout/FlowLayout.cpp \
    Layout/VBoxLayout.cpp \
    Navigation/NavigationInterface.cpp \
    Navigation/NavigationPanel.cpp \
    Navigation/NavigationWidget.cpp \
    QFluentWidgets.cpp \
    Common/AutoWrap.cpp \
    Common/ImageUtils.cpp \
    Common/StyleSheet.cpp \
    Settings/CustomColorSettingCard.cpp \
    Settings/ExpandGroupSettingCard.cpp \
    Settings/FolderListSettingCard.cpp \
    Settings/OptionsSettingCard.cpp \
    Settings/SettingCard.cpp \
    Settings/SettingCardGroup.cpp \
    Widgets/AcrylicLabel.cpp \
    Widgets/Button.cpp \
    Widgets/CheckBox.cpp \
    Widgets/ComboBox.cpp \
    Widgets/CycleListWidget.cpp \
    Widgets/IconWidget.cpp \
    Widgets/InfoBar.cpp \
    Widgets/Label.cpp \
    Widgets/LineEdit.cpp \
    Widgets/Menu.cpp \
    Widgets/MultiSelectComboBox.cpp \
    Widgets/ScrollArea.cpp \
    Widgets/Slider.cpp \
    Widgets/SpinBox.cpp \
    Widgets/StackedWidget.cpp \
    Widgets/StateToolTip.cpp \
    Widgets/SwitchButton.cpp \
    Widgets/ThreeStateButton.cpp \
    Widgets/ToolTip.cpp \
    Widgets/TreeView.cpp

HEADERS += \
    Common/Config.h \
    Common/Icon.h \
    Common/SmoothScroll.h \
    Common/Theme.h \
    Common/enum.h \
    DateTime/DatePicker.h \
    DateTime/PickerBase.h \
    DateTime/TimePicker.h \
    DialogBox/ColorDialog.h \
    DialogBox/Dialog.h \
    DialogBox/FolderListDialog.h \
    DialogBox/HuePanel.h \
    DialogBox/MaskDialogBase.h \
    DialogBox/MessageDialog.h \
    Layout/ExpandLayout.h \
    Layout/FlowLayout.h \
    Layout/VBoxLayout.h \
    Navigation/NavigationInterface.h \
    Navigation/NavigationPanel.h \
    Navigation/NavigationWidget.h \
    QFluentWidgets.h \
    Common/AutoWrap.h \
    Common/ImageUtils.h \
    Common/StyleSheet.h \
    Settings/CustomColorSettingCard.h \
    Settings/ExpandGroupSettingCard.h \
    Settings/FolderListSettingCard.h \
    Settings/OptionsSettingCard.h \
    Settings/SettingCard.h \
    Settings/SettingCardGroup.h \
    Widgets/AcrylicLabel.h \
    Widgets/Button.h \
    Widgets/CheckBox.h \
    Widgets/ComboBox.h \
    Widgets/CycleListWidget.h \
    Widgets/IconWidget.h \
    Widgets/InfoBar.h \
    Widgets/Label.h \
    Widgets/LineEdit.h \
    Widgets/Menu.h \
    Widgets/MultiSelectComboBox.h \
    Widgets/ScrollArea.h \
    Widgets/Slider.h \
    Widgets/SpinBox.h \
    Widgets/StackedWidget.h \
    Widgets/StateToolTip.h \
    Widgets/SwitchButton.h \
    Widgets/ThreeStateButton.h \
    Widgets/ToolTip.h \
    Widgets/TreeView.h

#-------------------------------------------------------------------------------
# Build options
#-------------------------------------------------------------------------------
DESTDIR = $$PWD/../bin      #指定生成的应用程序放置的路径
LIBS += -L$$PWD/../bin      #指定生成的库文件放置的路径，与应用程序放在一起

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qfluentwidgets.qrc
