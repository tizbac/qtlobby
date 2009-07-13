//
// C++ Interface: InfoChannel
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
