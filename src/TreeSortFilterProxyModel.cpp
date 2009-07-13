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
#include <QStringList>
#include <QDebug>

TreeSortFilterProxyModel::TreeSortFilterProxyModel( QObject* parent )
    : QSortFilterProxyModel( parent ) {
    setDynamicSortFilter(true);
}

TreeSortFilterProxyModel::~TreeSortFilterProxyModel() {}

void TreeSortFilterProxyModel::sort( int col, Qt::SortOrder order ) {
    if ( colRoleMap.contains( col ) )
        setSortRole( colRoleMap[col] );
    QSortFilterProxyModel::sort( col, order );
}

void TreeSortFilterProxyModel::setColRole( int col, Qt::ItemDataRole role ) {
    colRoleMap[col] = role;
}

void TreeSortFilterProxyModel::setFilterString(QString str) {
    m_filterString = str;
    invalidateFilter();
}

bool TreeSortFilterProxyModel::filterAcceptsRow (int source_row, const QModelIndex & source_parent) const {
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
