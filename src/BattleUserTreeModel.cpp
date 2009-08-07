// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
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
                            return tr("Ready, sync status unknown");
                    case 1:
                        if (role == Qt::DecorationRole)
                            return QIcon( QString( ":/icons/open_game.xpm" ));
                        else
                            return tr("Ready");
                    case 2:
                        if (role == Qt::DecorationRole)
                            return QIcon( QString( ":/icons/ready_unsync.xpm" ));
                        else
                            return tr("Ready, unsynced");
                    }
                } else {
                    switch (u.battleState.syncState()) {
                    case 0:
                        if (role == Qt::DecorationRole)
                            return QIcon( QString( ":/icons/nready_q.xpm" ));
                        else
                            return tr("Not ready, sync status unknown");
                    case 1:
                        if (role == Qt::DecorationRole)
                            return QIcon( QString( ":/icons/closed_game.xpm" ));
                        else
                            return tr("Not ready");
                    case 2:
                        if (role == Qt::DecorationRole)
                            return QIcon( QString( ":/icons/nready_unsync.xpm" ));
                        else
                            return tr("Not ready, unsynced");
                    }
                }

            } else {
                if (role == Qt::DecorationRole)
                    return QIcon( QString( ":/icons/spectator.xpm" ));
                else
                    return tr("Spectating");
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
    QString ret;
    switch ( col ) {
        case 4: ret = tr("State"); break;
        case 5: ret = tr("Faction"); break;
        case 6: ret = tr("Team Number"); break;
        case 7: ret = tr("Ally Team Number"); break;
        case 8: ret = tr("Color"); break;
        case 9: ret = tr("Handicap"); break;
        default: break;
    }
    if ( role == Qt::ToolTipRole ) {
        return ret;
    } else if ( role == Qt::DisplayRole ) {
        return ret.at(0);
    }
    return QVariant();
}

