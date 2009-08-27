// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef BATTLECLOSEFIRSTWIDGET_H
#define BATTLECLOSEFIRSTWIDGET_H

#include <QDialog>
#include <QDebug>

#include "Settings.h"
#include "ui_BattleCloseFirstWidget.h"

class BattleCloseFirstWidget : public QDialog, private Ui::BattleCloseFirstWidget {
    Q_OBJECT
public:
    BattleCloseFirstWidget( QDialog *parent = 0 );
    ~BattleCloseFirstWidget();
    void setBattleId( int );
    void setPassword( QString password );
private:
    unsigned int id;
    QString password;
signals:
    void wantJoinBattle( unsigned int battleId, QString password, bool closeFirst );
private slots:
    void acceptThis();
};

#endif
