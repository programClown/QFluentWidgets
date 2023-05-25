#ifndef MASKDIALOGBASE_H
#define MASKDIALOGBASE_H

#include <QDialog>
#include <QHBoxLayout>
#include <QFrame>

class MaskDialogBase : public QDialog
{
    Q_OBJECT
public:
    explicit MaskDialogBase(QWidget *parent = nullptr);

    void setShadowEffect(float blurRadius = 60, const QPointF &offset = QPoint(0, 10),
                         const QColor &color = QColor(0, 0, 0, 100));
    void setMaskColor(const QColor &color);

    QFrame *widget;
    QWidget *windowMask;

    // QWidget interface
    QHBoxLayout *hBoxLayout() const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

signals:

private:
    QHBoxLayout *m_hBoxLayout;
};

#endif  // MASKDIALOGBASE_H
