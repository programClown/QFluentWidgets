#include "FolderListSettingCard.h"
#include "Layout/VBoxLayout.h"
#include "DialogBox/Dialog.h"

#include <QFileDialog>
#include <QPainter>

FToolButton::FToolButton(FluentIconBase *icon, const QSize &size, const QSize &iconSize, QWidget *parent)
    : QToolButton(parent), m_isPressed(false), m_icon(icon), m_iconSize(iconSize)
{
    setFixedSize(size);
}

void FToolButton::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    QToolButton::mousePressEvent(event);
}

void FToolButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    QToolButton::mouseReleaseEvent(event);
}

void FToolButton::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    if (m_isPressed) {
        painter.setOpacity(0.63);
    } else {
        painter.setOpacity(1);
    }
    int w = m_iconSize.width();
    int h = m_iconSize.height();
    m_icon->render(&painter, QRectF((this->width() - w) / 2, (this->height() - h) / 2, w, h).toRect());
}

FPushButton::FPushButton(FluentIconBase *icon, const QString &text, QWidget *parent)
    : QPushButton(parent), m_isPressed(false), m_icon(icon)
{
    setText(text);
}

void FPushButton::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    QPushButton::mousePressEvent(event);
}

void FPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    QPushButton::mouseReleaseEvent(event);
}

void FPushButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    if (m_isPressed) {
        painter.setOpacity(0.63);
    } else {
        painter.setOpacity(1);
    }
    m_icon->render(&painter, QRectF(12, 8, 16, 16).toRect());
}

FolderItem::FolderItem(const QString &folder, QWidget *parent) : QWidget(parent), m_folder(folder)
{

    hBoxLayout     = new QHBoxLayout(this);
    m_folderLabel  = new QLabel(folder, this);
    m_removeButton = new FToolButton(NEWFLICON(FluentIcon, CLOSE), QSize(39, 29), QSize(12, 12), this);

    setFixedHeight(53);
    hBoxLayout->setContentsMargins(48, 0, 60, 0);
    hBoxLayout->addWidget(m_folderLabel, 0, Qt::AlignLeft);
    hBoxLayout->addSpacing(16);
    hBoxLayout->addStretch(1);
    hBoxLayout->addWidget(m_removeButton, 0, Qt::AlignRight);
    hBoxLayout->setAlignment(Qt::AlignVCenter);

    connect(m_removeButton, &FToolButton::clicked, [=]() { emit removed(this); });
}

QString FolderItem::folder() const
{
    return m_folder;
}

FolderListSettingCard::FolderListSettingCard(const QVariant &configItem, const QString &title, const QString &content,
                                             const QString &directory, QWidget *parent)
    : ExpandSettingCard(NEWFLICON(FluentIcon, FOLDER_ADD), title, content, parent),
      m_configItem(configItem),
      m_dialogDirectory(directory)
{
    m_addFolderButton = new FPushButton(NEWFLICON(FluentIcon, FOLDER_ADD), tr("Add folder"), this);
    m_folders         = m_configItem.toStringList();
    initWidget();
}

void FolderListSettingCard::initWidget()
{
    addWidget(m_addFolderButton);

    // initialize layout
    viewLayout->setSpacing(0);
    viewLayout->setAlignment(Qt::AlignTop);
    viewLayout->setContentsMargins(0, 0, 0, 0);

    for (const auto &f : m_folders) {
        addFolderItem(f);
    }

    connect(m_addFolderButton, &FPushButton::clicked, this, &FolderListSettingCard::showFolderDialog);
}

void FolderListSettingCard::addFolderItem(const QString &folder)
{
    FolderItem *item = new FolderItem(folder, view());
    connect(item, &FolderItem::removed, this, &FolderListSettingCard::showConfirmDialog);
    viewLayout->addWidget(item);
    adjustViewSize();
}

void FolderListSettingCard::showFolderDialog()
{
    QString folder = QFileDialog::getExistingDirectory(this, tr("Choose folder"), m_dialogDirectory);
    if (folder.isEmpty() || m_folders.contains(folder)) {
        return;
    }

    addFolderItem(folder);
    m_folders.append(folder);
    // qconfig.set(self.configItem, self.folders)
    emit folderChanged(m_folders);
}

void FolderListSettingCard::showConfirmDialog(FolderItem *item)
{
    QFileInfo fi(item->folder());
    QString name    = fi.baseName();
    QString title   = tr("Are you sure you want to delete the folder?");
    QString content = tr("If you delete the ") + name
            + tr(" folder and remove it from the list, the folder will no "
                 "longer appear in the list, but will not be deleted.");

    QScopedPointer<Dialog> w(new Dialog(title, content, this->window()));
    connect(w.data(), &Dialog::yesSignal, [=]() { removeFolder(item); });
    w->exec();
}

void FolderListSettingCard::removeFolder(FolderItem *item)
{
    if (!m_folders.contains(item->folder())) {
        return;
    }

    m_folders.removeAll(item->folder());
    viewLayout->deleteWidget(item);
    adjustViewSize();

    emit folderChanged(m_folders);
    // qconfig.set(self.configItem, self.folders)
}
