//
// C++ Interface: LobbyTabs
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LOBBYTABS_H
#define LOBBYTABS_H
#include <QList>
#include <QWidget>
#include <QStackedWidget>
#include <QTabBar>
#include <QStyleOptionTab>
#include <QApplication>
#include <QToolButton>
#include <QTimer>
#include <QDebug>

#include "AbstractStateClient.h"
#include "Battles.h"
#include "InfoChannel.h"
#include "Channel.h"
#include "PrivateChannel.h"
#include "BattleChannel.h"
#include "Command.h"
#include "UnitSyncLib.h"

#include <QDebug>

class MainWindow;

/**
  @author Mirko Windhoff <qtlobby.reg@ncnever.de>
*/
class LobbyTabs : public AbstractStateClient {
    Q_OBJECT
public:
    LobbyTabs( QObject * parent = 0, Battles* battles = 0, UnitSyncLib* unitSyncLib = 0, QTabBar* tabBar = 0, QStackedWidget * lobbyStackedWidget = 0 );
    ~LobbyTabs();
    //contains all tabs with their individual functionality
    QList<AbstractLobbyTab *> lobbyTabList;
    QStringList getChannelList();

signals:
    //outgoing commands to commandAssigner
    void sendCommand( Command command );
    void currentTabChanged( QString name, QString lobbyTabType );
    void changedToBattleTab();
    void changedFromBattleTab();

public slots:
    //sets the internal state occording the global connection state
    void connectionStateChanged( ConnectionState connectionState );
    //incoming commands from commandAssigner
    void receiveCommand( Command command );
    //incoming commands from inputLineEdit
    void receiveInput( QString input );
    //receives commands to be sent to commandAssigner
    void sendCommandSlot( Command command );
    //sets the tab icon and emits the update signal for the user list
    void currentTabChangedSlot( int index );
    //close tab
    void closeTab(int i);
    void closeTab();

private:
    //last tab index
    int lastIndex;
    //users and battles will acces this and update
    BattleChannel* battleChannel;
    //this is for displaying the lobbyTabs
    QTabBar* tabBar;
    QStackedWidget* lobbyStackedWidget;
    QList<QWidget*> widgets;
    Battles* battles;
    UnitSyncLib* unitSyncLib;
    QString myUserName;
    QMap<QString, QString> userNameCountryCodeMap;
    //adds a new tab to lobbyTabWidget and lobbyTabList
    void createLobbyTab( AbstractLobbyTab * lobbyTab );
    //refreshes the tab icon, needed when the unfocused channels get changed
    void setTabIcon( int index );
    //void updateCloseTabState();
    // opens an empty private channel if not existing
    void privateChannelOpen( QString userName );
    //needed to delegate the input to the active channel
    AbstractLobbyTab * getActiveLobbyTab();
    int mapToLobbyTabs(int index);
};

#endif
