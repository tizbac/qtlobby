//
// C++ Implementation: TreeSortFilterProxyModel
//
// Description: 
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "TreeSortFilterProxyModel.h"

TreeSortFilterProxyModel::TreeSortFilterProxyModel( QObject* parent )
 : QSortFilterProxyModel( parent ) { }

TreeSortFilterProxyModel::~TreeSortFilterProxyModel() {}

void TreeSortFilterProxyModel::sort( int col, Qt::SortOrder order ) {
  if( colRoleMap.contains( col ) )
    setSortRole( colRoleMap[col] );
  QSortFilterProxyModel::sort( col, order );
}

void TreeSortFilterProxyModel::setColRole( int col, Qt::ItemDataRole role ) {
  colRoleMap[col] = role;
}
