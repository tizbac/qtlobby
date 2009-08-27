// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "Battles.h"
#include <QStringList>
#include <algorithm>
#include <list>

using namespace std;

Battles::Battles( QWidget* parent ) : QTreeView( parent ) {
    battleManager = new BattleManager( this );
    battlePasswordWidget = new BattlePasswordWidget();
    battleCloseFirstWidget = new BattleCloseFirstWidget();

    settings = Settings::Instance();

    // FIXME: the descending sort of column 7 does not work.
    setModel( battleManager->proxyModel() );
    setSortingEnabled( true );
    sortByColumn( 7, Qt::DescendingOrder );
    battleManager->proxyModel()->invalidate();

    setColumnWidth( 0, 20 );
    setColumnWidth( 1, 20 );
    setColumnWidth( 2, 20 );
    setColumnWidth( 3, 170 );
    setColumnWidth( 4, 200 );
    setColumnWidth( 5, 80 );
    setColumnWidth( 6, 100 );
    setColumnWidth( 7, 60 );

    m_menu = new QMenu();
    openPrivateChannelAction = new QAction(tr("Private Chat to Host"), this);
    m_menu->addAction(openPrivateChannelAction);
    joinBattleAction = new QAction(tr("Join Battle"), this);
    m_menu->addAction(joinBattleAction);
    m_filterMenu = new QMenu(tr("Filter Battles"));
    filterPasswordedAction = new QAction(tr("Hide battles with password"), this);
    filterPasswordedAction->setCheckable(true);
    m_filterMenu->addAction(filterPasswordedAction);
    filterInGameAction = new QAction(tr("Hide running battles"), this);
    filterInGameAction->setCheckable(true);
    m_filterMenu->addAction(filterInGameAction);
    filterLockedAction = new QAction(tr("Hide locked battles"), this);
    filterLockedAction->setCheckable(true);
    m_filterMenu->addAction(filterLockedAction);
    filterUnavailableModsAction = new QAction(tr("Hide not installed mods"), this);
    filterUnavailableModsAction->setCheckable(true);
    m_filterMenu->addAction(filterUnavailableModsAction);
    filterUnavailableMapsAction = new QAction(tr("Hide not installed maps"), this);
    filterUnavailableMapsAction->setCheckable(true);
    m_filterMenu->addAction(filterUnavailableMapsAction);
    filterWithoutPlayersAction = new QAction(tr("Hide battles without players"), this);
    filterWithoutPlayersAction->setCheckable(true);
    m_filterMenu->addAction(filterWithoutPlayersAction);
    filterWithoutFriendsAction = new QAction(tr("Hide battles without friends"), this);
    filterWithoutFriendsAction->setCheckable(true);
    m_filterMenu->addAction(filterWithoutFriendsAction);
    m_menu->addMenu(m_filterMenu);

    connect( this, SIGNAL( customContextMenuRequested( const QPoint & ) ),
             this, SLOT( customContextMenuRequested( const QPoint & ) ) );
    connect( this, SIGNAL( doubleClicked( const QModelIndex & ) ),
             this, SLOT( doubleClicked( const QModelIndex & ) ) );
    connect( this, SIGNAL( wantJoinBattle( unsigned int, QString, bool ) ),
             this, SLOT( joinBattleCommand( unsigned int, QString, bool ) ) );
    connect( battlePasswordWidget, SIGNAL( wantJoinBattle( unsigned int, QString, bool ) ),
             this, SLOT( joinBattleCommand( unsigned int, QString, bool ) ) );
    connect( battleCloseFirstWidget, SIGNAL( wantJoinBattle( unsigned int, QString, bool ) ),
             this, SLOT( joinBattleCommand( unsigned int, QString, bool ) ) );
    connect( filterPasswordedAction, SIGNAL( toggled( bool ) ),
             this, SLOT( setFilterPasswordedSlot( bool ) ) );
    connect( filterInGameAction, SIGNAL( toggled( bool ) ),
             this, SLOT( setFilterInGameSlot( bool ) ) );
    connect( filterLockedAction, SIGNAL( toggled( bool ) ),
             this, SLOT( setFilterLockedSlot( bool ) ) );
    connect( filterUnavailableModsAction, SIGNAL( toggled( bool ) ),
             this, SLOT( setFilterUnavailableModsSlot( bool ) ) );
    connect( filterUnavailableMapsAction, SIGNAL( toggled( bool ) ),
             this, SLOT( setFilterUnavailableMapsSlot( bool ) ) );
    connect( filterWithoutPlayersAction, SIGNAL( toggled( bool ) ),
             this, SLOT( setFilterWithoutPlayersSlot( bool ) ) );
    connect( filterWithoutFriendsAction, SIGNAL( toggled( bool ) ),
             this, SLOT( setFilterWithoutFriendsSlot( bool ) ) );
    connect (UnitSyncLib::getInstance(), SIGNAL(rebooted()),
             this, SLOT(onReboot()));

    battleCount = 0;
    QSettings* s = Settings::Instance();
    if( s->contains("Battles/filterPassworded" ) )
        filterPasswordedAction->setChecked( s->value( "Battles/filterPassworded" ).toBool() );
    if( s->contains("Battles/filterInGame" ) )
        filterInGameAction->setChecked( s->value( "Battles/filterInGame" ).toBool() );
    if( s->contains("Battles/filterLocked" ) )
        filterLockedAction->setChecked( s->value( "Battles/filterLocked" ).toBool() );
    if( s->contains("Battles/filterUnavailableMods" ) )
        filterUnavailableModsAction->setChecked( s->value( "Battles/filterUnavailableMods" ).toBool() );
    if( s->contains("Battles/filterUnavailableMaps" ) )
        filterUnavailableMapsAction->setChecked( s->value( "Battles/filterUnavailableMaps" ).toBool() );
    if( s->contains("Battles/filterWithoutPlayers" ) )
        filterWithoutPlayersAction->setChecked( s->value( "Battles/filterWithoutPlayers" ).toBool() );
    if( s->contains("Battles/filterWithoutFriends" ) )
        filterWithoutFriendsAction->setChecked( s->value( "Battles/filterWithoutFriends" ).toBool() );

    m_portOverride = 0;
}

