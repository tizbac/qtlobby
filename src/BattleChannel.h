// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef BATTLECHANNEL_H
#define BATTLECHANNEL_H

#include <QDebug>

#include "Channel.h"
#include "ui_BattleWindowForm.h"
#include "UnitSyncLib.h"
#include "AudioBackend.h"
#include "MapInfoLoader.h"
#include "MapOverviewDialog.h"
#include "Users.h"
#include <QColorDialog>
#include "Notification.h"

/**
represents the battle tab with chat, map and options overview

 @author Mirko Windhoff <qtlobby.reg@ncnever.de>
*/

class BattleChannel : public AbstractChannel {
    Q_OBJECT
public:
    BattleChannel( QString id, Battles* battles, QObject * parent = 0 );
    void setupUi( QWidget * tab );
    QString getTabTitle();
    void refreshMapAndModOptions();
    ~BattleChannel();
    bool isBlocked() const;

signals:
    void playSample( SampleCollection sample );
    void newTrayMessage( QString );
    void colorChanged(QColor color);
public slots:
    void receiveInput( QString input );
    void receiveCommand( Command command );
    void requestMapInfo( QString mapName );
    void updateMapInfo( QString mapName );
    void fillModOptions();
    void fillSides();
    void onSpecCheckBoxChanged( int state );
    void onMyStateChanged(User u);
private slots:
    void onModOptionsAnchorClicked(QUrl url);
    void onStartPositionComboBoxChanged(int index);
    void onGameEndComboBoxChanged(int index);
    void onLimitDGunCheckBoxToggled(bool checked);
    void onGhostedBuildingsCheckBoxToggled(bool checked);
    void onDiminishingMetalMakersCheckBoxToggled(bool checked);
    void onUndeformableMapSpeedCheckBoxToggled(bool checked);
    void openMapOverview();
    void onChatSplitterMoved ( int pos, int index );
    void onBattleSplitterMoved ( int pos, int index );
    void onColorClicked();
    void onAddStartRect(int ally, QRect r);
    void onRemoveStartRect(int ally);
    void onSideComboBoxChanged(int side);
private:
    QString currentMap;
    Ui::BattleWindowForm * battleWindowForm;
    MapOverviewDialog* mapOverviewDialog;
    MapInfoLoader* loader;
    Battle m_battle;
    bool noMapUpdates;
    QColor currentcolor;
    bool locked;
    QByteArray splitterState;
    bool wasKicked;
    Notification* notify;
    QWidget* battleWindowForm_ui;
protected:
    Battles* battles;
};

#endif
