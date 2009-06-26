//
// C++ Implementation: Channel
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "Channel.h"

Channel::Channel( QString name, QObject * parent ) : AbstractChannel( name, parent ) {
}

Channel::~Channel() {}

void Channel::receiveCommand( Command command ) {
  command.name = command.name.toUpper();
  QString line = QString( "%1\n" );
  if ( command.name == "SAID" ) {
    if ( command.attributes.takeFirst() == objectName() ) {
      QString userName = command.attributes.takeFirst();
      insertLine( flag( userName ) + line
                  .arg( "&lt;%1&gt; %2" )
                  .arg( userName )
                  .arg( command.attributes.join( " " ) ) );
    }
  }
  else if ( command.name == "SAIDEX" ) {
    if ( command.attributes.takeFirst() == objectName() ) {
      QString userName = command.attributes.takeFirst();
      insertLine( flag( userName ) + line
                  .arg( "<span style=\"color:magenta;\">* %1 %2</span>" )
                  .arg( userName )
                  .arg( command.attributes.join( " " ) ) );
    }
  }
  else if ( command.name == "JOINED" ) {
    if ( command.attributes.takeFirst() == objectName() ) {
      QString userName = command.attributes.takeFirst();
      insertLine( flag( userName ) + line
                  .arg( "<span style=\"color:darkgreen;\">%1</span>" )
                  .arg( tr( "** %1 joined the channel." ) )
                  .arg( userName ) );
    }
  }
  else if ( command.name == "LEFT" ) {
    if ( command.attributes.takeFirst() == objectName() ) {
      QString userName = command.attributes.takeFirst();
      insertLine( flag( userName ) + line
                  .arg( "<span style=\"color:darkgreen;\">** %1</span>" )
                  .arg( tr( "%1 left the channel." ) )
                  .arg( userName ) );
    }
  }
  else if ( command.name == "JOIN" ) {
    if ( command.attributes.takeFirst() == objectName() ) {
      insertLine( line
                  .arg( "<span style=\"color:darkgreen;\">** %1</span>" )
                  .arg( tr( "%1 joined the channel." ) )
                  .arg( myUserName ) );
    }
  }
  else if ( command.name == "CLIENTMSG" ) {
    if ( command.attributes.takeFirst() == objectName() ) {
      insertLine( line
                  .arg( "<span style=\"color:blue;\">** CLIENTMSG ** %1</span>" )
                  .arg( command.attributes.join( " " ) ) );
    }
  }
  else if ( command.name == "CHANNELTOPIC" ) {
    if (command.attributes.takeFirst() == objectName())
      insertLine(line.arg(command.toQString()));
  }
}

void Channel::receiveInput( QString input ) {
  if ( executeChannelInput( input ) )
    return;
  input.replace( "<", "&lt;" );
  input.replace( ">", "&gt;" );
  QStringList inputList = input.split( " " );
  QString firstWord = inputList.takeFirst();
  Command ret;
  if ( "/me" == firstWord ) {
    ret.name = "SAYEX";
    ret.attributes << objectName() << inputList;
  }
  else if ( "/slap" == firstWord ) {
    ret.name = "SAYEX";
    ret.attributes << objectName() << QString( "slaps %1 around a bit with a large trout" ).arg( inputList.join( " " ) );
  }
  else if ( QString( "/leave" ).split( "," ).contains( firstWord, Qt::CaseInsensitive ) ) {
    ret.name = "LEAVE";
    ret.attributes << objectName() << ( inputList.empty() ? ( inputList << QString( "Quit" ) ) : inputList );
  }
  else {
    ret.name = "SAY";
    ret.attributes << objectName() << input;
  }
  emit sendCommand( ret );
}