Battles::~Battles() {}

void Battles::receiveCommand( Command command ) {
    //   qDebug()<< "command: " << command.toQString();
    command.name = command.name.toUpper();
    if ( command.name == "CLIENTSTATUS" ) {
        User u = users->getUser( command.attributes[0] );
        int joinedBattleId = u.joinedBattleId;
        if ( !battleManager->isBattleId( joinedBattleId ) )
            return;
        Battle b = battleManager->getBattle( joinedBattleId );
        if ( b.founder == u.name ) {
            u.userState.setState(command.attributes[1].toInt());
            b.isStarted = u.userState.isIngame();
            battleManager->modBattle( b );
            if ( users->getUser( url.userName() ).joinedBattleId == b.id && b.isStarted ) {
                User me = users->getUser( url.userName() );
                if(u.name == me.name) {
                    startGame( b, true );
                } else {
                    me.userState.setIngame(true);
                    emit sendCommand(Command(QString("MYSTATUS %1").arg(me.userState.getState())));
                    startGame( b, false );
                }
            }
        }
    } else if ( command.name == "JOINBATTLE" ) {
    } else if ( command.name == "JOINBATTLEFAILED" ) {
        QMessageBox::critical(this, "Join battle failed", command.attributes.join(" "));
    } else if ( command.name == "JOINEDBATTLE" ) {
        if ( !battleManager->isBattleId( command.attributes[0].toInt() ) )
            return;
        Battle b = battleManager->getBattle( command.attributes[0].toInt() );
        b.playerCount++;
        battleManager->modBattle( b );
    } else if ( command.name == "LEFTBATTLE" ) {
        if ( !battleManager->isBattleId( command.attributes[0].toInt() ) )
            return;
        Battle b = battleManager->getBattle( command.attributes[0].toInt() );
        b.playerCount--;
        battleManager->modBattle( b );
    } else if ( command.name == "BATTLEOPENED" ) {
        Q_ASSERT( command.attributes.size() >= 11 );
        Battle b;
        b.id                  = command.attributes.takeFirst().toInt();
        b.type                = command.attributes.takeFirst().toInt() > 0;
        b.natType             = command.attributes.takeFirst().toInt();
        b.founder             = command.attributes.takeFirst();
        b.ip                  = command.attributes.takeFirst();
        b.port                = command.attributes.takeFirst().toInt();
        b.maxPlayers          = command.attributes.takeFirst().toInt();
        b.isPasswordProtected = command.attributes.takeFirst().toInt() > 0;
        b.minRank             = command.attributes.takeFirst().toInt();
        b.mapHash             = command.attributes.takeFirst().toInt();
        command.attributes    = command.attributes.join( " " ).split( "\t" );
        QString mapName       = command.attributes.takeFirst();
        b.mapName             = mapName;
        b.title               = command.attributes.takeFirst();
        b.modName             = command.attributes.takeFirst();
        b.playerCount         = 1;
        b.countryCode = users->getUser( b.founder ).countryCode;
        battleManager->addBattle( b );
        battleCount++;
        emit statsChange(battleCount);
    } else if ( command.name == "BATTLECLOSED" ) {
        battleManager->delBattle( command.attributes[0].toInt() );
        battleCount--;
        emit statsChange(battleCount);
    } else if ( command.name == "UPDATEBATTLEINFO" ) {
        Q_ASSERT( command.attributes.size() >= 5 );
        int battleId = command.attributes.takeFirst().toInt();
        if ( !battleManager->isBattleId( battleId ) )
            return;
        Battle b = battleManager->getBattle( battleId );
        b.spectatorCount = command.attributes.takeFirst().toInt();
        b.isLocked = command.attributes.takeFirst().toInt() > 0;
        b.mapHash = command.attributes.takeFirst().toInt();
        b.mapName = command.attributes.join( " " );
        battleManager->modBattle( b );
        if ( users->getUser( url.userName() ).joinedBattleId == battleId ) {
            User u = users->getUser( url.userName() );
            u.battleState.setSyncState(resyncStatus());
            users->onMyBattleStateChanged( u );
        }
    } else if ( command.name == "SETSCRIPTTAGS" ) {
        int bi = users->getUser( username ).joinedBattleId;
        if( !battleManager->isBattleId( bi ) )
            return;
        Battle b = battleManager->getBattle( bi );
        command.attributes = command.attributes.join( " " ).split( "\t" );
        foreach( QString s, command.attributes ) {
            QString key = s.section( "=", 0, 0 ).toLower().replace( "game/", "" );
            b.options[key] = s.section( "=", 1, 1 ).toInt();
        }
        battleManager->modBattle( b );
    } else if ( command.name == "REMOVESCRIPTTAGS" ) {
        int bi = users->getUser( username ).joinedBattleId;
        if( !battleManager->isBattleId( bi ) )
            return;
        Battle b = battleManager->getBattle( bi );
        command.attributes = command.attributes.join( " " ).split( "\t" );
        foreach( QString s, command.attributes ) {
            QString key = s.section( "=", 0, 0 ).toLower().replace( "game/", "" );
            if ( b.options.contains( key ) )
                b.options.remove( key );
        }
        battleManager->modBattle( b );
    } else if ( command.name == "ADDSTARTRECT" ) { // allyno left top right bottom
        int bi = users->getUser( username ).joinedBattleId;
        if( !battleManager->isBattleId( bi ) )
            return;
        Battle b = battleManager->getBattle( bi );
        b.allyNumberStartRectMap[command.attributes[0].toInt()] = StartRect(
                command.attributes[1].toInt(),
                command.attributes[2].toInt(),
                command.attributes[3].toInt(),
                command.attributes[4].toInt()
                );
        QRect r;
        r.setCoords(command.attributes[1].toInt(),
                    command.attributes[2].toInt(),
                    command.attributes[3].toInt(),
                    command.attributes[4].toInt());
        emit addStartRect(command.attributes[0].toInt(), r);
        battleManager->modBattle( b );
    } else if ( command.name == "REMOVESTARTRECT" ) { // allyno
        int bi = users->getUser( username ).joinedBattleId;
        if( !battleManager->isBattleId( bi ) )
            return;
        Battle b = battleManager->getBattle( bi );
        b.allyNumberStartRectMap.remove( command.attributes[0].toInt() );
        battleManager->modBattle( b );
        emit removeStartRect(command.attributes[0].toInt());
    } else if ( command.name == "REQUESTBATTLESTATUS" ) {
        User u = users->getUser( url.userName() );
        QList<User> list = users->getUserList(u.joinedBattleId);
        int playersNum = 0;
        for (int i = 0; i < list.size(); i++) {
            if (list.at(i).battleState.isPlayer())
                playersNum++;
        }
        playersNum++;
        u.userState.setIngame(true);
        u.battleState.setTeamNo(playersNum);
        u.battleState.setAllyTeamNo(playersNum);
        Battle b = battleManager->getBattle(u.joinedBattleId);
        int preferedSide = settings->value("sidesPreferences/" + b.modName).toInt();
        u.battleState.setSide(preferedSide);
        u.m_color = QColor( "red" );
        u.battleState.setPlayer(true);
        u.battleState.setSyncState(resyncStatus());
        users->modUserInAllManagers(u);
        users->onMyBattleStateChanged( u );
    } else if ( command.name == "HOSTPORT" ) {
        m_portOverride = command.attributes.at(0).toInt();
    }
}

