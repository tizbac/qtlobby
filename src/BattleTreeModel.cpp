// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "BattleTreeModel.h"
#include "UserGroup.h"
#include "TLDList.h"
#include <QLinearGradient>

BattleTreeModel::BattleTreeModel( QObject* parent ) : QAbstractItemModel( parent ) {
    qRegisterMetaType<Battle>( "Battle" );
    m_users = 0;
}

BattleTreeModel::~BattleTreeModel() {
}

QVariant BattleTreeModel::data( const QModelIndex& index, int role ) const {
    if (index.row() >= m_battleList.size() || index.row() < 0)
        return QVariant();
    TLDList tldlist;
    UnitSyncLib* us = UnitSyncLib::getInstance();
    Battle b = m_battleList[index.row()];
    if ( role == Qt::BackgroundRole && m_users ) {
        QList<User> users = m_users->getUserList( m_battleList[index.row()].id );
        unsigned int usersSize = users.size();
        if ( usersSize > 0 ) {
            UserGroupList* ugl = UserGroupList::getInstance();
            QList<QColor> colors;
            for ( unsigned int i = 0; i < usersSize; i++ ) {
                QColor c = ugl->getUserColor( users.at( i ).name );
                if( !colors.contains( c ) && c.isValid() )
                    colors << c;
            }
            if ( !colors.size() )
                return QVariant();
            if ( colors.size() == 1 && colors.at( 0 ).isValid() )
                return colors.at( 0 );
            QFontMetrics fm( m_users->font() );
            QLinearGradient linearGrad( QPointF( 0, 3 ), QPointF( 0, fm.height() ) );
            float step = 1.0 / colors.size();
            float x = 0;
            for (int i = 0; i < colors.size(); i++, x += step) {
                linearGrad.setColorAt( x, colors.at( i ) );
            }
            return QBrush( linearGrad );
        }
        return QVariant();
    }
    if ( role == Qt::UserRole ) {
        QVariant a;
        a.setValue<Battle>( m_battleList[index.row()] );
        return a;
    }
    if ( role == Qt::UserRole+1 ) {
        switch ( index.column() ) {
        case 0: //status
            if ( b.isStarted && b.isPasswordProtected ) return 5;
            if ( b.isLocked && b.isPasswordProtected ) return 4;
            if ( b.isStarted ) return 3;
            if ( b.isLocked ) return 2;
            if ( b.isPasswordProtected ) return 1;
            return 0;
        case 7: // player count
            return QString::number(b.playerCount - b.spectatorCount).rightJustified(5,'0');
        }
    }
    if ( role == Qt::UserRole+2 ) { // for battle list filter
        unsigned int ret = 0;
        if( b.isPasswordProtected )
            ret += 1;
        if( b.isStarted )
            ret += 2;
        if( b.isLocked )
            ret += 4;
        if( !us->getModNames().contains( b.modName ) )
            ret += 8;
        if( !us->getMapNames().contains( b.mapName ) )
            ret += 16;
        if( b.playerCount - b.spectatorCount < 1 )
            ret += 32;
        bool containsGroupUser = false;
        if ( m_users ) {
            QList<User> users = m_users->getUserList(m_battleList[index.row()].id);
            UserGroupList* ugl = UserGroupList::getInstance();
            foreach( User u, users ) {
                if( ugl->containsUserName( u.name ) ) {
                    containsGroupUser = true;
                    break;
                }
            }
        }
        if( !containsGroupUser )
            ret += 64;
        return ret;
    }
    switch ( index.column() ) {
    case 0: //status
        if ( role == Qt::DecorationRole ) {
            if ( b.isStarted )
                return QIcon( ":/icons/ingame.xpm" );
            if ( b.isLocked && b.isPasswordProtected )
                return QIcon( ":/icons/closed_pw_game.xpm" );
            if ( b.isLocked )
                return QIcon( ":/icons/closed_game.xpm" );
            if ( b.isPasswordProtected )
                return QIcon( ":/icons/open_pw_game.xpm" );
            if ( b.playerCount - b.spectatorCount == 0 )
                return QIcon( ":/icons/open_game_empty.xpm" );
            return QIcon( ":/icons/open_game.xpm" );
        }
        if ( role == Qt::ToolTipRole ) {
            if ( b.isStarted )
                return tr( "Battle is started" );
            if ( b.isLocked && b.isPasswordProtected )
                return tr( "Battle is locked and password protected" );
            if ( b.isLocked )
                return tr( "Battle is locked" );
            if ( b.isPasswordProtected )
                return tr( "Battle is  password protected" );
            return tr( "Battle is  opened" );
        }
        break;
    case 1: //flags
        if ( role == Qt::DecorationRole ) {
            QString filename = QString( ":/flags/%1.xpm" ).arg( m_battleList[index.row()].countryCode );
            return QIcon( filename );
        }
        if ( role == Qt::ToolTipRole ) {
            QString t = m_battleList[index.row()].countryCode;
            return tldlist.TLDMap->value( t );
            return t;
        }
        break;
    case 2: //rank
        if ( role == Qt::DecorationRole ) {
            QString filename = getRankIconFilename( m_battleList[index.row()].minRank );
            return QIcon( filename );
        }
        if ( role == Qt::ToolTipRole ) {
            return stringifyRank(m_battleList[index.row()].minRank);
        }
        break;
    case 3: // title
        if ( role == Qt::DisplayRole || role == Qt::ToolTipRole )
            return m_battleList[index.row()].title;
        break;
    case 4: // map
        if ( role == Qt::DisplayRole )
            return m_battleList[index.row()].shortMapName();
        if ( role == Qt::DecorationRole && !Settings::Instance()->value("Battles/filterUnavailableMaps").toBool() ) {
            if(us->getMapNames().contains(b.mapName))
                return QIcon(":/icons/exists.xpm");
            return QIcon(":/icons/nexists.xpm");
        }
        if ( role == Qt::ToolTipRole )
            return m_battleList[index.row()].mapName;
        break;
    case 5: // mod
        if ( role == Qt::DisplayRole )
            return m_battleList[index.row()].shortModName();
        if( role == Qt::DecorationRole && !Settings::Instance()->value("Battles/filterUnavailableMods").toBool() ) {
            if(us->getModNames().contains(b.modName))
                return QIcon(":/icons/exists.xpm");
            return QIcon(":/icons/nexists.xpm");
        }
        if ( role == Qt::ToolTipRole )
            return m_battleList[index.row()].modName;
        break;
    case 6: // host
        if ( role == Qt::DisplayRole || role == Qt::ToolTipRole )
            return m_battleList[index.row()].founder;
        break;
    case 7: // player/maxplayer/spectator count
        if ( role == Qt::DisplayRole )
            return QString( "%1/%2+%3" )
            .arg( m_battleList[index.row()].playerCount - m_battleList[index.row()].spectatorCount )
            .arg( m_battleList[index.row()].maxPlayers )
            .arg( m_battleList[index.row()].spectatorCount );
        if ( role == Qt::ToolTipRole ) {
            QString tip = tr("<b>%1</b> players in battle<br/><b>%2</b> players maximum<br/><b>%3</b> spectators in battle")
                          .arg( b.playerCount - b.spectatorCount )
                          .arg( b.maxPlayers )
                          .arg( b.spectatorCount ) + "<hr/><b>Player list:</b><table>";
            float averageRank = 0;
            QList<User> users = m_users->getUserList(b.id);
            foreach(const User& u, users) {
                int rank = u.userState.getRank();
                tip += QString("<tr><td><img width=\"16\" height=\"16\" src=\"%1\"/></td><td>%2</td></tr>")
                       .arg(getRankIconFilename(rank))
                       .arg(u.name);
                averageRank += rank;
            }
            averageRank /= users.size();
            tip +="</table><br/>"
                  +tr("Average rank:")
                  +" "
                  +QString("<img width=\"16\" height=\"16\" src=\"%1\"/>").arg(getRankIconFilename(qRound(averageRank)))
                  +QString::number(averageRank, 'g', 2);
            return tip;
        }
        break;
    }
    return QVariant();
}

