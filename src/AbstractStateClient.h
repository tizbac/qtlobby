// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef ABSTRACTSTATECLIENT_H
#define ABSTRACTSTATECLIENT_H

#include <QObject>

/**
 * The main important class in the state hierarchy.
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
