//
// C++ Implementation: PrivateChannel
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "PrivateChannel.h"

PrivateChannel::PrivateChannel( QString name, QObject * parent ) : AbstractChannel( name, parent ) {
  activeIcon = QIcon( ":/icons/userchat.xpm" );
}

PrivateChannel::~PrivateChannel() {}

void PrivateChannel::receiveCommand( Command command ) {
  command.name = command.name.toUpper();
  QString line = QString( "%1\n" );
  if ( command.name == "SAIDPRIVATE" ) {
    if ( command.attributes.takeFirst() == objectName() && command.attributes.join( " " ).trimmed() != "" ) {
      insertLine( flag( objectName() ) + line
                  .arg( "&lt;%1&gt; %2" )
                  .arg( objectName() )
                  .arg( command.attributes.join( " " ) ) );
    }
  }
  else if ( command.name == "SAYPRIVATE" ) {
    if ( command.attributes.takeFirst() == objectName() && command.attributes.join( " " ).trimmed() != "" ) {
      insertLine( line.arg( "&lt;%1&gt; %2" )
                  .arg( myUserName )
                  .arg( command.attributes.join( " " ) ) );
    }
  }
  else if ( command.name == "CLIENTMSG" ) {
    if ( command.attributes.takeFirst() == objectName() ) {
      insertLine( line.arg( tr( "<span style=\"color:blue;\">** CLIENTMSG ** %1</span>" )
                            .arg( command.attributes.join( " " ) ) ) );
    }
  }
  else if ( command.name == "REMOVEUSER" ) {
    if ( command.attributes.takeFirst() == objectName() ) {
      insertLine( line.arg( tr( "<span style=\"color:darkgreen;\">** %1 is now offline.</span>" ).arg( objectName() ) ) );
    }
  }
  else if ( command.name == "ADDUSER" ) {
    if ( command.attributes.takeFirst() == objectName() ) {
      insertLine( line.arg( tr( "<span style=\"color:darkgreen;\">** %1 is now online.</span>" ).arg( objectName() ) ) );
    }
  }
}

void PrivateChannel::receiveInput( QString input ) {
  if ( executeChannelInput( input ) )
    return;
  input.replace( "<", "&lt;" );
  input.replace( ">", "&gt;" );
  QStringList inputList = input.split( " " );
  QString firstWord = inputList.takeFirst();
  if ( firstWord == "/leave" )
    return;
  Command ret;
  ret.name = "SAYPRIVATE";
  ret.attributes << objectName() << input;
  emit sendCommand( ret );
}
