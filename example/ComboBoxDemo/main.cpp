#include <QApplication>
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QList>
#include <Widgets/ComboBox.h>
#include <Common/Icon.h>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QTextStream>

#include <Widgets/MultiSelectComboBox.h>

class Demo : public QWidget
{
    Q_OBJECT
public:
    Demo(QWidget *parent = nullptr) : QWidget(parent)
    {
        resize(400, 400);
        QVBoxLayout *vlay = new QVBoxLayout(this);

        m_comboListBox = new QComboBox(this);
        m_comboListBox->resize(200, 35);
        m_comboListBox->setCurrentIndex(0);

        m_comboTableBox = new QComboBox(this);
        m_comboTableBox->resize(200, 35);
        m_comboTableBox->setCurrentIndex(0);
        initModel();

        MultiSelectComboBox *mulCombo = new MultiSelectComboBox(this);
        mulCombo->addItems({ "刘英", "玉田", "刘能" });

        QComboBox *pComboBox = new QComboBox(this);
        pComboBox->addItems({ "刘英", "赵玉田", "刘能", "尼古拉斯赵四" });
        pComboBox->setView(new QListView());  // 添加这句，设置下拉列表项高才能生效
        pComboBox->setStyleSheet(qss());

        vlay->addWidget(m_comboListBox);
        vlay->addWidget(m_comboTableBox);
        vlay->addWidget(mulCombo);
        vlay->addWidget(pComboBox);
    }

    void initModel()
    {
        m_listModel    = new ComboListModel();
        m_tableModel   = new ComboTableModel();
        m_listDelegate = new ComboListDelegate(this);
        setModelData();
        m_comboListBox->setModel(m_listModel);            // 设置模型
        m_comboListBox->setItemDelegate(m_listDelegate);  // 设置委托

        QTableView *view = new QTableView(m_comboTableBox);
        view->setMinimumHeight(200);
        view->horizontalHeader()->setVisible(false);
        view->verticalHeader()->setVisible(false);
        view->setAlternatingRowColors(true);

        m_comboTableBox->setModelColumn(2);  // 没有这一行不能多列
        m_comboTableBox->setModel(m_tableModel);
        m_comboTableBox->setView(view);
    }

    void setModelData()
    {
        QList<UserInfo> InfoList;
        UserInfo info;
        info.icon     = FluentIcon(FluentIcon::ADD).icon();
        info.isonline = true;
        info.name     = "张三";
        info.number   = "0123456789";
        InfoList.append(info);

        UserInfo info1;
        info1.icon     = FluentIcon(FluentIcon::CUT).icon();
        info1.isonline = true;
        info1.name     = "李四";
        info1.number   = "44564664521";
        InfoList.append(info1);

        UserInfo info2;
        info2.icon     = FluentIcon(FluentIcon::INFO).icon();
        info2.isonline = true;
        info2.name     = "王二";
        info2.number   = "9876543210";
        InfoList.append(info2);

        m_listModel->setModelData(InfoList);
        m_tableModel->setModelData(InfoList, 2);
    }

    QString qss()
    {
        QFile f(":/res/style.qss");
        QString str = "";
        if (f.open(QIODevice::ReadOnly)) {
            // 用QTextStream读取样式文件不用区分文件编码 带bom也行
            QTextStream in(&f);
            in.setCodec("utf-8");
            while (!in.atEnd()) {
                QString line = in.readLine();
                str += line + "\r\n";
            }
            f.close();
        }

        return str;
    }

private:
    QComboBox *m_comboListBox;
    QComboBox *m_comboTableBox;
    ComboListModel *m_listModel;
    ComboTableModel *m_tableModel;
    ComboListDelegate *m_listDelegate;
};

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(qfluentwidgets);
    QApplication a(argc, argv);
    Demo w;
    w.show();
    return a.exec();
}

#include "main.moc"
