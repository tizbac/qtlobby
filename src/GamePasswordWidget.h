//
// C++ Interface: GamePasswordWidget
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GAMEPASSWORDWIDGET_H
#define GAMEPASSWORDWIDGET_H

#include <QDialog>
#include <QDebug>

#include "ui_gamePasswordWidget.h"

/**
	@author Joachim Schiele <js@lastlog.de>
*/
class GamePasswordWidget : public QDialog, private Ui::gamePasswordWidget
{
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
