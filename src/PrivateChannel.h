// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef PRIVATECHANNEL_H
#define PRIVATECHANNEL_H
#include "AbstractChannel.h"
/**
a private chat channel

 @author Mirko Windhoff <qtlobby.reg@ncnever.de>
*/
class PrivateChannel : public AbstractChannel {
    Q_OBJECT
public:
    PrivateChannel( QString name, QObject * parent = 0 );
    virtual ~PrivateChannel();

public slots:
    void receiveInput( QString input );
    void receiveCommand( Command command );
};


#endif
