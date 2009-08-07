// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "Channel.h"
#include "UserGroup.h"

Channel::Channel( QString name, QObject * parent ) : AbstractChannel( name, parent ) {
    isDefault = true;
    uiSetup = false;
}

Channel::~Channel() {}

void Channel::setupUi( QWidget * tab ) {
    AbstractChannel::setupUi(tab);
    connect(channelTextBrowser, SIGNAL(enableJoinLeave(bool)), SLOT(onEnableJoinLeave(bool)));
    connect(channelTextBrowser, SIGNAL(enableJoinLeaveDefault(bool)), SIGNAL(enableJoinLeaveDefault(bool)));
    QSettings* s = Settings::Instance();
    if(s->contains("joinLeaveFilter/" + objectName())) {
        isDefault = false;
        showJoinLeave = s->value("joinLeaveFilter/" + objectName()).toBool();
    }
    channelTextBrowser->setEnableJoinLeave(showJoinLeave);
    channelTextBrowser->setEnableJoinLeaveDefault(showJoinLeaveDefault);
    uiSetup = true;
}

void Channel::receiveCommand( Command command ) {
    command.name = command.name.toUpper();
    QString line = QString( "%1\n" );
    if ( command.name == "SAID" ) {
        if ( command.attributes.takeFirst() == objectName() ) {//BROKEN
            QString userName = command.attributes.takeFirst();
			if(!UserGroupList::getInstance()->getIgnore(userName)) {
				insertLine( flag( userName ) + line
					.arg( "&lt;%1&gt; %2" )
					.arg( userName )
					.arg( processInput(command.attributes.join( " " ))));
			}
        }
    } else if ( command.name == "SAIDEX" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            QString userName = command.attributes.takeFirst();
			if(!UserGroupList::getInstance()->getIgnore(userName)) {
				insertLine( flag( userName ) + line
					.arg( "<span style=\"color:magenta;\">* %1 %2</span>" )
					.arg( userName )
					.arg( processInput(command.attributes.join( " " ), false)));
			}
        }
    } else if ( command.name == "JOINED"  && showJoinLeave ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            QString userName = command.attributes.takeFirst();
            insertLine( flag( userName ) + line
                        .arg( "<span style=\"color:darkgreen;\">%1</span>" )
                        .arg( tr( "** %1 joined the channel." ) )
                        .arg( userName ) );
        }
    } else if ( command.name == "LEFT" && showJoinLeave) {
        if ( command.attributes.takeFirst() == objectName()) {
            QString userName = command.attributes.takeFirst();
            insertLine( flag( userName ) + line
                        .arg( "<span style=\"color:darkkhaki;\">** %1</span>" )
                        .arg( tr( "%1 left the channel." ) )
                        .arg( userName ) );
        }
    } else if ( command.name == "JOIN" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            insertLine( line
                        .arg( "<span style=\"color:darkgreen;\">** %1</span>" )
                        .arg( tr( "%1 joined the channel." ) )
                        .arg( myUserName ) );
        }
    } else if ( command.name == "CLIENTMSG" ) {
        if ( command.attributes.takeFirst() == objectName() ) {
            insertLine( line
                        .arg( "<span style=\"color:blue;\">** CLIENTMSG ** %1</span>" )
                        .arg( command.attributes.join( " " ) ) );
        }
    } else if ( command.name == "CHANNELTOPIC" ) {
        if (command.attributes.takeFirst() == objectName()) {
            QString name = command.attributes.takeFirst();
            QString time_t = command.attributes.takeFirst();
            QString msg = command.attributes.join(" ");
            msg.replace("\\n","<br/>");

            QDateTime date;
            date.setTime_t(uint(time_t.toULong()/1000));

            insertLine(line
                       .arg("<span style=\"color: darkred;\">** %1</span>")
                       .arg(tr("Topic is ' %1 ' set by %2 %3"))
                       .arg( urlify(msg) )
                       .arg( name )
                       .arg( date.toString(tr("dd.MM.yyyy hh:mm")) ));
        }
    }
}

void Channel::receiveInput( QString input ) {
    if ( executeChannelInput( input ) )
        return;
    //input.replace( "<", "&lt;" );
    //input.replace( ">", "&gt;" );

    // Remove Right to Left mark so text doesnt get messed up
    input.replace(QChar(0x202E),"");

    QStringList inputList = input.split( " " );
    QString firstWord = inputList.takeFirst();
    Command ret;
    if ( "/me" == firstWord ) {
        ret.name = "SAYEX";
        ret.attributes << objectName() << inputList;
    } else if ( "/slap" == firstWord ) {
        ret.name = "SAYEX";
        ret.attributes << objectName() << QString( "slaps %1 around a bit with a large trout" ).arg( inputList.join( " " ) );
    } else if ( QString( "/leave" ).split( "," ).contains( firstWord, Qt::CaseInsensitive ) ) {
        ret.name = "LEAVE";
        ret.attributes << objectName() << ( inputList.empty() ? ( inputList << QString( "Quit" ) ) : inputList );
    } else {
        ret.name = "SAY";
        ret.attributes << objectName() << input;
    }
    emit sendCommand( ret );
}

void Channel::setShowJoinLeave(bool b) { //Setting default from lobby tabs
    if(uiSetup) {
        channelTextBrowser->setEnableJoinLeaveDefault(b);
        if(isDefault)
            channelTextBrowser->setEnableJoinLeave(b);
    }
    showJoinLeaveDefault = b;
    if(isDefault) {
        showJoinLeave = b;
    }
}

void Channel::onEnableJoinLeave(bool b) {
    if(uiSetup)
        channelTextBrowser->setEnableJoinLeave(b);
    isDefault = false;
    showJoinLeave = b;
    Settings::Instance()->setValue("joinLeaveFilter/" + objectName(), b);
}
