//
// C++ Implementation: InfoChannel
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "InfoChannel.h"

InfoChannel::InfoChannel( QString name, QObject * parent ) : AbstractChannel( name, parent ) {
    activeIcon = QIcon( ":/icons/chanop.xpm" );
    inactiveIcon = QIcon( ":/icons/chanop.xpm" );
}

InfoChannel::~InfoChannel() {}

void InfoChannel::receiveCommand( Command command ) {
    command.name = command.name.toUpper();
    QString line = "<span style=\"color:darkgreen;\">** %1 ** %2</span>\n";
    if ( command.name == "TASSERVER"
         || command.name == "MOTD"
         || command.name == "SERVERMSG"
         || command.name == "BROADCAST"
         || command.name == "CLIENTMSG" ) {
        insertLine( line.arg( command.name, command.attributes.join( " " ) ) );
    }
}

void InfoChannel::receiveInput( QString input ) {
    if ( executeChannelInput( input ) )
        return;
    emit sendCommand( Command( input ) );
}
