//
// C++ Implementation: User
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "User.h"

User::User() {
  battleState.state = 0;
  userState.state = 0;
  joinedBattleId = -1; // -1 == not in a battle
  m_color = QColor( "red" );
}
User::~User() { }

/**
 * @param TASColor Color in the TAS representation 0x00bbggrr.
 */
void User::setColor( int TASColor ) {
  int red = TASColor & 0xff;
  int green = TASColor & 0x00ff;
  int blue = TASColor & 0x0000ff;
  m_color = QColor( red, green, blue );
}

/**
 * @return the TAS representation of the color.
 */
int User::color() {
  int ret = m_color.blue();
  ret <<= 8;
  ret += m_color.green();
  ret <<= 8;
  ret += m_color.red();
  return ret;
}

QString User::colorForScript() {
  return QString( "%1 %2 %3" )
      .arg( m_color.redF()   / 255.0, 0, 'f', 5 )
      .arg( m_color.greenF() / 255.0, 0, 'f', 5 )
      .arg( m_color.blueF()  / 255.0, 0, 'f', 5 );
}
