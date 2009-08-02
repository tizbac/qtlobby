// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "BattleManager.h"

BattleManager::BattleManager( QObject* parent ) : QObject( parent ) {
    qRegisterMetaType<Battle>( "Battle" );
    m_model = new BattleTreeModel( this );

    m_proxyModel = new TreeSortFilterProxyModel( m_model );
    m_selectionModel = new QItemSelectionModel( m_model );

    m_proxyModel->setSourceModel( m_model );
    m_proxyModel->setSortCaseSensitivity( Qt::CaseInsensitive );
    m_proxyModel->setDynamicSortFilter( true );
    m_proxyModel->colRoleMap[0] = Qt::ToolTipRole;
    m_proxyModel->colRoleMap[1] = Qt::ToolTipRole;
    m_proxyModel->colRoleMap[2] = Qt::ToolTipRole;
    m_proxyModel->colRoleMap[3] = Qt::DisplayRole;
    m_proxyModel->colRoleMap[4] = Qt::DisplayRole;
    m_proxyModel->colRoleMap[5] = Qt::DisplayRole;
    m_proxyModel->colRoleMap[6] = Qt::DisplayRole;
    m_proxyModel->colRoleMap[7] = (Qt::ItemDataRole)(Qt::UserRole+1);
}

BattleManager::~BattleManager() {
    delete m_model;
}

void BattleManager::delBattle( int id ) {
    int i = m_model->rowPositionForBattle( id );
    if ( i != -1 ) {
        m_model->removeRow( i, QModelIndex() );
    } else {
        qDebug() << "Error, can't delete battle since battleID:  " << id << " is not found in the container for all battles";
    }
}

void BattleManager::addBattle( Battle b ) {
    m_model->insertRow( 0 );
    QVariant a;
    a.setValue( b );
    m_model->setData( m_model->index( 0, 0, QModelIndex() ), a, Qt::UserRole );
}

void BattleManager::modBattle( Battle b ) {
    int i = m_model->rowPositionForBattle( b.id );
    //There is a check here and it stops from doing something bad(crashing?)
    if ( i == -1 )
        return;
    QVariant a;
    a.setValue( b );
    m_model->setData( m_model->index( i, 0, QModelIndex() ), a, Qt::UserRole );
}

Battle BattleManager::getBattle( int id ) {
    int i = m_model->rowPositionForBattle( id );

    if ( i == -1 ) {
        qDebug() << "Error: battle not found, returing Battle() for BattleID:" << id;
        return Battle();
    }
    QModelIndex n = m_model->index( i, 0, QModelIndex() );
    QVariant a = m_model->data( n, Qt::UserRole );
    return a.value<Battle>();
}

BattleTreeModel* BattleManager::model() {
    return m_model;
}

TreeSortFilterProxyModel* BattleManager::proxyModel() {
    return m_proxyModel;
}

QItemSelectionModel* BattleManager::selectionModel() {
    return m_selectionModel;
}

void BattleManager::setUsers(Users* users) {
    m_model->setUsers(users);
}
