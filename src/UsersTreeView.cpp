#include "UsersTreeView.h"
#include "UserMenuBuilder.h"

UsersTreeView::UsersTreeView(QWidget * parent) : QTreeView(parent) {
    connect( this, SIGNAL( customContextMenuRequested( const QPoint & ) ),
             this, SLOT( customContextMenuRequestedSlot( const QPoint & ) ) );
    connect( this, SIGNAL( doubleClicked( const QModelIndex & ) ),
             this, SLOT( doubleClickedSlot( const QModelIndex & ) ) );
}

UsersTreeView::~UsersTreeView() {}

void UsersTreeView::doubleClickedSlot( const QModelIndex & index ) {
    emit sendInputAndFocus( QString( "/query " ) + index.data( Qt::DisplayRole ).toString() );
}

void UsersTreeView::customContextMenuRequestedSlot( const QPoint & point ) {
    if ( selectedIndexes().size() == 0 ) return;
    QModelIndex index = selectedIndexes().first();
    if ( !index.isValid() ) return;
    User u = index.data( Qt::UserRole ).value<User>();
    UserMenuBuilder* b = UserMenuBuilder::getInstance();
    b->setBattle(false);
    QMenu* menu = b->buildMenu(u);
    QAction *action = menu->exec( this->viewport()->mapToGlobal( point ) );
    b->processMenuAction(action);
}
