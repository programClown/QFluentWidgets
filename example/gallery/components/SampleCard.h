#ifndef SAMPLECARD_H
#define SAMPLECARD_H

#include <QFrame>

class FluentIconBase;
class QLabel;
class QVBoxLayout;
class FlowLayout;
class IconWidget;
class QHBoxLayout;

class SampleCard : public QFrame
{
public:
    SampleCard(FluentIconBase *icon, QString const title, QString const content, QString const routeKey, int index,
               QWidget *parent = nullptr);

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    int m_index;
    QString m_routeKey;
    IconWidget *m_iconWidget;
    QLabel *m_titleLabel;
    QLabel *m_contentLabel;
    QHBoxLayout *m_hBoxLayout;
    QVBoxLayout *m_vBoxLayout;
};

class SampleCardView : public QWidget
{
public:
    SampleCardView(QString const title, QWidget *parent = nullptr);

    void addSampleCard(FluentIconBase *icon, QString const title, QString const content, QString const routeKey,
                       int index);

private:
    QLabel *m_titleLabel;
    QVBoxLayout *m_vBoxLayout;
    FlowLayout *m_flowLayout;
};

#endif  // SAMPLECARD_H
