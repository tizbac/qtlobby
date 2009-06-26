//
// C++ Interface: StatusTracker
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CLIENTSERVERSTATUS_H
#define CLIENTSERVERSTATUS_H

#include <QObject>
#include <QStatusBar>

/**
 @author Joachim Schiele <js@lastlog.de>
*/
class StatusTracker : public QObject
{
  Q_OBJECT
public:
  StatusTracker( QStatusBar *s );
  ~StatusTracker();
private:
  QStatusBar *statusbar;

};

#endif
