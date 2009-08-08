// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef INFOCHANNEL_H
#define INFOCHANNEL_H
#include "AbstractChannel.h"
/**
 * This channel displays the server messages and server commands
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
