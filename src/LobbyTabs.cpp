// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "LobbyTabs.h"
#include "Settings.h"
#include "UnitSyncLib.h"
#include "UserGroup.h"

LobbyTabs::LobbyTabs( QObject * parent, Battles* battles, UnitSyncLib* unitSyncLib, QTabBar* tabBar, QStackedWidget * lobbyStackedWidget) : AbstractStateClient( parent ) {
    this->battles = battles;
    this->unitSyncLib = unitSyncLib;
    this->tabBar = tabBar;
    this->lobbyStackedWidget = lobbyStackedWidget;
    lastIndex = -1;

    connect( tabBar, SIGNAL(tabCloseRequested(int)),
             this, SLOT( closeTab(int) ) );
    //change the icon for unfocused channels with changed contents / restore when focused
    connect( tabBar, SIGNAL( currentChanged(int)),
             this, SLOT( currentTabChangedSlot( int ) ) );
    connect (UnitSyncLib::getInstance(), SIGNAL(rebooted()), SLOT(onMapsModsReload()));
    //first we need an InfoChannel to display different status messages, this will not close with the close tab
    createLobbyTab( new InfoChannel( "info", lobbyStackedWidget ) );
    currentTabChangedSlot(0);
    showJoinLeaveDefault = Settings::Instance()->value("joinLeaveFilterDefault", true).toBool();
}

LobbyTabs::~LobbyTabs() {
    foreach( AbstractLobbyTab * tab, lobbyTabList ) {
        delete tab;
    }
}

void LobbyTabs::connectionStateChanged( ConnectionState connectionState ) {
    this->connectionState = connectionState;

    if ( connectionState == AUTHENTICATED ) {
        bool qtlobbyChannelFound = false, mainChannelFound = false;
/*        foreach( AbstractLobbyTab * l, lobbyTabList ) {
            if ( QString(l->metaObject()->className()) == "Channel" ) {
                if ( QString(l->objectName()) == "qtlobby" )
                    qtlobbyChannelFound = true;
                receiveInput( QString( "/j " ) + l->objectName() );
            }
        }
        */
        //Restoring channels
        QSettings * settings = Settings::Instance();
        QStringList channels = settings->value("channels").toStringList();
        foreach(QString channel, channels) {
            receiveInput("/j " + channel);
            if ( channel == "qtlobby" )
                qtlobbyChannelFound = true;
            if ( channel == "main" )
                mainChannelFound = true;
        }
        if ( !qtlobbyChannelFound ) {
            if ( !settings->contains("Chat/joinQtlobby") || settings->value("Chat/joinQtlobby").toBool() )
                lobbyTabList[0]->receiveInput( "/j qtlobby" );
        }
        if ( !mainChannelFound ) {
            if ( !settings->contains("Chat/joinMain") || settings->value("Chat/joinMain").toBool() )
                lobbyTabList[0]->receiveInput( "/j main" );
        }
    }
}

void LobbyTabs::receiveCommand( Command command ) {
    // when server ACKs the join we open the channel with its name, if it is not opened already
    if ( command.name == "JOIN" ) {
        bool found = false;
        foreach( AbstractLobbyTab * l, lobbyTabList ) {
            if ( l->objectName() == command.attributes.first() ) {
                found = true;
                break;
            }
        }
        if ( !found )
            createLobbyTab( new Channel( command.attributes.first(), lobbyStackedWidget ) );
    } else if ( command.name == "JOINBATTLE" ) {
        bool found = false;
        foreach( AbstractLobbyTab * l, lobbyTabList ) {
            if ( l->objectName() == command.attributes.first() && l->metaObject()->className() == QString( "BattleChannel" ) ) {
                found = true;
                break;
            }
        }
        if ( !found ) {
            battleChannel = new BattleChannel( command.attributes.first(), battles, lobbyStackedWidget );
            createLobbyTab( battleChannel );
            connect( battleChannel, SIGNAL( playSample( SampleCollection ) ),
                     parent(), SLOT( playSample( SampleCollection ) ) );
        }
    } else if ( command.name == "BATTLECLOSED" /*|| command.name == "FORCEQUITBATTLE"*/ ) {
        bool found = false;
        int index = 0;
        for ( int i = 0; i < lobbyTabList.count(); ++i ) {
            if (( lobbyTabList[i]->objectName() == command.attributes.first()
                //             || command.name == "FORCEQUITBATTLE"
                ) && lobbyTabList[i]->metaObject()->className() == QString( "BattleChannel" )
                ) {
                found = true;
                index = i;
                break;
            }
        }
        if ( found ) {
            QWidget * win = lobbyTabList[index]->currentWidget;
            lobbyStackedWidget->removeWidget(win);
            tabBar->removeTab(lobbyTabList[index]->currentTabIndex);
            QTimer::singleShot( 0, win, SLOT( deleteLater() ) );
            lobbyTabList.removeAt(index);
        }
    } else if ( command.name == "SAIDPRIVATE" ) {
        if( !UserGroupList::getInstance()->getIgnore(command.attributes.first()) )
            privateChannelOpen( command.attributes.first(), false );
    } else if ( command.name == "SAYPRIVATE" ) {
        privateChannelOpen( command.attributes.first(), false );
    } else if ( command.name == "ADDUSER" ) {
        userNameCountryCodeMap[command.attributes[0]] = command.attributes[1];
    } else if ( command.name == "ADDUSER" ) {
        userNameCountryCodeMap.remove( command.attributes.first() );
    } else if ( command.name == "ACCEPTED" ) {
        myUserName = command.attributes.first(); //FIXME this should by done in AbstractStateClient some how
        foreach( AbstractLobbyTab* l, lobbyTabList )
            l->myUserName = myUserName;
    }
    // send the command to every lobbyTab
    int currentIndex = mapToLobbyTabs(tabBar->currentIndex());
    for ( int i = 0; i < lobbyTabList.count(); ++i ) {
        AbstractLobbyTab * l = lobbyTabList[i];
        //when focused set as active, needed to choose the icon
        l->setActive( i == currentIndex );
        l->receiveCommand( command );
        //set the icon
        tabBar->setTabIcon( l->currentTabIndex, l->icon );
        tabBar->setTabTextColor( l->currentTabIndex, l->color );
    }
}

