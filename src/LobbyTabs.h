// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
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
#include <QStandardItemModel>
#include <QStandardItem>

#include "AbstractStateClient.h"
#include "Battles.h"
#include "InfoChannel.h"
#include "Channel.h"
#include "PrivateChannel.h"
#include "BattleChannel.h"
#include "Command.h"
#include "UnitSyncLib.h"

class MainWindow;

class LobbyTabs : public AbstractStateClient {
    Q_OBJECT
public:
    LobbyTabs( QObject * parent = 0, Battles* battles = 0, UnitSyncLib* unitSyncLib = 0, QTreeView* tabTreeView = 0, QStackedWidget * lobbyStackedWidget = 0 );
    ~LobbyTabs();
    //contains all tabs with their individual functionality
    QList<AbstractLobbyTab *> lobbyTabList;
    QStringList getChannelList();
    void activateChannel(AbstractLobbyTab* channel);

signals:
    //outgoing commands to commandAssigner
    void sendCommand( Command command );
    void currentTabChanged( QString name, QString lobbyTabType );
    void changedToBattleTab();
    void changedFromBattleTab();
    void blockInput(bool b);

public slots:
    void connectionStateChanged( ConnectionState connectionState );
    void receiveCommand( Command command );
    void receiveInput( QString input, bool focus = false );
    void receiveInputAndFocus( QString input );
    void sendCommandSlot( Command command );
    void closeTab(AbstractLobbyTab* tab = 0);
    void onCloseBattleChannel();
    void onEnableJoinLeaveDefault(bool b);
    void onMapsModsReload();
    void onBattleHosted(int id);

private slots:
    void channelActivated(const QModelIndex & index);

private:
    //last tab
    AbstractLobbyTab* lastTab;
    AbstractLobbyTab* currentTab;
    //users and battles will acces this and update
    BattleChannel* battleChannel;
    InfoChannel* infoChannel;
    //this is for displaying the lobbyTabs
    QTreeView* tabTreeView;
    QStackedWidget* lobbyStackedWidget;
    QList<QWidget*> widgets;
    Battles* battles;
    UnitSyncLib* unitSyncLib;
    QString myUserName;
    QMap<QString, QString> userNameCountryCodeMap;
    bool showJoinLeaveDefault;

    //adds a new tab to lobbyTabWidget and lobbyTabList
    void createLobbyTab( AbstractLobbyTab * lobbyTab, bool focus = false );
    //refreshes the tab icon, needed when the unfocused channels get changed
    //void setTabIcon( int index );
    //void updateCloseTabState();
    // opens an empty private channel if not existing
    void privateChannelOpen( QString userName, bool popup = false );
    //needed to delegate the input to the active channel
    AbstractLobbyTab * getActiveLobbyTab();
    int mapToLobbyTabs(int index);

    QStandardItemModel* model;
    QStandardItem* battle;
    QStandardItem* channels;
    QStandardItem* privates;

    QAction* closeChannelAction;
};

#endif
