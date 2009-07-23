// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "GamePasswordWidget.h"

GamePasswordWidget::GamePasswordWidget( QDialog *parent ) : QDialog( parent ) {
    setupUi( this );
    id = 0;
    connect( gamePasswordButtonBox, SIGNAL( accepted() ), this, SLOT( acceptThis() ) );
    connect( gamePasswordButtonBox, SIGNAL( rejected() ), this, SLOT( close() ) );
}

GamePasswordWidget::~GamePasswordWidget() { }

void GamePasswordWidget::acceptThis( ) {
    QString pw = gamePasswordLineEdit->text();
    hide();
    emit wantJoinBattle( id, pw );
}

void GamePasswordWidget::setBattleId( int id ) {
    this->id = id;
}

void GamePasswordWidget::resetPassword() {
    gamePasswordLineEdit->clear();
}

