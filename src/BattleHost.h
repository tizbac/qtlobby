#ifndef BATTLEHOST_H
#define BATTLEHOST_H

#include <QThread>
#include "Command.h"
#include "UnitSyncLib.h"
#include "User.h"
#include <QScriptEngine>
#include <QScriptEngineDebugger>

#include "sqadsprototypes.h"

class BattleHost : public QThread {
    Q_OBJECT
    Q_PROPERTY(SqadsUserListPtr users READ getUsers)
            //formatter sucks
        public:
            BattleHost(QString host, QObject* parent = 0);
    void setHostingParams(quint8 type, quint8 natType, QString password, quint16 port, quint8 maxplayers, quint8 rank, QString map, QString mod, QString title);
    SqadsUserListPtr getUsers() const;
    void setHandicap(User* u, quint8 handicap);
    void kick(User* u);
    void forceTeamNo(User* u, quint8 no);
    void forceAllyTeamNo(User* u, quint8 no);
    void forceColor(User* u, QColor c);
    void forceSpectator(User* u);

signals:
    void sendCommand(Command command);
    void sendFakeMessage(QString msg);
    void errorMessage(QString msg);
    void hosted(int id);
    void closed();

    //For scripting
    void userJoined(User u);
    void userLeft(User u);
    void chatCommand(QString command, QString user);
public slots:
    void receiveCommand(Command command);

    //For scripting
    void closeBattle();
    void setMap(QString mapname);
    void setLocked(bool b);
    void broadcastBattleInfo();
    void sayBattle(QString message);
    void sayBattleEx(QString message);
    void sayUser(QString user, QString message);
protected:
    void run();
private:
    quint8 m_type;
    quint8 m_natType;
    QString m_password;
    quint16 m_port;
    quint8 m_maxplayers;
    quint8 m_rank;
    QString m_map;
    QString m_mod;
    QString m_title;
    int m_id;
    int m_hash;
    QScriptEngine m_engine;
    QScriptEngineDebugger m_debugger;
    bool m_locked;
    QScriptValue m_sqads;
    QList<User> m_users;
    QString m_host;

    SqadsUserListPrototype sqadsUserList;
    SqadsUserPrototype sqadsUser;
};

#endif // BATTLEHOST_H
