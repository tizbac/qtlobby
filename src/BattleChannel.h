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

#include <QGraphicsPixmapItem>
#include <QDebug>

#include "Channel.h"
#include "ui_battleWidget.h"
#include "UnitSyncLib.h"
#include "AudioBackend.h"

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
  void readyStateChanged( bool isReady );
  void playSample( SampleCollection sample );
  void newTrayMessage( QString );
public slots:
  void receiveInput( QString input );
  void receiveCommand( Command command );
  void updateMapImage( QString );
  void fillModOptions();
  void onReadyCheckBoxChanged( bool isChecked );
  void onStartScriptPushButtonClicked( bool checked );
private:
  Ui::battleWindowForm * battleWindowForm;
  QGraphicsScene* scene;
  Battle m_battle;
protected:
  Battles* battles;
};

#endif
