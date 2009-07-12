//
// C++ Implementation: CommandAssigner
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "CommandAssigner.h"

CommandAssigner::CommandAssigner( QObject* parent ) : QObject( parent ) {}
CommandAssigner::~CommandAssigner() {}

void CommandAssigner::receiveMessage( QString message ) {
// #ifdef NETWORKDEBUG
//   qDebug() << " << " << message;
// #endif
  //initialize the command object and parse the command
  Command command( message );
  /** login, logoff, register, newAccount, change password|username */
  QStringList serverContextStateCommands;
  serverContextStateCommands
  << "TASSERVER"
  << "MOTD"
  << "ACCEPTED"
  << "LOGININFOEND"
  << "LOGIN"
  << "PONG"
  << "AGREEMENT"
  << "AGREEMENTEND"
  << "DENIED"
  << "REGISTRATIONDENIED"
  << "REGISTRATIONACCEPTED"
  << "SERVERMSG";

  if ( serverContextStateCommands.contains( command.name, Qt::CaseInsensitive ) ) {
    emit serverContextStateCommand( command );
  }

  /** battlestatus related */
  QString battleCommands = "RING,JOINBATTLE,JOINEDBATTLE,UPDATEBATTLEINFO,LEFTBATTLE,BATTLEOPENED,BATTLECLOSED,CLIENTBATTLESTATUS,CLIENTSTATUS,SETSCRIPTTAGS,REQUESTBATTLESTATUS,ADDSTARTRECT,REMOVESTARTRECT";
  if ( battleCommands.split( "," ).contains( command.name, Qt::CaseInsensitive ) ) {
    emit battleCommand( command );
  }

  /** channel and chat related */
  QString userCommands = "ADDUSER,REMOVEUSER,CLIENTS,CLIENTBATTLESTATUS,JOINBATTLE,JOINEDBATTLE,LEFTBATTLE,FORCELEAVECHANNEL,BATTLEOPENED,BATTLECLOSED,FORCEQUITBATTLE,CLIENTSTATUS,SAID,JOINED,JOIN,LEFT";
// not needed: CHANNELTOPIC,JOIN,SAIDEX,SAIDPRIVATE,
  if ( userCommands.split( "," ).contains( command.name, Qt::CaseInsensitive ) ) {
    emit userCommand( command );
  }

  /* server context */
  /*misc*/
  emit lobbyTabCommand( command );
}

void CommandAssigner::sendCommand( Command command ) {
  emit sendMessage( command.toQString() );
}
