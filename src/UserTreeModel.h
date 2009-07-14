//
// C++ Interface: UserTreeModel
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef USERTREEMODEL_H
#define USERTREEMODEL_H

#include <QAbstractItemModel>
#include <QDebug>
#include <QIcon>

#include "User.h"
#include "TLDList.h"

/**
 @author Joachim Schiele <js@lastlog.de>
*/
class UserTreeModel : public QAbstractItemModel {
    Q_OBJECT
public:
    UserTreeModel( QObject* parent = 0 );
    ~UserTreeModel();

    QModelIndex index( int row, int col, const QModelIndex& parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex& child ) const;
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;

    int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role ) const;
    virtual QVariant headerData( int col, Qt::Orientation, int role ) const;

    bool setData( QModelIndex index, QVariant value, int role ) ;
    bool insertRows( int position, int rows, const QModelIndex& parent = QModelIndex() );
    bool removeRows( int position, int rows, const QModelIndex& parent = QModelIndex() );
    int rowPositionForUser( QString username );
    void clear();
    QList<User>& userList();
    void invalidateModel();
public slots:
    void onGroupChanged();
protected:
    QList<User> m_userList;
    TLDList tldlist;
};

#endif
