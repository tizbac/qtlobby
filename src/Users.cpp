//
// C++ Implementation: Users
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "Users.h"
#include "UserGroup.h"

Users* Users::lastThis = 0;

Users::Users( QWidget* parent ) : QTreeView( parent ) {
    infoChannelUserManager = new UserManager( this );

    setModel( infoChannelUserManager->proxyModel() );
    sortByColumn( 3, Qt::AscendingOrder );

    //   selectionModel = infoChannelUserManager->selectionModel();
    proxyModel = infoChannelUserManager->proxyModel();

    openPrivateChannelAction = new QAction( "Start private chat", this );
    slapAction = new QAction( "Slap around", this );
    joinSameBattleAction = new QAction( "Join same battle", this );

    userMenu = new QMenu( "userListContextMenu", this );
    userMenu->addAction( openPrivateChannelAction );
    userMenu->addAction( slapAction );
    userMenu->addAction( joinSameBattleAction );
    userMenu->addSeparator();
    groupsMenu = new QMenu("Add to group", this);
    removeFromGroupAction = new QAction("Remove from group", userMenu);
    clanGroupsMenu = new QMenu("Add clan to group", this);
    removeClanFromGroupAction = new QAction("Remove clan from group", userMenu);

    connect( this, SIGNAL( customContextMenuRequested( const QPoint & ) ),
             this, SLOT( customContextMenuRequestedSlot( const QPoint & ) ) );
    connect( this, SIGNAL( doubleClicked( const QModelIndex & ) ),
             this, SLOT( doubleClickedSlot( const QModelIndex & ) ) );
    clanRegexp.setPattern(".*(\\[.*\\]).*");
    lastThis = this;
    userCount = 0;
    moderatorCount = 0;
}

Users::~Users() { }

User Users::getUser( const QString userName ) {
    return infoChannelUserManager->getUser( userName );
}

const QList<User> Users::getUserList( const int battleId ) {
    if (battleIdUserManagerMap.contains(battleId))
        return battleIdUserManagerMap[battleId]->model()->userList();
    else return QList<User>();
}

QStringList Users::getUsernamesList() {
    QList<User>& users = infoChannelUserManager->model()->userList();
    QStringList ret;
    for (int i = 0; i < users.size(); i++) {
        ret << users[i].name;
    }
    return ret;
}

void Users::receiveCommand( Command command ) {
    //   qDebug() << "command: " << command.toQString();
    command.name = command.name.toUpper();
    if ( command.name == "CLIENTSTATUS" ) {
        User u = infoChannelUserManager->getUser( command.attributes[0] );
        UserState ns;
        ns.setState(command.attributes[1].toInt());
        if(ns.isModerator() && !u.userState.isModerator())
            moderatorCount++;
        else if(!ns.isModerator() && u.userState.isModerator())
            moderatorCount--;
        emit statsChange(userCount, moderatorCount);
        u.userState.setState(ns.getState());
        modUserInAllManagers( u );
    } else if ( command.name == "ADDUSER" ) {
        User u;
        u.name = command.attributes[0];
        u.countryCode = command.attributes[1];
        u.cpu = command.attributes[2];
        userCount++;
        emit statsChange(userCount, moderatorCount);
        infoChannelUserManager->addUser( u );
    } else if ( command.name == "REMOVEUSER" ) {
        User u = infoChannelUserManager->getUser( command.attributes[0] );
        userCount--;
        if(u.userState.isModerator())
            moderatorCount++;
        emit statsChange(userCount, moderatorCount);
        infoChannelUserManager->delUser( u.name );
    } else if ( command.name == "CLIENTS" ) {
        QString channelName = command.attributes.takeFirst();
        foreach( QString userName, command.attributes )
            channelUserManagerMap[channelName]->addUser( infoChannelUserManager->getUser( userName ) );
    } else if ( command.name == "CLIENTBATTLESTATUS" ) {
        User u = infoChannelUserManager->getUser( command.attributes.takeFirst() );
        u.battleState.setState(command.attributes.takeFirst().toInt());
        u.setColor( command.attributes.takeFirst().toInt() );
        modUserInAllManagers( u );
        if (u.name == url.userName())
            emit myStateChanged(u);
    } else if ( command.name == "JOINBATTLE" ) {
        int id = command.attributes.takeFirst().toInt();
        if ( !battleIdUserManagerMap.contains( id ) )
            battleIdUserManagerMap[id] = new UserManager( this, true );
        User u = infoChannelUserManager->getUser( url.userName() );
        u.joinedBattleId = id;
        modUserInAllManagers( u );
        battleIdUserManagerMap[id]->addUser( u );
    } else if ( command.name == "JOINEDBATTLE" ) {
        int id = command.attributes.takeFirst().toInt();
        if ( !battleIdUserManagerMap.contains( id ) )
            battleIdUserManagerMap[id] = new UserManager( this, true );
        if ( url.userName() != command.attributes.first() ) {
            User u = infoChannelUserManager->getUser( command.attributes.first() );
            u.joinedBattleId = id;
            modUserInAllManagers( u );
            battleIdUserManagerMap[id]->addUser( u );
        }
    } else if ( command.name == "BATTLEOPENED" ) {
        //      BATTLE_ID type natType founder
        int id = command.attributes.takeFirst().toInt();
        command.attributes.removeFirst();
        command.attributes.removeFirst();
        battleIdUserManagerMap[id] = new UserManager( this, true );
        User u = infoChannelUserManager->getUser( command.attributes.first() );
        u.joinedBattleId = id;
        modUserInAllManagers( u );
        battleIdUserManagerMap[id]->addUser( u );
    } else if ( command.name == "LEFTBATTLE" ) {
        battleIdUserManagerMap[command.attributes[0].toInt()]->delUser( command.attributes[1] );
        User u = infoChannelUserManager->getUser( command.attributes[1] );
        u.joinedBattleId = -1;
        u.userState.setIngame(false);
        modUserInAllManagers( u );
    } else if ( command.name == "FORCELEAVECHANNEL" ) {
        channelUserManagerMap[command.attributes[0]]->delUser( command.attributes[1] );
    } else if ( command.name == "BATTLECLOSED" ) {
        int id = command.attributes[0].toInt();
        // FIXME right now the map isn't removed, this leads to (js)
        //       high memory usage since we do not free it!
        //       Q: why did i comment the next line?
        //       A: because if not, then we get a segfault on BATTLECLOSED
        //     battleIdUserManagerMap.remove( id );
        updateUserList();
    } else if ( command.name == "FORCEQUITBATTLE" ) {
        User u = infoChannelUserManager->getUser( url.userName() );
        battleIdUserManagerMap[u.joinedBattleId]->delUser( url.userName() );
        u.joinedBattleId = -1;
        modUserInAllManagers( u );
        updateUserList();
    } else if ( command.name == "JOINED" ) {
        channelUserManagerMap[command.attributes[0]]->addUser( infoChannelUserManager->getUser( command.attributes[1] ) );
    } else if ( command.name == "JOIN" ) {
        channelUserManagerMap.insert( command.attributes[0], new UserManager( this ) );
    } else if ( command.name == "LEFT" ) {
        channelUserManagerMap[command.attributes[0]]->delUser( command.attributes[1] );
    }
}

