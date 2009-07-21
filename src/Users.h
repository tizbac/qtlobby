//
// C++ Interface: Users
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef USERS_H
#define USERS_H

// #include <QObject>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QDebug>
#include <QMap>
#include <QUrl>
#include <QItemSelectionModel>
#include <QTreeView>
#include <QMenu>
#include <QMessageBox>
#include <QTreeWidgetItem>

#include "User.h"
#include "Command.h"
#include "UserManager.h"
#include "AbstractStateClient.h"

/**
 @author Joachim Schiele <js@lastlog.de>
*/
class Users : public QTreeView {
    Q_OBJECT
public:
    Users( QWidget * parent = 0 );
    ~Users();
    User getUser( const QString userName );
    const QList<User> getUserList( const int battleId );
    QStringList getUsernamesList();
    static Users* getCurrentUsers();
    UserTreeModel* getUserModel(int battleId);
    int usersCountInCurrentChannel();
    void wipeModels();
    QString getCurrentUsername();
signals:
    void sendCommand( Command command );
    void sendInput( QString input );
    void myStateChanged(User u);
    void openGroupsDialog();
    void statsChange(int userCount, int moderatorCount);

public slots:
    void setRegExp( QString regExp );
    void receiveCommand( Command command );
    void setConfiguration( QUrl url );
    void currentTabChanged( QString name, QString lobbyTabType );
    void inv();
    void onMyBattleStateChanged( User u );
    void onMyStateChanged( User u );
    void onReadyStateChanged( int state );
    void onSpecStateChanged( int state );
    void onSideComboBoxChanged( int index );
    void onColorChanged(QColor c);
    void onTeamNumberChanged( int i );
    void onAllyTeamNumberChanged( int i );
    void invalidateModel();
    void connectionStateChanged(ConnectionState state);
    void onSpringStopped();
protected slots:
    void customContextMenuRequestedSlot( const QPoint & point );
    void doubleClickedSlot( const QModelIndex & index );
    void joinSameBattle( User u );
    void addUserToGroup(QString user, QString group);
    void removeUserFromGroup(QString user);

protected:
    QString currentTabType;
    QString currentTabName;
    void delUserFromAllManagers( QString ); // delete user from all managers
    void modUserInAllManagers( User );    // modify user in all managers
    QMap<QString, User> userMap;
    QMap<QString, UserManager*> channelUserManagerMap;
    QMap<int, UserManager*> battleIdUserManagerMap;
    UserManager* infoChannelUserManager;
    QMenu* userMenu;
    QMenu* groupsMenu;
    QAction* removeFromGroupAction;
    QMenu* clanGroupsMenu;
    QAction* removeClanFromGroupAction;
    QAction* openPrivateChannelAction;
    QAction* slapAction;
    QAction* joinSameBattleAction;
    void updateUserList();
    TreeSortFilterProxyModel* proxyModel;
    QRegExp clanRegexp;
    QUrl url;
    int userCount;
    int moderatorCount;
    static Users* lastThis;
};

#endif
