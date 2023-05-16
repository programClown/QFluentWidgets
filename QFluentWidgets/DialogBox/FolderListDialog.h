#ifndef FOLDERLISTDIALOG_H
#define FOLDERLISTDIALOG_H

#include "MaskDialogBase.h"

#include <QLabel>

class ScrollArea;
class AddFolderCard;
class FolderCard;

class FolderListDialog : public MaskDialogBase
{
    Q_OBJECT

public:
    FolderListDialog(const QStringList &folderList, const QString &title, const QString &content,
                     QWidget *parent = nullptr);
    ~FolderListDialog();

signals:
    void folderChanged(const QStringList &list);

private:
    void initWidget();
    void setQss();
    void initLayout();
    void adjustWidgetSize();
    void deleteFolderCard(FolderCard *card);

private slots:
    void showFileDialog();
    void onButtonClicked();
    void showDeleteFolderCardDialog();

private:
    QStringList m_folderPaths;
    QStringList m_originalPaths;
    QString m_title;
    QString m_content;

    QVBoxLayout *m_vBoxLayout;
    QLabel *m_titleLabel;
    QLabel *m_contentLabel;
    ScrollArea *m_scrollArea;
    QWidget *m_scrollWidget;
    QPushButton *m_completeButton;
    AddFolderCard *m_addFolderCard;
    QList<FolderCard *> m_folderCards;
    QVBoxLayout *m_scrollLayout;
};

class ClickableWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ClickableWindow(QWidget *parent = nullptr);

    // QWidget interface
    bool isPressed() const;
    bool isEnter() const;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void clicked();

private:
    bool m_isPressed;
    bool m_isEnter;
};

class FolderCard : public ClickableWindow
{
    Q_OBJECT
public:
    explicit FolderCard(const QString &folderPath, QWidget *parent = nullptr);

    QString folderName() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawText(QPainter *painter, int x1, int fontSize1, int x2, int fontSize2);

private:
    QString m_folderPath;
    QString m_folderName;
    QPixmap m_closeIcon;
};

class AddFolderCard : public ClickableWindow
{
    Q_OBJECT
public:
    explicit AddFolderCard(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_iconPix;
};

#endif  // FOLDERLISTDIALOG_H
