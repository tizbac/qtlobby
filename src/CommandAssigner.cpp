// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "CommandAssigner.h"

CommandAssigner::CommandAssigner( QObject* parent ) : QObject( parent ) {}
CommandAssigner::~CommandAssigner() {}

void CommandAssigner::receiveMessage( QString message ) {
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
    QString battleCommands = "RING,JOINBATTLE,JOINEDBATTLE,UPDATEBATTLEINFO,LEFTBATTLE,"
        "BATTLEOPENED,BATTLECLOSED,CLIENTBATTLESTATUS,CLIENTSTATUS,SETSCRIPTTAGS,"
        "REQUESTBATTLESTATUS,ADDSTARTRECT,REMOVESTARTRECT,JOINBATTLEFAILED";
    if ( battleCommands.split( "," ).contains( command.name, Qt::CaseInsensitive ) ) {
        emit battleCommand( command );
    }

    /** channel and chat related */
    QString userCommands = "ADDUSER,REMOVEUSER,CLIENTS,CLIENTBATTLESTATUS,JOINBATTLE,"
        "JOINEDBATTLE,LEFTBATTLE,FORCELEAVECHANNEL,BATTLEOPENED,BATTLECLOSED,"
        "FORCEQUITBATTLE,CLIENTSTATUS,SAID,JOINED,JOIN,LEFT,OPENBATTLE";
    // not needed: CHANNELTOPIC,JOIN,SAIDEX,SAIDPRIVATE,
    if ( userCommands.split( "," ).contains( command.name, Qt::CaseInsensitive ) ) {
        emit userCommand( command );
    }
    /*Battle host*/
    QString battleHostCommands = "OPENBATTLE,OPENBATTLEFAILED,SAIDBATTLE,SAIDPRIVATE,"
                                 "JOINEDBATTLE,LEFTBATTLE,BATTLECLOSED,CLIENTBATTLESTATUS";
    if ( battleHostCommands.split( "," ).contains( command.name, Qt::CaseInsensitive ) ) {
        emit battleHostCommand( command );
    }
    /* server context */
    /*misc*/
    emit lobbyTabCommand( command );
}

void CommandAssigner::sendCommand( Command command ) {
    emit sendMessage( command.toQString() );
}
