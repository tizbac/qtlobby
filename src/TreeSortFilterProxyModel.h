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

#define TB(a, n) (a & (1 << (n))) // test bit
#define SB(a, n) a |= (1 << (n))  // set bit to 1
#define CB(a, n) a &= ~(1 << (n)) // clear bit
#define SETBIT(a, n, state) if (state) SB(a, n); else CB(a, n);

#include <QSortFilterProxyModel>
#include <QMap>

/**
Sets the sort roles for the rows before sorting.

        @author Mirko Windhoff <qtlobby.reg@ncnever.de>
*/
class TreeSortFilterProxyModel : public QSortFilterProxyModel {
public:
    TreeSortFilterProxyModel( QObject* parent = 0 );
    ~TreeSortFilterProxyModel();
    void sort( int col, Qt::SortOrder order = Qt::AscendingOrder );
    void setColRole( int col, Qt::ItemDataRole role = Qt::DisplayRole );
    void setFilterString(QString str);
    QMap<int, Qt::ItemDataRole> colRoleMap;
    void setBitState( unsigned int bit, bool state );

protected:
    bool filterAcceptsRow (int source_row, const QModelIndex & source_parent) const;
private:
    QString m_filterString;
    unsigned int m_filterState;
};

#endif
