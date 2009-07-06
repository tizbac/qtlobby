//
// C++ Interface: BattleTreeModel
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BATTLETREEMODEL_H
#define BATTLETREEMODEL_H

#include <QAbstractItemModel>
#include <QDebug>
#include <QIcon>

#include "Battle.h"
#include "TLDList.h"
#include "Users.h"

/**
 @author Joachim Schiele <js@lastlog.de>
 */
class BattleTreeModel : public QAbstractItemModel {
    Q_OBJECT
public:
    BattleTreeModel( QObject* parent = 0 );
    ~BattleTreeModel();

    QModelIndex index( int row, int col, const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& child ) const;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const;

    int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    QVariant headerData( int col, Qt::Orientation, int role ) const;

    bool setData( QModelIndex index, QVariant value, int role ) ;
    bool insertRows( int position, int rows, const QModelIndex& parent = QModelIndex() );
    bool removeRows( int position, int rows, const QModelIndex& parent = QModelIndex() );
    int rowPositionForBattle( int battleId );
    Battle battleForRowPosition( int row );
    QList<Battle>& userList();
    void setUsers(Users* users);
public slots:
    void onGroupChanged();
private:
    QList<Battle> m_battleList;
    Users* m_users;
    //   TLDList tldlist;
};

#endif
