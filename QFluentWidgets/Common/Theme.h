#ifndef THEME_H
#define THEME_H

#include <QString>

namespace Qfw
{
    enum Theme
    {
        LIGHT = 0,
        DARK,
        AUTO,
    };

    static inline QString ThemeString(Theme t)
    {
        switch (t) {
            case LIGHT:
                return "Light";
            case DARK: {
                return "Dark";
            }
            case AUTO: {
                return "Auto";
            }
        }

        // default
        return "Light";
    }

}  // namespace Qfw

#endif  // THEME_H