void LobbyTabs::sendCommandSlot( Command command ) {
    emit sendCommand( command );
}

void LobbyTabs::receiveInput( QString input ) {
    QString chatCommand = input.section( " ", 0, 0 );
    if ( chatCommand == "/query" || chatCommand == "/msg" )
        privateChannelOpen( input.section( " ", 1, 1 ) );
    if ( chatCommand == "/leave" || chatCommand == "/quit" || chatCommand == "/wc" )
        closeTab();
    else
        getActiveLobbyTab()->receiveInput( input );
}

void LobbyTabs::createLobbyTab( AbstractLobbyTab * lobbyTab ) {
    lobbyTab->myUserName = myUserName;
    lobbyTab->setUserNameCountryCodeMap( &userNameCountryCodeMap );
    if(QString(lobbyTab->metaObject()->className()) == "Channel") {
        connect((Channel*)lobbyTab, SIGNAL(enableJoinLeaveDefault(bool)),
                this, SLOT(onEnableJoinLeaveDefault(bool)));
        ((Channel*)lobbyTab)->setShowJoinLeave(showJoinLeaveDefault);
    }
    QWidget * widget = new QWidget( lobbyStackedWidget );
    //at new chat windows near the active window
    int c = tabBar->count();
    lobbyTab->currentWidget = widget;
    lobbyTab->currentTabIndex = c;
    //add the widget to the lobbyStackedWidget

    // qt 4.4 bug line
    tabBar->addTab( lobbyTab->icon, lobbyTab->getTabTitle() );
    lobbyStackedWidget->addWidget(widget);
    //connect the widget to the lobbyTab and fill with default content
    lobbyTab->setupUi( widget );
    //connect the lobbyTab to that it can send commands
    connect( lobbyTab, SIGNAL( sendCommand( Command ) ),
             this, SLOT( sendCommandSlot( Command ) ) );
    connect( lobbyTab, SIGNAL(sendInput(QString)),
             this, SLOT(receiveInput(QString)));
    //save the lobbyTab
    lobbyTabList.append(lobbyTab);
    if (QString(lobbyTab->metaObject()->className()) == "BattleChannel" ||
        (QString(lobbyTab->metaObject()->className()) == "PrivateChannel" && Settings::Instance()->value("Chat/popupNewPrivateChannel").toBool())) {
        lobbyStackedWidget->setCurrentWidget(widget);
        tabBar->setCurrentIndex(c);
    }
}

AbstractLobbyTab * LobbyTabs::getActiveLobbyTab() {
    return lobbyTabList[mapToLobbyTabs(tabBar->currentIndex())];
}

void LobbyTabs::currentTabChangedSlot( int index ) {
    // why do we need the following two lines to get this working with qt 4.4 and
    // why did it run without any problem on qt 4.3? (js)
    if ( lobbyTabList.size() == 0 )
        return;
    index = mapToLobbyTabs(index);
    lobbyStackedWidget->setCurrentWidget(lobbyTabList[index]->currentWidget);
    if (index < 0) return;
    setTabIcon( index );
    tabBar->setTabTextColor( index, lobbyTabList[index]->color );
    bool isBattleTab = QString( lobbyTabList[index]->metaObject()->className() ) == "BattleChannel";
    if(isBattleTab) {
        BattleChannel* bc = qobject_cast<BattleChannel*>(lobbyTabList[index]);
        if(bc->isBlocked())
            emit blockInput(true);
        else
            emit blockInput(false);
    } else {
        emit blockInput(false);
    }
    //updateCloseTabState();
    emit currentTabChanged( lobbyTabList[index]->objectName(),
                            lobbyTabList[index]->metaObject()->className() );

    if (lastIndex >= 0) {
        if (QString( lobbyTabList[lastIndex]->metaObject()->className() ) == "BattleChannel" && QString( lobbyTabList[index]->metaObject()->className() ) != "BattleChannel")
            emit changedFromBattleTab();
        else if (QString( lobbyTabList[lastIndex]->metaObject()->className() ) != "BattleChannel" && QString( lobbyTabList[index]->metaObject()->className() ) == "BattleChannel") {
            emit changedToBattleTab();
        }
    }
    lastIndex = index;
}

