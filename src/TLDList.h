// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef TLDLIST_H
#define TLDLIST_H
#include <QMap>
#include <QString>
#include <QStringList>
/**
 * Contains all Top Level Domains for the countries.
 */
class TLDList : public QObject {
    Q_OBJECT
public:
    TLDList( QObject* parent = 0);
    ~TLDList();
    static QMap<QString, QString>* TLDMap;
};

#endif
