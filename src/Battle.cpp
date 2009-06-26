//
// C++ Implementation: Battle
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "Battle.h"

Battle::Battle() :
    id( 0 ), type( 0 ), natType( 0 ), port( 0 ), maxPlayers( 16 ), isPasswordProtected( false ), minRank( 0 ), mapHash( 0 ), isLocked( false ), spectatorCount( 0 ), playerCount(0), isStarted( false ), countryCode( "DE" ) {
}

Battle::~Battle() {
}

QString Battle::shortModName() const {
  QString ret = modName;
  ret.replace( "Absolute Annihilation", "AA" );
  ret.replace( "Complete Annihilation", "CA" );
  ret.replace( "Balanced Annihilation", "BA" );
  ret.replace( "Expand and Exterminate", "EE" );
  ret.replace( "War Evolution", "WarEv" );
  ret.replace( "TinyComm", "TC" );
  ret.replace( "Public ", "p", Qt::CaseInsensitive  );
  ret.replace( "Alpha", "a", Qt::CaseInsensitive  );
  ret.replace( "Beta", "b", Qt::CaseInsensitive );
  ret.replace( "Kernel Panic", "KP" );
  ret.replace( "stable-", "s" );
  return ret;
}

QString Battle::shortMapName() const {
  QString ret = mapName;
  ret.replace( "_", " " );
  ret.replace( "-", " " );
  ret.replace( ".smf", "" );
  return ret.trimmed();
}