void Battles::setConfiguration( QUrl url ) {
    this->url = url;
}

void Battles::setRegExp( QString regExp ) {
    battleManager->proxyModel()->setFilterString(regExp);
    //battleManager->proxyModel()->setFilterKeyColumn( -1 );
    //   proxyModel->setFilterRole( Qt::ToolTipRole );
}

void Battles::doubleClicked( const QModelIndex & index ) {
    Battle b = battleManager->model()-> data(
            battleManager->proxyModel()->mapToSource( index ), Qt::UserRole ).value<Battle>();
    if ( !b.isLocked ) {
        User me = this->users->getUser( url.userName() );
        if ( me.joinedBattleId == b.id ) // don't rejoin same
            return;
        if ( b.isPasswordProtected ) {
            battlePasswordWidget->setBattleId( b.id );
            battlePasswordWidget->resetPassword();
            battlePasswordWidget->show();
            return;
        }
    }
    emit wantJoinBattle( b.id, "", false );
}

void Battles::joinBattleCommand( unsigned int id, QString password, bool firstClose ) {
    if ( firstClose ) {
        emit closeBattleChannel();
    } else {
        User me = this->users->getUser( url.userName() );
        if ( me.joinedBattleId >= 0 ) { // in battle
            if ( !Settings::Instance()->value( "Battle/autoCloseFirst", false ).toBool() ) {
                battleCloseFirstWidget->setPassword( password );
                battleCloseFirstWidget->setBattleId( id );
                battleCloseFirstWidget->show();
                return;
            }
            emit closeBattleChannel();
        }
    }
    Command command( "JOINBATTLE" );
    command.attributes << QString( "%1 %2" ).arg( id ).arg( password );
    emit sendCommand( command );
}

