// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef GAMEPASSWORDWIDGET_H
#define GAMEPASSWORDWIDGET_H

#include <QDialog>
#include <QDebug>

#include "ui_GamePasswordWidget.h"

/**
        @author Joachim Schiele <js@lastlog.de>
*/
class GamePasswordWidget : public QDialog, private Ui::GamePasswordWidget {
    Q_OBJECT
public:
    GamePasswordWidget(QDialog *parent = 0);
    ~GamePasswordWidget();
    void setBattleId(int);
    void resetPassword();
private:
    unsigned int id;
signals:
    void wantJoinBattle(unsigned int,QString);
private slots:
    void acceptThis();
};

#endif
