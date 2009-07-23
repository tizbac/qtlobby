// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "UserManager.h"

UserManager::UserManager( QObject* parent, bool useBattleUserTreeModel ) : QObject( parent ) {
    this->useBattleUserTreeModel = useBattleUserTreeModel;
    qRegisterMetaType<User>( "User" );
    if ( useBattleUserTreeModel ) {
        m_battleUserModel = new BattleUserTreeModel( this );
        m_userModel = m_battleUserModel;
        m_proxyModel = new TreeSortFilterProxyModel( m_battleUserModel );
        m_selectionModel = new QItemSelectionModel( m_battleUserModel );
        m_proxyModel->setSourceModel( m_battleUserModel );
    } else {
        m_userModel = new UserTreeModel( this );
        m_battleUserModel = m_userModel;
        m_proxyModel = new TreeSortFilterProxyModel( m_userModel );
        m_selectionModel = new QItemSelectionModel( m_userModel );
        m_proxyModel->setSourceModel( m_userModel );
    }
    m_proxyModel->setSortCaseSensitivity( Qt::CaseInsensitive );
    m_proxyModel->setDynamicSortFilter( true );
    m_proxyModel->colRoleMap[0] = Qt::ToolTipRole;
    m_proxyModel->colRoleMap[1] = Qt::ToolTipRole;
    m_proxyModel->colRoleMap[2] = Qt::ToolTipRole;
    m_proxyModel->colRoleMap[3] = Qt::DisplayRole;
    m_proxyModel->colRoleMap[4] = (Qt::ItemDataRole)(Qt::UserRole+1);
    m_proxyModel->colRoleMap[5] = (Qt::ItemDataRole)(Qt::UserRole+1);
    m_proxyModel->colRoleMap[6] = (Qt::ItemDataRole)(Qt::UserRole+1);
    m_proxyModel->colRoleMap[7] = (Qt::ItemDataRole)(Qt::UserRole+1);
    m_proxyModel->colRoleMap[8] = (Qt::ItemDataRole)(Qt::UserRole+1);
    m_proxyModel->colRoleMap[9] = (Qt::ItemDataRole)(Qt::UserRole+1);

    connect( m_proxyModel, SIGNAL( rowsAboutToBeInserted( const QModelIndex & , int , int ) ),
             this, SLOT( rowsAboutToBeInserted( const QModelIndex &, int, int ) ) );
    connect( m_proxyModel, SIGNAL( rowsInserted( const QModelIndex &, int , int ) ),
             this, SLOT( rowsInserted( const QModelIndex &, int, int ) ) );
}

UserManager::~UserManager() {}

void UserManager::delUser( QString username ) {
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
    m_userModel->onGroupChanged();
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

TreeSortFilterProxyModel* UserManager::proxyModel() {
    return m_proxyModel;
}

QItemSelectionModel* UserManager::selectionModel() {
    return m_selectionModel;
}

void UserManager::rowsAboutToBeInserted( const QModelIndex &, int, int ) {
    //   qDebug() <<"rowsAboutToBeInserted";
}


void UserManager::rowsInserted( const QModelIndex &, int, int ) {
    //   qDebug() <<"rowsInserted";
}