void Battles::customContextMenuRequested( const QPoint & point ) {
    if ( selectedIndexes().size() == 0 ) { // no battles in list
        openPrivateChannelAction->setVisible(false);
        joinBattleAction->setVisible(false);
        m_menu->exec( this->viewport()->mapToGlobal( point ) );
        openPrivateChannelAction->setVisible(true);
        joinBattleAction->setVisible(true);
    } else {
        QModelIndex index = selectedIndexes().first();
        if ( !index.isValid() ) return;
        Battle b = battleManager->model()->data(
                battleManager->proxyModel()->mapToSource( index ), Qt::UserRole ).value<Battle>();
        QAction *action = m_menu->exec( this->viewport()->mapToGlobal( point ) );
        if ( action ) {
            if ( action == openPrivateChannelAction ) {
                emit sendInput( b.founder.prepend( "/query " ) );
            } else if ( action == joinBattleAction ) {
                emit doubleClicked( index );
            }
        }
    }
}

void Battles::setUsers( Users* users ) {
    this->users = users;
    battleManager->setUsers(users);
}

void Battles::setCurrentUsername(QString a_username) {
    username = a_username;
}

void Battles::startGame( Battle b, bool host ) {
    QString springDir = settings->value("spring_user_dir").toString();
    QFile scriptFile( springDir + "/script_qtlobby.txt" );
    if ( !scriptFile.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) ) {
        qDebug() << tr("Error: Could not open script file for write: ") << scriptFile.fileName();
        return;
    }
    QTextStream out( &scriptFile );
    out << generateScript( b, host );
    scriptFile.close();
    qDebug() << tr("Script file %1 written.").arg(springDir + "/script_qtlobby.txt");
    emit start();
}