void LobbyTabs::setTabIcon( int index ) {
    //set the current tab to active, to restore the icon
    lobbyTabList[index]->setActive( mapToLobbyTabs(tabBar->currentIndex()) == index );
    //we refresh the icon for the current tab
    tabBar->setTabIcon( lobbyTabList[index]->currentTabIndex, lobbyTabList[index]->icon );
}

/*void LobbyTabs::updateCloseTabState() {
        lobbyStackedWidget->cornerWidget( Qt::TopRightCorner )->setEnabled( lobbyStackedWidget->currentIndex() != 0 );
    }*/


void LobbyTabs::closeTab() {
    closeTab(tabBar->currentIndex());
}

void LobbyTabs::closeTab(int i) {
    int index = mapToLobbyTabs(i);
    if (index < 0) return;
    if (QString(lobbyTabList[index]->metaObject()->className()) == "InfoChannel")
        return;

    QWidget * win = lobbyTabList[index]->currentWidget;
    //send the leave command
    lobbyTabList[index]->receiveInput("/leave");
    lobbyStackedWidget->removeWidget(win);
    tabBar->removeTab(i);
    win->deleteLater();
    //updateCloseTabState();
    //delete the lobbyTab from the list
    for (; i < lobbyTabList.size(); i++) {
        lobbyTabList[mapToLobbyTabs(i)]->currentTabIndex--;
    }
    emit currentTabChanged( lobbyTabList[mapToLobbyTabs(tabBar->currentIndex())]->objectName(),
                            lobbyTabList[mapToLobbyTabs(tabBar->currentIndex())]->metaObject()->className() );
    lobbyTabList[index]->deleteLater();
    lobbyTabList.removeAt( index );
}

void LobbyTabs::privateChannelOpen( QString userName, bool popup ) {
    bool found = false;
    foreach( AbstractLobbyTab * l, lobbyTabList ) {
        if ( l->objectName() == userName && l->metaObject()->className() == QString( "PrivateChannel" ) ) {
            found = true;
            if(popup && Settings::Instance()->value("Chat/popupNewPrivateChannel").toBool())
                tabBar->setCurrentIndex(l->currentTabIndex);
            break;
        }
    }
    if ( !found ) {
        createLobbyTab( new PrivateChannel( userName, lobbyStackedWidget ) );
    }
}

QStringList LobbyTabs::getChannelList() {
    QStringList ret;
    foreach( AbstractLobbyTab * l, lobbyTabList ) {
        if ( QString(l->metaObject()->className()) == "Channel" ) {
            ret << l->objectName();
        }
    }
    return ret;
}


int LobbyTabs::mapToLobbyTabs(int i) {
    int index = -1;
    for (int x = 0; x < lobbyTabList.size(); x++) {
        if (lobbyTabList[x]->currentTabIndex == i) {
            index = x;
            break;
        }
    }
    return index;
}

void LobbyTabs::onTabMoved( int from, int to ) {
    if (from == to) return;
    AbstractLobbyTab* t_from = lobbyTabList[mapToLobbyTabs(from)];
    AbstractLobbyTab* t_to = lobbyTabList[mapToLobbyTabs(to)];
    int fromIndex = t_from->currentTabIndex;
    t_from->currentTabIndex = t_to->currentTabIndex;
    t_to->currentTabIndex = fromIndex;
    currentTabChangedSlot(from);
    //setTabIcon(mfrom);
    //setTabIcon(mto);
}

void LobbyTabs::onEnableJoinLeaveDefault(bool b) {
    showJoinLeaveDefault = b;
    Settings::Instance()->setValue("joinLeaveFilterDefault", b);
    foreach( AbstractLobbyTab * l, lobbyTabList ) {
        if ( QString(l->metaObject()->className()) == "Channel" ) {
            ((Channel*)l)->setShowJoinLeave(b);
        }
    }
}

void LobbyTabs::onMapsModsReload() {
    foreach( AbstractLobbyTab * l, lobbyTabList ) {
        if ( QString(l->metaObject()->className()) == "BattleChannel" ) {
            ((BattleChannel*)l)->refreshMapAndModOptions();
            break;
        }
    }
}

void LobbyTabs::onBattleHosted(int id) {
    battleChannel = new BattleChannel( QString::number(id), battles, lobbyStackedWidget );
    createLobbyTab( battleChannel );
    connect( battleChannel, SIGNAL( playSample( SampleCollection ) ),
             parent(), SLOT( playSample( SampleCollection ) ) );
}
