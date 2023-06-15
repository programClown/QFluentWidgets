#include "ViewInterface.h"

#include "common/Translator.h"
#include "common/GalleryStyleSheet.h"

#include <Widgets/TreeView.h>

#include <QBoxLayout>

AFrame::AFrame(QWidget *parent) : QFrame(parent)
{
    m_hBoxLayout = new QHBoxLayout(this);
    m_hBoxLayout->setContentsMargins(0, 8, 0, 0);

    setObjectName("frame");
    GalleryStyleSheet::apply(GalleryStyleSheet::VIEW_INTERFACE, this);
}

void AFrame::addWidget(QWidget *widget)
{
    m_hBoxLayout->addWidget(widget);
}

TreeFrame::TreeFrame(QWidget *parent, bool enableClick) : AFrame(parent)
{
    m_tree = new TreeWidget(this);
    addWidget(m_tree);

    QTreeWidgetItem *item1 = new QTreeWidgetItem({ "JoJo 1 - Phantom Blood" });
    item1->addChildren({ new QTreeWidgetItem({ "Jonathan Joestar" }), new QTreeWidgetItem({ "Dio Brando" }),
                         new QTreeWidgetItem({ "Will A. Zeppeli" }) });
    m_tree->addTopLevelItem(item1);

    QTreeWidgetItem *item2  = new QTreeWidgetItem({ "JoJo 3 - Stardust Crusaders" });
    QTreeWidgetItem *item21 = new QTreeWidgetItem({ "Jotaro Kujo" });
    item21->addChildren({ new QTreeWidgetItem({ "空条承太郎" }), new QTreeWidgetItem({ "空条蕉太狼" }),
                          new QTreeWidgetItem({ "阿强" }), new QTreeWidgetItem({ "卖鱼强" }),
                          new QTreeWidgetItem({ "那个无敌的男人" }) });
    item2->addChild(item21);
    m_tree->addTopLevelItem(item2);
    m_tree->expandAll();
    m_tree->setHeaderHidden(true);

    setFixedSize(300, 300);

    if (enableClick) {
        QTreeWidgetItemIterator it = QTreeWidgetItemIterator(m_tree);
        while (*it) {
            (*it)->setCheckState(0, Qt::Unchecked);
            ++it;
        }
    }
}

ViewInterface::ViewInterface(QWidget *parent)
    : GalleryInterface(Translator::ins()->view, "qfluentwidgets.components.widgets", parent)
{
    TreeFrame *frame = new TreeFrame(this);
    addExampleCard("A simple TreeView", frame,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/tree_view/demo.py");

    frame = new TreeFrame(this, true);
    addExampleCard("A TreeView with Multi-selection enabled", frame,
                   "https://github.com/zhiyiYo/PyQt-Fluent-Widgets/blob/master/examples/tree_view/demo.py");
}
