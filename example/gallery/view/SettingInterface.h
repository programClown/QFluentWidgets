#ifndef SETTINGINTERFACE_H
#define SETTINGINTERFACE_H

#include <Widgets/ScrollArea.h>

class ExpandLayout;
class QLabel;
class SettingCardGroup;
class FolderListSettingCard;
class PushSettingCard;
class OptionsSettingCard;
class CustomColorSettingCard;
class ComboBoxSettingCard;
class RangeSettingCard;
class SwitchSettingCard;
class HyperlinkCard;
class PrimaryPushSettingCard;

class SettingInterface : public ScrollArea
{
    Q_OBJECT
public:
    SettingInterface(QWidget *parent = nullptr);

private:
    void initWidget();

    void initLayout();

    void connectSignalToSlot();

private slots:

    void showRestartToolTip();

    void onDownloadFolderCardClicked();

signals:
    void checkUpdateSig();
    void musicFoldersChanged(QList<QString>);
    void acrylicEnableChanged(bool);
    void downloadFolderChanged(QString);
    void minimizeToTrayChanged(bool);

private:
    QWidget *m_scrollWidget;
    ExpandLayout *m_expandLayout;

    QLabel *m_settingLabel;

    SettingCardGroup *m_musicInThisPCGroup;
    FolderListSettingCard *m_musicFolderCard;
    PushSettingCard *m_downloadFolderCard;

    SettingCardGroup *m_personalGroup;
    OptionsSettingCard *m_themeCard;
    CustomColorSettingCard *m_themeColorCard;
    OptionsSettingCard *m_zoomCard;
    ComboBoxSettingCard *m_languageCard;

    SettingCardGroup *m_materialGroup;
    RangeSettingCard *m_blurRadiusCard;

    SettingCardGroup *m_updateSoftwareGroup;
    SwitchSettingCard *m_updateOnStartUpCard;

    SettingCardGroup *m_aboutGroup;
    HyperlinkCard *m_helpCard;
    PrimaryPushSettingCard *m_feedbackCard;
    PrimaryPushSettingCard *m_aboutCard;
};

#endif  // SETTINGINTERFACE_H
