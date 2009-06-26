//
// C++ Interface: BattleManager
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BATTLEMANAGER_H
#define BATTLEMANAGER_H

#include <QObject>
#include <QItemSelectionModel>
#include <QDebug>

#include "BattleTreeModel.h"
#include "Battle.h"
#include "TreeSortFilterProxyModel.h"

/**
 @author Joachim Schiele <js@lastlog.de>
 */
class BattleManager : public QObject
{
  friend class Battles;
  Q_OBJECT
public:
  BattleManager( QObject* parent = 0 );
  ~BattleManager();
  /** get a complete battle record by name */
  Battle getBattle( int id );

  BattleTreeModel* model();
  TreeSortFilterProxyModel* proxyModel();
  QItemSelectionModel* selectionModel();
  void getBattleList(); // returns a list of all users

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
