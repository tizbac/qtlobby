// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "Users.h"
#include "UserGroup.h"
#include "UserMenuBuilder.h"
#include <QInputDialog>
#include <QColorDialog>

Users* Users::lastThis = 0;

Users::Users( QWidget* parent ) : UsersTreeView( parent ) {
    infoChannelUserManager = new UserManager( this );

    setModel( infoChannelUserManager->proxyModel() );
    sortByColumn( 7, Qt::AscendingOrder );

    //   selectionModel = infoChannelUserManager->selectionModel();
    proxyModel = infoChannelUserManager->proxyModel();

    openPrivateChannelAction = new QAction( tr("Start private chat"), this );
    slapAction = new QAction( tr("Slap around"), this );
    joinSameBattleAction = new QAction( tr("Join same battle"), this );
    ignoreAction = new QAction( tr("Toggle ignore"), this );

    lastThis = this;
    userCount = 0;
    moderatorCount = 0;
    chatCol = 3;
    chatSortOrder = Qt::DescendingOrder;
    battleCol = 7;
    battleSortOrder = Qt::DescendingOrder;
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
        // seems, that this happens to occure, why?
        if( command.attributes.size() > 0 )
            u.setColor( command.attributes.takeFirst().toInt() );
        modUserInAllManagers( u );
        if (u.name == url.userName())
            emit myStateChanged(u);
        if ( currentTabType == "BattleChannel" && currentTabName.toInt() == u.joinedBattleId )
            emit teamPlayerSpecCountChanged(teamPlayerSpecCount());
    } else if ( command.name == "JOINBATTLE" ) {
        int id = command.attributes.takeFirst().toInt();
        if ( !battleIdUserManagerMap.contains( id ) )
            battleIdUserManagerMap[id] = new UserManager( this, true );
        User u = infoChannelUserManager->getUser( url.userName() );
        u.joinedBattleId = id;
        u.battleState.setPlayer(true);
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
        u.founder = true;
        modUserInAllManagers( u );
        battleIdUserManagerMap[id]->addUser( u );
    } else if ( command.name == "LEFTBATTLE" ) {
        battleIdUserManagerMap[command.attributes[0].toInt()]->delUser( command.attributes[1] );
        User u = infoChannelUserManager->getUser( command.attributes[1] );
        u.joinedBattleId = -1;
        u.founder = false;
        u.userState.setIngame(false);
        modUserInAllManagers( u );
        if ( currentTabType == "BattleChannel" && currentTabName.toInt() == u.joinedBattleId )
            emit teamPlayerSpecCountChanged(teamPlayerSpecCount());
    } else if ( command.name == "FORCELEAVECHANNEL" ) {
        channelUserManagerMap[command.attributes[0]]->delUser( command.attributes[1] );
    } else if ( command.name == "BATTLECLOSED" ) {
        int id = command.attributes[0].toInt();
        User u = infoChannelUserManager->getUser( url.userName() );
        if( u.joinedBattleId == id )
            u.joinedBattleId = -1;
        modUserInAllManagers( u );
        battleIdUserManagerMap[id]->model()->clear();
        battleIdUserManagerMap.remove( id );
    } else if ( command.name == "FORCEQUITBATTLE" ) {
        User u = infoChannelUserManager->getUser( url.userName() );
        battleIdUserManagerMap[u.joinedBattleId]->delUser( url.userName() );
        u.joinedBattleId = -1;
        modUserInAllManagers( u );
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
    QString oldTabType = currentTabType;
    currentTabType = lobbyTabType;
    currentTabName = name;
    updateUserList();
    if ( oldTabType == "Channel" && currentTabType == "BattleChannel" ) {
        chatCol = proxyModel->sortColumn();
        chatSortOrder = proxyModel->sortOrder();
        sortByColumn(battleCol, battleSortOrder);
    } else if ( oldTabType == "BattleChannel" && currentTabType == "Channel" ) {
        battleCol = proxyModel->sortColumn();
        battleSortOrder = proxyModel->sortOrder();
        sortByColumn(chatCol, chatSortOrder);
    }
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
    UserMenuBuilder* b = UserMenuBuilder::getInstance();
    b->setBattle(currentTabType == "BattleChannel");
    QMenu* menu = b->buildMenu(u);
    QAction *action = menu->exec( this->viewport()->mapToGlobal( point ) );
    b->processMenuAction(action);
}

void Users::inv() {
    infoChannelUserManager->proxyModel()->invalidate();
}

void Users::onMyBattleStateChanged( User u ) {
    Command command( "MYBATTLESTATUS" );
    command.attributes << QString( "%1 %2" ).arg( u.battleState.getState() ).arg( u.color() );
    emit sendCommand( command );
    emit teamPlayerSpecCountChanged(teamPlayerSpecCount());
}

void Users::onMyStateChanged( User u ) {
    Command command( "MYSTATUS" );
    command.attributes << QString::number( u.userState.getState() );
    emit sendCommand( command );
}

void Users::onSideComboBoxChanged( int index ) {
    User u = infoChannelUserManager->getUser( url.userName() );
    u.battleState.setSide(index);
    onMyBattleStateChanged( u );
}

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
    //fixed (ko)
    managers << battleIdUserManagerMap.values();
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

QString Users::teamPlayerSpecCount() {
    if ( currentTabType == "BattleChannel" ) {
        QMap<int,int> teamCounter;
        foreach(User u, battleIdUserManagerMap[currentTabName.toInt()]->model()->userList()) {
            int tn = u.battleState.isPlayer() ? u.battleState.getAllyTeamNo()+1 : 0;
            if( teamCounter.contains(tn) )
                teamCounter[tn]++;
            else
                teamCounter[tn] = 1;
        }
        QList<int> vals = teamCounter.values();
        if( !vals.empty() ) {
            int specs = vals.takeFirst();
            QStringList ret;
            int sum = 0;
            foreach( int val, vals) {
                ret.append(QString::number(val));
                sum += val;
            }
            QString ratio = ret.size() > 0 ? QString(" (%1)").arg(ret.join(":")) : "";
            return QString("Users: %1+%2").arg(sum).arg(QString::number(specs)).append(ratio);
        }
    }
    return "";
}


int Users::usersInChanCount() {
    int users(0);
    if ( currentTabType == "BattleChannel" )
        foreach(User u, battleIdUserManagerMap[currentTabName.toInt()]->model()->userList())
            if (u.battleState.isPlayer())
                users++;
    return users;
}

// This slot is contained in AbstractStateClient, but we inherit already from QTreeView
void Users::connectionStateChanged(ConnectionState state) {
    if( state == CONNECTED) {
        userCount = moderatorCount = 0;
        emit statsChange(userCount, moderatorCount);
    }
}

void Users::wipeModels() {
    foreach( UserManager* um, channelUserManagerMap.values() )
        um->model()->clear();
    foreach( UserManager* um, battleIdUserManagerMap.values() )
        um->model()->clear();
    infoChannelUserManager->model()->clear();
}

QString Users::getCurrentUsername() {
    return url.userName();
}

void Users::onSpringStopped() {
    User u = infoChannelUserManager->getUser( url.userName() );
    u.userState.setIngame(false);
    onMyStateChanged( u );
}
