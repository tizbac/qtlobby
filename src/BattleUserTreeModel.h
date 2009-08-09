// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef BATTLEUSERTREEMODEL_H
#define BATTLEUSERTREEMODEL_H

#include "UserTreeModel.h"

class BattleUserTreeModel : public UserTreeModel {
    Q_OBJECT
public:
    BattleUserTreeModel( QObject* parent = 0 );
    ~BattleUserTreeModel();

    int columnCount( const QModelIndex& /*parent*/ ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    QVariant headerData( int col, Qt::Orientation o, int role ) const;
};
#endif
