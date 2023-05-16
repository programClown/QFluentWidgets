#include "Dialog.h"
#include "FolderListDialog.h"
#include "QFluentWidgets.h"
#include "Common/Icon.h"
#include "Widgets/ScrollArea.h"

#include <QApplication>
#include <QFileDialog>
#include <QFileInfo>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>

FolderListDialog::FolderListDialog(const QStringList &folderList, const QString &title, const QString &content,
                                   QWidget *parent)
    : MaskDialogBase(parent)
{
    m_title         = title;
    m_content       = content;
    m_originalPaths = folderList;
    m_folderPaths   = folderList;

    m_vBoxLayout     = new QVBoxLayout(widget);
    m_titleLabel     = new QLabel(title, widget);
    m_contentLabel   = new QLabel(content, widget);
    m_scrollArea     = new ScrollArea(widget);
    m_scrollWidget   = new QWidget(m_scrollArea);
    m_completeButton = new QPushButton(tr("Done"), widget);
    m_addFolderCard  = new AddFolderCard(m_scrollWidget);

    for (auto f : m_folderPaths) {
        m_folderCards.append(new FolderCard(f, m_scrollWidget));
    }

    initWidget();
}

FolderListDialog::~FolderListDialog() { }

void FolderListDialog::initWidget()
{
    setQss();

    int w = qMax(qMax(m_titleLabel->width() + 48, m_contentLabel->width() + 48), 352);
    widget->setFixedWidth(w);
    m_scrollArea->resize(294, 72);
    m_scrollWidget->resize(292, 72);
    m_scrollArea->setFixedWidth(294);
    m_scrollWidget->setFixedWidth(292);
    m_scrollArea->setMaximumHeight(400);
    m_scrollArea->setViewportMargins(0, 0, 0, 0);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setWidget(m_scrollWidget);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    initLayout();

    connect(m_addFolderCard, &FolderCard::clicked, this, &FolderListDialog::showFileDialog);
    connect(m_completeButton, &QPushButton::clicked, this, &FolderListDialog::onButtonClicked);
    for (auto card : m_folderCards) {
        connect(card, &FolderCard::clicked, this, &FolderListDialog::showDeleteFolderCardDialog);
    }
}

void FolderListDialog::setQss()
{
    m_titleLabel->setObjectName("titleLabel");
    m_contentLabel->setObjectName("contentLabel");
    m_completeButton->setObjectName("completeButton");
    m_scrollWidget->setObjectName("scrollWidget");

    FluentStyleSheet::apply("FOLDER_LIST_DIALOG", this);
    setStyle(QApplication::style());

    m_titleLabel->adjustSize();
    m_contentLabel->adjustSize();
    m_completeButton->adjustSize();
}

void FolderListDialog::initLayout()
{
    // initialize layout
    m_vBoxLayout->setContentsMargins(24, 24, 24, 24);
    m_vBoxLayout->setSizeConstraint(QVBoxLayout::SetFixedSize);
    m_vBoxLayout->setAlignment(Qt::AlignTop);
    m_vBoxLayout->setSpacing(0);

    // labels
    QVBoxLayout *layout_1 = new QVBoxLayout();
    layout_1->setContentsMargins(0, 0, 0, 0);
    layout_1->setSpacing(6);
    layout_1->addWidget(m_titleLabel, 0, Qt::AlignTop);
    layout_1->addWidget(m_contentLabel, 0, Qt::AlignTop);
    m_vBoxLayout->addLayout(layout_1, 0);
    m_vBoxLayout->addSpacing(12);

    // cards
    QHBoxLayout *layout_2 = new QHBoxLayout();
    layout_2->setAlignment(Qt::AlignCenter);
    layout_2->setContentsMargins(4, 0, 4, 0);
    layout_2->addWidget(m_scrollArea, 0, Qt::AlignCenter);
    m_vBoxLayout->addLayout(layout_2, 1);
    m_vBoxLayout->addSpacing(24);

    m_scrollLayout = new QVBoxLayout(m_scrollWidget);
    m_scrollLayout->setAlignment(Qt::AlignTop);
    m_scrollLayout->setContentsMargins(0, 0, 0, 0);
    m_scrollLayout->setSpacing(8);
    m_scrollLayout->addWidget(m_addFolderCard, 0, Qt::AlignTop);

    for (auto card : m_folderCards) {
        m_scrollLayout->addWidget(card, 0, Qt::AlignTop);
    }

    // buttons
    QHBoxLayout *layout_3 = new QHBoxLayout();
    layout_3->setContentsMargins(0, 0, 0, 0);
    layout_3->addStretch(1);
    layout_3->addWidget(m_completeButton);
    m_vBoxLayout->addLayout(layout_3, 0);

    adjustWidgetSize();
}

void FolderListDialog::adjustWidgetSize()
{
    int N = m_folderCards.count();
    int h = 72 * (N + 1) + 8 * N;
    m_scrollArea->setFixedHeight(qMin(h, 400));
}

/// delete selected folder card
void FolderListDialog::deleteFolderCard(FolderCard *card)
{
    m_scrollLayout->removeWidget(card);
    int index = m_folderCards.indexOf(card);
    m_folderPaths.removeAt(index);
    m_folderCards.removeAt(index);
    card->deleteLater();

    // adjust height
    adjustWidgetSize();
}

