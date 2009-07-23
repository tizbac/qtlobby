// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef TLDLIST_H
#define TLDLIST_H
#include <QMap>
#include <QString>
#include <QStringList>
/**
        @author Joachim Schiele <js@lastlog.de>
*/

class TLDList {
public:
    TLDList();
    ~TLDList();
    static QMap<QString,QString>* TLDMap;
};

#endif
