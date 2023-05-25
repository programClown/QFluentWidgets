#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include "MaskDialogBase.h"

#include <QLabel>
#include <QPushButton>

class MessageDialog : public MaskDialogBase
{
    Q_OBJECT
public:
    explicit MessageDialog(const QString &title, const QString &content, QWidget *parent = nullptr);

private:
    void initWidget();
    void initLayout();
    void setQss();

private slots:
    void onCancelButtonClicked();
    void onYesButtonClicked();

signals:
    void yesSignal();
    void cancelSignal();

private:
    QString m_content;
    QLabel *m_titleLabel;
    QLabel *m_contentLabel;
    QPushButton *m_yesButton;
    QPushButton *m_cancelButton;
};

#endif  // MESSAGEDIALOG_H