void Users::setConfiguration( QUrl url ) {
    this->url = url;
}

void Users::currentTabChanged( QString name, QString lobbyTabType ) {
    currentTabName = name;
    currentTabType = lobbyTabType;
    updateUserList();
}

void Users::updateUserList() {
    if ( currentTabType == "Channel" )
        proxyModel->setSourceModel( channelUserManagerMap[currentTabName]->model() );
    else if ( currentTabType == "BattleChannel" )
        proxyModel->setSourceModel( battleIdUserManagerMap[currentTabName.toInt()]->model() );
    else
        proxyModel->setSourceModel( infoChannelUserManager->model() );
}

void Users::setRegExp( QString regExp ) {
    proxyModel->setFilterString(regExp);
    /*proxyModel->setFilterRegExp(
            QRegExp( regExp, Qt::CaseInsensitive, QRegExp::FixedString ) );
    proxyModel->setFilterKeyColumn( 3 );*/
}

void Users::delUserFromAllManagers( QString userName ) {
    foreach( UserManager* um, channelUserManagerMap.values() )
        um->delUser( userName );
    foreach( UserManager* um, battleIdUserManagerMap.values() )
        um->delUser( userName );
    infoChannelUserManager->delUser( userName );
}

void Users::modUserInAllManagers( User u ) {
    foreach( UserManager* um, channelUserManagerMap.values() )
        um->modUser( u );
    foreach( UserManager* um, battleIdUserManagerMap.values() )
        um->modUser( u );
    infoChannelUserManager->modUser( u );
}

void Users::doubleClickedSlot( const QModelIndex & index ) {
    emit sendInput( QString( "/query " ) + index.data( Qt::DisplayRole ).toString() );
}

void Users::joinSameBattle( User u ) {
    if ( !u.userState.isIngame() && u.joinedBattleId >= 0 ) {
        Command command( "JOINBATTLE" );
        command.attributes << QString( "%1" ).arg( u.joinedBattleId );
        emit sendCommand( command );
    }
}

