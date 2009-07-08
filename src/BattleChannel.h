//
// C++ Interface: BattleChannel
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BATTLECHANNEL_H
#define BATTLECHANNEL_H

#include <QDebug>

#include "Channel.h"
#include "ui_battleWidget.h"
#include "UnitSyncLib.h"
#include "AudioBackend.h"
#include "MapInfoLoader.h"
#include "MapOverviewDialog.h"
#include "Users.h"

/**
represents the battle tab with chat, map and options overview

 @author Mirko Windhoff <qtlobby.reg@ncnever.de>
*/

class BattleChannel : public AbstractChannel {
  Q_OBJECT
public:
  BattleChannel( QString id, Battles* battles, QObject * parent = 0 );
  void setupUi( QWidget * tab );

signals:
  void playSample( SampleCollection sample );
  void newTrayMessage( QString );
public slots:
  void receiveInput( QString input );
  void receiveCommand( Command command );
  void requestMapInfo( QString mapName );
  void updateMapInfo( QString mapName );
  void fillModOptions();
  void fillSides(); // NEW
  void onSpecCheckBoxChanged( int state ); // NEW
private slots:
  void openMapOverview();
private:
  QString currentMap;
  Ui::battleWindowForm * battleWindowForm;
  MapOverviewDialog* mapOverviewDialog;
  MapInfoLoader* loader;
  Battle m_battle;
  bool noMapUpdates;
protected:
  Battles* battles;
};

#endif
