#include "HomeInterface.h"

#include "components/LinkCard.h"
#include "common/GalleryConfig.h"
#include "common/GalleryStyleSheet.h"
#include "components/SampleCard.h"

#include "Common/Icon.h"

#include <QBoxLayout>
#include <QLabel>
#include <QPainter>

BannerWidget::BannerWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(336);
    m_vBoxLayout   = new QVBoxLayout(this);
    m_galleryLabel = new QLabel("Fluent Gallery", this);
    m_banner       = QPixmap(":/resource/images/header1.png");
    m_linkCardView = new LinkCardView(this);

    m_galleryLabel->setObjectName("galleryLabel");

    m_vBoxLayout->setSpacing(0);
    m_vBoxLayout->setContentsMargins(0, 20, 0, 0);
    m_vBoxLayout->addWidget(m_galleryLabel);
    m_vBoxLayout->addWidget(m_linkCardView, 1, Qt::AlignBottom);
    m_vBoxLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    m_linkCardView->addCard(new FluentIcon(":/resource/images/logo.png"), tr("Getting started"),
                            tr("An overview of app development."), HELP_URL);
    m_linkCardView->addCard(
            NEWFLICON(FluentIcon, GITHUB), tr("GitHub repo"),
            tr("The latest fluent design controls and styles for your applications applications applications  options "
               "and samples  options and samples  options and samples  options and samples  options and samples."),
            REPO_URL);
    m_linkCardView->addCard(NEWFLICON(FluentIcon, CODE), tr("Code samples"),
                            tr("Find samples that demonstrate specific tasks, features and APIs."), EXAMPLE_URL);
    m_linkCardView->addCard(NEWFLICON(FluentIcon, FEEDBACK), tr("Send feedback"),
                            tr("Help us improve PyQt-Fluent-Widgets by providing feedback."), FEEDBACK_URL);
}

void BannerWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter = QPainter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QPainterPath path = QPainterPath();
    path.setFillRule(Qt::WindingFill);
    int w = width(), h = 200;
    path.addRoundedRect(QRectF(0, 0, w, h), 10, 10);
    path.addRect(QRectF(0, h - 50, 50, 50));
    path.addRect(QRectF(w - 50, 0, 50, 50));
    path.addRect(QRectF(w - 50, h - 50, 50, 50));
    path = path.simplified();

    painter.fillPath(path, QColor(206, 216, 228));

    QPixmap pixmap = m_banner.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    path.addRect(QRectF(0, h, w, height() - h));
    painter.fillPath(path, QBrush(pixmap));
}

HomeInterface::HomeInterface(QWidget *parent) : ScrollArea(parent)
{
    m_banner     = new BannerWidget(this);
    m_view       = new QWidget(this);
    m_vBoxLayout = new QVBoxLayout(m_view);

    initWidget();
    loadSamples();
}

void HomeInterface::initWidget()
{
    m_view->setObjectName("view");
    GalleryStyleSheet::apply(GalleryStyleSheet::HOME_INTERFACE, this);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWidget(m_view);
    setWidgetResizable(true);

    m_vBoxLayout->setContentsMargins(0, 0, 0, 36);
    m_vBoxLayout->setSpacing(40);
    m_vBoxLayout->addWidget(m_banner);
    m_vBoxLayout->setAlignment(Qt::AlignTop);
}

