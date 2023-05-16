#ifndef PICKERBASE_H
#define PICKERBASE_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QBoxLayout>
#include <QPropertyAnimation>
#include <QGraphicsDropShadowEffect>

#include "Common/StyleSheet.h"

class CycleListWidget;
class TransparentToolButton;
class PickerPanel;

class SeparatorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SeparatorWidget(Qt::Orientation orient, QWidget *parent = nullptr) : QWidget(parent)
    {
        if (orient == Qt::Horizontal) {
            setFixedHeight(1);
        } else {
            setFixedWidth(1);
        }

        setAttribute(Qt::WA_StyledBackground);
        FluentStyleSheet::apply("TIME_PICKER", this);
    }
};

class ItemMaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ItemMaskWidget(const QList<CycleListWidget *> &list, QWidget *parent = nullptr);

    QList<CycleListWidget *> listWidgets;

protected:
    virtual void paintEvent(QPaintEvent *event) override;

    void drawText(QListWidgetItem *item, QPainter *p, int y);
};

class PickerColumnFormatter : public QObject
{
public:
    PickerColumnFormatter(QObject *parent = nullptr) : QObject(parent) { }

    virtual QVariant encode(const QVariant &value) { return value; }

    virtual QVariant decode(const QVariant &value) { return value; }
};

class DigitFormatter : public PickerColumnFormatter
{
    Q_OBJECT
public:
    DigitFormatter(QObject *parent = nullptr) : PickerColumnFormatter(parent) { }

    //    virtual QVariant decode(const QVariant &value) { return value.toInt(); }
};

class PickerColumnButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PickerColumnButton(const QString &name, const QVariantList &items, int width,
                                Qt::AlignmentFlag align = Qt::AlignLeft, PickerColumnFormatter *formatter = nullptr,
                                QWidget *parent = nullptr);

    QVariantList items() const;
    void setItems(const QVariantList &items);
    QString value() const;
    void setValue(const QString &value);
    Qt::AlignmentFlag alignment() const;
    void setAlignment(const Qt::AlignmentFlag &align);
    PickerColumnFormatter *formatter() const;
    void setFormatter(PickerColumnFormatter *formatter);
    QString name() const;
    void setName(const QString &name);

signals:

private:
    QString m_name;
    QString m_value;
    QVariantList m_items;
    Qt::AlignmentFlag m_align;

    PickerColumnFormatter *m_formatter;
};

class PickerBase : public QPushButton
{
    Q_OBJECT
public:
    explicit PickerBase(QWidget *parent = nullptr);
    void addColumn(const QString &name, const QVariantList &items, int width, Qt::AlignmentFlag align = Qt::AlignLeft,
                   PickerColumnFormatter *formatter = nullptr);

    void setColumnAlignment(int index, Qt::AlignmentFlag align = Qt::AlignCenter);
    void setColumnWidth(int index, int width);
    void setColumnTight(int index);
    void setColumnVisible(int index, bool visible);
    QStringList value() const;
    void setColumnValue(int index, int value);
    void setColumnFormatter(int index, PickerColumnFormatter *formatter);
    void setColumnItems(int index, const QVariantList &items);
    QVariant encodeValue(int index, const QVariant &value);
    QVariant decodeValue(int index, const QVariant &value);

    QStringList panelInitialValue();

    void setColumn(int index, const QString &name, const QVariantList &items, int width,
                   Qt::AlignmentFlag align = Qt::AlignCenter);

    void clearColumns();

public:
    QList<PickerColumnButton *> columns;
    QHBoxLayout *hBoxLayout;

signals:

public slots:
    void showPanel();
    virtual void onConfirmed(const QStringList &value);
    virtual void onColumnValueChanged(PickerPanel *panel, int index, const QString &value);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void setButtonProperty(const char *name, const QVariant &value);
};

class PickerPanel : public QWidget
{
    Q_OBJECT
public:
    explicit PickerPanel(QWidget *parent = nullptr);

    void setShadowEffect(int blurRadius = 30, const QPointF &offset = QPointF(0, 8),
                         const QColor &color = QColor(0, 0, 0, 30));

    void addColumn(const QVariantList &items, int width, Qt::AlignmentFlag align = Qt::AlignCenter);

    QStringList value() const;
    void setValue(const QStringList &value);

    QString columnValue(int index) const;
    void setColumnValue(int index, const QString &value);

    CycleListWidget *column(int index) const;
    void exec(const QPoint &pos, bool ani = true);

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:
    void confirmed(const QStringList &);
    void columnValueChanged(int, const QString &);

private:
    void initWidget();

private slots:
    void fadeOut();
    void onAniValueChanged(const QVariant &value);

private:
    int m_itemHeight;
    QList<CycleListWidget *> m_listWidgets;
    QFrame *m_view;
    ItemMaskWidget *m_itemMaskWidget;
    SeparatorWidget *m_hSeparatorWidget;
    TransparentToolButton *m_yesButton;
    TransparentToolButton *m_cancelButton;
    QHBoxLayout *m_hBoxLayout;
    QHBoxLayout *m_listLayout;
    QHBoxLayout *m_buttonLayout;
    QVBoxLayout *m_vBoxLayout;
    bool m_isExpanded;
    QPropertyAnimation *m_ani;
    QGraphicsDropShadowEffect *m_shadowEffect;
};

#endif  // PICKERBASE_H
