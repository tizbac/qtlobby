#ifndef SQADSPROTOTYPES_H
#define SQADSPROTOTYPES_H

#include <QObject>
#include <QScriptable>
#include "User.h"

class BattleHost;

struct SqadsUserListPtr {
    SqadsUserListPtr() {ptr = 0; bh = 0;}
    QList<User>* ptr;
    BattleHost* bh;
};

struct SqadsUserPtr {
    SqadsUserPtr() {ptr = 0; bh = 0;}
    User* ptr;
    BattleHost* bh;
};

class SqadsUserListPrototype : public QObject, protected QScriptable {
    Q_OBJECT
    Q_PROPERTY(int length READ getLength)
public:
    SqadsUserListPrototype(QObject *parent = 0);
    int getLength();
public slots:
    SqadsUserPtr at(int i);
    SqadsUserPtr at(QString username);
};


class SqadsUserPrototype : public QObject, protected QScriptable {
    Q_OBJECT
    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(bool ingame READ isIngame WRITE setIngame)
    Q_PROPERTY(bool away READ isAway)
    Q_PROPERTY(quint8 rank READ getRank)
    Q_PROPERTY(bool moderator READ isModerator)
    Q_PROPERTY(bool bot READ isBot)
    Q_PROPERTY(bool ready READ isReady)
    Q_PROPERTY(quint8 team READ getTeamNo WRITE  setTeamNo)
    Q_PROPERTY(quint8 ally READ getAllyTeamNo WRITE setAllyTeamNo)
    Q_PROPERTY(bool player READ isPlayer WRITE setPlayer)
    Q_PROPERTY(quint8 handicap READ getHandicap WRITE setHandicap)
    Q_PROPERTY(QString color READ getColor WRITE setColor)
    Q_PROPERTY(quint8 sync READ syncState)
    Q_PROPERTY(quint8 side READ getSide)
    Q_PROPERTY(bool valid READ isValid)
public:
    SqadsUserPrototype(QObject *parent = 0);
    QString getName() const;
    bool isIngame() const;
    void setIngame(bool b);
    bool isAway() const;
    quint8 getRank() const;
    bool isModerator();
    bool isBot() const;
    bool isReady() const;
    quint8 getTeamNo() const;
    void setTeamNo(quint8 b);
    quint8 getAllyTeamNo() const;
    void setAllyTeamNo(quint8 b);
    bool isPlayer() const;
    void setPlayer(bool b);
    quint8 getHandicap() const;
    void setHandicap(quint8 b);
    QString getColor() const;
    void setColor(QString c);
    quint8 syncState() const;
    quint8 getSide() const;
    bool isValid() const;
public slots:
    void kick();
};

Q_DECLARE_METATYPE(SqadsUserListPtr)
Q_DECLARE_METATYPE(SqadsUserPtr)

#endif // SQADSPROTOTYPES_H
