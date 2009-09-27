// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "AbstractLobbyTab.h"
#include "PathManager.h"

AbstractLobbyTab::AbstractLobbyTab( QObject * parent ) {
    this->setParent( parent );
    isActive = false;
    widget = 0;
    currentTabIndex = -1;
    userNameCountryCodeMap = 0;
}
AbstractLobbyTab::~AbstractLobbyTab() {}

void AbstractLobbyTab::setUserNameCountryCodeMap( QMap<QString, QString>* userNameCountryCodeMap ) {
    this->userNameCountryCodeMap = userNameCountryCodeMap;
}

QString AbstractLobbyTab::getTabTitle() {
    return objectName();
}
