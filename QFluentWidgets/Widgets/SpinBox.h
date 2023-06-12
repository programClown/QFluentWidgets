#ifndef SPINBOX_H
#define SPINBOX_H

#include "Common/Icon.h"

#include <QToolButton>
#include <QSpinBox>
#include <QBoxLayout>
#include <QTimeEdit>

class SpinIcon : public FluentIconBase
{
public:
    enum IconType
    {
        UP = 0,
        DOWN
    };
    //    const QString UP = "UP";
    //    const QString DOWN     = "DOWN";
    static QString iconName(IconType type);

    SpinIcon(IconType type, Qfw::Theme t = Qfw::DARK);
    ~SpinIcon();

    QString iconPath();

    // FluentIconBase interface
    QIcon icon() override;
    QString typeName() const override;
    FluentIconBase *clone() override;
    Qfw::Theme theme() const;
    void setTheme(const Qfw::Theme &theme) override;

private:
    Qfw::Theme m_theme;
    IconType m_type;
};

class SpinButton : public QToolButton
{
    Q_OBJECT
public:
    explicit SpinButton(FluentIconBase *icon, QWidget *parent = nullptr);

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    QScopedPointer<FluentIconBase> m_icon;
};

class SpinBoxBase : public QObject
{
    Q_OBJECT
public:
    SpinBoxBase(QAbstractSpinBox *parent = nullptr) : QObject(parent), m_parent(parent) { }

    QHBoxLayout *hBoxLayout;
    SpinButton *upButton;
    SpinButton *downButton;

protected:
    virtual void setUpUi();
    virtual void showContextMenu(const QPoint &pos);
    virtual void drawBorderBottom();
    virtual QLineEdit *lineEdit() const = 0;

private:
    QAbstractSpinBox *m_parent;
};

class SpinBox : public QSpinBox, public SpinBoxBase
{
    Q_OBJECT
public:
    explicit SpinBox(QWidget *parent = nullptr);

    // SpinBoxBase interface
protected:
    virtual QLineEdit *lineEdit() const override;
    virtual void paintEvent(QPaintEvent *event) override;

signals:
};

class DoubleSpinBox : public QDoubleSpinBox, public SpinBoxBase
{
    Q_OBJECT
public:
    explicit DoubleSpinBox(QWidget *parent = nullptr);

    // SpinBoxBase interface
protected:
    virtual QLineEdit *lineEdit() const override;
    virtual void paintEvent(QPaintEvent *event) override;

signals:
};

class TimeEdit : public QTimeEdit, public SpinBoxBase
{
    Q_OBJECT
public:
    explicit TimeEdit(QWidget *parent = nullptr);

    // SpinBoxBase interface
protected:
    virtual QLineEdit *lineEdit() const override;
    virtual void paintEvent(QPaintEvent *event) override;

signals:
};

class DateTimeEdit : public QDateTimeEdit, public SpinBoxBase
{
    Q_OBJECT
public:
    explicit DateTimeEdit(QWidget *parent = nullptr);

    // SpinBoxBase interface
protected:
    virtual QLineEdit *lineEdit() const override;
    virtual void paintEvent(QPaintEvent *event) override;

signals:
};

class DateEdit : public QDateEdit, public SpinBoxBase
{
    Q_OBJECT
public:
    explicit DateEdit(QWidget *parent = nullptr);

    // SpinBoxBase interface
protected:
    virtual QLineEdit *lineEdit() const override;
    virtual void paintEvent(QPaintEvent *event) override;

signals:
};

#endif  // SPINBOX_H
