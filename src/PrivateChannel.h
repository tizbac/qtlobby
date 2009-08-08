// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef PRIVATECHANNEL_H
#define PRIVATECHANNEL_H
#include "AbstractChannel.h"
#include "Notification.h"
/**
 * A private chat channel
 */
class PrivateChannel : public AbstractChannel {
    Q_OBJECT
public:
    PrivateChannel( QString name, QObject * parent = 0 );
    virtual ~PrivateChannel();
private:
    Notification* notify;
public slots:
    void receiveInput( QString input );
    void receiveCommand( Command command );
};


#endif
