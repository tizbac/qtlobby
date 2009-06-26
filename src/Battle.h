//
// C++ Interface: Battle
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BATTLE_H
#define BATTLE_H

#include <QString>
#include <QVariant>
#include <QMap>
#include "ModOption.h"

/**
represents the structure of a battle

 @author Joachim Schiele <js@lastlog.de>
*/

struct StartRect {
  int left;
  int top;
  int right;
  int bottom;
  StartRect( int left = 0, int top = 0, int right = 0, int bottom = 0 ) {
    this->left = left;
    this->top = top;
    this->right = right;
    this->bottom = bottom;
  }
};

namespace StartPosType {
  enum {
    Fixed = 0,
    Random = 1,
    Choose = 2,
    Pick = 3
  };
}
namespace GameType {
  enum  {
    LastUnit = 0,
    CommanderEnds = 1,
    Lineage = 2
  };
}
namespace BattleType {
  enum {
    Unknown = 0,
    Multi = 1,
    Single = 2
  };
}

class Battle {
public:
  Battle();
  ~Battle();

  int id;                   //!< battle id 0 to 2^32-1
  bool type;                //!< 0 = normal, 1 = replay
  int natType;              //!< nat traversal method, 0 = no NAT traversal technique
  QString founder;          //!< founder userName
  QString ip;               //!< host IP address
  int port;                 //!< host port number
  int maxPlayers;           //!< 0 to 16
  bool isPasswordProtected; //!< set if battle needs password to join
  int minRank;              //!< 0 to 6, prevents users with lower rank from joining
  int mapHash;              //!< 0 to 2^32-1

  QString mapName;          //!< selected map name, can change while opened
  QString title;            //!< title describing the battle
  QString modName;          //!< selected mod name, can not change while opened

  bool isLocked;            //!< set if locked, prevents users from joining
  int spectatorCount;       //!< number of users in battle with spectator flag
  int playerCount;          //!< comes from the users object

  bool isStarted;           //!< set if started, prevents users from joining
  QString countryCode;      //!< comes from the founders user object

  QMap<int, StartRect> allyNumberStartRectMap;
  QMap<QString, ModOption> options; //!< contains: StartMetal, StartEnergy, MaxUnits, StartPosType, GameMode, LimitDGun, DiminishingMMs, GhostedBuildings
  

  QList<QString> disabledUnits;
  QString shortModName() const;
  QString shortMapName() const;
};

Q_DECLARE_METATYPE(Battle);
#endif
