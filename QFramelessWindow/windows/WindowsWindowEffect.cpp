#include "WindowsWindowEffect.h"
#include "utils/Win32Utils.h"

#include <dwmapi.h>
#include <QMessageBox>
#include <QtWin>

WindowsWindowEffect::WindowsWindowEffect(QWidget *parent) : QWidget(parent)
{
    winCompAttrData.Attribute  = WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY;
    winCompAttrData.SizeOfData = sizeof(accentPolicy);
    winCompAttrData.Data       = &accentPolicy;
}

bool WindowsWindowEffect::setAcrylicEffect(HWND hWnd, std::string gradientColor, bool enableShadow,
                                           uint32_t animationId)
{
    if (!Win32Utils::isGreaterEqualWin10()) {
        QMessageBox::warning(nullptr, "warning", "The acrylic effect is only available on Win10+");
        return false;
    }

    std::string gradientColorBigEndian;
    for (int index = gradientColor.size(); index > 0; index -= 2) {
        gradientColorBigEndian += gradientColor.substr(index - 2, 2);
    }

    DWORD gradientColorValue = strtol(gradientColorBigEndian.c_str(), NULL, 16);
    DWORD accentFlags        = 0;
    if (enableShadow) {
        accentFlags = 0x20 | 0x40 | 0x80 | 0x100;
    }

    this->accentPolicy.AccentState   = ACCENT_STATE::ACCENT_ENABLE_ACRYLICBLURBEHIND;
    this->accentPolicy.GradientColor = gradientColorValue;
    this->accentPolicy.AccentFlags   = accentFlags;
    this->accentPolicy.AnimationId   = animationId;
    this->winCompAttrData.Attribute  = WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY;

    HMODULE huser = GetModuleHandle(L"user32.dll");
    if (huser) {
        SetWindowCompositionAttribute setWindowCompositionAttribute =
                (SetWindowCompositionAttribute)GetProcAddress(huser, "SetWindowCompositionAttribute");
        if (setWindowCompositionAttribute) {
            setWindowCompositionAttribute(hWnd, &this->winCompAttrData);
            return true;
        }
    }

    return false;
}

/**
 * @brief WindowsWindowEffect::setMicaEffect
 * @param hWnd: Window handle
 * @param isDarkMode: whether to use dark mode mica effect
 * @return
 */
bool WindowsWindowEffect::setMicaEffect(HWND hWnd, bool isDarkMode)
{
    if (!Win32Utils::isGreaterEqualWin11()) {
        QMessageBox::warning(nullptr, "warning", "The Mica effect is only available on Win11");
        return false;
    }

    MARGINS margins = { -1, -1, -1, -1 };
    if (S_OK != DwmExtendFrameIntoClientArea(hWnd, &margins)) {
        return false;
    }

    this->winCompAttrData.Attribute = WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY;
    this->accentPolicy.AccentState  = ACCENT_STATE::ACCENT_ENABLE_HOSTBACKDROP;

    HMODULE huser = GetModuleHandle(L"user32.dll");
    if (!huser) {
        return false;
    }
    SetWindowCompositionAttribute setWindowCompositionAttribute =
            (SetWindowCompositionAttribute)GetProcAddress(huser, "SetWindowCompositionAttribute");
    if (!setWindowCompositionAttribute) {
        return false;
    }
    setWindowCompositionAttribute(hWnd, &this->winCompAttrData);

    if (isDarkMode) {
        this->winCompAttrData.Attribute = WINDOWCOMPOSITIONATTRIB::WCA_USEDARKMODECOLORS;
        setWindowCompositionAttribute(hWnd, &this->winCompAttrData);
    }

    if (QOperatingSystemVersion::current().microVersion() < 22523) {
        DWORD V = 1;
        if (S_OK != DwmSetWindowAttribute(hWnd, 1029, &V, 4)) {
            return false;
        }
    } else {
        DWORD V = 2;
        if (S_OK != DwmSetWindowAttribute(hWnd, 38, &V, 4)) {
            return false;
        }
    }

    return true;
}

