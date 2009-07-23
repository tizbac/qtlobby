// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QDebug>
#include <QSortFilterProxyModel>
// #include <modeltest.h>
#include <QItemSelectionModel>

#include "UserTreeModel.h"
#include "User.h"
#include "UserTreeModel.h"
#include "BattleUserTreeModel.h"
#include "TreeSortFilterProxyModel.h"

/**
 @author Joachim Schiele <js@lastlog.de>
*/
class UserManager : public QObject {
    Q_OBJECT
public:
    UserManager( QObject* parent = 0, bool useBattleUserTreeModel = false );
    ~UserManager();
    /** delete a user based on his name */
    void delUser( QString );
    /** add a User */
    void addUser( User );
    /** modify a complete user record */
    void modUser( User );
    /** get a complete user record by name */
    User getUser( QString );
    UserTreeModel* model();
    TreeSortFilterProxyModel* proxyModel();
    QItemSelectionModel* selectionModel();
private:
    bool useBattleUserTreeModel;
    UserTreeModel* m_userModel;
    UserTreeModel* m_battleUserModel;
    QItemSelectionModel* m_selectionModel;
    TreeSortFilterProxyModel* m_proxyModel;
private slots:
    void rowsAboutToBeInserted( const QModelIndex &, int, int );
    void rowsInserted( const QModelIndex &, int, int );
};

#endif
