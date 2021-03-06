// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "User.h"

User::User() {
    joinedBattleId = -1; // -1 == not in a battle
    m_color = QColor( "red" );
    founder = false;
}
User::~User() { }

/**
 * @param TASColor Color in the TAS representation 0x00bbggrr.
 */
void User::setColor( int TASColor ) {
    int red   = (TASColor & 0x000000ff);
    int green = (TASColor & 0x0000ff00) >> 8;
    int blue  = (TASColor & 0x00ff0000) >> 16;
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
            .arg( m_color.redF(), 0, 'f', 5 )
            .arg( m_color.greenF(), 0, 'f', 5 )
            .arg( m_color.blueF(), 0, 'f', 5 );
}

void User::setScriptPass(QString pass)
{
    qWarning("User %s , set pass: %s",name.toStdString().c_str(),pass.toStdString().c_str());
    script_pass = pass;
}
