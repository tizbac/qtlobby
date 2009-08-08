// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "UserGroup.h"
#include <QDebug>
#include <QDomDocument>

UserGroupList::UserGroupList() {
    clanRegexp.setPattern(".*(\\[.*\\]).*");
}

UserGroupList::~UserGroupList() {
}

QStringList UserGroupList::getGroupNames() {
    QStringList ret;
    for (int i = 0; i < groups.size(); i++) {
        ret << groups.at(i)->name;
    }
    ret.sort();
    return ret;
}

void UserGroupList::append(UserGroup* g) {
    groups << g;
    groupsMap[g->name] = g;
    for (int i = 0; i < g->members.size(); i++) {
        colorsMap[g->members.at(i)] = g->color;
    }
}

UserGroup* UserGroupList::findGroup(QString name) {
    if (groupsMap.contains(name))
        return groupsMap[name];
    return (UserGroup*)0;
}

QString UserGroupList::toXml() {
    QDomDocument doc("groups");
    QDomElement groupsEl = doc.createElement("groups");
    doc.appendChild(groupsEl);
    for (int i = 0; i < groups.size(); i++) {
        QDomElement group = doc.createElement("group");
        group.setAttribute("name", groups.at(i)->name);
        group.setAttribute("description", groups.at(i)->description);
        group.setAttribute("color", groups.at(i)->color.name());
        for (int j = 0; j < groups.at(i)->members.size(); j++) {
            QDomElement member = doc.createElement("member");
            member.setAttribute("name", groups.at(i)->members.at(j));
            group.appendChild(member);
        }
        groupsEl.appendChild(group);
    }
    return doc.toString(0);
}

void UserGroupList::fromXml(QString xml) {
    QDomDocument doc;
    if (!doc.setContent(xml))
        return;
    QDomNodeList list = doc.elementsByTagName("group");
    for (int i = 0; i < list.size(); i++) {
        QDomElement group = list.at(i).toElement();
        UserGroup* g = new UserGroup();
        g->name = group.attribute("name");
        g->description = group.attribute("description");
        g->color = QColor(group.attribute("color"));
        QDomNodeList  members = group.elementsByTagName("member");
        for (int j = 0; j < members.size(); j++) {
            QDomElement member = members.at(j).toElement();
            g->members << member.attribute("name");
        }
        append(g);
    }
}

void UserGroupList::removeGroup(QString name) {
    for (int i = 0; i < groups.size(); i++) {
        if (groups.at(i)->name == name) {
            for (int j = 0; j < groups.at(i)->members.size(); j++) {
                colorsMap.remove(groups.at(i)->members.at(j));
            }
            delete groups.at(i);
            groups.removeAt(i);
            groupsMap.remove(name);
            return;
        }
    }
}

int UserGroupList::size() const {
    return groups.size();
}

QColor UserGroupList::getUserColor(QString name) {
    if (colorsMap.contains(name))
        return colorsMap[name];
    if (clanRegexp.indexIn(name) >= 0) {
        if (colorsMap.contains(clanRegexp.capturedTexts()[1]))
            return colorsMap[clanRegexp.capturedTexts()[1]];
    }
    return QColor();
}

void UserGroupList::updateMappings() {
    colorsMap.clear();
    groupsMap.clear();
    for (int i = 0; i < groups.size(); i++) {
        UserGroup* g = groups.at(i);
        groupsMap[g->name] = g;
        for (int i = 0; i < g->members.size(); i++) {
            colorsMap[g->members.at(i)] = g->color;
        }
    }
}

UserGroup* UserGroupList::at(int i) {
    return groups.at(i);
}

void UserGroupList::save() {
    QSettings* s = Settings::Instance();
    s->setValue("userGroups", toXml());

    QString ignorestr;
    foreach (QString ignorename, ignoreSet) {
        ignorestr += ignorename + ";";
    }
    s->setValue("Ignore", ignorestr);
}

void UserGroupList::load() {
    QSettings* s = Settings::Instance();
    fromXml(s->value("userGroups").toString());

    QStringList ignorestr = s->value("Ignore").toString().split( ";" );
    foreach(QString ignorename, ignorestr) {
        if(ignorename != "")
            ignoreSet.insert(ignorename);
    }
}

bool UserGroupList::containsUserName( QString name ) {
    foreach( UserGroup* g, groups ) {
        if ( g->members.contains(name) )
            return true;
    }
    return false;
}

void UserGroupList::toggleIgnore(QString name) {
    if(!getIgnore(name))
        ignoreSet.insert(name);
    else
        ignoreSet.remove(name);
}

bool UserGroupList::getIgnore(QString name) {
    return ignoreSet.contains(name);
}