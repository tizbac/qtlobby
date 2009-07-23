#include "sqadsprototypes.h"

#include <QScriptEngine>
#include <QScriptValue>
#include <QRegExp>
#include "BattleHost.h"

SqadsUserListPrototype::SqadsUserListPrototype(QObject *parent) : QObject(parent) {

}

int SqadsUserListPrototype::getLength() {
    QList<User>* list = qscriptvalue_cast<SqadsUserListPtr>(thisObject()).ptr;
    if(!list) {
        context()->throwError("Member function called on invalid object");
        return -1;
    }
    return list->size();
}

SqadsUserPtr SqadsUserListPrototype::at(int i) {
    SqadsUserListPtr listptr = qscriptvalue_cast<SqadsUserListPtr>(thisObject());
    QList<User>* list = listptr.ptr;
    if(!list) {
        context()->throwError("Member function called on invalid object");
        return SqadsUserPtr();
    }
    if(i < 0 || i >= list->size()) {
        context()->throwError("Index out of bounds");
        return SqadsUserPtr();
    }
    SqadsUserPtr ptr;
    ptr.ptr = &(list->operator [](i));
    ptr.bh = listptr.bh;
    return ptr;
}

SqadsUserPtr SqadsUserListPrototype::at(QString username) {
    SqadsUserListPtr listptr = qscriptvalue_cast<SqadsUserListPtr>(thisObject());
    QList<User>* list = listptr.ptr;
    if(!list) {
        context()->throwError("Member function called on invalid object");
        return SqadsUserPtr();
    }
    SqadsUserPtr ptr;
    ptr.bh = listptr.bh;
    ptr.ptr = 0;
    username = username.replace("[", "\\[").replace("]", "\\]");
    QRegExp re("^.*" + username + ".*$", Qt::CaseInsensitive, QRegExp::RegExp2);
    for(int i = 0; i < list->size(); i++) {
        if(re.exactMatch(list->at(i).name)) {
            ptr.ptr = &(list->operator[](i));
        }
    }
    return ptr;
}

SqadsUserPrototype::SqadsUserPrototype(QObject *parent) : QObject(parent) {

}

QString SqadsUserPrototype::getName() const {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return QString();
    }
    return u->name;
}

bool SqadsUserPrototype::isIngame() const {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return false;
    }
    return u->userState.isAway();
}

void SqadsUserPrototype::setIngame(bool b) {
    SqadsUserPtr uptr = qscriptvalue_cast<SqadsUserPtr>(thisObject());
    User* u = uptr.ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return;
    }
    u->userState.setIngame(b);
    uptr.bh->broadCastMyUserStatus(u);
}

bool SqadsUserPrototype::isAway() const {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return false;
    }
    return u->userState.isAway();
}

quint8 SqadsUserPrototype::getRank() const {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return 0;
    }
    return u->userState.getRank();
}

bool SqadsUserPrototype::isModerator() {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return false;
    }
    return u->userState.isModerator();
}

bool SqadsUserPrototype::isBot() const {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return false;
    }
    return u->userState.isBot();
}

bool SqadsUserPrototype::isReady() const {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return false;
    }
    return u->battleState.isReady();
}

quint8 SqadsUserPrototype::getTeamNo() const {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return 0;
    }
    return u->battleState.getTeamNo();
}

void SqadsUserPrototype::setTeamNo(quint8 b) {
    SqadsUserPtr uptr = qscriptvalue_cast<SqadsUserPtr>(thisObject());
    User* u = uptr.ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return;
    }
    if(b < 1 || b > 16) {
        context()->throwError("Wrong team number");
        return;
    }
    u->battleState.setTeamNo(b - 1);
    uptr.bh->forceTeamNo(u, b - 1);
}

quint8 SqadsUserPrototype::getAllyTeamNo() const {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return 0;
    }
    return u->battleState.getAllyTeamNo();
}

void SqadsUserPrototype::setAllyTeamNo(quint8 b) {
    SqadsUserPtr uptr = qscriptvalue_cast<SqadsUserPtr>(thisObject());
    User* u = uptr.ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return;
    }
    if(b < 1 || b > 16) {
        context()->throwError("Wrong ally team number");
        return;
    }
    u->battleState.setAllyTeamNo(b - 1);
    uptr.bh->forceAllyTeamNo(u, b - 1);
}

bool SqadsUserPrototype::isPlayer() const {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return false;
    }
    return u->battleState.isPlayer();
}

void SqadsUserPrototype::setPlayer(bool b) {
    SqadsUserPtr uptr = qscriptvalue_cast<SqadsUserPtr>(thisObject());
    User* u = uptr.ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return;
    }
    if(!b) {
        u->battleState.setPlayer(b);
        uptr.bh->forceSpectator(u);
    }
}

quint8 SqadsUserPrototype::getHandicap() const {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return 0;
    }
    return u->battleState.getHandicap();
}

void SqadsUserPrototype::setHandicap(quint8 b) {
    SqadsUserPtr uptr = qscriptvalue_cast<SqadsUserPtr>(thisObject());
    User* u = uptr.ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return;
    }
    if(b > 100) {
        context()->throwError("Wrong hadicap value");
        return;
    }
    u->battleState.setHandicap(b);
    uptr.bh->setHandicap(u, b);
}

QString SqadsUserPrototype::getColor() const {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return "#000000";
    }
    return u->m_color.name();
}

void SqadsUserPrototype::setColor(QString c) {
    SqadsUserPtr uptr = qscriptvalue_cast<SqadsUserPtr>(thisObject());
    User* u = uptr.ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return;
    }
    u->m_color.setNamedColor(c);
    uptr.bh->forceColor(u, u->m_color);
}

quint8 SqadsUserPrototype::syncState() const {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return 0;
    }
    return u->battleState.syncState();
}

quint8 SqadsUserPrototype::getSide() const {
    User* u = qscriptvalue_cast<SqadsUserPtr>(thisObject()).ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return 0;
    }
    return u->battleState.getSide();
}

void SqadsUserPrototype::kick() {
    SqadsUserPtr uptr = qscriptvalue_cast<SqadsUserPtr>(thisObject());
    User* u = uptr.ptr;
    if(!u) {
        context()->throwError("Member function called on invalid object");
        return;
    }
    uptr.bh->kick(u);
}


bool SqadsUserPrototype::isValid() const {
    SqadsUserPtr uptr = qscriptvalue_cast<SqadsUserPtr>(thisObject());
    return uptr.ptr != 0;
}
