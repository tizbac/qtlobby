//
// C++ Implementation: LobbyTab
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "AbstractLobbyTab.h"

AbstractLobbyTab::AbstractLobbyTab( QObject * parent ) {
  this->setParent( parent );
  isActive = false;
}
AbstractLobbyTab::~AbstractLobbyTab() {}

QString AbstractLobbyTab::flag( const QString userName ) {
  QString flag = "<img width=\"10\" height=\"10\" src=\":/flags/%1.xpm\" />&nbsp;";
  if( userNameCountryCodeMap->contains( userName ) )
    return flag.arg( userNameCountryCodeMap->value( userName ) );
  return "";
}

void AbstractLobbyTab::setUserNameCountryCodeMap( QMap<QString, QString>* userNameCountryCodeMap ) {
  this->userNameCountryCodeMap = userNameCountryCodeMap;
}
