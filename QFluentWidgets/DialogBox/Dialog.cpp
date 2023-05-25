#include "Dialog.h"
#include "Widgets/Button.h"
#include "Common/StyleSheet.h"
#include "Common/AutoWrap.h"

#include <QDialog>
#include <QDebug>
#include <QEvent>

void MessageBoxBase::setUpUi(const QString &title, const QString &content, QWidget *parent)
{
    m_content     = content;
    m_selfWidget  = parent;  //真正的窗口子类
    QString magic = title;
    for (int i = 0; i < title.count(); ++i) {
        magic.append(" ");
    }
    titleLabel   = new QLabel(magic, parent);
    contentLabel = new QLabel(content, parent);
    contentLabel->setWordWrap(true);

    buttonGroup  = new QFrame(parent);
    yesButton    = new PrimaryPushButton(QObject::tr("OK"), buttonGroup);
    cancelButton = new QPushButton(QObject::tr("Cancel"), buttonGroup);

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
            w     = qMax(titleLabel->width(), ((QWidget *)m_selfWidget->parent())->width());
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

Dialog::Dialog(const QString &title, const QString &content, QWidget *parent) : QDialog(parent), MessageBoxBase()
{
    //补丁使用
    setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    setFixedSize(240, 192);
    setUpUi(title, content, this);

    connect(yesButton, &PrimaryPushButton::clicked, this, &Dialog::onYesButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &Dialog::onCancelButtonClicked);

    m_windowTitleLabel = new QLabel(title, this);

    vBoxLayout->insertWidget(0, m_windowTitleLabel, 0, Qt::AlignTop);
    m_windowTitleLabel->setObjectName("windowTitleLabel");
    FluentStyleSheet::apply("DIALOG", this);
}

void Dialog::setTitleBarVisible(bool isVisible)
{
    m_windowTitleLabel->setVisible(isVisible);
}

void Dialog::onYesButtonClicked()
{
    this->accept();
    emit yesSignal();
}

void Dialog::onCancelButtonClicked()
{
    this->reject();
    emit cancelSignal();
}

MessageBox::MessageBox(const QString &title, const QString &content, QWidget *parent)
    : MaskDialogBase(parent), MessageBoxBase()
{
    //补丁使用
    setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    setUpUi(title, content, this->widget);

    connect(yesButton, &PrimaryPushButton::clicked, this, &MessageBox::onYesButtonClicked);
    connect(cancelButton, &QPushButton::clicked, this, &MessageBox::onCancelButtonClicked);

    setShadowEffect(60, QPointF(0, 10), QColor(0, 0, 0, 50));
    setMaskColor(QColor(0, 0, 0, 76));

    hBoxLayout()->removeWidget(widget);
    hBoxLayout()->addWidget(widget, 1, Qt::AlignCenter);

    buttonGroup->setMinimumWidth(280);
    widget->setFixedSize(qMax(contentLabel->width(), titleLabel->width()) + 48,
                         contentLabel->y() + contentLabel->height() + 205);
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

void MessageBox::onYesButtonClicked()
{
    this->accept();
    emit yesSignal();
}

void MessageBox::onCancelButtonClicked()
{
    this->reject();
    emit cancelSignal();
}
