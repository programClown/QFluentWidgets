#ifndef WINDOWCOMPOSITIONATTRIBUTE_H
#define WINDOWCOMPOSITIONATTRIBUTE_H

#include <Windows.h>
#include <dwmapi.h>

enum WINDOWCOMPOSITIONATTRIB
{
    WCA_UNDEFINED                     = 0,
    WCA_NCRENDERING_ENABLED           = 1,
    WCA_NCRENDERING_POLICY            = 2,
    WCA_TRANSITIONS_FORCEDISABLED     = 3,
    WCA_ALLOW_NCPAINT                 = 4,
    WCA_CAPTION_BUTTON_BOUNDS         = 5,
    WCA_NONCLIENT_RTL_LAYOUT          = 6,
    WCA_FORCE_ICONIC_REPRESENTATION   = 7,
    WCA_EXTENDED_FRAME_BOUNDS         = 8,
    WCA_HAS_ICONIC_BITMAP             = 9,
    WCA_THEME_ATTRIBUTES              = 10,
    WCA_NCRENDERING_EXILED            = 11,
    WCA_NCADORNMENTINFO               = 12,
    WCA_EXCLUDED_FROM_LIVEPREVIEW     = 13,
    WCA_VIDEO_OVERLAY_ACTIVE          = 14,
    WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 15,
    WCA_DISALLOW_PEEK                 = 16,
    WCA_CLOAK                         = 17,
    WCA_CLOAKED                       = 18,
    WCA_ACCENT_POLICY                 = 19,
    WCA_FREEZE_REPRESENTATION         = 20,
    WCA_EVER_UNCLOAKED                = 21,
    WCA_VISUAL_OWNER                  = 22,
    WCA_HOLOGRAPHIC                   = 23,
    WCA_EXCLUDED_FROM_DDA             = 24,
    WCA_PASSIVEUPDATEMODE             = 25,
    WCA_USEDARKMODECOLORS             = 26,
    WCA_CORNER_STYLE                  = 27,
    WCA_PART_COLOR                    = 28,
    WCA_DISABLE_MOVESIZE_FEEDBACK     = 29,
    WCA_LAST                          = 30
};

/**
 * @brief Client area status enumeration class
 */
enum ACCENT_STATE
{
    ACCENT_DISABLED                   = 0,
    ACCENT_ENABLE_GRADIENT            = 1,
    ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
    ACCENT_ENABLE_BLURBEHIND          = 3,  // Aero effect
    ACCENT_ENABLE_ACRYLICBLURBEHIND   = 4,  // Acrylic effect
    ACCENT_ENABLE_HOSTBACKDROP        = 5,  // Mica effect
    ACCENT_INVALID_STATE              = 6
};

/**
 * @brief Specific attributes of client area
 */
struct ACCENT_POLICY
{
    ACCENT_STATE AccentState;
    DWORD AccentFlags;
    DWORD GradientColor;
    DWORD AnimationId;
};

struct WINDOWCOMPOSITIONATTRIBDATA
{
    WINDOWCOMPOSITIONATTRIB Attribute;
    PVOID Data;  // Pointer() receives any ctypes type and returns a pointer type
    SIZE_T SizeOfData;
};

// user32.dll
// WINUSERAPI BOOL WINAPI SetWindowCompositionAttribute(_In_ HWND hWnd, _Inout_ WINDOWCOMPOSITIONATTRIBDATA *pAttrData);
typedef BOOL(WINAPI *SetWindowCompositionAttribute)(HWND, WINDOWCOMPOSITIONATTRIBDATA *);

#endif  // WINDOWCOMPOSITIONATTRIBUTE_H
