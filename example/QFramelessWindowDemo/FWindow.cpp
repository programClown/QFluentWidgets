#include "FWindow.h"

#include "WindowsFramelessDialog.h"
#include "TitleBar.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QLayout>
#include <QDebug>
#include <QTextEdit>

FWindow::FWindow(QWidget *parent) : WindowsFramelessWindow(parent)
{

    // add title bar

    StandardTitleBar *ttb = new StandardTitleBar(this);
    setWindowTitle("Frameless-Window");

    QIcon icon = QIcon(":/images/logo.png");
    ttb->setIcon(icon);
    setWindowIcon(icon);

    // add menu bar
    QMenuBar *menuBar = new QMenuBar(ttb);

    QMenu *menu = new QMenu("File(&F)", this);
    menu->addAction("open");
    menu->addAction("save");
    menuBar->addMenu(menu);

    menuBar->addAction("Edit(&E)");
    menuBar->addAction("Select(&S)");

    ttb->hBoxLayout->insertWidget(0, menuBar, 0, Qt::AlignLeft);
    ttb->hBoxLayout->insertStretch(1, 1);
    setMenuWidget(ttb);

    // status bar
    QStatusBar *statusBar = new QStatusBar(this);
    statusBar->addWidget(new QLabel("row 1"));
    statusBar->addWidget(new QLabel("column 1"));
    setStatusBar(statusBar);

    setStyleSheet("QMenuBar{background: #F0F0F0; padding: 5px 5px}"
                  "QTextEdit{border: none; font-size: 15px}"
                  "QDialog > QLabel{font-size: 15px}");

    QTextEdit *text = new QTextEdit();
    setCentralWidget(text);

    resize(800, 600);
}