void HomeInterface::loadSamples()
{
    // basic input samples
    SampleCardView *basicInputView = new SampleCardView(tr("Basic input samples"), m_view);
    basicInputView->addSampleCard(new FluentIcon(":/resource/images/controls/Button.png"), "Button",
                                  tr("A control that responds to user input and emit clicked signal."),
                                  "basicInputInterface", 0);
    basicInputView->addSampleCard(new FluentIcon(":/resource/images/controls/Checkbox.png"), "CheckBox",
                                  tr("A control that a user can select or clear."), "basicInputInterface", 4);
    basicInputView->addSampleCard(new FluentIcon(":/resource/images/controls/ComboBox.png"), "ComboBox",
                                  tr("A drop-down list of items a user can select from."), "basicInputInterface", 6);
    basicInputView->addSampleCard(new FluentIcon(":/resource/images/controls/RadioButton.png"), "RadioButton",
                                  tr("A control that allows a user to select a single option from a group of options."),
                                  "basicInputInterface", 7);
    basicInputView->addSampleCard(
            new FluentIcon(":/resource/images/controls/Slider.png"), "Slider",
            tr("A control that lets the user select from a range of values by moving a Thumb control along a track."),
            "basicInputInterface", 8);
    basicInputView->addSampleCard(new FluentIcon(":/resource/images/controls/ToggleSwitch.png"), "SwitchButton",
                                  tr("A switch that can be toggled between 2 states."), "basicInputInterface", 9);

    m_vBoxLayout->addWidget(basicInputView);

    // date time samples
    SampleCardView *dateTimeView = new SampleCardView(tr("Date & time samples"), m_view);
    dateTimeView->addSampleCard(new FluentIcon(":/resource/images/controls/DatePicker.png"), "DatePicker",
                                tr("A control that lets a user pick a date value."), "dateTimeInterface", 0);
    dateTimeView->addSampleCard(new FluentIcon(":/resource/images/controls/TimePicker.png"), "TimePicker",
                                tr("A configurable control that lets a user pick a time value."), "dateTimeInterface",
                                2);
    m_vBoxLayout->addWidget(dateTimeView);

    // dialog samples
    SampleCardView *dialogView = new SampleCardView(tr("Dialog samples"), m_view);
    dialogView->addSampleCard(new FluentIcon(":/resource/images/controls/ContentDialog.png"), "MessageBox",
                              tr("A message dialog with mask."), "dialogInterface", 1);
    dialogView->addSampleCard(new FluentIcon(":/resource/images/controls/ColorPicker.png"), "ColorDialog",
                              tr("A dialog that allows user to select color."), "dialogInterface", 2);
    m_vBoxLayout->addWidget(dialogView);

    // layout samples
    SampleCardView *layoutView = new SampleCardView(tr("Layout samples"), m_view);
    layoutView->addSampleCard(new FluentIcon(":/resource/images/controls/Grid.png"), "FlowLayout",
                              tr("A layout arranges components in a left-to-right flow, wrapping to the next row when "
                                 "the current row is full."),
                              "layoutInterface", 0);
    m_vBoxLayout->addWidget(layoutView);

    // material samples
    SampleCardView *materialView = new SampleCardView(tr("Material samples"), m_view);
    materialView->addSampleCard(new FluentIcon(":/resource/images/controls/Acrylic.png"), "AcrylicLabel",
                                tr("A translucent material recommended for panel background."), "materialInterface", 0);
    m_vBoxLayout->addWidget(materialView);

    // menu samples
    SampleCardView *menuView = new SampleCardView(tr("Menu samples"), m_view);
    menuView->addSampleCard(new FluentIcon(":/resource/images/controls/MenuFlyout.png"), "RoundMenu",
                            tr("Shows a contextual list of simple commands or options."), "menuInterface", 0);
    m_vBoxLayout->addWidget(menuView);

    // scroll samples
    SampleCardView *scrollView = new SampleCardView(tr("Scrolling samples"), m_view);
    scrollView->addSampleCard(new FluentIcon(":/resource/images/controls/ScrollViewer.png"), "ScrollArea",
                              tr("A container control that lets the user pan and zoom its content smoothly."),
                              "scrollInterface", 0);
    m_vBoxLayout->addWidget(scrollView);

    // state info samples
    SampleCardView *stateInfoView = new SampleCardView(tr("Status & info samples"), m_view);
    stateInfoView->addSampleCard(
            new FluentIcon(":/resource/images/controls/ProgressRing.png"), "StateToolTip",
            tr("Shows the apps progress on a task,or that the app is performing ongoing work that does block user "
               "interaction."),
            "statusInfoInterface", 0);
    stateInfoView->addSampleCard(new FluentIcon(":/resource/images/controls/ToolTip.png"), "ToolTip",
                                 tr("Displays information for an element in a pop-up window."), "statusInfoInterface",
                                 1);
    stateInfoView->addSampleCard(new FluentIcon(":/resource/images/controls/InfoBar.png"), "InfoBar",
                                 tr("An inline message to display app-wide status change information."),
                                 "statusInfoInterface", 3);
    m_vBoxLayout->addWidget(stateInfoView);

    // text samples
    SampleCardView *textView = new SampleCardView(tr("Text & info samples"), m_view);
    textView->addSampleCard(new FluentIcon(":/resource/images/controls/TextBox.png"), "LineEdit",
                            tr("A single-line plain text field."), "textInterface", 0);
    textView->addSampleCard(new FluentIcon(":/resource/images/controls/NumberBox.png"), "SpinBox",
                            tr("A text control used for numeric input and evaluation of algebraic equations."),
                            "textInterface", 1);
    textView->addSampleCard(
            new FluentIcon(":/resource/images/controls/RichEditBox.png"), "TextEdit",
            tr("A rich text editing control that supports formatted text, hyperlinks, and other rich content."),
            "textInterface", 3);
    m_vBoxLayout->addWidget(textView);

    // view samples
    SampleCardView *collectionView = new SampleCardView(tr("View samples"), m_view);
    collectionView->addSampleCard(new FluentIcon(":/resource/images/controls/TreeView.png"), "TreeView",
                                  tr("The TreeView control is a hierarchical list pattern with expanding and "
                                     "collapsing nodes that contain nested items."),
                                  "viewInterface", 0);
    m_vBoxLayout->addWidget(collectionView);
}
