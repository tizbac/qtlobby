//
// C++ Implementation: Battles
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "Battles.h"
#include <QStringList>
#include <algorithm>
#include <list>

using namespace std;

Battles::Battles( QWidget* parent ) : QTreeView( parent ) {
    battleManager = new BattleManager( this );
    gamePasswordWidget = new GamePasswordWidget();

    settings = Settings::Instance();

    setModel( battleManager->proxyModel() );
    setSortingEnabled( true );
    sortByColumn( 0, Qt::AscendingOrder );
    setColumnWidth( 0, 20 );
    setColumnWidth( 1, 20 );
    setColumnWidth( 2, 20 );
    setColumnWidth( 3, 170 );
    setColumnWidth( 4, 200 );
    setColumnWidth( 5, 80 );
    setColumnWidth( 6, 100 );
    setColumnWidth( 7, 60 );

    connect( this, SIGNAL( customContextMenuRequested( const QPoint & ) ),
             this, SLOT( customContextMenuRequested( const QPoint & ) ) );
    connect( this, SIGNAL( doubleClicked( const QModelIndex & ) ),
             this, SLOT( doubleClicked( const QModelIndex & ) ) );
    connect( this, SIGNAL( wantJoinBattle( unsigned int, QString ) ),
             this, SLOT( joinBattleCommand( unsigned int, QString ) ) );
    connect( gamePasswordWidget, SIGNAL( wantJoinBattle( unsigned int, QString ) ),
             this, SLOT( joinBattleCommand( unsigned int, QString ) ) );
}

Battles::~Battles() {}

