#include "BattleHost.h"
#include <QMainWindow>

BattleHost::BattleHost(QString host, QObject* parent) : QThread(parent) {
    m_host = host;
    User u;
    u.name = host;
    u.battleState.setState(0);
    m_users.append(u);
}

void BattleHost::setHostingParams(quint8 type, quint8 natType, QString password, quint16 port, quint8 maxplayers, quint8 rank, QString map, QString mod, QString title) {
    m_type = type;
    m_natType = natType;
    m_password = password;
    m_port = port;
    m_maxplayers = maxplayers;
    m_rank = rank;
    m_map = map;
    m_mod = mod;
    m_title = title;
    m_locked = false;
    m_debugger.attachTo(&m_engine);
    m_engine.setDefaultPrototype(qMetaTypeId<SqadsUserListPtr>(), m_engine.newQObject(&sqadsUserList));
    m_engine.setDefaultPrototype(qMetaTypeId<SqadsUserPtr>(), m_engine.newQObject(&sqadsUser));
    qRegisterMetaType<Command>("Command");
}


void BattleHost::run() {
    //OPENBATTLE type natType password port maxplayers hashcode rank maphash {map} {title} {modname}
    QString openBattle = "OPENBATTLE %1 %2 %3 %4 %5 %6 %7 %8 %9";
    UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();
    int maphash = unitSyncLib->mapChecksum(m_map);
    if(!maphash) {
        emit errorMessage("Map not found");
        qDebug() << "Map not found";
        return;
    }
    int modhash = unitSyncLib->modChecksum(m_mod);
    if(!modhash) {
        emit errorMessage("Mod not found");
        qDebug() << "Mod not found";
        return;
    }
    Command c(openBattle
              .arg(m_type)
              .arg(m_natType)
              .arg(m_password)
              .arg(m_port)
              .arg(m_maxplayers)
              .arg(modhash)
              .arg(m_rank)
              .arg(maphash)
              .arg(m_map + "\t" + m_title + "\t" + m_mod)
              );
    emit sendCommand(c);
    exec();
}

void BattleHost::closeBattle() {
    emit sendCommand(Command("LEAVEBATTLE"));
    emit closed();
    quit();
}

void BattleHost::receiveCommand( Command command ) {
    command.name = command.name.toUpper();
    if ( command.name == "OPENBATTLE" ) {
        m_id = command.attributes[0].toInt();
        //For SQADS debugging uncomment this
        //m_debugger.standardWindow()->show();
        QFile scriptFile(":/src/sqads.js");
        scriptFile.open(QIODevice::ReadOnly);
        m_engine.evaluate(scriptFile.readAll(), "sqads.js");
        scriptFile.close();
        QScriptValue ctor = m_engine.evaluate("Sqads");
        QScriptValue scriptBattleHost = m_engine.newQObject(this);
        m_sqads = ctor.construct(QScriptValueList() << scriptBattleHost);
        emit hosted(command.attributes[0].toInt());
        return;
    } else if ( command.name == "OPENBATTLEFAILED" ) {
        QString reason = command.attributes[0];
        emit errorMessage("Battle hosting failed: " + reason);
        quit();
    } else if ( command.name == "JOINEDBATTLE" ) {
        int id = command.attributes[0].toInt();
        if(id == m_id) {
            QString user = command.attributes[1];
            User u;
            u.name = user;
            u.battleState = 0;
            m_users.append(u);
        }
    } else if ( command.name == "LEFTBATTLE" ) {
        int id = command.attributes[0].toInt();
        if(id == m_id) {
            QString user = command.attributes[1];
            for(int i = 0; i < m_users.size(); i++) {
                if(m_users[i].name == user) {
                    m_users.removeAt(i);
                    break;
                }
            }
        }
    } else if ( command.name == "OPENBATTLEFAILED" ) {
        QString reason = command.attributes[0];
        emit errorMessage("Battle hosting failed: " + reason);
        quit();
    } else if ( command.name == "SAIDPRIVATE" || command.name == "SAIDBATTLE" ) {
        QString user = command.attributes[0];
        command.attributes.removeFirst();
        QString cmd = command.attributes.join(" ");
        if(cmd.startsWith("!")) {
            emit chatCommand(cmd.right(cmd.length() - 1), user);
        }
    } else if ( command.name == "BATTLECLOSED" ) {
        int id = command.attributes[0].toInt();
        if(id == m_id) {
            emit closed();
            quit();
        }
    } else if ( command.name == "CLIENTBATTLESTATUS" ) {
        QString user = command.attributes.takeFirst();
        for(int i = 0; i < m_users.size(); i++) {
            if(m_users[i].name == user) {
                m_users[i].battleState.setState(command.attributes.takeFirst().toInt());
                emit clientStatusChanged(user);
            }
        }
    }
}

void BattleHost::setMap(QString mapname) {
    UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();
    m_hash = unitSyncLib->mapChecksum(m_map);
    if(!m_hash) {
        emit errorMessage("Map not found");
        qDebug() << "Map not found";
        return;
    }
    m_map = mapname;
    broadcastBattleInfo();
}

void BattleHost::broadcastBattleInfo() {
    int specCount = 0;
    foreach(User u, m_users) {
        if(!u.battleState.isPlayer()) specCount++;
    }
    emit sendCommand(Command(QString("UPDATEBATTLEINFO %1 %2 %3 %4")
                             .arg(specCount)
                             .arg(m_locked ? "1" : "0")
                             .arg(m_hash)
                             .arg(m_map)));
}

void BattleHost::setLocked(bool b) {
    if(m_locked == b) return;
    m_locked = b;
    broadcastBattleInfo();
}

void BattleHost::sayBattle(QString message) {
    emit sendCommand(Command("SAYBATTLE " + message));
}

void BattleHost::sayBattleEx(QString message) {
    emit sendCommand(Command("SAYBATTLEEX " + message));
}

void BattleHost::sayUser(QString name, QString message) {
    emit sendCommand(Command("SAYPRIVATE " + name + " " + message));
}

SqadsUserListPtr BattleHost::getUsers() const {
    SqadsUserListPtr ptr;
    ptr.ptr = const_cast<QList<User>*>(&m_users);
    ptr.bh = const_cast<BattleHost*>(this);
    return ptr;
}

void BattleHost::setHandicap(User* u, quint8 handicap) {
    emit sendCommand(Command("HANDICAP " + u->name + " " + QString::number(handicap)));
}

void BattleHost::kick(User* u) {
    emit sendCommand(Command("KICKFROMBATTLE " + u->name));
}

void BattleHost::forceTeamNo(User* u, quint8 no) {
    emit sendCommand(Command("FORCETEAMNO " + u->name + " " + QString::number(no)));
}

void BattleHost::forceAllyTeamNo(User* u, quint8 no) {
    emit sendCommand(Command("FORCEALLYNO " + u->name + " " + QString::number(no)));
}

void BattleHost::forceColor(User* u, QColor c) {
    u->m_color = c;
    emit sendCommand(Command("FORCETEAMCOLOR " + u->name + " " + QString::number(u->color())));
}

void BattleHost::forceSpectator(User* u) {
    emit sendCommand(Command("FORCESPECTATORMODE " + u->name));
}


void BattleHost::broadCastMyUserStatus(User* u) {
    emit sendCommand(Command(QString("MYSTATUS %1").arg(u->userState.getState())));
}
