// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef CLIENTSERVERSTATUS_H
#define CLIENTSERVERSTATUS_H

#include <QObject>
#include <QStatusBar>

/**
 * 
 */
class StatusTracker : public QObject {
    Q_OBJECT
public:
    StatusTracker( QStatusBar *s );
    ~StatusTracker();
private:
    QStatusBar *statusbar;

};

#endif
