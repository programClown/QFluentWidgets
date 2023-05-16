#include "AutoWrap.h"

#include <QStringList>

static CharWidthType char_widths[] = {
    { 126, 1 },   { 159, 0 },   { 687, 1 },   { 710, 0 },    { 711, 1 },    { 727, 0 },     { 733, 1 },   { 879, 0 },
    { 1154, 1 },  { 1161, 0 },  { 4347, 1 },  { 4447, 2 },   { 7467, 1 },   { 7521, 0 },    { 8369, 1 },  { 8426, 0 },
    { 9000, 1 },  { 9002, 2 },  { 11021, 1 }, { 12350, 2 },  { 12351, 1 },  { 12438, 2 },   { 12442, 0 }, { 19893, 2 },
    { 19967, 1 }, { 55203, 2 }, { 63743, 1 }, { 64106, 2 },  { 65039, 1 },  { 65059, 0 },   { 65131, 2 }, { 65279, 1 },
    { 65376, 2 }, { 65500, 1 }, { 65510, 2 }, { 120831, 1 }, { 262141, 2 }, { 1114109, 1 },
};

TextWrap::TextWrap() { }

int TextWrap::getWidth(QChar c)
{
    if (c == 0xe || c == 0xf) {
        return 0;
    }

    for (auto nw : char_widths) {
        if (c <= nw.num) {
            return nw.wid;
        }
    }

    return 1;
}

/**
 * @brief Wrap according to string length
 * @param text: the text to be wrapped
 * @param width: the maximum length of a single line, the length of Chinese characters is 2
 * @param once: whether to wrap only once
 * @return <wrap_text(QString): text after auto word wrap process, is_wrapped(bool): whether a line break occurs in the
 * text>
 */
QPair<QString, bool> TextWrap::wrap(const QString &text, int width, bool once)
{
    int count     = 0;
    int lastCount = 0;
    QString chars;
    bool isWrapped    = false;
    int breakPos      = 0;
    bool isBreakAlpha = true;
    int nInsideBreak  = 0;

    int i       = 0;
    QString txt = text.trimmed();
    while (i < txt.count()) {
        QChar c    = txt.at(i);
        int length = TextWrap::getWidth(c);
        count += length;

        // record the position of blank character
        if (c == " " || length > 1) {
            breakPos     = i + nInsideBreak;
            lastCount    = count;
            isBreakAlpha = (length == 1);
        }

        // No line breaks
        if (count <= width) {
            chars.append(c);
            i++;
            continue;
        }

        // wrap at the position of the previous space
        if (breakPos > 0 && isBreakAlpha) {
            if (c != " ") {
                chars[breakPos] = '\n';
                chars.append(c);

                // Wrap inside long words
                if (lastCount != 0) {
                    count -= lastCount;
                    lastCount = 0;
                } else {
                    chars.insert(i, '\n');
                    breakPos = i;
                    nInsideBreak += 1;
                }
            } else {
                chars.append('\n');
                count     = 0;
                lastCount = 0;
            }
        } else {
            chars.append('\n');
            chars.append(c);
            count = length;
        }

        isWrapped = true;

        // early return
        if (once) {
            return QPair<QString, bool>(chars + txt.mid(i + 1), true);
        }
    }

    return QPair<QString, bool>(chars, true);
}
