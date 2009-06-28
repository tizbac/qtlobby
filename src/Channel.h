//
// C++ Interface: Channel
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CHANNEL_H
#define CHANNEL_H
#include <QDateTime>
#include "AbstractChannel.h"

/**
this channel displays the chat messages

 @author Mirko Windhoff <qtlobby.reg@ncnever.de>
*/
class Channel : public AbstractChannel
{
  Q_OBJECT
public:
  Channel( QString name, QObject * parent = 0 );
  virtual ~Channel();

public slots:
  void receiveInput( QString input );
  void receiveCommand( Command command );
};

#endif
