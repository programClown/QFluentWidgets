#include "Dialog.h"
#include "Widgets/Button.h"
#include "Common/StyleSheet.h"
#include "Common/AutoWrap.h"

#include <QDialog>
#include <QDebug>
#include <QEvent>

MessageBoxBase::MessageBoxBase(QObject *parent) : QObject(parent) { }

void MessageBoxBase::setUpUi(const QString &title, const QString &content, QWidget *parent)
{
    m_content    = content;
    m_selfWidget = parent;  //真正的窗口子类
    titleLabel   = new QLabel(title, parent);
    contentLabel = new QLabel(content, parent);

    buttonGroup  = new QFrame(parent);
    yesButton    = new PrimaryPushButton(tr("OK"), buttonGroup);
    cancelButton = new QPushButton(tr("Cancel"), buttonGroup);

    vBoxLayout   = new QVBoxLayout(parent);
    textLayout   = new QVBoxLayout();
    buttonLayout = new QHBoxLayout(buttonGroup);

    initWidget();
}

void MessageBoxBase::adjustText()
{
    int w = 0;
    int chars;

    if (!m_selfWidget) {
        return;
    }

    if (m_selfWidget->isWindow()) {
        if (m_selfWidget->parent()) {
            w     = qMax(titleLabel->width(), m_selfWidget->width());
            chars = qMax(qMin(w / 9, 140), 30);
        } else {
            chars = 100;
        }
    } else {
        w     = qMax(titleLabel->width(), m_selfWidget->window()->width());
        chars = qMax(qMin(w / 9, 100), 30);
    }

    contentLabel->setText(TextWrap::wrap(m_content, chars, false).first);
}

void MessageBoxBase::initWidget()
{
    setQss();
    initLayout();

    // fixes https://github.com/zhiyiYo/PyQt-Fluent-Widgets/issues/19
    yesButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);
    cancelButton->setAttribute(Qt::WA_LayoutUsesWidgetRect);

    yesButton->setFocus();
    buttonGroup->setFixedHeight(81);

    adjustText();

    connect(yesButton, &PrimaryPushButton::clicked, this, &MessageBoxBase::onYesButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &MessageBoxBase::onCancelButtonClicked);
}

void MessageBoxBase::setQss()
{
    // 设置层叠样式
    titleLabel->setObjectName("titleLabel");
    contentLabel->setObjectName("contentLabel");
    buttonGroup->setObjectName("buttonGroup");
    cancelButton->setObjectName("cancelButton");

    FluentStyleSheet::apply("DIALOG", m_selfWidget);

    yesButton->adjustSize();
    cancelButton->adjustSize();
}

void MessageBoxBase::initLayout()
{
    vBoxLayout->setSpacing(0);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->addLayout(textLayout, 1);
    vBoxLayout->addWidget(buttonGroup, 0, Qt::AlignBottom);
    vBoxLayout->setSizeConstraint(QVBoxLayout::SetMinimumSize);

    textLayout->setSpacing(12);
    textLayout->setContentsMargins(24, 24, 24, 24);
    textLayout->addWidget(titleLabel, 0, Qt::AlignTop);
    textLayout->addWidget(contentLabel, 0, Qt::AlignTop);

    buttonLayout->setSpacing(12);
    buttonLayout->setContentsMargins(24, 24, 24, 24);
    buttonLayout->addWidget(yesButton, 1, Qt::AlignVCenter);
    buttonLayout->addWidget(cancelButton, 1, Qt::AlignVCenter);
}

void MessageBoxBase::onYesButtonClicked()
{
    QDialog *p = qobject_cast<QDialog *>(m_selfWidget);
    if (p) {
        p->accept();
    }
    emit yesSignal();
}

void MessageBoxBase::onCancelButtonClicked()
{
    QDialog *p = qobject_cast<QDialog *>(m_selfWidget);
    if (p) {
        p->reject();
    }
    emit cancelSignal();
}

Dialog::Dialog(const QString &title, const QString &content, QWidget *parent) : QDialog(parent), MessageBoxBase(parent)
{
    setUpUi(title, content, this);

    m_windowTitleLabel = new QLabel(title, this);

    //    setResizeEnabled(false);
    resize(240, 192);

    vBoxLayout->insertWidget(0, m_windowTitleLabel, 0, Qt::AlignTop);
    m_windowTitleLabel->setObjectName("windowTitleLabel");
    FluentStyleSheet::apply("DIALOG", this);
    setFixedSize(size());
}

void Dialog::setTitleBarVisible(bool isVisible)
{
    m_windowTitleLabel->setVisible(isVisible);
}

MessageBox::MessageBox(const QString &title, const QString &content, QWidget *parent)
    : MaskDialogBase(parent), MessageBoxBase(parent)
{

    setUpUi(title, content, this);

    setShadowEffect(60, QPointF(0, 10), QColor(0, 0, 0, 50));
    setMaskColor(QColor(0, 0, 0, 76));

    // TODO 布局有冲突，设计有问题

    hBoxLayout()->removeWidget(widget);
    hBoxLayout()->addWidget(widget, 1, Qt::AlignCenter);

    buttonGroup->setMinimumWidth(280);
    widget->setFixedSize(qMax(contentLabel->width(), titleLabel->width()) + 48,
                         contentLabel->y() + contentLabel->height() + 105);
}

bool MessageBox::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == this->window()) {
        if (event->type() == QEvent::Resize) {
            adjustText();
        }
    }

    return MaskDialogBase::eventFilter(watched, event);
}
