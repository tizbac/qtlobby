// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef BATTLEPASSWORDWIDGET_H
#define BATTLEPASSWORDWIDGET_H

#include <QDialog>
#include <QDebug>

#include "ui_BattlePasswordWidget.h"

class BattlePasswordWidget : public QDialog, private Ui::BattlePasswordWidget {
    Q_OBJECT
public:
    BattlePasswordWidget( QDialog *parent = 0 );
    ~BattlePasswordWidget();
    void setBattleId( int );
    void resetPassword();
private:
    unsigned int id;
signals:
    void wantJoinBattle( unsigned int battleId, QString password, bool closeFirst );
private slots:
    void acceptThis();
};

#endif
