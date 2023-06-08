#include "ComboBox.h"

#include <QPainter>
#include <QDebug>

ComboListModel::ComboListModel(QObject *parent) : QAbstractListModel(parent) { }

ComboListModel::~ComboListModel() { }

QVariant ComboListModel::data(const QModelIndex &index, int role) const
{
    if (Qt::DisplayRole == role) {
        return m_infoList.at(index.row()).name;

    } else if (Qt::DecorationRole == role) {
        return m_infoList.at(index.row()).icon;

    } else if (Qt::UserRole == role) {
        return m_infoList.at(index.row()).number;

    } else if (Qt::UserRole + 1 == role) {
        return m_infoList.at(index.row()).isonline;
    }

    return QVariant();
}

int ComboListModel::rowCount(const QModelIndex & /*parent*/) const
{
    return m_infoList.size();
}

void ComboListModel::setModelData(const QList<UserInfo> &data)
{
    if (!m_infoList.isEmpty())
        m_infoList.clear();

    m_infoList = data;
}

ComboListDelegate::ComboListDelegate(QObject *parent) : QStyledItemDelegate(parent) { }

ComboListDelegate::~ComboListDelegate() { }

void ComboListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        painter->save();
        QString name      = index.data(Qt::DisplayRole).toString();
        QVariant icondata = index.data(Qt::DecorationRole);
        QIcon icon        = icondata.value<QIcon>();
        QString number    = index.data(Qt::UserRole).toString();
        bool isonline     = index.data(Qt::UserRole + 1).toBool();
        qDebug() << name << number << isonline;

        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width() - 1);
        rect.setHeight(option.rect.height() - 1);

        QPainterPath path;
        path.moveTo(rect.topRight() - QPointF(m_radius, 0));
        path.lineTo(rect.topLeft() + QPointF(m_radius, 0));
        path.quadTo(rect.topLeft(), rect.topLeft() + QPointF(0, m_radius));
        path.lineTo(rect.bottomLeft() + QPointF(0, -m_radius));
        path.quadTo(rect.bottomLeft(), rect.bottomLeft() + QPointF(m_radius, 0));
        path.lineTo(rect.bottomRight() - QPointF(m_radius, 0));
        path.quadTo(rect.bottomRight(), rect.bottomRight() + QPointF(0, -m_radius));
        path.lineTo(rect.topRight() + QPointF(0, m_radius));
        path.quadTo(rect.topRight(), rect.topRight() + QPointF(-m_radius, -0));

        if (option.state.testFlag(QStyle::State_Selected)) {
            painter->setPen(QPen(Qt::blue));
            painter->setBrush(QColor(0, 255, 127));
            painter->drawPath(path);
        } else if (option.state.testFlag(QStyle::State_Raised)) {
            painter->setPen(QPen(Qt::green));
            painter->setBrush(QColor(0, 127, 255));
            painter->drawPath(path);
        } else {
            painter->setPen(QPen(Qt::gray));
            painter->setBrush(QColor(255, 255, 255));
            painter->drawPath(path);
        }

        QRect iconRect   = QRect(rect.left() + 3, rect.top() + 3, 30, 30);
        QRect nameRect   = QRect(iconRect.right() + 5, rect.top() + 3, rect.width() - 80, 20);
        QRect circle     = QRect(nameRect.right() + 5, nameRect.top(), 10, 10);
        QRect numberRect = QRect(nameRect.x(), nameRect.y() + 20, 100, 20);
        qDebug() << iconRect << nameRect << circle << numberRect;

        painter->drawEllipse(circle);
        painter->setPen(QPen(QColor(0, 0, 0)));
        painter->setFont(QFont("微软雅黑", 9, QFont::Bold));
        painter->drawText(nameRect, Qt::AlignLeft, name);
        painter->drawText(numberRect, Qt::AlignLeft, number);

        painter->drawPixmap(iconRect, icon.pixmap(iconRect.size()));
        painter->restore();
    } else {
        qDebug() << "index is vaild?";
    }
}

QSize ComboListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    return QSize(200, 40);
}

ComboTableModel::ComboTableModel(QObject *parent) : QAbstractTableModel(parent), m_column(1) { }

QVariant ComboTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    int offset = row * m_column + col;
    if ((row < 0) || (col < 0) || (offset >= m_infoList.count())) {
        return QVariant();
    }

    // Q_UNUSED(role);
    if (role == Qt::DisplayRole) {
        return m_infoList.at(offset).name;
    } else if (role == Qt::FontRole) {
        return QFont("微软雅黑", 15);
    } else if (role == Qt::DecorationRole) {
        return m_infoList.at(offset).icon;
    } else if (role == Qt::TextAlignmentRole) {
        return int(Qt::AlignHCenter | Qt::AlignVCenter);
    } else if (role == Qt::TextColorRole) {
        return QColor(255, 0, 0);
    } else if (Qt::UserRole == role) {
        return m_infoList.at(offset).number;

    } else if (Qt::UserRole + 1 == role) {
        return m_infoList.at(offset).isonline;
    }

    return QVariant();
}
// rowCount和columnCount必须重写
int ComboTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return (m_infoList.count() + m_column - 1) / m_column;
}

int ComboTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_column;
}

QVariant ComboTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

void ComboTableModel::setModelData(const QList<UserInfo> &data, int column)
{
    if (!m_infoList.isEmpty())
        m_infoList.clear();

    if (data.count() < column) {
        m_column = data.count();
    } else {
        m_column = column;
    }

    m_infoList = data;
}

Qt::ItemFlags ComboTableModel::flags(const QModelIndex &index) const
{
    int row    = index.row();
    int col    = index.column();
    int offset = row * m_column + col;
    if ((row < 0) || (col < 0) || (offset >= m_infoList.count())) {
        return Qt::NoItemFlags;
    }

    return QAbstractTableModel::flags(index);
}

ComboBox::ComboBox(QWidget *parent) : QComboBox(parent) { }

ComboBoxMenu::ComboBoxMenu(QWidget *parent) : QWidget(parent) { }
