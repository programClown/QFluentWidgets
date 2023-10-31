#include "SettingInterface.h"

#include "common/GalleryConfig.h"

#include <Layout/ExpandLayout.h>
#include <Settings/CustomColorSettingCard.h>
#include <Settings/ExpandGroupSettingCard.h>
#include <Settings/FolderListSettingCard.h>
#include <Settings/OptionsSettingCard.h>
#include <Settings/SettingCard.h>
#include <Settings/SettingCardGroup.h>

#include <QStandardPaths>

SettingInterface::SettingInterface(QWidget *parent) : ScrollArea(parent)
{
    //    m_scrollWidget = new QWidget();
    //    m_expandLayout = new ExpandLayout(m_scrollWidget);

    //    // setting label
    //    m_settingLabel = new QLabel(tr("Settings"), this);

    //    // music folders
    //    m_musicInThisPCGroup = new SettingCardGroup(tr("Music on this PC"), m_scrollWidget);
    //    m_musicFolderCard    = new FolderListSettingCard(
    //            GalleryConfig::cfg()->musicFolders->value(), tr("Local music library"), "",
    //            QStandardPaths::writableLocation(QStandardPaths::MusicLocation), m_musicInThisPCGroup);
    //    m_downloadFolderCard =
    //            new PushSettingCard(tr("Choose folder"), NEWFLICON(FluentIcon, DOWNLOAD), tr("Download directory"),
    //            "");
}

void SettingInterface::initWidget() { }

void SettingInterface::initLayout() { }

void SettingInterface::connectSignalToSlot() { }

void SettingInterface::showRestartToolTip() { }

void SettingInterface::onDownloadFolderCardClicked() { }
