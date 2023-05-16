#ifndef DIALOG_H
#define DIALOG_H

#include "MaskDialogBase.h"

#include <QBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QObject>
#include <QPushButton>

class PrimaryPushButton;

class MessageBoxBase : public QObject
{
    Q_OBJECT
public:
    explicit MessageBoxBase(QObject *parent = nullptr);

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

private slots:
    void onYesButtonClicked();
    void onCancelButtonClicked();

signals:
    void yesSignal();
    void cancelSignal();

private:
    QString m_content;
    QWidget *m_selfWidget = nullptr;
};

class Dialog : public QDialog, public MessageBoxBase
{
public:
    explicit Dialog(const QString &title, const QString &content, QWidget *parent = nullptr);

    void setTitleBarVisible(bool isVisible);

private:
    QLabel *m_windowTitleLabel;
};

class MessageBox : public MaskDialogBase, public MessageBoxBase
{
public:
    explicit MessageBox(const QString &title, const QString &content, QWidget *parent = nullptr);

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif  // DIALOG_H
