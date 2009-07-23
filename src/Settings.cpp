// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "Settings.h"

Settings::Settings( QObject *parent ) : QObject( parent ) { }
Settings::~Settings() { }

QSettings *Settings::Instance() {
    static QSettings inst(QSettings::IniFormat, QSettings::UserScope, "qtlobby", "qtlobby" );
    return &inst;
}
