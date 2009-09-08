// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "PrivateChannel.h"
#include "UserGroup.h"
#include "PathManager.h"
#include "config.h"

PrivateChannel::PrivateChannel( QString name, QObject * parent ) : AbstractChannel( name, parent ) {
    activeIcon = QIcon( P("icons/userchat.xpm") );
    notify = Notification::getInstance();
}

PrivateChannel::~PrivateChannel() {}

void PrivateChannel::receiveCommand( Command command ) {
    command.name = command.name.toUpper();
    QString line = QString( "%1\n" );
    if ( command.name == "SAIDPRIVATE" ) {
        if( !UserGroupList::getInstance()->getIgnore(command.attributes.first()) ) {
            if ( command.attributes.takeFirst() == objectName() && command.attributes.join( " " ).trimmed() != "" ) {
                insertLine( flag( objectName() ) + line
                    .arg( "&lt;%1&gt; %2" )
                    .arg( objectName() )
                    .arg( processInput(command.attributes.join( " " ))));
                                if(isActive == false && Settings::Instance()->value("Chat/notifyNewPrivateMessages").toBool())
                                {
                                    QString title = QString("PM from " + objectName());
                                    QString message = QString(processInput(command.attributes.join( " " )));
                                    notify->showMessage(title, message);
                                }
            }
        }
    } else if ( command.name == "SAYPRIVATE" ) {
        if ( command.attributes.takeFirst() == objectName() && command.attributes.join( " " ).trimmed() != "" ) {
            insertLine( line.arg( "%1&lt;%2&gt; %3" )
                        .arg( flag( myUserName ) )
                        .arg( myUserName )
                        .arg( processInput(command.attributes.join( " " ))));
        }
    } else if ( command.name == "CLIENTMSG" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            insertLine( line.arg( tr( "<span style=\"color:blue;\">** CLIENTMSG ** %1</span>" )
                                  .arg( command.attributes.join( " " ) ) ) );
        }
    } else if ( command.name == "REMOVEUSER" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            insertLine( line.arg( tr( "<span style=\"color:darkgreen;\">** %1 is now offline.</span>" ).arg( objectName() ) ) );
        }
    } else if ( command.name == "ADDUSER" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            insertLine( line.arg( tr( "<span style=\"color:darkgreen;\">** %1 is now online.</span>" ).arg( objectName() ) ) );
        }
    }
}

void PrivateChannel::receiveInput( QString input ) {
    if ( executeChannelInput( input ) )
        return;
    QStringList inputList = input.split( " " );
    QString firstWord = inputList.takeFirst();
    Command ret;
    ret.name = "SAYPRIVATE";
    if ( firstWord == "/leave" )
        return;
    else if ( firstWord.toLower() == "/sayver" ) {
        ret.attributes << objectName() << QString("i am using QtLobby v%1 rev %2").arg(QTLOBBY_VERSION).arg(SVN_REV);
    } else {
        ret.attributes << objectName() << input;
    }
    emit sendCommand( ret );
}