bool WindowsWindowEffect::setAeroEffect(HWND hWnd)
{
    this->winCompAttrData.Attribute = WINDOWCOMPOSITIONATTRIB::WCA_ACCENT_POLICY;
    this->accentPolicy.AccentState  = ACCENT_STATE::ACCENT_ENABLE_BLURBEHIND;
    HMODULE huser                   = GetModuleHandle(L"user32.dll");
    if (huser) {
        SetWindowCompositionAttribute setWindowCompositionAttribute =
                (SetWindowCompositionAttribute)GetProcAddress(huser, "SetWindowCompositionAttribute");
        if (setWindowCompositionAttribute) {
            setWindowCompositionAttribute(hWnd, &this->winCompAttrData);
            return true;
        }
    }

    return false;
}

bool WindowsWindowEffect::removeBackgroundEffect(HWND hWnd)
{
    this->accentPolicy.AccentState = ACCENT_STATE::ACCENT_DISABLED;
    HMODULE huser                  = GetModuleHandle(L"user32.dll");
    if (huser) {
        SetWindowCompositionAttribute setWindowCompositionAttribute =
                (SetWindowCompositionAttribute)GetProcAddress(huser, "SetWindowCompositionAttribute");
        if (setWindowCompositionAttribute) {
            setWindowCompositionAttribute(hWnd, &this->winCompAttrData);
            return true;
        }
    }

    return false;
}

void WindowsWindowEffect::moveWindow(HWND hWnd)
{
    ReleaseCapture();
    SendMessage(hWnd, WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
}

/**
 * @brief Add DWM shadow to window
 * @param hWnd
 * @return
 */
bool WindowsWindowEffect::addShadowEffect(HWND hWnd)
{
    if (!QtWin::isCompositionEnabled()) {
        return false;
    }

    MARGINS margins = { -1, -1, -1, -1 };
    if (S_OK != DwmExtendFrameIntoClientArea(hWnd, &margins)) {
        return false;
    }

    return true;
}

/**
 * @brief Add DWM shadow to menu
 * @param hWnd
 * @return
 */
bool WindowsWindowEffect::addMenuShadowEffect(HWND hWnd)
{
    if (!QtWin::isCompositionEnabled()) {
        return false;
    }

    DWORD attribute = DWMNCRENDERINGPOLICY::DWMNCRP_ENABLED;
    if (S_OK != DwmSetWindowAttribute(hWnd, DWMWINDOWATTRIBUTE::DWMWA_NCRENDERING_POLICY, &attribute, 4)) {
        return false;
    }

    MARGINS margins = { -1, -1, -1, -1 };
    if (S_OK != DwmExtendFrameIntoClientArea(hWnd, &margins)) {
        return false;
    }

    return true;
}

/**
 * @brief Remove DWM shadow from the window
 * @param hWnd
 * @return
 */
bool WindowsWindowEffect::removeShadowEffect(HWND hWnd)
{
    DWORD attribute = DWMNCRENDERINGPOLICY::DWMNCRP_DISABLED;
    if (S_OK != DwmSetWindowAttribute(hWnd, DWMWINDOWATTRIBUTE::DWMWA_NCRENDERING_POLICY, &attribute, 4)) {
        return false;
    }
    return true;
}

/**
 * @brief Remove shadow from pop-up menu
 * @param hWnd
 * @return
 */
void WindowsWindowEffect::removeMenuShadowEffect(HWND hWnd)
{
    DWORD style = GetClassLong(hWnd, GCL_STYLE);
    style &= ~CS_DROPSHADOW;

    SetClassLong(hWnd, GCL_STYLE, style);
}

/**
 * @brief Enables the maximize and minimize animation of the window
 * @param hWnd
 */
void WindowsWindowEffect::addWindowAnimation(HWND hWnd)
{
    DWORD style = GetClassLong(hWnd, GCL_STYLE);
    SetWindowLong(hWnd, GWL_STYLE, style | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CAPTION | CS_DBLCLKS | WS_THICKFRAME);
}
