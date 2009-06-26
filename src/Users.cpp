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

Users::Users( QWidget* parent ) : QTreeView( parent )
{
  infoChannelUserManager = new UserManager( this );

  setModel( infoChannelUserManager->proxyModel() );
  sortByColumn( 3, Qt::AscendingOrder );
  setColumnWidth( 0, 20 );
  setColumnWidth( 1, 20 );
  setColumnWidth( 2, 20 );

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
  userMenu->addAction( "not connected" );

  connect( this, SIGNAL( customContextMenuRequested( const QPoint & ) ),
           this, SLOT( customContextMenuRequestedSlot( const QPoint & ) ) );
  connect( this, SIGNAL( doubleClicked( const QModelIndex & ) ),
           this, SLOT( doubleClickedSlot( const QModelIndex & ) ) );
}

Users::~Users() { }

User Users::getUser( const QString userName ) {
  return infoChannelUserManager->getUser( userName );
}

QList<User>& Users::getUserList( const int battleId ) {
  Q_ASSERT( battleIdUserManagerMap.contains( battleId ) );
    return battleIdUserManagerMap[battleId]->model()->userList();
}

void Users::receiveCommand( Command command ) {
//   qDebug() << "command: " << command.toQString();
  command.name = command.name.toUpper();
  if ( command.name == "CLIENTSTATUS" ) {
    User u = infoChannelUserManager->getUser( command.attributes[0] );
    u.userState.state = command.attributes[1].toInt();
    modUserInAllManagers( u );
  } else if ( command.name == "ADDUSER" ) {
    User u;
    u.name = command.attributes[0];
    u.countryCode = command.attributes[1];
    u.cpu = command.attributes[2];
    u.userState.s.skill = 0;
    infoChannelUserManager->addUser( u );
  }
  else if ( command.name == "REMOVEUSER" ) {
    infoChannelUserManager->delUser( command.attributes[0] );
  }
  else if ( command.name == "CLIENTS" ) {
    QString channelName = command.attributes.takeFirst();
    foreach( QString userName, command.attributes )
    channelUserManagerMap[channelName]->addUser( infoChannelUserManager->getUser( userName ) );
  }
  else if ( command.name == "CLIENTBATTLESTATUS" ) {
    User u = infoChannelUserManager->getUser( command.attributes.takeFirst() );
    u.battleState.state = command.attributes.takeFirst().toInt();
    u.setColor( command.attributes.takeFirst().toInt() );
    modUserInAllManagers( u );
  }
  else if ( command.name == "JOINBATTLE" ) {
    int id = command.attributes.takeFirst().toInt();
    if ( !battleIdUserManagerMap.contains( id ) )
      battleIdUserManagerMap[id] = new UserManager( this );
    User u = infoChannelUserManager->getUser( url.userName() );
    u.joinedBattleId = id;
    modUserInAllManagers( u );
    battleIdUserManagerMap[id]->addUser( u );
  }
  else if ( command.name == "JOINEDBATTLE" ) {
    int id = command.attributes.takeFirst().toInt();
    if ( !battleIdUserManagerMap.contains( id ) )
      battleIdUserManagerMap[id] = new UserManager( this );
    if ( url.userName() != command.attributes.first() ) {
      User u = infoChannelUserManager->getUser( command.attributes.first() );
      u.joinedBattleId = id;
      modUserInAllManagers( u );
      battleIdUserManagerMap[id]->addUser( u );
    }
  }
  else if ( command.name == "BATTLEOPENED" ) {
//      BATTLE_ID type natType founder
    int id = command.attributes.takeFirst().toInt();
    command.attributes.removeFirst();
    command.attributes.removeFirst();
    battleIdUserManagerMap[id] = new UserManager( this );
    User u = infoChannelUserManager->getUser( command.attributes.first() );
    u.joinedBattleId = id;
    modUserInAllManagers( u );
    battleIdUserManagerMap[id]->addUser( u );
  }
  else if ( command.name == "LEFTBATTLE" ) {
    battleIdUserManagerMap[command.attributes[0].toInt()]->delUser( command.attributes[1] );
    User u = infoChannelUserManager->getUser( command.attributes[1] );
    u.joinedBattleId = -1;
    u.userState.s.isInGame = false;
    modUserInAllManagers( u );
  }
  else if ( command.name == "FORCELEAVECHANNEL" ) {
    channelUserManagerMap[command.attributes[0]]->delUser( command.attributes[1] );
  }
  else if ( command.name == "BATTLECLOSED" ) {
    int id = command.attributes[0].toInt();
    // FIXME right now the map isn't removed, this leads to (js)
    //       high memory usage since we do not free it!
    //       Q: why did i comment the next line?
    //       A: because if not, then we get a segfault on BATTLECLOSED
    //     battleIdUserManagerMap.remove( id );
    updateUserList();
  }
  else if ( command.name == "FORCEQUITBATTLE" ) {
    User u = infoChannelUserManager->getUser( url.userName() );
    battleIdUserManagerMap[u.joinedBattleId]->delUser( url.userName() );
    u.joinedBattleId = -1;
    modUserInAllManagers( u );
    updateUserList();
  }
  else if ( command.name == "JOINED" ) {
    channelUserManagerMap[command.attributes[0]]->addUser( infoChannelUserManager->getUser( command.attributes[1] ) );
  }
  else if ( command.name == "JOIN" ) {
    channelUserManagerMap.insert( command.attributes[0], new UserManager( this ) );
  }
  else if ( command.name == "LEFT" ) {
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
  proxyModel->setFilterRegExp(
    QRegExp( regExp, Qt::CaseInsensitive, QRegExp::FixedString ) );
  proxyModel->setFilterKeyColumn( 3 );
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
  if ( !u.userState.s.isInGame && u.joinedBattleId >= 0 ) {
    Command command( "JOINBATTLE" );
    command.attributes << QString( "%1" ).arg( u.joinedBattleId );
    emit sendCommand( command );
  }
}

void Users::customContextMenuRequestedSlot( const QPoint & point ) {
  if ( selectedIndexes().size() == 0 ) return;
  QModelIndex index = selectedIndexes().first();
  if ( !index.isValid() ) return;

  QAction *action = userMenu->exec( this->viewport()->mapToGlobal( point ) );
  if ( action ) {
    User u = index.data( Qt::UserRole ).value<User>();
    if ( action == openPrivateChannelAction ) {
      emit sendInput( u.name.prepend( "/query " ) );
    } else if ( action == slapAction ) {
      emit sendInput( u.name.prepend( "/slap " ) );
    } else if ( action == joinSameBattleAction ) {
      joinSameBattle( u );
    }
  }
}

void Users::inv() {
  infoChannelUserManager->proxyModel()->invalidate();
}

void Users::onMyBattleStateChanged( User u ) {
  Command command( "MYBATTLESTATUS" );
  command.attributes << QString( "%1 %2" ).arg( u.battleState.state ).arg( u.color() );
  emit sendCommand( command );
}

void Users::onReadyStateChanged( bool isReady ) {
  User u = infoChannelUserManager->getUser( url.userName() );
  u.battleState.s.isReady = isReady;
  emit onMyBattleStateChanged( u );
}
