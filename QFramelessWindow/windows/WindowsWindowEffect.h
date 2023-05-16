#ifndef WINDOWSWINDOWEFFECT_H
#define WINDOWSWINDOWEFFECT_H

#include <QWidget>

#include "WindowCompositionAttribute.h"

struct ACCENT_POLICY;
struct WINDOWCOMPOSITIONATTRIBDATA;

class WindowsWindowEffect : public QWidget
{
    Q_OBJECT

public:
    WindowsWindowEffect(QWidget *parent);

    bool setAcrylicEffect(HWND hWnd, std::string gradientColor = "F2F2F299", bool enableShadow = true,
                          uint32_t animationId = 0);
    bool setMicaEffect(HWND hWnd, bool isDarkMode = true);
    bool setAeroEffect(HWND hWnd);
    bool removeBackgroundEffect(HWND hWnd);

    static void moveWindow(HWND hWnd);

    bool addShadowEffect(HWND hWnd);
    bool addMenuShadowEffect(HWND hWnd);
    bool removeShadowEffect(HWND hWnd);

    static void removeMenuShadowEffect(HWND hWnd);
    static void addWindowAnimation(HWND hWnd);

private:
    ACCENT_POLICY accentPolicy;
    WINDOWCOMPOSITIONATTRIBDATA winCompAttrData;
};

#endif  // WINDOWSWINDOWEFFECT_H
