#ifndef DIALOG_H
#define DIALOG_H

#include "MaskDialogBase.h"

#include <QBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QObject>
#include <QPushButton>

class PrimaryPushButton;

class MessageBoxBase
{

public:
    QLabel *titleLabel   = nullptr;
    QLabel *contentLabel = nullptr;

    QFrame *buttonGroup          = nullptr;
    PrimaryPushButton *yesButton = nullptr;
    QPushButton *cancelButton    = nullptr;

    QVBoxLayout *vBoxLayout   = nullptr;
    QVBoxLayout *textLayout   = nullptr;
    QHBoxLayout *buttonLayout = nullptr;

protected:
    void setUpUi(const QString &title, const QString &content, QWidget *parent);
    void adjustText();

private:
    void initWidget();
    void setQss();
    void initLayout();

private:
    QString m_content;
    QWidget *m_selfWidget = nullptr;
};

class Dialog : public QDialog, public MessageBoxBase
{
    Q_OBJECT
public:
    explicit Dialog(const QString &title, const QString &content, QWidget *parent = nullptr);

    void setTitleBarVisible(bool isVisible);

signals:
    void yesSignal();
    void cancelSignal();

private slots:
    void onYesButtonClicked();
    void onCancelButtonClicked();

private:
    QLabel *m_windowTitleLabel;
};

class MessageBox : public MaskDialogBase, public MessageBoxBase
{
    Q_OBJECT
public:
    explicit MessageBox(const QString &title, const QString &content, QWidget *parent = nullptr);

    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void yesSignal();
    void cancelSignal();

private slots:
    void onYesButtonClicked();
    void onCancelButtonClicked();
};

#endif  // DIALOG_H