QString Battles::generateScript( Battle b, bool host ) {
    /** begin with refactored code from springlobby */
    //get and sort the battle users
    QList<User> battleUsersUnsorted = users->getUserList( b.id );
    // invert the order, now users are in order as they joined the battle
    // this is because in the UserTreeModel users are prepended to the list, when joining
    QList<User> battleUsers;
    foreach( User u, battleUsersUnsorted )
        battleUsers.prepend( u );
    /// process ordered users, convert teams and allies
    QMap<int, int> teamConv, allyConv, allyReverseConv;
    int  numTeams = 0, numAllies = 0, myPlayerNum = -1;
    for ( int i = 0; i < battleUsers.size(); ++i ) {
        User u = battleUsers[i];
        if ( u.name == url.userName() )
            myPlayerNum = i;
        if ( !u.battleState.isPlayer() )
            continue;
        if ( !teamConv.contains( u.battleState.getTeamNo() ) )
            teamConv[u.battleState.getTeamNo()] = numTeams++;  // to skip empty teams
        if ( !allyConv.contains( u.battleState.getAllyTeamNo() ) ) {
            allyConv[u.battleState.getAllyTeamNo()] = numAllies++; // to skip empty allies
            allyReverseConv[i] = u.battleState.getAllyTeamNo();
        }
    }
    /// sort and process bots now
    //   for ( std::list<BattleBot*>::size_type i = 0; i < battle.GetNumBots(); i++ ) {
    //     UserOrder tmp;
    //     tmp.index = i;
    //     tmp.order = battle.GetBot( i )->bs.order;
    //     ordered_bots.push_back( tmp );
    //   }
    //   std::sort( ordered_bots.begin(), ordered_bots.end() );
    //   for ( int i = 0;i < int( ordered_bots.size() );++i ) {
    //     BattleBot &bot = *battle.GetBot( ordered_bots[i].index );
    //     if ( bot.bs.ally > int( AllyConv.size() ) - 1 ) {
    //       AllyConv.resize( bot.bs.ally + 1, -1 );
    //     }
    //     if ( AllyConv[bot.bs.ally] == -1 ) {
    //       AllyConv[bot.bs.ally] = AllyRevConv.size();
    //       AllyRevConv.push_back( bot.bs.ally );
    //     }
    //   }
    int numBots = 0; //ordered_bots.size();
    // first the general game options
    QMap<QString, QString> gameOptions;
    gameOptions["Mapname"]  = b.mapName;
    gameOptions["GameType"] = UnitSyncLib::getInstance()->modArchive( UnitSyncLib::getInstance()->modIndex( b.modName ) );
    //append options from the battle
    foreach( QString k, b.options.keys() ) {
        if (k.contains("/")) continue;
        gameOptions[k] = QString::number( b.options[k].toFloat() );
    }
    gameOptions["HostPort"]     = QString::number(m_portOverride > 0 ? m_portOverride : b.port );
    gameOptions["HostIP"]       = b.founder == url.userName() ? QString( "localhost" ) : b.ip;
    gameOptions["IsHost"]       = host ? "1" : "0";
    gameOptions["MyPlayerNum"]  = QString::number( myPlayerNum );
    gameOptions["MyPlayerName"] = username;
    gameOptions["NumPlayers"]   = QString::number( b.playerCount );
    gameOptions["NumTeams"]     = QString::number( numTeams + numBots );
    gameOptions["NumAllyTeams"] = QString::number( numAllies );

    //fill now all sections with options
    QMap<QString, QMap<QString, QString> > sectionsOptionMap;
    QMap<QString, QString> options;
    for ( int i = 0; i < battleUsers.size(); ++i ) {
        options.clear();
        options["Name"]        = battleUsers[i].name;
        options["CountryCode"] = battleUsers[i].countryCode;
        options["Spectator"]   = battleUsers[i].battleState.isPlayer() ? "0" : "1";
        options["Team"]        = QString::number( teamConv[battleUsers[i].battleState.getTeamNo()] );
        sectionsOptionMap[QString( "PLAYER%1" ).arg( i )] = options;
    }

    // first user in team is the leader, though he's the last one who will overwrite the value for the team
    QMap<int, int> teamNumberLeaderUserNumberMap;
    for ( int j = battleUsers.size() - 1; j >= 0; --j ) {
        teamNumberLeaderUserNumberMap[battleUsers[j].battleState.getTeamNo()] = j;
        qDebug() << battleUsers[j].battleState.getTeamNo() << " " << j;
    }

    for ( int i = 0; i < numTeams; i++ ) {
        options.clear();
        int teamLeader = teamNumberLeaderUserNumberMap.values()[i];
        User u = battleUsers[teamLeader];
        options["TeamLeader"] = QString::number( teamLeader );
        options["AllyTeam"] = QString::number( allyConv[u.battleState.getAllyTeamNo()] );
        options["RGBColor"] = u.colorForScript();
        UnitSyncLib::getInstance()->setCurrentMod(b.modName);
        options["Side"] = UnitSyncLib::getInstance()->sideName( u.battleState.getSide() );
        options["Handicap"] = QString::number( u.battleState.getHandicap() );
        sectionsOptionMap[QString( "TEAM%1" ).arg( i )] = options;
    }

    //   for ( int i = 0; i < numBots; i++ ) {
    //     options.clear();
    //     int teamLeader = teamNumberLeaderUserNumberMap[i];
    //     User u = battleUsers[teamLeader];
    //     options["TeamLeader"] = QString::number( teamLeader );
    //     options["AllyTeam"] = QString::number( allyConv[u.battleState.s.ally] );
    //     options["RGBColor"] = u.colorForScript();
    //     options["Side"] = usync()->GetSideName( b.modName, u.battleState.s.side );
    //     options["Handicap"] = QString::number( u.battleState.s.handicap );
    //     options["AIDLL"] = "bot.aidll";
    //     sectionsOptionMap[QString( "TEAM%1" ).arg( i + numTeams )] = options;
    //   }

    int startpostype = b.options["StartPosType"].toFloat();
    for ( int i = 0; i < numAllies; i++ ) {
        int numInAlly = 0;
        options.clear();
        options["NumAllies"] = QString::number( numInAlly );
        if ( b.allyNumberStartRectMap.contains( allyReverseConv[i] )
            && startpostype == StartPosType::Choose ) {
            StartRect sr = b.allyNumberStartRectMap[allyReverseConv[i]];
            options["StartRectLeft"]   = QString( "%1" ).arg( sr.left   / 200.0, 0, 'f', 3 );
            options["StartRectTop"]    = QString( "%1" ).arg( sr.top    / 200.0, 0, 'f', 3 );
            options["StartRectRight"]  = QString( "%1" ).arg( sr.right  / 200.0, 0, 'f', 3 );
            options["StartRectBottom"] = QString( "%1" ).arg( sr.bottom / 200.0, 0, 'f', 3 );
        }
        sectionsOptionMap[QString( "ALLYTEAM%1" ).arg( i )] = options;
    }
    options.clear();
    // restrictions
    gameOptions["NumRestrictions"] = QString::number( b.disabledUnits.size() );
    for ( int i = 0; i < b.disabledUnits.size(); ++i ) {
        options[QString( "Unit%1" ).arg( i )] = b.disabledUnits[i];
        options[QString( "Limit%1" ).arg( i )] = "0";
    }
    sectionsOptionMap["RESTRICT"] = options;
    options.clear();
    sectionsOptionMap["MAPOPTIONS"] = options;
    sectionsOptionMap["MODOPTIONS"] = options;

    // the scripttags like GAME/MODOPTIONS/...
    foreach( QString tag, b.options.keys() ) {
        options.clear();
        if ( tag.contains( "/" ) ) {
            if ( sectionsOptionMap.contains( tag.section( "/", 0, 0 ) ) ) {
                sectionsOptionMap[tag.section( "/", 0, 0 )][tag.section( "/", 1, 1 )] = b.options.value(tag).toString();
            } else {
                options[tag.section( "/", 1, 1 )] = b.options.value( tag ).toString();
                sectionsOptionMap[tag.section( "/", 0, 0 )] = options;
            }
        }
    }

    // generate the script from the options
    QString ret; //!< the returned script file string
    QString sectionPattern = "%1[%2]\n%1{\n%3%1}\n"; // %1 "\t" or "", %2 name, %3 values
    QString optionPattern  = "%1%2=%3;\n"; // %1 "\t" or "\t\t", %2 name, %3 value
    foreach( QString k, gameOptions.keys() ) {
        ret += optionPattern.arg( "\t" ).arg( k ).arg( gameOptions[k] );
    }
    foreach( QString k, sectionsOptionMap.keys() ) {
        QString optionString = "";
        foreach( QString l, sectionsOptionMap[k].keys() ) {
            optionString += optionPattern.arg( "\t\t" ).arg( l ).arg( sectionsOptionMap[k][l] );
        }
        ret += sectionPattern.arg( "\t", k, optionString );
    }
    return sectionPattern.arg( "", "GAME", ret );
}

