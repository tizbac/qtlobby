//
// C++ Implementation: Settings
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "Settings.h"

Settings::Settings( QObject *parent ) : QObject( parent ) { }
Settings::~Settings() { }

QSettings *Settings::Instance() {
    static QSettings inst(QSettings::IniFormat, QSettings::UserScope, "qtlobby", "qtlobby" );
    return &inst;
}
