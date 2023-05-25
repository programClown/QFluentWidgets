#ifndef SETTINGCARDGROUP_H
#define SETTINGCARDGROUP_H

#include <QLabel>
#include <QVBoxLayout>

class ExpandLayout;

class SettingCardGroup : public QWidget
{
    Q_OBJECT
public:
    explicit SettingCardGroup(const QString &title, QWidget *parent = nullptr);

    void addSettingCard(QWidget *card);
    void addSettingCards(const QList<QWidget *> &cards);
    void adjustSize();

    QVBoxLayout *vBoxLayout;
    ExpandLayout *cardLayout;

signals:

private:
    QLabel *m_titleLabel;
};

#endif  // SETTINGCARDGROUP_H
