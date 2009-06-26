//
// C++ Interface: User
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef USER_H
#define USER_H

#include <QVariant>
#include <QColor>
#include <QDebug>

union UserState {
  unsigned state;       //!<
  struct {
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    unsigned empty:25;  //!<
    bool isBot:1;       //!< set when player is a bot (read only)
    bool isModerator:1; //!< set when player has moderator rights (read only)
    unsigned skill:3;   //!< 0 to 7 indicating the players skill
    bool isAway:1;      //!< set if player is away
    bool isInGame:1;    //!< set if player is in game
#else
    bool isInGame:1;
    bool isAway:1;
    unsigned skill:3;
    bool isModerator:1;
    bool isBot:1;
    unsigned empty:25;
#endif
  } s;
};

union BattleState {
  unsigned state; //!<
  struct {
#if Q_BYTE_ORDER == Q_BIG_ENDIAN
    unsigned empty3:4;     //!<
    unsigned side:4;       //!< stands for arm, core, tll, ...
    unsigned sync:2;       //!< 0 = unknown, 1 = sync, 2 = not sync
    unsigned empty2:4;     //!<
    unsigned handicap:7;   //!< 0 to 100 handicap in %
    bool isNotSpectator:1; //!< set when player is spectator
    unsigned ally:4;       //!< 0 to 15 ally team number
    unsigned team:4;       //!< 0 to 15 team number
    bool isReady:1;        //!< set when player hit the ready button
    unsigned empty:1;      //!<
#else
    unsigned empty:1;
    bool isReady:1;
    unsigned team:4;
    unsigned ally:4;
    bool isNotSpectator:1;
    unsigned handicap:7;
    unsigned empty2:4;
    unsigned sync:2;
    unsigned side:4;
    unsigned empty3:4;
#endif
  } s;
};
/**
        @author Joachim Schiele <js@lastlog.de>
*/
class User /*: public QObject*/ {
// Q_OBJECT
public:
  User();
  ~User();

  void setColor( int TASColor );
  int color();
  QString colorForScript();

  /* UserInformation */
  QString name;            //!< like "[NC]qknight"
  QString countryCode;     //!< like "DE"
  QString cpu;             //!< like "2100"
  int joinedBattleId;      //!< like 1234
  BattleState battleState; //!< like 0 to 2^32-1
  UserState userState;     //!< like 0 to 2^7-1 = 127
  QColor m_color;          //!< the in game color for the units
};
Q_DECLARE_METATYPE( User );
#endif
