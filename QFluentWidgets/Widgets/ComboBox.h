#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>
#include <QStyledItemDelegate>

struct UserInfo
{
    QString name;
    QString number;
    QIcon icon;
    bool isonline;
};

class ComboListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ComboListModel(QObject *parent = 0);
    ~ComboListModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    void setModelData(const QList<UserInfo> &);

private:
    QList<UserInfo> m_infoList;
};

class ComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit ComboBox(QWidget *parent = nullptr);

signals:
};

class ComboListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ComboListDelegate(QObject *parent = 0);
    ~ComboListDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    qreal m_radius = 8;
};

class ComboTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ComboTableModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void setModelData(const QList<UserInfo> &data, int column);
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
    QList<UserInfo> m_infoList;
    int m_column;
};

class ComboBoxMenu : public QWidget
{
public:
    explicit ComboBoxMenu(QWidget *parent = nullptr);
};

#endif  // COMBOBOX_H
