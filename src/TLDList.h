//
// C++ Interface: TLDList
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
