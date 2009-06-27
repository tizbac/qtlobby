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

/**
 @author Joachim Schiele <js@lastlog.de>
*/
class Users : public QTreeView
{
  Q_OBJECT
public:
  Users( QWidget * parent = 0 );
  ~Users();
  User getUser( const QString userName );
  QList<User>& getUserList( const int battleId );
  QUrl url;
signals:
  void sendCommand( Command command );
  void sendInput( QString input );
public slots:
  void setRegExp( QString regExp );
  void receiveCommand( Command command );
  void setConfiguration( QUrl url );
  void currentTabChanged( QString name, QString lobbyTabType );
  void inv();
  void onMyBattleStateChanged( User u );
  void onReadyStateChanged( bool isReady );
protected slots:
  void customContextMenuRequestedSlot( const QPoint & point );
  void doubleClickedSlot( const QModelIndex & index );
  void joinSameBattle( User u );

protected:
  QString currentTabType;
  QString currentTabName;
  void delUserFromAllManagers( QString ); // delete user from all managers
  void modUserInAllManagers( User );    // modify user in all manangers
  QMap<QString, User> userMap;
  QMap<QString, UserManager*> channelUserManagerMap;
  QMap<int, UserManager*> battleIdUserManagerMap;
  UserManager* infoChannelUserManager;
  QMenu* userMenu;
  QAction* openPrivateChannelAction;
  QAction* slapAction;
  QAction* joinSameBattleAction;
  void updateUserList();
  TreeSortFilterProxyModel* proxyModel;
};

#endif