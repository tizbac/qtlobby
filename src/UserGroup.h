// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef USERGROUP_H
#define USERGROUP_H

#include <QObject>
#include "Singleton.h"
#include <QString>
#include <QStringList>
#include <QColor>
#include <QMetaType>
#include <QDataStream>
#include <QMap>
#include "Settings.h"


struct UserGroup {
    QString name;
    QString description;
    QColor color;
    QStringList members;
};

class UserGroupList : public QObject, public Singleton<UserGroupList> {
    Q_OBJECT
    friend class Singleton<UserGroupList>;
public:
    QStringList getGroupNames();
    UserGroup* findGroup(QString name);
    UserGroup* at(int i);
    QColor getUserColor(QString name);
    QString toXml();
    void fromXml(QString xml);
    void removeGroup(QString name);
    void append(UserGroup* g);
    int size() const;
    void updateMappings();
    void save();
    void load();
    bool containsUserName(QString name);
private:
    QMap<QString, UserGroup*> groupsMap;
    QMap<QString, QColor> colorsMap;
    QList<UserGroup*> groups;
    QRegExp clanRegexp;
    UserGroupList();
    ~UserGroupList();
};


#endif // USERGROUP_H
