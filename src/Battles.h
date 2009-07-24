// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
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
#include "AbstractStateClient.h"

/**
 @author Joachim Schiele <js@lastlog.de>
 */
class Battles : public QTreeView {
    Q_OBJECT
public:
    Battles( QWidget* parent = 0 );
    ~Battles();
    void setUsers( Users* users );
    void wipeModels();
signals:
    void sendCommand( Command command );
    void wantJoinBattle(unsigned int,QString);
    void start();
    void addStartRect(int ally, QRect r);
    void removeStartRect(int ally);
    void statsChange(int battles);
    void sendInput(QString input);

public slots:
    void setRegExp( QString regExp );
    void receiveCommand( Command command );
    void setConfiguration( QUrl url );
    void joinBattleCommand(unsigned int id, QString password);
    void setCurrentUsername(QString username);
    void invalidateModel();
    void connectionStateChanged(ConnectionState state);

protected slots:
    void customContextMenuRequested( const QPoint & point );
    void doubleClicked( const QModelIndex & index );
    /* Battle filter slots */
    void setFilterPasswordedSlot( bool state );
    void setFilterInGameSlot( bool state );
    void setFilterLockedSlot( bool state );
    void setFilterUnavailableModsSlot( bool state );
    void setFilterUnavailableMapsSlot( bool state );
    void setFilterWithoutPlayersSlot( bool state );
    void setFilterWithoutFriendsSlot( bool state );
    void onReboot();

protected:
    int resyncStatus();
    void startGame( Battle b, bool host );
    QString generateScript( Battle b, bool host );
    QString currentTabType;
    QString currentTabName;
    QString username;
    Users* users;
    QUrl url;
    GamePasswordWidget* gamePasswordWidget;
    QSettings* settings;
    int battleCount;
    QMenu* m_menu;
    QAction* openPrivateChannelAction;
    QAction* joinBattleAction;
    QMenu* m_filterMenu;
    QAction* filterPasswordedAction;
    QAction* filterInGameAction;
    QAction* filterLockedAction;
    QAction* filterUnavailableModsAction;
    QAction* filterUnavailableMapsAction;
    QAction* filterWithoutPlayersAction;
    QAction* filterWithoutFriendsAction;
public:
    BattleManager* battleManager;
};

#endif
