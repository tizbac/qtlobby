// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef BATTLEMANAGER_H
#define BATTLEMANAGER_H

#include <QObject>
#include <QItemSelectionModel>
#include <QDebug>

#include "BattleTreeModel.h"
#include "Battle.h"
#include "TreeSortFilterProxyModel.h"
#include "Users.h"

/**
 * Stores and manages the Battles and the models for the battle list.
 */
class BattleManager : public QObject {
    friend class Battles;
    Q_OBJECT
public:
    BattleManager(QObject* parent = 0 );
    ~BattleManager();
    /** test if there is a battle with this id */
    bool isBattleId( int id );
    /** get a complete battle record by name */
    Battle getBattle( int id );

    BattleTreeModel* model();
    TreeSortFilterProxyModel* proxyModel();
    QItemSelectionModel* selectionModel();
    void getBattleList(); // returns a list of all users
    void setUsers(Users* users);

private:
    /** delete a battle based on his name */
    void delBattle( int id );
    /** add a Battle */
    void addBattle( Battle );
    /** modify a complete battle record */
    void modBattle( Battle );

private:
    BattleTreeModel* m_model;
    QItemSelectionModel* m_selectionModel;
    TreeSortFilterProxyModel* m_proxyModel;
};

#endif
