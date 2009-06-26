//
// C++ Implementation: BattleTreeModel
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "BattleTreeModel.h"

BattleTreeModel::BattleTreeModel( QObject* parent ) : QAbstractItemModel( parent ) {
  qRegisterMetaType<Battle>( "Battle" );
}

BattleTreeModel::~BattleTreeModel() {
}

QVariant BattleTreeModel::data( const QModelIndex& index, int role ) const {
  if ( role == Qt::UserRole ) {
    QVariant a;
    a.setValue<Battle>( m_battleList[index.row()] );
    return a;
  }
  switch ( index.column() ) {
    case 0: //status
      if ( role == Qt::DecorationRole ) {
        Battle b = m_battleList[index.row()];
        if ( b.isStarted )
          return QIcon( ":/icons/ingame.xpm" );
        if ( b.isLocked && b.isPasswordProtected )
          return QIcon( ":/icons/closed_pw_game.xpm" );
        if ( b.isLocked )
          return QIcon( ":/icons/closed_game.xpm" );
        if ( b.isPasswordProtected )
          return QIcon( ":/icons/open_pw_game.xpm" );
        return QIcon( ":/icons/open_game.xpm" );
      }
      if ( role == Qt::ToolTipRole ) {
        Battle b = m_battleList[index.row()];
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
        //         return tldlist.TLDMap->value( t );
        return t;
      }
      break;
    case 2: //rank
      if ( role == Qt::DecorationRole ) {
        QString filename = QString( ":/icons/rank%1.xpm" ).arg( m_battleList[index.row()].minRank );
        return QIcon( filename );
      }
      if ( role == Qt::ToolTipRole ) {
        switch ( m_battleList[index.row()].minRank ) {
          case 0:
            return "1/7 Newbie";
          case 1:
            return "2/7 Beginner";
          case 2:
            return "3/7 Average";
          case 3:
            return "4/7 Above average";
          case 4:
            return "5/7 Experienced";
          case 5:
            return "6/7 Highly experienced";
          case 6:
            return "7/7 Veteran";
        }
        return "no rank";
      }
      break;
    case 3: // title
      if ( role == Qt::DisplayRole || role == Qt::ToolTipRole )
        return m_battleList[index.row()].title;
      break;
    case 4: // map
      if ( role == Qt::DisplayRole )
        return m_battleList[index.row()].shortMapName();
      if ( role == Qt::ToolTipRole )
        return m_battleList[index.row()].mapName;
      break;
    case 5: // mod
      if ( role == Qt::DisplayRole )
        return m_battleList[index.row()].shortModName();
      if ( role == Qt::ToolTipRole )
        return m_battleList[index.row()].modName;
      break;
    case 6: // host
      if ( role == Qt::DisplayRole || role == Qt::ToolTipRole )
        return m_battleList[index.row()].founder;
      break;
    case 7: // spectator count
      if ( role == Qt::DisplayRole )
        return QString( "%1/%2+%3" )
            .arg( m_battleList[index.row()].playerCount - m_battleList[index.row()].spectatorCount )
               .arg( m_battleList[index.row()].maxPlayers )
               .arg( m_battleList[index.row()].spectatorCount );
      if ( role == Qt::ToolTipRole ) {
        Battle b = m_battleList[index.row()];
        return QString( "%1 players in game\n%2 players maximum\n%3 spectators in game" )
               .arg( b.playerCount -b.spectatorCount )
               .arg( b.maxPlayers )
               .arg( b.spectatorCount );
      }
      break;
  }
  return QVariant();
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
        return role == Qt::ToolTipRole ? "State: green open, red locked, swords started" : "S";
      case 1:
        return role == Qt::ToolTipRole ? "Country" : "C";
      case 2:
        return role == Qt::ToolTipRole ? "Minimum rank needed to join" : "R";
      case 3:
        return role == Qt::ToolTipRole ? "" : "Title";
      case 4:
        return role == Qt::ToolTipRole ? "" : "Map";
      case 5:
        return role == Qt::ToolTipRole ? "" : "Mod";
      case 6:
        return role == Qt::ToolTipRole ? "" : "Host";
      case 7:
        return role == Qt::ToolTipRole ? "# of Players / # of max. Players + # of Spectators" : "Players";
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
    if ( battleId == m_battleList[i].id ) {
//       qDebug() << i << ": " << m_battleList[i].id;
      return i;
    }
  qDebug() << "FIXME: Couldn't find rowPositionForBattle " << battleId << ". This should not happen!";
  return -1;
}

Battle BattleTreeModel::battleForRowPosition( int row ) {
  if (( row < 0 || row > rowCount( QModelIndex() ) ) )
    return Battle();
  else
    return m_battleList[row];
}
