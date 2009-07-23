// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "Command.h"
Command::Command() : id( 0 ) {}

Command::Command( QString commandString ) {
    id = 0;
    commandString = commandString.trimmed();
    attributes = commandString.split( " " );
    name = attributes.takeFirst();
    //command may begin with id in format #n, where n is a number between 0 and 2^31
    if ( commandString.contains( QRegExp( "^#[0-9]{1,10} " ) ) ) {
        id = commandString.section( " ", 0, 0 ).section( "#", 1, 1 ).toUInt();
        name = attributes.takeFirst();
    }
}

QString Command::toQString() {
    return QString( "%1%2 %3" )
            .arg( id > 0 ? QString( "#%1 " ).arg( id ) : "" )
            .arg( name ).arg( attributes.join( " " ) );
}
