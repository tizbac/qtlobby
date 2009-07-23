// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "StatusTracker.h"

/*
** generate interesting statistics about the server in general
*/
StatusTracker::StatusTracker( QStatusBar *s ) {
    statusbar = s;



    // connection status: CONNECTED:LOGIN:ACCEPTED:...
    // connectiontime
    // users
    // clanmembers
    // games
    // mods

    statusbar->showMessage( "Status: Not connected (-h:-min) | - users | - games | - mods | - kb up / - kb down", 20000 );
}

StatusTracker::~StatusTracker() { }