QString BattleTreeModel::stringifyRank(int rank) const {
    switch ( rank ) {
    case 0:
        return tr("1/7 Newbie");
    case 1:
        return tr("2/7 Beginner");
    case 2:
        return tr("3/7 Average");
    case 3:
        return tr("4/7 Above average");
    case 4:
        return tr("5/7 Experienced");
    case 5:
        return tr("6/7 Highly experienced");
    case 6:
        return tr("7/7 Veteran");
    }
    return tr("no rank");
}

QString BattleTreeModel::getRankIconFilename(int rank) const {
    return QString( ":/icons/rank%1-icon.png" ).arg(rank);
}

bool BattleTreeModel::setData( const QModelIndex index, QVariant v, int /*role*/ ) {
    m_battleList[index.row()] = v.value<Battle>();
    dataChanged( index, index );
    return true;
}

QVariant BattleTreeModel::headerData( int col, Qt::Orientation, int role ) const {
    if ( role == Qt::DisplayRole || role == Qt::ToolTipRole ) {
        switch ( col ) {
        case 0:
            return role == Qt::ToolTipRole ? tr("State: green open, red locked, swords started") : tr("S");
        case 1:
            return role == Qt::ToolTipRole ? tr("Country") : tr("C");
        case 2:
            return role == Qt::ToolTipRole ? tr("Minimum rank needed to join") : tr("R");
        case 3:
            return role == Qt::ToolTipRole ? "" : tr("Title");
        case 4:
            return role == Qt::ToolTipRole ? "" : tr("Map");
        case 5:
            return role == Qt::ToolTipRole ? "" : tr("Mod");
        case 6:
            return role == Qt::ToolTipRole ? "" : tr("Host");
        case 7:
            return role == Qt::ToolTipRole ? tr("# of Players / # of max. Players + # of Spectators") : tr("Players");
        }
    }
    return QVariant();
}

