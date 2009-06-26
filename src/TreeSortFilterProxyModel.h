//
// C++ Interface: TreeSortFilterProxyModel
//
// Description: 
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TREESORTFILTERPROXYMODEL_H
#define TREESORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QMap>

/**
Sets the sort roles for the rows before sorting.

	@author Mirko Windhoff <qtlobby.reg@ncnever.de>
*/
class TreeSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    TreeSortFilterProxyModel( QObject* parent = 0 );
    ~TreeSortFilterProxyModel();
    void sort( int col, Qt::SortOrder order = Qt::AscendingOrder );
    void setColRole( int col, Qt::ItemDataRole role = Qt::DisplayRole );
    QMap<int, Qt::ItemDataRole> colRoleMap;
};

#endif
