// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef COMMANDASSIGNER_H
#define COMMANDASSIGNER_H

#include <QThread>
#include <QStringList>
#include <QRegExp>
#include <QDebug>
#include "Command.h"

/**
 * This class delegates parsed commands to all potential receivers.
 */
class CommandAssigner : public QThread {
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
    void battleHostCommand( Command );
public slots:
    void receiveMessage( QString message );
    void sendCommand( Command command );
protected:
    virtual void run();
};

#endif
