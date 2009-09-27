// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef INFOCHANNEL_H
#define INFOCHANNEL_H
#include "AbstractChannel.h"
#include "PathManager.h"
/**
 * This channel displays the server messages and server commands
 */
class InfoChannel : public AbstractChannel {
    Q_OBJECT
public:
    InfoChannel( QString name, QObject * parent = 0 );
    virtual ~InfoChannel();
    virtual void setItem(QStandardItem* item);

public slots:
    void receiveInput( QString input );
    void receiveCommand( Command command );
};

inline void InfoChannel::setItem(QStandardItem* item) {
    AbstractLobbyTab::setItem(item);
    item->setIcon(QIcon( P("icons/chanop.xpm") ));
}

#endif
