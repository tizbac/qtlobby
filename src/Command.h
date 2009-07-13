//
// C++ Interface: Command
//
// Description: $Revision$
//
//
// Author: Mirko Windhoff <qtlobby.reg@ncnever.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMAND_H
#define COMMAND_H
#include <QString>
#include <QStringList>
class Command {
public:
    Command();
    Command( QString commandString );
    ~Command() {};
    QString toQString();

    quint32 id;
    QString name;
    QStringList attributes;
    bool executed;
};

#endif
