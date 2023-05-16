#include "MultiSelectComboBox.h"

#include <QStandardItem>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>

MultiSelectComboBox::MultiSelectComboBox(QWidget *parent) : QComboBox(parent), isPermitHidePopup(false)
{
    //去掉下拉箭头
    setStyleSheet("QComboBox::drop-down{border-style:none;}");
    //设置可编辑
    setEditable(true);
    // selectItemView盖住QComboBox
    selectItemView = new QListView(this);
    selectItemView->setMinimumHeight(30);
    // QListView{outline: none;} 去掉项虚线框 设置正常状态和选中为状态item的背景色相同
    selectItemView->setStyleSheet("QListView{outline: none;} QListView::item {background:#BDD7FD;} "
                                  "QListView::item:hover {background: #BDD7FD;}");
    selectItemView->setIconSize(QSize(12, 12));
    //设置各项的间隔
    selectItemView->setSpacing(3);
    //设置为不触发，不然点击可修改项内容
    selectItemView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置会调整
    selectItemView->setResizeMode(QListView::Adjust);
    //设置为自动换行
    selectItemView->setWrapping(true);
    //设置浮动方向
    selectItemView->setFlow(QListView::LeftToRight);
    //水平滚动条设置不不显示
    selectItemView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //选择模式设置为不显示
    selectItemView->setSelectionMode(QAbstractItemView::NoSelection);
    selectModel = new QStandardItemModel();
    selectItemView->setModel(selectModel);
    QHBoxLayout *lineLayout = new QHBoxLayout(this);
    lineLayout->setMargin(0);
    lineLayout->setSpacing(0);
    lineLayout->addWidget(selectItemView);
    //安装事件过滤器，处理点击x删除
    selectItemView->viewport()->installEventFilter(this);
    popupView  = new QListView();
    popupModel = new QStandardItemModel();
    popupView->setModel(popupModel);
    setView(popupView);
    setModel(popupModel);
    installEventFilter(this);
    //激活下拉列表某行的信号处理函数
    connect(this, QOverload<int>::of(&QComboBox::activated), [=](int index) {
        bool hasExist            = false;
        QStandardItem *clickItem = popupModel->item(index);
        QString text             = clickItem->text();
        //判断项是否已经选过，选过则删除选中项
        for (int row = 0; row < selectModel->rowCount(); row++) {
            QStandardItem *item = selectModel->item(row);
            if (item->text() == text) {
                selectModel->removeRow(row);
                hasExist = true;
                break;
            }
        }
        if (!hasExist) {
            //没选过则添加，并改变选项的状态
            clickItem->setCheckState(Qt::Checked);
            QStandardItem *item = new QStandardItem(QIcon("://del.png"), text);
            selectModel->appendRow(item);
        } else {
            //选中过点击改为未选中状态
            clickItem->setCheckState(Qt::Unchecked);
        }
    });
    // frame为弹出下拉列表框
    QFrame *frame = this->findChild<QFrame *>();
    if (frame) {
        frame->installEventFilter(this);
    }
    installEventFilter(this);
}

MultiSelectComboBox::~MultiSelectComboBox() { }

//所有添加项的方法都选调用父类方式再设置可以check
void MultiSelectComboBox::addItem(const QString &text, const QVariant &userData)
{
    QComboBox::addItem(text, userData);
    popupModel->item(findText(text))->setCheckable(true);
}

void MultiSelectComboBox::addItem(const QIcon &icon, const QString &text, const QVariant &userData)
{
    QComboBox::addItem(icon, text, userData);
    popupModel->item(findText(text))->setCheckable(true);
}
void MultiSelectComboBox::addItems(const QStringList &texts)
{
    QComboBox::addItems(texts);
    for (int row = 0; row < popupModel->rowCount(); row++) {
        popupModel->item(row)->setCheckable(true);
    }
}

//返回所有选中项的内容，用QStringList存储
QStringList MultiSelectComboBox::currentText()
{
    QStringList items;
    for (int row = 0; row < popupModel->rowCount(); row++) {
        QStandardItem *item = popupModel->item(row);
        if (item->checkState() == Qt::Checked) {
            items << item->text();
        }
    }
    return items;
}

//返回所有选中项的索引，用QList<int>存储
QList<int> MultiSelectComboBox::currentIndex()
{
    QList<int> indexs;
    for (int row = 0; row < popupModel->rowCount(); row++) {
        QStandardItem *item = popupModel->item(row);
        if (item->checkState() == Qt::Checked) {
            indexs << row;
        }
    }
    return indexs;
}

bool MultiSelectComboBox::eventFilter(QObject *watched, QEvent *event)
{
    QFrame *frame = this->findChild<QFrame *>();
    if (frame && frame == watched) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            QPoint globalPos        = mouseEvent->globalPos();
            QRect rect(frame->mapToGlobal(QPoint(0, 0)), QSize(frame->width(), frame->height()));
            QRect rect1(selectItemView->viewport()->mapToGlobal(QPoint(0, 0)),
                        QSize(selectItemView->viewport()->width(), selectItemView->viewport()->height()));
            //不在下拉列表框和显示选项项的viewport范围内的按下则允许关闭
            if (!rect.contains(globalPos) && !rect1.contains(globalPos)) {
                isPermitHidePopup = true;
            } else {
                selectItemViewPress(selectItemView->viewport()->mapFromGlobal(globalPos));
            }
        }
    }
    if (watched == selectItemView->viewport()) {
        //处理selectItemView->viewport的鼠标按下事件
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            selectItemViewPress(mouseEvent->pos());
        }
    }
    //屏蔽滑轮事件
    if (watched == this) {
        if (event->type() == QEvent::Wheel) {
            return true;
        }
    }
    return QComboBox::eventFilter(watched, event);
}

void MultiSelectComboBox::showPopup()
{
    QComboBox::showPopup();
    isPermitHidePopup = false;
}

void MultiSelectComboBox::hidePopup()
{
    if (isPermitHidePopup) {
        QComboBox::hidePopup();
    }
}

void MultiSelectComboBox::selectItemViewPress(QPoint pos)
{
    QModelIndex index = selectItemView->indexAt(pos);
    //有效，则该坐标有选中项
    if (index.isValid()) {
        QRect rect     = selectItemView->visualRect(index);
        QSize iconSize = selectItemView->iconSize();
        // x图标的区域
        QRect iconRect = QRect(rect.left(), rect.top(), iconSize.width(), iconSize.height());
        //鼠标在x图标的区域内
        if (iconRect.contains(pos)) {
            QStandardItem *item = selectModel->itemFromIndex(index);
            //修改状态为未选中
            popupModel->item(findText(item->text()))->setCheckState(Qt::Unchecked);
            //删除项
            selectModel->removeRow(index.row());
        }
    }
    showPopup();
}
