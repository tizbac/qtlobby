// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "LobbyTabs.h"
#include "Settings.h"
#include "UnitSyncLib.h"
#include "UserGroup.h"
#include "History.h"
#include "PathManager.h"
#include "MainWindow.h"

LobbyTabs::LobbyTabs( QObject * parent, Battles* battles, UnitSyncLib* unitSyncLib, QTreeView* tabTreeView, QStackedWidget * lobbyStackedWidget) : AbstractStateClient( parent ) {
    this->battles = battles;
    this->unitSyncLib = unitSyncLib;
    this->tabTreeView = tabTreeView;
    this->lobbyStackedWidget = lobbyStackedWidget;

    tabTreeView->setContextMenuPolicy(Qt::ActionsContextMenu);
    closeChannelAction = new QAction(QIcon(P("icons/closetab.png")), tr("Close"), tabTreeView);
    closeChannelAction->setShortcut(Qt::Key_Delete);
    tabTreeView->addAction(closeChannelAction);
    connect(closeChannelAction, SIGNAL(triggered()), this, SLOT(closeTab()));

    lastTab = NULL;

    connect(tabTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(channelActivated(QModelIndex)));
    connect(tabTreeView, SIGNAL(activated(QModelIndex)), this, SLOT(channelActivated(QModelIndex)));
    connect( UnitSyncLib::getInstance(), SIGNAL(rebooted()), SLOT(onMapsModsReload()));
    connect( battles, SIGNAL( closeBattleChannel() ), SLOT( onCloseBattleChannel() ) );

    showJoinLeaveDefault = Settings::Instance()->value("joinLeaveFilterDefault", true).toBool();

    model = new QStandardItemModel();
    battle = new QStandardItem(QIcon(P("icons/battle.xpm")), tr("Battle"));
    battle->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    model->appendRow(battle);
    channels = new QStandardItem(QIcon(P("icons/channel.xpm")), tr("Channels"));
    channels->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    model->appendRow(channels);
    privates = new QStandardItem(QIcon(P("icons/userchat.xpm")), tr("Privates"));
    privates->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    model->appendRow(privates);
    tabTreeView->setModel(model);

    infoChannel = new InfoChannel( "info", lobbyStackedWidget );
    createLobbyTab( infoChannel );
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
                     MainWindow::getInstance(), SLOT( playSample( SampleCollection ) ) );
        }
    } else if ( command.name == "BATTLECLOSED" ) {
        for ( int i = 0; i < lobbyTabList.count(); ++i ) {
            if ( ( lobbyTabList[i]->objectName() == command.attributes.first()
                ) && lobbyTabList[i]->metaObject()->className() == QString( "BattleChannel" )
                ) {
                closeTab(lobbyTabList[i]);
                break;
            }
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
    for ( int i = 0; i < lobbyTabList.count(); ++i ) {
        AbstractLobbyTab * l = lobbyTabList[i];
        l->receiveCommand( command );
    }
}

void LobbyTabs::sendCommandSlot( Command command ) {
    emit sendCommand( command );
}

void LobbyTabs::receiveInput( QString input, bool focus) {
    QString chatCommand = input.section( " ", 0, 0 );
    if ( chatCommand == "/query" || chatCommand == "/msg" )
        privateChannelOpen( input.section( " ", 1, 1 ), focus );
    if ( chatCommand == "/leave" || chatCommand == "/quit" || chatCommand == "/wc" )
        closeTab();
    else
        getActiveLobbyTab()->receiveInput( input );
}

void LobbyTabs::receiveInputAndFocus( QString input ) {
    receiveInput ( input, true );
}

void LobbyTabs::createLobbyTab( AbstractLobbyTab* lobbyTab, bool focus) {
    lobbyTab->myUserName = myUserName;
    lobbyTab->setUserNameCountryCodeMap( &userNameCountryCodeMap );
    if(QString(lobbyTab->metaObject()->className()) == "Channel") {
        connect((Channel*)lobbyTab, SIGNAL(enableJoinLeaveDefault(bool)),
                this, SLOT(onEnableJoinLeaveDefault(bool)));
        ((Channel*)lobbyTab)->setShowJoinLeave(showJoinLeaveDefault);
    }
    QWidget * widget = new QWidget( lobbyStackedWidget );
    lobbyTab->setupUi( widget );
    lobbyTab->widget = widget;
    lobbyStackedWidget->addWidget(widget);
    QString tabClass = QString(lobbyTab->metaObject()->className());
    QStandardItem* item = new QStandardItem();
    item->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    lobbyTab->setItem(item);
    lobbyTab->getItem()->setData(qVariantFromValue(lobbyTab), Qt::UserRole);
    if(tabClass == "InfoChannel") {
        model->invisibleRootItem()->insertRow(0, lobbyTab->getItem());
        channelActivated(lobbyTab->getItem()->index());
    } else if(tabClass == "BattleChannel") {
        battle->appendRow(lobbyTab->getItem());
        tabTreeView->expand(battle->index());
    } else if(tabClass == "Channel") {
        channels->appendRow(lobbyTab->getItem());
        tabTreeView->expand(channels->index());
    } else if(tabClass == "PrivateChannel") {
        privates->appendRow(lobbyTab->getItem());
        tabTreeView->expand(privates->index());
    }

    //connect the lobbyTab to that it can send commands
    connect( lobbyTab, SIGNAL( sendCommand( Command ) ),
             this, SLOT( sendCommandSlot( Command ) ) );
    connect( lobbyTab, SIGNAL(sendInput(QString)),
             this, SLOT(receiveInput(QString)));
    //save the lobbyTab
    lobbyTabList.append(lobbyTab);
    if (focus || QString(lobbyTab->metaObject()->className()) == "BattleChannel" ||
        (QString(lobbyTab->metaObject()->className()) == "PrivateChannel" && Settings::Instance()->value("Chat/popupNewPrivateChannel").toBool())) {
        activateChannel(lobbyTab);
        BattleChannel* bc = (BattleChannel*)lobbyTab;
        connect(bc, SIGNAL(closeMe()), this, SLOT(closeTab()));
    }
    AbstractChannel* ac = qobject_cast<AbstractChannel*>(lobbyTab);
    if(ac && Settings::Instance()->value("History/showInChannels").toBool()) {
        connect(ac, SIGNAL(needReplay(QDate,QDate)), History::getInstance(), SLOT(play(QDate,QDate)));
        connect(History::getInstance(), SIGNAL(historyMessage(QDateTime,QString)), ac, SLOT(historyMessage(QDateTime,QString)));
        connect(History::getInstance(), SIGNAL(finished()), ac, SLOT(onHistoryFinished()));
        int days = Settings::Instance()->value("History/showInChannelsDays").toInt();
        ac->requestHistoryReplay(QDate::currentDate().addDays(-days-1), QDate::currentDate().addDays(1));
    }
}

AbstractLobbyTab* LobbyTabs::getActiveLobbyTab() {
    return currentTab;
}

void LobbyTabs::channelActivated(const QModelIndex & index) {
    if(!index.isValid()) return;
    AbstractLobbyTab* tab = index.data(Qt::UserRole).value<AbstractLobbyTab*>();
    if(!tab) return;
    currentTab = tab;
    lobbyStackedWidget->setCurrentWidget(tab->widget);
    emit currentTabChanged( tab->objectName(), tab->metaObject()->className() );
    if (lastTab) {
        lastTab->setActive(false);
        if (QString( lastTab->metaObject()->className() ) == "BattleChannel" && QString( tab->metaObject()->className() ) != "BattleChannel")
            emit changedFromBattleTab();
        else if (QString( lastTab->metaObject()->className() ) != "BattleChannel" && QString( tab->metaObject()->className() ) == "BattleChannel") {
            emit changedToBattleTab();
        }
    }
    lastTab = tab;
    AbstractChannel* ac = qobject_cast<AbstractChannel*>(tab);
    if(ac)
        ac->scrollToMaximum();
    tab->setActive(true);
}

void LobbyTabs::closeTab(AbstractLobbyTab* tab) {
    if(tab) {
        tab->receiveInput("/leave");
        tab->getItem()->parent()->removeRow(currentTab->getItem()->row());
        lobbyTabList.removeAt(lobbyTabList.indexOf(tab));
        tab->deleteLater();
        if(tab == currentTab)
            activateChannel(infoChannel);
    } else {
        QModelIndex index = tabTreeView->currentIndex();
        AbstractLobbyTab* selected = index.data(Qt::UserRole).value<AbstractLobbyTab*>();
        selected->receiveInput("/leave");
        if(selected) {
            selected->getItem()->parent()->removeRow(currentTab->getItem()->row());
            lobbyTabList.removeAt(lobbyTabList.indexOf(selected));
            selected->deleteLater();
            if(currentTab == selected) {
                lastTab = NULL;
                activateChannel(infoChannel);
            }
        }
    }
}

void LobbyTabs::onCloseBattleChannel() {
    for ( int i = 0; i < lobbyTabList.count(); ++i ) {
        if ( lobbyTabList[i]->metaObject()->className() == QString( "BattleChannel" ) ) {
            closeTab(lobbyTabList[i]);
            break;
        }
    }
}

void LobbyTabs::privateChannelOpen( QString userName, bool popup ) {
    bool found = false;
    foreach( AbstractLobbyTab * l, lobbyTabList ) {
        if ( l->objectName() == userName && l->metaObject()->className() == QString( "PrivateChannel" ) ) {
            found = true;
            if(popup || Settings::Instance()->value("Chat/popupNewPrivateChannel").toBool())
                activateChannel(l);
            break;
        }
    }
    if ( !found ) {
        createLobbyTab( new PrivateChannel( userName, lobbyStackedWidget ), popup );
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
             MainWindow::getInstance(), SLOT( playSample( SampleCollection ) ) );
}

void LobbyTabs::activateChannel(AbstractLobbyTab* channel) {
    QModelIndex idx = channel->getItem()->index();
    tabTreeView->setCurrentIndex(idx);
    channelActivated(idx);
}

void LobbyTabs::onNextTab() {
    qDebug() << "onNextTab()";
    QModelIndex idx = tabTreeView->model()->sibling( (currentTab->getItem()->row() + 1) % tabTreeView->model()->rowCount(tabTreeView->rootIndex()), 0, tabTreeView->rootIndex() );
    tabTreeView->setCurrentIndex(idx);
    channelActivated(idx);
}

void LobbyTabs::onPreviousTab() {
}