void Users::customContextMenuRequestedSlot( const QPoint & point ) {
    if ( selectedIndexes().size() == 0 ) return;
    QModelIndex index = selectedIndexes().first();
    if ( !index.isValid() ) return;
    User u = index.data( Qt::UserRole ).value<User>();
    QColor c = index.data( Qt::BackgroundRole ).value<QColor>();
    QString clan;
    if (clanRegexp.indexIn(u.name) >= 0) {
        clan = clanRegexp.capturedTexts().at(1);
    }
    userMenu->removeAction(groupsMenu->menuAction());
    userMenu->removeAction(removeFromGroupAction);
    userMenu->removeAction(clanGroupsMenu->menuAction());
    userMenu->removeAction(removeClanFromGroupAction);
    UserGroupList* list = UserGroupList::getInstance();
    if (c.isValid()) {
        userMenu->addAction(removeFromGroupAction);
        if (!clan.isEmpty())
            userMenu->addAction(removeClanFromGroupAction);
    } else {
        groupsMenu->clear();
        QAction* newGroup = groupsMenu->addAction("New Group");
        newGroup->setObjectName("create_new_group");
        groupsMenu->addSeparator();
        if (!clan.isEmpty()) {
            clanGroupsMenu->clear();
            clanGroupsMenu->addAction(newGroup);
            clanGroupsMenu->addSeparator();
        }
        QStringList groups = list->getGroupNames();
        for (int i = 0; i < groups.size(); i++) {
            QAction* a = groupsMenu->addAction(groups.at(i));
            a->setObjectName("add_to_group");
            if (!clan.isEmpty()) {
                a = clanGroupsMenu->addAction(groups.at(i));
                a->setObjectName("add_clan_to_group");
            }
        }
        userMenu->addMenu(groupsMenu);
        if (!clan.isEmpty())
            userMenu->addMenu(clanGroupsMenu);
    }

    QAction *action = userMenu->exec( this->viewport()->mapToGlobal( point ) );
    if ( action ) {
        if ( action == openPrivateChannelAction ) {
            emit sendInput( u.name.prepend( "/query " ) );
        } else if ( action == slapAction ) {
            emit sendInput( u.name.prepend( "/slap " ) );
        } else if ( action == joinSameBattleAction ) {
            joinSameBattle( u );
        } else if ( action == removeFromGroupAction ) {
            removeUserFromGroup(u.name);
        } else if ( action == removeClanFromGroupAction ) {
            removeUserFromGroup(clan);
        } else if ( action->objectName() == "add_to_group") {
            addUserToGroup(u.name, action->text());
        } else if ( action->objectName() == "add_clan_to_group") {
            addUserToGroup(clan, action->text());
        } else if (action->objectName() == "create_new_group") {
            emit openGroupsDialog();
        }
    }
}

void Users::addUserToGroup(QString user, QString group) {
    UserGroupList* list = UserGroupList::getInstance();
    UserGroup* g = list->findGroup(group);
    if (!g->members.contains(user)) {
        g->members.append(user);
        list->updateMappings();
        invalidateModel();
    }
    list->save();
}

void Users::removeUserFromGroup(QString user) {
    UserGroupList* list = UserGroupList::getInstance();
    for (int i = 0; i < list->size(); i++) {
        UserGroup* g = list->at(i);
        int index = g->members.indexOf(user);
        if (index >= 0)
            g->members.removeAt(index);
    }
    list->updateMappings();
    invalidateModel();
    list->save();
}

void Users::inv() {
    infoChannelUserManager->proxyModel()->invalidate();
}

void Users::onMyBattleStateChanged( User u ) {
    Command command( "MYBATTLESTATUS" );
    command.attributes << QString( "%1 %2" ).arg( u.battleState.getState() ).arg( u.color() );
    emit sendCommand( command );
}

/* NEW */
void Users::onSideComboBoxChanged( int index ) {
    User u = infoChannelUserManager->getUser( url.userName() );
    u.battleState.setSide(index);
    onMyBattleStateChanged( u );
}

/* NEW */
void Users::onSpecStateChanged( int state ) {
    bool isSpec = state == Qt::Checked;
    User u = infoChannelUserManager->getUser( url.userName() );
    u.battleState.setPlayer(!isSpec);
    onMyBattleStateChanged( u );
}

void Users::onReadyStateChanged( int state ) {
    bool isReady = state == Qt::Checked;
    User u = infoChannelUserManager->getUser( url.userName() );
    u.battleState.setReady(isReady);
    onMyBattleStateChanged( u );
}

void Users::onColorChanged(QColor c) {
    User u = infoChannelUserManager->getUser( url.userName() );
    u.m_color = c;
    onMyBattleStateChanged(u);
}

void Users::onTeamNumberChanged( int i ) {
    User u = infoChannelUserManager->getUser( url.userName() );
    u.battleState.setTeamNo(i-1);
    onMyBattleStateChanged(u);
}

void Users::onAllyTeamNumberChanged( int i ) {
    User u = infoChannelUserManager->getUser( url.userName() );
    u.battleState.setAllyTeamNo(i-1);
    onMyBattleStateChanged(u);
}

void Users::invalidateModel() {
    QList<UserManager*> managers;
    managers << infoChannelUserManager;
    managers << channelUserManagerMap.values();
    managers << channelUserManagerMap.values();
    for (int i = 0; i < managers.size(); i++) {
        managers.at(i)->model()->onGroupChanged();
    }
}

Users* Users::getCurrentUsers() {
    return lastThis;
}

UserTreeModel* Users::getUserModel(int battleId) {
    return battleIdUserManagerMap[battleId]->model();
}

int Users::usersCountInCurrentChannel() {
    return model()->rowCount(QModelIndex());
}

void Users::wipeModels() {
    foreach( UserManager* um, channelUserManagerMap.values() )
        um->model()->clear();
    foreach( UserManager* um, battleIdUserManagerMap.values() )
        um->model()->clear();
    infoChannelUserManager->model()->clear();
}
