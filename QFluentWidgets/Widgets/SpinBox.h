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
    enum Type
    {
        UP = 0,
        DOWN
    };
    //    const QString UP = "UP";
    //    const QString DOWN     = "DOWN";

    SpinIcon(Type type, Qfw::Theme t = Qfw::DARK);

    // FluentIconBase interface
public:
    QString path() override;
    QIcon icon() override;
    void setTheme(const Qfw::Theme &theme) override;
    Type type() const;
    QString typeName() const;

private:
    Type m_type;
    QString m_name;
    Qfw::Theme m_theme;
};

typedef QSharedPointer<SpinIcon> SpinIconSPtr;

class SpinButton : public QToolButton
{
    Q_OBJECT
public:
    explicit SpinButton(SpinIconSPtr icon, QWidget *parent = nullptr);

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    SpinIconSPtr m_icon;
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
