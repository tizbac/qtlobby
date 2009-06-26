//
// C++ Interface: CommandAssigner
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMANDASSIGNER_H
#define COMMANDASSIGNER_H

#include <QObject>
#include <QStringList>
#include <QRegExp>
#include <QDebug>
#include "Command.h"

/**
 This class delegates parsed commands to all potential receivers.
 @author Joachim Schiele <js@lastlog.de>
*/
class CommandAssigner : public QObject
{
  Q_OBJECT
public:
  CommandAssigner( QObject* parent = 0 );
  ~CommandAssigner();

signals:
  void serverContextStateCommand( Command );
  void sendMessage( QString message );
  void lobbyTabCommand( Command command );
  void userCommand( Command command );
  void battleCommand( Command command );
public slots:
  void receiveMessage( QString message );
  void sendCommand( Command command );
};

#endif
