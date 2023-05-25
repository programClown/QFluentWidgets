#ifndef AUTOWRAP_H
#define AUTOWRAP_H

#include <QPair>
#include <QString>

struct CharWidthType
{
    int num;
    int width;
};

class TextWrap
{
public:
    /// Return the screen column width for a char
    static int getWidth(QChar c);

    static QPair<QString, bool> wrap(const QString &text, int width, bool once = true);
};

#endif  // AUTOWRAP_H