bool BattleTreeModel::insertRows( int /*position*/, int /*rows*/, const QModelIndex& /*parent*/ ) {
    Battle f;
    f.title = QString( "unknown %1" ).arg( rowCount( QModelIndex() ) );
    beginInsertRows( QModelIndex(), 0, 0 );
    m_battleList.prepend( f );
    endInsertRows();
    return true;
}

bool BattleTreeModel::removeRows( int position, int /*rows*/, const QModelIndex& /*parent*/ ) {
    beginRemoveRows( QModelIndex(), position, position );
    m_battleList.removeAt( position );
    endRemoveRows();
    return true;
}

QModelIndex BattleTreeModel::index( int row, int column, const QModelIndex &/*parent*/ ) const {
    return createIndex( row, column );
}

QModelIndex BattleTreeModel::parent( const QModelIndex& /*child*/ ) const {
    return QModelIndex();
}

int BattleTreeModel::columnCount( const QModelIndex& /*parent*/ ) const {
    return 8;
}

int BattleTreeModel::rowCount( const QModelIndex& parent ) const {
    if ( parent.internalPointer() == NULL )
        return m_battleList.size();
    return 0;
}

int BattleTreeModel::rowPositionForBattle( int battleId ) {
    for ( int i = 0; i < rowCount( QModelIndex() ); ++i )
        if ( battleId == m_battleList[i].id )
            return i;
    return -1;
}

Battle BattleTreeModel::battleForRowPosition( int row ) {
    if (( row < 0 || row > rowCount( QModelIndex() ) ) )
        return Battle();
    return m_battleList[row];
}

void BattleTreeModel::setUsers(Users* users) {
    m_users = users;
}

void BattleTreeModel::onGroupChanged() {
    reset();
}

QList<Battle>& BattleTreeModel::battleList() {
    return m_battleList;
}

void BattleTreeModel::clear() {
    m_battleList.clear();
    reset();
}

void BattleTreeModel::retranslateUi() {
    reset();
}