/**
 * returns the sync status for map checksums
 */
int Battles::resyncStatus() {
    int battleId = users->getUser( url.userName() ).joinedBattleId;

    if ( !battleManager->isBattleId( battleId ) ) {
        // set battle status to unsync
        // popup messagebox with error, that this should not happen
        qDebug() << __FILE__ << __LINE__<< " error: should I resync for a none existing battle?!";
        return 0;
    }
    Battle b = battleManager->getBattle( battleId );
    QString modName = b.modName;

    if ( UnitSyncLib::getInstance()->mapChecksum( b.mapName ) != ( unsigned int ) b.mapHash )
        return 2; // 2 = not sync
    if (UnitSyncLib::getInstance()->modIndex(modName) < 0)
        return 2; // 2 = not sync

    return 1; // 1 = sync is 'in sync'
}

void Battles::invalidateModel() {
    battleManager->model()->onGroupChanged();
}

// This slot is contained in AbstractStateClient, but we inherit already from QTreeView
void Battles::connectionStateChanged(ConnectionState state) {
    if( state == CONNECTED) {
        battleCount = 0;
        emit statsChange(battleCount);
    }
}

void Battles::wipeModels() {
    battleManager->model()->clear();
}

/* Battle filter slots */
void Battles::setFilterPasswordedSlot( bool state ) {
    battleManager->proxyModel()->setBitState( 0, state );
    Settings::Instance()->setValue("Battles/filterPassworded", state);
}

