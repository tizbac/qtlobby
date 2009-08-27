// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "BattlePasswordWidget.h"

BattlePasswordWidget::BattlePasswordWidget( QDialog *parent ) : QDialog( parent ) {
    setupUi( this );
    id = 0;
    connect( battlePasswordButtonBox, SIGNAL( accepted() ), this, SLOT( acceptThis() ) );
    connect( battlePasswordButtonBox, SIGNAL( rejected() ), this, SLOT( close() ) );
}

BattlePasswordWidget::~BattlePasswordWidget() { }

void BattlePasswordWidget::acceptThis() {
    QString pw = battlePasswordLineEdit->text();
    hide();
    emit wantJoinBattle( id, pw, false );
}

void BattlePasswordWidget::setBattleId( int id ) {
    this->id = id;
}

void BattlePasswordWidget::resetPassword() {
    battlePasswordLineEdit->clear();
}
