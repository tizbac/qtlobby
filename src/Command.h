// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef COMMAND_H
#define COMMAND_H
#include <QString>
#include <QStringList>
#include <QMetaType>

class Command {
public:
    Command();
    Command( QString commandString );
    ~Command() {}
    QString toQString();

    quint32 id;
    QString name;
    QStringList attributes;
    bool executed;
};

Q_DECLARE_METATYPE(Command);

#endif
