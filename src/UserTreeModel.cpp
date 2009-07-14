//
// C++ Implementation: UserTreeModel
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "UserTreeModel.h"
#include "UserGroup.h"

UserTreeModel::UserTreeModel( QObject* parent ) : QAbstractItemModel( parent ) {
    qRegisterMetaType<User>( "User" );
}

UserTreeModel::~UserTreeModel() {}

QVariant UserTreeModel::data( const QModelIndex& index, int role ) const {
    if (role == Qt::BackgroundRole) {
        QColor c = UserGroupList::getInstance()->getUserColor(m_userList[index.row()].name);
        if (c.isValid()) {
            return c;
        }
        return QVariant();
    }
    if ( role == Qt::UserRole ) {
        QVariant a;
        a.setValue<User>( m_userList[index.row()] );
        return a;
    }
    switch ( index.column() ) {
    case 0: //status
        if ( role == Qt::DecorationRole || role == Qt::ToolTipRole ) {
            User u = m_userList[index.row()];
            QString tip;
            QStringList fileNameParts;
            if ( u.userState.isBot() ) {
                fileNameParts << "bot";
                tip += tr( "Bot" );
            } else if ( u.userState.isModerator() ) {
                fileNameParts << "chanop";
                tip += tr( "Moderator" );
            }   else {
                tip += tr( "Player" );
            }
            if ( u.userState.isAway() ) {
                fileNameParts << "away";
                tip += " is away";
            } else if ( u.userState.isIngame() ) {
                fileNameParts << "ingame";
                tip += " is in game";
            } else if ( u.joinedBattleId != -1 ) {
                fileNameParts << "broom";
                tip += " is in battle room";
            } else
                tip += " is available";
            if ( role == Qt::DecorationRole )
                return QIcon( QString( ":/icons/%1.xpm" ).arg( fileNameParts.join( "_" ) ) );
            return tip;
        }
        break;
    case 1: //flags
        if ( role == Qt::DecorationRole ) {
            QString filename = QString( ":/flags/%1.xpm" ).arg( m_userList[index.row()].countryCode );
            return QIcon( filename );
        }
        if ( role == Qt::ToolTipRole ) {
            QString c = tldlist.TLDMap->value( m_userList[index.row()].countryCode );
            if ( c != "" )
                return c;
            else
                return m_userList[index.row()].countryCode;
        }
        break;
    case 2: //rank
        if ( role == Qt::DecorationRole ) {
            int rank = m_userList[index.row()].userState.getRank();
            if ( rank > 6 ) return QVariant();
            return QIcon( QString( ":/icons/rank%1.xpm" ).arg( rank ) );
        }
        if ( role == Qt::ToolTipRole ) {
            switch ( m_userList[index.row()].userState.getRank() ) {
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
            return "no rank set";
        }
        break;
    case 3: //username
        if ( role == Qt::DisplayRole )
            return m_userList[index.row()].name;
        break;
    }
    return QVariant();
}

bool UserTreeModel::setData( const QModelIndex index, QVariant v, int /*role*/ ) {
    m_userList[index.row()] = v.value<User>();
    // next line is needed to get stuff working but to test the model it's commented right now (js)
    // FIXME in case the model is working and debugged and i have forgotten to uncomment the next line
    // then please remove this comment and uncomment the next line (js)
    dataChanged( index, index );
    return true;
}

QVariant UserTreeModel::headerData( int col, Qt::Orientation, int role ) const {
    if ( role == Qt::DisplayRole )
        if ( col == 3 )
            return "Player name";
    if ( role == Qt::ToolTipRole )
        switch ( col ) {
    case 0:
        return tr("Player status");
    case 1:
        return tr("Player location (guess)");
    case 2:
        return tr("Player rank");
    case 3:
        return tr("Player Name");
    default:
        break;
    }
    return QVariant();
}

bool UserTreeModel::insertRows( int /*position*/, int /*rows*/, const QModelIndex& /*parent*/ ) {
    User f;
    f.name = QString( "unknown %1" ).arg( rowCount( QModelIndex() ) );
    f.countryCode = "XX";
    beginInsertRows( QModelIndex(), 0, 0 );
    m_userList.prepend( f );
    endInsertRows();
    return true;
}

bool UserTreeModel::removeRows( int position, int /*rows*/, const QModelIndex& /*parent*/ ) {
    beginRemoveRows( QModelIndex(), position, position );
    m_userList.removeAt( position );
    endRemoveRows();
    return true;
}

QModelIndex UserTreeModel::index( int row, int column, const QModelIndex &parent ) const {
    if ( !hasIndex( row, column, parent ) )
        return QModelIndex();
    return createIndex( row, column/*, &User()*/ );
}

QModelIndex UserTreeModel::parent( const QModelIndex& /*child*/ ) const {
    return QModelIndex();
}

int UserTreeModel::columnCount( const QModelIndex& /*parent*/ ) const {
    return 4;
}

int UserTreeModel::rowCount( const QModelIndex& parent ) const {
    if ( parent.internalPointer() == NULL )
        return m_userList.size();
    return 0;
}

int UserTreeModel::rowPositionForUser( QString username ) {
    int c = m_userList.size();
    for ( int i = 0; i < c; ++i )
        if ( m_userList[i].name == username )
            return i;
    return -1;
}

QList<User>& UserTreeModel::userList() {
    return m_userList;
}

void UserTreeModel::onGroupChanged() {
    //    qDebug() << "Invalidating user model...";
    //    int rows = rowCount(QModelIndex());
    //    int cols = columnCount(QModelIndex());
    //    QModelIndex index1 = index(0,0, QModelIndex());
    //    QModelIndex index2 = index(rows-1,cols-1, QModelIndex());
    //    emit dataChanged(index1, index2);
    reset();
}

