// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef INFOCHANNEL_H
#define INFOCHANNEL_H
#include "AbstractChannel.h"
/**
this channel displays the server messages and server commands

 @author Mirko Windhoff <qtlobby.reg@ncnever.de>
*/
class InfoChannel : public AbstractChannel {
    Q_OBJECT
public:
    InfoChannel( QString name, QObject * parent = 0 );
    virtual ~InfoChannel();

public slots:
    void receiveInput( QString input );
    void receiveCommand( Command command );
};

#endif
