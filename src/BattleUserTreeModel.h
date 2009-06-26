//
// C++ Interface: BattleUserTreeModel
//
// Description: 
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BATTLEUSERTREEMODEL_H
#define BATTLEUSERTREEMODEL_H

#include <QAbstractItemModel>

/**
for battlemanagement, teams, ally, colors...

	@author Mirko Windhoff <qtlobby.reg@ncnever.de>
*/
class BattleUserTreeModel : public QAbstractItemModel
{
public:
    BattleUserTreeModel();

    ~BattleUserTreeModel();

};

#endif
