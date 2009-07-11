//
// C++ Interface: LobbyTab
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ABSTRACTLOBBYTAB_H
#define ABSTRACTLOBBYTAB_H
#include <QWidget>
#include <QObject>
#include <QIcon>
#include <QMap>

#include "Command.h"
#include "Battles.h"

/**
This is an abstract class, all lobby tab objects should derive from it

 @author Mirko Windhoff <qtlobby.reg@ncnever.de>
*/
class AbstractLobbyTab : public QObject
{
  Q_OBJECT
public:
  AbstractLobbyTab(  QObject * parent );
  virtual ~AbstractLobbyTab();
  virtual void setupUi( QWidget * tab ) = 0;
  virtual void setActive( bool isActive ) = 0;
  void setUserNameCountryCodeMap( QMap<QString, QString>* userNameCountryCodeMap );

  QString myUserName;
  QIcon icon;
  QColor color;
  bool isActive;
  QWidget* currentWidget;
  int currentTabIndex;

signals:
  void sendCommand( Command );

public slots:
  virtual void receiveInput( QString input ) = 0;
  virtual void receiveCommand( Command command ) = 0;

protected:
  QString flag( const QString );
  QMap<QString, QString>* userNameCountryCodeMap;
};

#endif
