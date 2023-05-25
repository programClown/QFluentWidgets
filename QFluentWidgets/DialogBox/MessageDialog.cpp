#include "MessageDialog.h"
#include "Common/AutoWrap.h"
#include "Common/StyleSheet.h"

MessageDialog::MessageDialog(const QString &title, const QString &content, QWidget *parent) : MaskDialogBase(parent)
{
    m_content      = content;
    m_titleLabel   = new QLabel(title, widget);
    m_contentLabel = new QLabel(content, widget);
    m_yesButton    = new QPushButton("OK", widget);
    m_cancelButton = new QPushButton("Cancel", widget);
    initWidget();
}

void MessageDialog::initWidget()
{
    windowMask->resize(this->size());
    widget->setMaximumWidth(540);
    m_titleLabel->move(24, 24);
    m_contentLabel->move(24, 56);
    m_contentLabel->setText(TextWrap::wrap(m_content, 71).first);

    setQss();
    initLayout();

    connect(m_yesButton, &QPushButton::clicked, this, &MessageDialog::onYesButtonClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &MessageDialog::onCancelButtonClicked);
}

void MessageDialog::initLayout()
{
    m_contentLabel->adjustSize();
    widget->setFixedSize(48 + m_contentLabel->width(), m_contentLabel->y() + m_contentLabel->height() + 92);
    m_yesButton->resize((widget->width() - 54) / 2, 32);
    m_cancelButton->resize(m_yesButton->width(), 32);
    m_yesButton->move(24, widget->height() - 56);
    m_cancelButton->move(widget->width() - 24 - m_cancelButton->width(), widget->height() - 56);
}

void MessageDialog::setQss()
{
    windowMask->setObjectName("windowMask");
    m_titleLabel->setObjectName("titleLabel");
    m_contentLabel->setObjectName("contentLabel");
    FluentStyleSheet::apply("MESSAGE_DIALOG", this);
}

void MessageDialog::onCancelButtonClicked()
{
    emit cancelSignal();
    setResult(QDialog::Rejected);
    close();
}

void MessageDialog::onYesButtonClicked()
{
    setEnabled(false);
    emit yesSignal();
    setResult(QDialog::Accepted);
    close();
}
