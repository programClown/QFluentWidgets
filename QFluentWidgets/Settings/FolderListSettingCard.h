#ifndef FOLDERLISTSETTINGCARD_H
#define FOLDERLISTSETTINGCARD_H

#include "Common/Icon.h"

#include "ExpandGroupSettingCard.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>

class FToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit FToolButton(FluentIconBase *icon, const QSize &size, const QSize &iconSize, QWidget *parent = nullptr);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_isPressed;
    QScopedPointer<FluentIconBase> m_icon;
    QSize m_iconSize;
};

class FPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit FPushButton(FluentIconBase *icon, const QString &text, QWidget *parent = nullptr);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_isPressed;
    QScopedPointer<FluentIconBase> m_icon;
};

class FolderItem : public QWidget
{
    Q_OBJECT
public:
    explicit FolderItem(const QString &folder, QWidget *parent = nullptr);

    QString folder() const;

    QHBoxLayout *hBoxLayout;

signals:
    void removed(FolderItem *);

private:
    QString m_folder;
    QLabel *m_folderLabel;
    FToolButton *m_removeButton;
};

class FolderListSettingCard : public ExpandSettingCard
{
    Q_OBJECT
public:
    explicit FolderListSettingCard(const QVariant &configItem, const QString &title, const QString &content = "",
                                   const QString &directory = "./", QWidget *parent = nullptr);

private:
    void initWidget();
    void addFolderItem(const QString &folder);

private slots:
    void showFolderDialog();
    void showConfirmDialog(FolderItem *item);
    void removeFolder(FolderItem *item);

signals:
    void folderChanged(const QStringList &);

private:
    QVariant m_configItem;
    QString m_dialogDirectory;
    FPushButton *m_addFolderButton;
    QStringList m_folders;
};

#endif  // FOLDERLISTSETTINGCARD_H
