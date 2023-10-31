#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QToolButton>
#include "Common/Icon.h"

class SmoothScroll;

class LineEditButton : public QToolButton
{
    Q_OBJECT
public:
    LineEditButton(FluentIconBase *ficon, QWidget *parent = nullptr);

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QScopedPointer<FluentIconBase> m_ficon;
};

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEdit(QWidget *parent = nullptr);

    bool isClearButtonEnabled() const;
    void setClearButtonEnabled(bool enable);

    QHBoxLayout *hBoxLayout() const;
    LineEditButton *clearButton() const;

signals:

private slots:
    void onTextChanged(const QString &text);

    // QWidget interface
protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QHBoxLayout *m_hBoxLayout;
    bool m_isClearButtonEnabled;
    LineEditButton *m_clearButton;
};

class SearchLineEdit : public LineEdit
{
    Q_OBJECT
public:
    SearchLineEdit(QWidget *parent = nullptr);

signals:
    void searchSignal(const QString &);
    void clearSignal();

protected slots:
    void search();

private:
    LineEditButton *m_searchButton;
};

class PasswordLineEdit : public LineEdit
{
    Q_OBJECT
public:
    PasswordLineEdit(QWidget *parent = nullptr);

signals:
    void echoModeChanged();
    void clearSignal();

protected:
    void switchEchoMode();

private:
    LineEditButton *m_echoButton;
};

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent = nullptr);

    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    SmoothScroll *m_verticalSmoothScroll;
    SmoothScroll *m_horizonSmoothScroll;
};

class PlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    PlainTextEdit(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    SmoothScroll *m_verticalSmoothScroll;
    SmoothScroll *m_horizonSmoothScroll;
};
#endif  // LINEEDIT_H