void FolderListDialog::showFileDialog()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Choose folder"), "./");

    if (path.isEmpty() || m_folderPaths.contains(path)) {
        return;
    }

    // create folder card
    FolderCard *card = new FolderCard(path, m_scrollWidget);
    m_scrollLayout->addWidget(card, 0, Qt::AlignTop);
    connect(card, &FolderCard::clicked, this, &FolderListDialog::showDeleteFolderCardDialog);
    card->show();

    m_folderPaths.append(path);
    m_folderCards.append(card);

    adjustWidgetSize();
}

void FolderListDialog::onButtonClicked()
{
    for (auto f : m_folderPaths) {
        if (!m_originalPaths.contains(f)) {
            setEnabled(false);
            QApplication::processEvents();
            emit folderChanged(m_folderPaths);
            break;
        }
    }

    close();
}

void FolderListDialog::showDeleteFolderCardDialog()
{
    FolderCard *card = qobject_cast<FolderCard *>(sender());
    QString title    = tr("Are you sure you want to delete the folder?");
    QString content  = tr("If you delete the ") + card->folderName()
            + tr(" folder and remove it from the list, the folder will no longer appear in the list, but will not be "
                 "deleted.");

    Dialog *dialog = new Dialog(title, content, this->window());
    connect(dialog, &Dialog::yesSignal, [=]() { deleteFolderCard(card); });
    dialog->exec();
    static_cast<QDialog *>(dialog)->deleteLater();
}

ClickableWindow::ClickableWindow(QWidget *parent) : QWidget(parent), m_isPressed(false), m_isEnter(false)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    setFixedSize(292, 72);
}

void ClickableWindow::mousePressEvent(QMouseEvent * /*event*/)
{
    m_isPressed = true;
    update();
}

void ClickableWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    update();
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

void ClickableWindow::enterEvent(QEvent * /*event*/)
{
    m_isEnter = true;
    update();
}

void ClickableWindow::leaveEvent(QEvent * /*event*/)
{
    m_isEnter = true;
    update();
}

void ClickableWindow::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    bool isDark = QFWIns.isDarkTheme();
    int bg      = isDark ? 51 : 204;

    QBrush brush(QColor(bg, bg, bg));
    painter.setPen(Qt::NoPen);

    if (!m_isEnter) {
        painter.setBrush(brush);
        painter.drawRoundedRect(rect(), 4, 4);
    } else {
        painter.setPen(QPen(QColor(bg, bg, bg), 2));
        painter.drawRect(1, 1, width() - 2, height() - 2);
        painter.setPen(Qt::NoPen);

        if (!m_isPressed) {
            bg = isDark ? 24 : 230;
            brush.setColor(QColor(bg, bg, bg));
            painter.setBrush(brush);
            painter.drawRect(2, 2, width() - 4, height() - 4);
        } else {
            bg = isDark ? 102 : 230;
            brush.setColor(QColor(153, 153, 153));
            painter.setBrush(brush);
            painter.drawRoundedRect(5, 1, width() - 10, height() - 2, 2, 2);
        }
    }
}

bool ClickableWindow::isEnter() const
{
    return m_isEnter;
}

bool ClickableWindow::isPressed() const
{
    return m_isPressed;
}

FolderCard::FolderCard(const QString &folderPath, QWidget *parent) : ClickableWindow(parent), m_folderPath(folderPath)
{
    QFileInfo fi(m_folderPath);
    m_folderName = fi.baseName();
    QString c    = getIconColor();

    m_closeIcon = QPixmap(QString(":/qfluentwidgets/images/folder_list_dialog/Close_%1.png").arg(c))
                          .scaled(12, 12, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void FolderCard::paintEvent(QPaintEvent *event)
{
    ClickableWindow::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    QColor color = QFWIns.isDarkTheme() ? Qt::white : Qt::black;
    painter.setPen(color);

    if (isPressed()) {
        drawText(&painter, 12, 8, 12, 7);
        painter.drawPixmap(width() - 26, 18, m_closeIcon);
    } else {
        drawText(&painter, 10, 9, 10, 8);
        painter.drawPixmap(width() - 24, 20, m_closeIcon);
    }
}

void FolderCard::drawText(QPainter *painter, int x1, int fontSize1, int x2, int fontSize2)
{
    // paint folder name;
    QFont font("Microsoft YaHei", fontSize1, 75);
    painter->setFont(font);
    QString name = QFontMetrics(font).elidedText(m_folderName, Qt::ElideRight, width() - 48);
    painter->drawText(x1, 30, name);

    // paint folder path
    font = QFont("Microsoft YaHei", fontSize2);
    painter->setFont(font);
    QString path = QFontMetrics(font).elidedText(m_folderPath, Qt::ElideRight, width() - 24);
    painter->drawText(x2, 37, width() - 16, 18, Qt::AlignLeft, path);
}

QString FolderCard::folderName() const
{
    return m_folderName;
}

AddFolderCard::AddFolderCard(QWidget *parent) : ClickableWindow(parent)
{
    QString c = getIconColor();
    m_iconPix = QPixmap(QString(":/qfluentwidgets/images/folder_list_dialog/Add_%1.png").arg(c))
                        .scaled(22, 22, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void AddFolderCard::paintEvent(QPaintEvent *event)
{
    ClickableWindow::paintEvent(event);
    QPainter painter(this);

    int w  = width();
    int h  = height();
    int pw = m_iconPix.width();
    int ph = m_iconPix.height();

    if (isPressed()) {
        painter.drawPixmap(int(w / 2 - pw / 2), int(h / 2 - ph / 2), m_iconPix);
    } else {
        painter.drawPixmap(int(w / 2 - (pw - 4) / 2), int(h / 2 - (ph - 4) / 2), pw - 4, ph - 4, m_iconPix);
    }
}
