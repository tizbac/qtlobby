//
// C++ Interface: Battles
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BATTLES_H
#define BATTLES_H

#include <QDebug>
#include <QUrl>
#include <QItemSelectionModel>
#include <QTreeView>
#include <QMenu>
#include <QMessageBox>
#include <QMap>
#include <QFile>

#include "Battle.h"
#include "Command.h"
#include "BattleTreeModel.h"
#include "BattleManager.h"
#include "Users.h"
#include "GamePasswordWidget.h"
#include "TreeSortFilterProxyModel.h"
#include "UnitSyncLib.h"
#include "Settings.h"

/**
 @author Joachim Schiele <js@lastlog.de>
 */
class Battles : public QTreeView {
    Q_OBJECT
public:
    Battles( QWidget* parent = 0 );
    ~Battles();
    void setUsers( Users* users );
signals:
    void sendCommand( Command command );
    void wantJoinBattle(unsigned int,QString);
    void start();
    void addStartRect(int ally, QRect r);

public slots:
    void setRegExp( QString regExp );
    void receiveCommand( Command command );
    void setConfiguration( QUrl url );
    void joinBattleCommand(unsigned int id, QString password);
    void setCurrentUsername(QString username);
    void invalidateModel();

protected slots:
    void customContextMenuRequested( const QPoint & point );
    void doubleClicked( const QModelIndex & index );

protected:
    int resyncStatus();
    void startGame( Battle b );
    QString generateScript( Battle b );
    QString currentTabType;
    QString currentTabName;
    QString username;
    Users* users;
    QUrl url;
    GamePasswordWidget* gamePasswordWidget;
    QSettings* settings;
public:
    BattleManager* battleManager;
};

#endif
