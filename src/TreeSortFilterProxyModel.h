// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
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
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
private:
    QString m_filterString;
    unsigned int m_filterState;
};

#endif
