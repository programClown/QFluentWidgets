#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <QCheckBox>

class CheckBox : public QCheckBox
{
    Q_OBJECT
public:
    explicit CheckBox(const QString &text, QWidget *parent = nullptr);

signals:
};

#endif  // CHECKBOX_H