void Battles::receiveCommand( Command command ) {
    //   qDebug()<< "command: " << command.toQString();
    command.name = command.name.toUpper();
    if ( command.name == "CLIENTSTATUS" ) {
        User u = users->getUser( command.attributes[0] );
        int joinedBattleId = u.joinedBattleId;
        if ( joinedBattleId == -1 )
            return;
        Battle b = battleManager->getBattle( joinedBattleId );
        if ( b.founder == u.name ) {
            u.userState.setState(command.attributes[1].toInt());
            b.isStarted = u.userState.isIngame();
            battleManager->modBattle( b );
            if ( users->getUser( url.userName() ).joinedBattleId == b.id && b.isStarted )
                startGame( b );
        }
    } else if ( command.name == "JOINBATTLE" ) {
    } else if ( command.name == "JOINBATTLEFAILED" ) {
        QMessageBox::critical(this, "Join battle failed", command.attributes.join(" "));
    } else if ( command.name == "JOINEDBATTLE" ) {
        Battle b = battleManager->getBattle( command.attributes[0].toInt() );
        b.playerCount++;
        battleManager->modBattle( b );
    } else if ( command.name == "LEFTBATTLE" ) {
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
        command.attributes = command.attributes.join( " " ).split( "\t" );
        QString mapName       = command.attributes.takeFirst();
        b.mapName             = mapName;
        b.title               = command.attributes.takeFirst();
        b.modName             = command.attributes.takeFirst();
        b.playerCount         = 1;
        b.countryCode = users->getUser( b.founder ).countryCode;
        battleManager->addBattle( b );
    } else if ( command.name == "BATTLECLOSED" ) {
        battleManager->delBattle( command.attributes[0].toInt() );
    } else if ( command.name == "UPDATEBATTLEINFO" ) {
        Q_ASSERT( command.attributes.size() >= 5 );
        int battleId = command.attributes.takeFirst().toInt();
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
        Battle b = battleManager->getBattle( bi );
        command.attributes = command.attributes.join( " " ).split( "\t" );
        foreach( QString s, command.attributes ) {
            QString key = s.section( "=", 0, 0 ).toLower().replace( "game/", "" );
            b.options[key] = s.section( "=", 1, 1 ).toInt();
        }
        battleManager->modBattle( b );
    } else if ( command.name == "REMOVESCRIPTTAGS" ) {
        int bi = users->getUser( username ).joinedBattleId;
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
        Battle b = battleManager->getBattle( bi );
        b.allyNumberStartRectMap.remove( command.attributes[0].toInt() );
        battleManager->modBattle( b );
        emit removeStartRect(command.attributes[0].toInt());
    } else if ( command.name == "REQUESTBATTLESTATUS" ) {
        User u = users->getUser( url.userName() );
        QList<User> list = users->getUserList(u.joinedBattleId);
        int playersNum;
        for (int i = 0; i < list.size(); i++) {
            if (list.at(i).battleState.isPlayer())
                playersNum++;
        }
        playersNum++;
        u.userState.setIngame(true);
        u.battleState.setTeamNo(playersNum);
        u.battleState.setAllyTeamNo(playersNum);
        u.battleState.setSide(0);
        u.m_color = QColor( "red" );
        u.battleState.setPlayer(true);
        u.battleState.setSyncState(resyncStatus());
        users->onMyBattleStateChanged( u );
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
    if ( !b.isLocked && !b.isStarted && b.isPasswordProtected ) {
        gamePasswordWidget->setBattleId( b.id );
        gamePasswordWidget->resetPassword();
        gamePasswordWidget->show();
    } else {
        emit wantJoinBattle( b.id, "" );
    }
}

void Battles::joinBattleCommand( unsigned int id, QString password ) {
    Command command( "JOINBATTLE" );
    command.attributes << QString( "%1 %2" ).arg( id ).arg( password );
    emit sendCommand( command );
}

void Battles::customContextMenuRequested( const QPoint & point ) {
    // here you can either use data() to get the User object which is easy and might be ok
    // the other way would be to query everything via the model. to keep it simple especially
    // since we don't own the lobby server communiation protocol i would recommend to user
    // the User object directly

    // maybe we need this?! ;-) (js)
    //   if ( !selectionModel->hasSelection() )
    //     return;
    QModelIndex index = battleManager->selectionModel()->currentIndex();

    QMenu menu( "contextMenu", this );
    menu.addAction( "being funny" );
    menu.addAction( "start private chat", this, SLOT( unsetSlot() ) );
    //menu.addSeparator();
    //menu.addAction( "being funny" );

    menu.exec( QCursor::pos() );
    mapToGlobal( point );
}

void Battles::setUsers( Users* users ) {
    this->users = users;
    battleManager->setUsers(users);
}

void Battles::setCurrentUsername(QString a_username) {
    username = a_username;
}

void Battles::startGame( Battle b ) {
    QString springDir = settings->value("spring_user_dir").toString();
    QFile scriptFile( springDir + "/script_qtlobby.txt" );
    if ( !scriptFile.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) ) {
        qDebug() << "Error: Could not open script file for write: " << scriptFile.fileName();
        return;
    }
    QTextStream out( &scriptFile );
    out << generateScript( b );
    scriptFile.close();
    qDebug() << "Script file " << springDir + "/script_qtlobby.txt" << " written";
    emit start();
}

QString Battles::generateScript( Battle b ) {
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
    gameOptions["HostPort"]     = QString::number( b.port );
    gameOptions["HostIP"]       = b.founder == url.userName() ? QString( "localhost" ) : b.ip;
    gameOptions["IsHost"]       = "0"; //need to change when hosting will be done!
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

    if ( battleId < 0 ) {
        // set battle status to unsync
        // popup messagebox with error, that this should not happen
        qDebug() << __FILE__ << " error: should I resync for a none existing battle?!";
        return 0;
    }
    Battle b = battleManager->getBattle( battleId );
    QString modName = b.modName;

    if ( UnitSyncLib::getInstance()->mapChecksum( b.mapName ) != ( unsigned int ) b.mapHash )
        return 2; // 2 = not sync

    //qDebug() << "flux: modindex is: " << UnitSyncLib::getInstance()->modIndex(modName);
    if (UnitSyncLib::getInstance()->modIndex(modName) < 0)
        return 2; // 2 = not sync

    return 1; // 1 = sync is 'in sync'
}

void Battles::invalidateModel() {
    battleManager->model()->onGroupChanged();
}

