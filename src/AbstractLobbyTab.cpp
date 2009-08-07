// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "AbstractLobbyTab.h"

AbstractLobbyTab::AbstractLobbyTab( QObject * parent ) {
    this->setParent( parent );
    isActive = false;
    currentWidget = 0;
    currentTabIndex = -1;
}
AbstractLobbyTab::~AbstractLobbyTab() {}

QString AbstractLobbyTab::flag( const QString userName ) {
    QString flag = "<img width=\"16\" height=\"16\" src=\":/flags/%1.xpm\" />&nbsp;";
    if( Settings::Instance()->value("Chat/showFlags").toBool() )
        if ( userNameCountryCodeMap->contains( userName ) )
            return flag.arg( userNameCountryCodeMap->value( userName ) );
    return "";
}

QString AbstractLobbyTab::userNameLink( const QString userName ) {
    return QString( "<a href=\"qtlobby://%1\">%1</a>" ).arg(userName);
}

void AbstractLobbyTab::setUserNameCountryCodeMap( QMap<QString, QString>* userNameCountryCodeMap ) {
    this->userNameCountryCodeMap = userNameCountryCodeMap;
}

QString AbstractLobbyTab::getTabTitle() {
    return objectName();
}