void Battles::setFilterInGameSlot( bool state ) {
    battleManager->proxyModel()->setBitState( 1, state );
    Settings::Instance()->setValue("Battles/filterInGame", state);
}

void Battles::setFilterLockedSlot( bool state ) {
    battleManager->proxyModel()->setBitState( 2, state );
    Settings::Instance()->setValue("Battles/filterLocked", state);
}

void Battles::setFilterUnavailableModsSlot( bool state ) {
    battleManager->proxyModel()->setBitState( 3, state );
    Settings::Instance()->setValue("Battles/filterUnavailableMods", state);
}

void Battles::setFilterUnavailableMapsSlot( bool state ) {
    battleManager->proxyModel()->setBitState( 4, state );
    Settings::Instance()->setValue("Battles/filterUnavailableMaps", state);
}

void Battles::setFilterWithoutPlayersSlot( bool state ) {
    battleManager->proxyModel()->setBitState( 5, state );
    Settings::Instance()->setValue("Battles/filterWithoutPlayers", state);
}

void Battles::setFilterWithoutFriendsSlot( bool state ) {
    battleManager->proxyModel()->setBitState( 6, state );
    Settings::Instance()->setValue("Battles/filterWithoutFriends", state);
}

void Battles::onReboot() {
    qDebug() << "Resyncing";
    User u = users->getUser( url.userName() );
    u.battleState.setSyncState(resyncStatus());
    users->onMyBattleStateChanged( u );
}

/**
 * handling enter pressed and translation
 */
bool Battles::event(QEvent* event) {
    QKeyEvent* keyEvent;
    switch(event->type()) {
    case QEvent::KeyPress:
        keyEvent = static_cast<QKeyEvent*>(event);
        if ( ( keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return ) ) {
            onJoin();
            return true;
        }
        break;
    case QEvent::LanguageChange:
        qDebug() << "Battles QEvent::LanguageChange entered";
        this->retranslateUi();
        break;
    default:
        break;
    }
    return QTreeView::event(event);
}

void Battles::onJoin() {
    if( selectedIndexes().size() > 0 )
        doubleClicked(selectedIndexes().first());
}

void Battles::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected ) {
    if( ( deselected.indexes().size() > 0 ) != ( selected.indexes().size() > 0 ) )
        emit battleSelected(selected.indexes().size() > 0);
    QTreeView::selectionChanged( selected, deselected );
}

void Battles::retranslateUi() {
    this->battleManager->model()->retranslateUi();
}
