// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef ABSTRACTSTATECLIENT_H
#define ABSTRACTSTATECLIENT_H

#include <QObject>

/**
 @author Joachim Schiele <js@lastlog.de>
*/

/*
** The main important class in the state hierarchy is the
**
**
**
*/
enum ConnectionState {
    DISCONNECTED,
    CONNECTING,
    CONNECTED,
    AUTHENTICATING,
    AUTHENTICATED
};

class AbstractStateClient : public QObject {
    Q_OBJECT
public:
    AbstractStateClient( QObject *parent = 0 );
    ~AbstractStateClient();

    ConnectionState connectionState;

public slots:
    virtual void connectionStateChanged( ConnectionState ) = 0;

};

#endif
