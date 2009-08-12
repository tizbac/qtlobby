// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "TreeSortFilterProxyModel.h"
#include <QStringList>
#include <QDebug>

TreeSortFilterProxyModel::TreeSortFilterProxyModel( QObject* parent )
    : QSortFilterProxyModel( parent ) {
    setDynamicSortFilter(true);
    m_filterState = 0;
}

TreeSortFilterProxyModel::~TreeSortFilterProxyModel() {}

void TreeSortFilterProxyModel::sort( int col, Qt::SortOrder order ) {
    if ( colRoleMap.contains( col ) )
        setSortRole( colRoleMap[col] );
    QSortFilterProxyModel::sort( col, order );
    invalidate();
}

void TreeSortFilterProxyModel::setColRole( int col, Qt::ItemDataRole role ) {
    colRoleMap[col] = role;
}

void TreeSortFilterProxyModel::setFilterString(QString str) {
    m_filterString = str;
    invalidateFilter();
}

void TreeSortFilterProxyModel::setBitState( unsigned int bit, bool state ) {
    SETBIT( m_filterState, bit, state );
    invalidateFilter();
}

bool TreeSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const {
    if( sourceModel()->data(left,Qt::UserRole+2).isValid() ) {
        if( sourceModel()->data(left,Qt::UserRole+2).toBool() && !sourceModel()->data(right,Qt::UserRole+2).toBool())
            return true;
        else if( !sourceModel()->data(left,Qt::UserRole+2).toBool() && sourceModel()->data(right,Qt::UserRole+2).toBool())
            return false;
    }
    return QSortFilterProxyModel::lessThan(left, right);
}

bool TreeSortFilterProxyModel::filterAcceptsRow (int source_row, const QModelIndex & source_parent) const {
    unsigned int state = sourceModel()->data(sourceModel()->index(source_row, 0, source_parent), Qt::UserRole+2).value<unsigned int>();
    if ( state & m_filterState ) // some filter hits a filter state in bitmask
        return false;
    if (m_filterString.isEmpty()) return true;
    if (m_filterString == "!") return true;
    QStringList terms = m_filterString.simplified().split(" ");
    QStringList notTerms;
    for (int i = 0; i < terms.size(); i++) {
        if (terms[i].startsWith("!")) {
            if (terms[i].length() > 1) {
                notTerms << terms[i].right(terms[i].length()-1);
            }
            terms.removeAt(i);
        }
    }
    for (int j = 0; j < notTerms.size(); j++) {
        bool found = false;
        for (int i = 0; i < sourceModel()->rowCount(source_parent); i++) {
            if (sourceModel()->data(sourceModel()->index(source_row, i, source_parent), Qt::DisplayRole).toString().contains(notTerms[j], Qt::CaseInsensitive)) {
                found=true;
                break;
            }
        }
        if (found) return false;
    }
    for (int j = 0; j < terms.size(); j++) {
        bool found = false;
        for (int i = 0; i < sourceModel()->rowCount(source_parent); i++) {
            if (sourceModel()->data(sourceModel()->index(source_row, i, source_parent), Qt::DisplayRole).toString().contains(terms[j], Qt::CaseInsensitive)) {
                found=true;
                break;
            }
        }
        if (!found) return false;
    }
    return true;
}
