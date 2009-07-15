//
// C++ Implementation: BattleUserTreeModel
//
// Description:
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "BattleUserTreeModel.h"
#include "UnitSyncLib.h"

BattleUserTreeModel::BattleUserTreeModel(QObject* parent)
    : UserTreeModel(parent) {
}


BattleUserTreeModel::~BattleUserTreeModel() {
}

int BattleUserTreeModel::columnCount( const QModelIndex& /*parent*/ ) const {
    return 10;
}

QVariant BattleUserTreeModel::data( const QModelIndex& index, int role ) const {
    if (role == Qt::BackgroundRole || role == Qt::UserRole) {
        return UserTreeModel::data(index, role);
    }
    if (role == Qt::UserRole+1) {
        User u = m_userList[index.row()];
        switch ( index.column() ) {
        case 0: //status
        case 1: //flags
        case 2: //rank
        case 3: //username
            return UserTreeModel::data(index, Qt::ToolTipRole);
        case 4: //ready/spec
            if (u.battleState.isPlayer() && u.battleState.isReady()) return 2;
            if (u.battleState.isPlayer() && !u.battleState.isReady()) return 1;
            if (!u.battleState.isPlayer()) return 0;
        case 5: //side
            return !u.battleState.isPlayer() ? QString("") : QString::number(u.battleState.getSide()).rightJustified(5,'0');
        case 6: //team
			return !u.battleState.isPlayer() ? QString("") : QString::number(u.battleState.getTeamNo()).rightJustified(5,'0');
        case 7: //ally
            return !u.battleState.isPlayer() ? QString("") : QString::number(u.battleState.getAllyTeamNo()).rightJustified(5,'0');
        case 8: //color
            return !u.battleState.isPlayer() ? QString("") : u.m_color.name();
        case 9: //handicap
            return !u.battleState.isPlayer() ? QString("") : QString::number(u.battleState.getHandicap()).rightJustified(5,'0');
        }
        return QVariant();
    }
    switch ( index.column() ) {
    case 0: //status
    case 1: //flags
    case 2: //rank
    case 3: //username
        return UserTreeModel::data(index, role);
    case 4: //ready/spec
        if ( role == Qt::DecorationRole || role == Qt::ToolTipRole) {
            User u = m_userList[index.row()];
            if (u.battleState.isPlayer()) {
                if (u.battleState.isReady()) {
                    switch (u.battleState.syncState()) {
                    case 0:
                        if (role == Qt::DecorationRole)
                            return QIcon( QString( ":/icons/ready_q.xpm" ));
                        else
                            return "Ready, sync status unknown";
                    case 1:
                        if (role == Qt::DecorationRole)
                            return QIcon( QString( ":/icons/open_game.xpm" ));
                        else
                            return "Ready";
                    case 2:
                        if (role == Qt::DecorationRole)
                            return QIcon( QString( ":/icons/ready_unsync.xpm" ));
                        else
                            return "Ready, unsynced";
                    }
                } else {
                    switch (u.battleState.syncState()) {
                    case 0:
                        if (role == Qt::DecorationRole)
                            return QIcon( QString( ":/icons/nready_q.xpm" ));
                        else
                            return "Not ready, sync status unknown";
                    case 1:
                        if (role == Qt::DecorationRole)
                            return QIcon( QString( ":/icons/closed_game.xpm" ));
                        else
                            return "Not ready";
                    case 2:
                        if (role == Qt::DecorationRole)
                            return QIcon( QString( ":/icons/nready_unsync.xpm" ));
                        else
                            return "Not ready, unsynced";
                    }
                }

            } else {
                if (role == Qt::DecorationRole)
                    return QIcon( QString( ":/icons/spectator.xpm" ));
                else
                    return "Spectating";
            }
        }
        break;
    case 5: //side
        if ( role == Qt::DecorationRole || role == Qt::ToolTipRole) {
            UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();

            User u = m_userList[index.row()];
            QString sideName = unitSyncLib->sideName(u.battleState.getSide());
			if (role == Qt::DecorationRole) {
				if( !u.battleState.isPlayer() )
					return QIcon();
                return QIcon( unitSyncLib->getSideIcon(sideName));
			}
            else
                return sideName;
        }
        break;
    case 6: //team
        if ( role == Qt::DisplayRole) {
            User u = m_userList[index.row()];
			if( !u.battleState.isPlayer() )
				return QString("");
            return QString::number(u.battleState.getTeamNo()+1);
        }
        break;
    case 7: //ally
        if ( role == Qt::DisplayRole) {
            User u = m_userList[index.row()];
			if( !u.battleState.isPlayer() )
				return QString("");
            return QString::number(u.battleState.getAllyTeamNo()+1);
        }
        break;
    case 8: //color
        if ( role == Qt::DecorationRole) {
            User u = m_userList[index.row()];
            QPixmap p(16,16);
			QColor col = u.m_color;
			if( !u.battleState.isPlayer() )
				col.setAlpha(0);
            p.fill(col);
            return p;
        }
        if ( role == Qt::ToolTipRole) {
            User u = m_userList[index.row()];
            return u.m_color.name();
        }
        break;
    case 9: //handicap
        if ( role == Qt::DisplayRole) {
            User u = m_userList[index.row()];
			if( !u.battleState.isPlayer() )
				return QString("");
            return QString::number(u.battleState.getHandicap())+"%";
        }
        break;
    }
    return QVariant();
}

QVariant BattleUserTreeModel::headerData( int col, Qt::Orientation o, int role ) const {
    if (col <= 3) return UserTreeModel::headerData(col, o, role);
    if ( role == Qt::ToolTipRole )
        switch ( col ) {
    case 4:
        return tr("State");
    case 5:
        return tr("Faction");
    case 6:
        return tr("Team Number");
    case 7:
        return tr("Ally team number");
    case 8:
        return tr("Color");
    case 9:
        return tr("Handicap");
    default:
        break;
    }
    return QVariant();
}

