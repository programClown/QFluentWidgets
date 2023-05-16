#ifndef MULTISELECTCOMBOBOX_H
#define MULTISELECTCOMBOBOX_H

#include <QComboBox>
#include <QListView>
#include <QStandardItemModel>

class MultiSelectComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit MultiSelectComboBox(QWidget *parent = nullptr);
    ~MultiSelectComboBox();

    void addItem(const QString &text, const QVariant &userData);
    void addItem(const QIcon &icon, const QString &text, const QVariant &userData);
    void addItems(const QStringList &texts);

    QStringList currentText();
    QList<int> currentIndex();
    void selectItemViewPress(QPoint pos);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void showPopup() override;
    void hidePopup() override;

signals:

private:
    QListView *selectItemView;
    QStandardItemModel *selectModel;
    QListView *popupView;
    QStandardItemModel *popupModel;
    bool isPermitHidePopup;
};

#endif  // MULTISELECTCOMBOBOX_H
