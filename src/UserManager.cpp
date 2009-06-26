//
// C++ Implementation: UserManager
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "UserManager.h"

UserManager::UserManager( QObject* parent, bool useBattleUserTreeModel ) : QObject( parent ) {
  this->useBattleUserTreeModel = useBattleUserTreeModel;
  qRegisterMetaType<User>( "User" );
  m_userModel = new UserTreeModel( this );
//   m_battleUserModel = new BattleUserTreeModel( this );
  if ( useBattleUserTreeModel ) {
    /*    m_proxyModel = new TreeSortFilterProxyModel( m_battleUserModel );
        m_selectionModel = new QItemSelectionModel( m_battleUserModel );
        m_proxyModel->setSourceModel( m_battleUserModel );*/
  } else {
    m_proxyModel = new TreeSortFilterProxyModel( m_userModel );
    m_selectionModel = new QItemSelectionModel( m_userModel );
    m_proxyModel->setSourceModel( m_userModel );
  }
  m_proxyModel->setSortCaseSensitivity( Qt::CaseInsensitive );
  m_proxyModel->setDynamicSortFilter( false );
  m_proxyModel->setColRole( 0, Qt::ToolTipRole );
  m_proxyModel->setColRole( 1, Qt::ToolTipRole );
  m_proxyModel->setColRole( 2, Qt::ToolTipRole );
  m_proxyModel->setColRole( 3, Qt::DisplayRole );

  connect( m_proxyModel, SIGNAL( rowsAboutToBeInserted( const QModelIndex & , int , int ) ),
           this, SLOT( rowsAboutToBeInserted( const QModelIndex &, int, int ) ) );
  connect( m_proxyModel, SIGNAL( rowsInserted( const QModelIndex &, int , int ) ),
           this, SLOT( rowsInserted( const QModelIndex &, int, int ) ) );
}

UserManager::~UserManager() {}

void UserManager::delUser( QString username ) {
  if ( useBattleUserTreeModel ) {
//     int i = m_battleUserModel->rowPositionForUser( username );
//     if ( i != -1 )
//       m_battleUserModel->removeRow( i, QModelIndex() );
    return;
  }
  int i = m_userModel->rowPositionForUser( username );
  if ( i != -1 )
    m_userModel->removeRow( i, QModelIndex() );
}

void UserManager::addUser( User u ) {
  m_userModel->insertRow( 0 );
  QVariant a;
  a.setValue( u );
  m_userModel->setData( m_userModel->index( 0, 0/*, QModelIndex()*/ ), a, Qt::UserRole );

  // strange findings: (do not remove this comment) (js)
  // the next line is needed because the sort/filter model will only resort after insertRows/
  // removeRows but not on dataChanged which is either a BUG in qtlobby or QT4
  // solution: we invoke it manually
  m_proxyModel->invalidate();
}

void UserManager::modUser( User u ) {
  int i = m_userModel->rowPositionForUser( u.name );
  if ( i == -1 )
    return;
  QVariant a;
  a.setValue( u );
  m_userModel->setData( m_userModel->index( i, 0/*, QModelIndex()*/ ), a, Qt::UserRole );
}

User UserManager::getUser( QString username ) {
  int i = m_userModel->rowPositionForUser( username );
  if ( i == -1 )
    return User();
  QModelIndex n = m_userModel->index( i, 0/*, QModelIndex()*/ );
  QVariant a = m_userModel->data( n, Qt::UserRole );
  return a.value<User>();
}

UserTreeModel* UserManager::model() {
  return m_userModel;
}

TreeSortFilterProxyModel* UserManager::proxyModel(){
  return m_proxyModel;
}

QItemSelectionModel* UserManager::selectionModel(){
  return m_selectionModel;
}

void UserManager::rowsAboutToBeInserted( const QModelIndex &, int, int ) {
//   qDebug() <<"rowsAboutToBeInserted";
}


void UserManager::rowsInserted( const QModelIndex &, int, int ){
//   qDebug() <<"rowsInserted";
}
