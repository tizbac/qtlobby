// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "BattleHost.h"
#include <QMainWindow>
#include "UnitSyncLib.h"

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
        fillScriptTags();
        broadcastScriptTags();
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

bool BattleHost::hasMap(QString mapname) {
    return UnitSyncLib::getInstance()->mapChecksum(mapname);
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

void BattleHost::fillScriptTags() {
    UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();
    if (!unitSyncLib->setCurrentMod(m_mod)) {
        qDebug() << "Failed to load " << m_mod;
        return;
    }
    //Mod options
    int num_options = unitSyncLib->getModOptionCount();
    for (int i = 0; i < num_options; i++) {
        if (unitSyncLib->getOptionType(i) == SECTION)
            continue;
        switch (unitSyncLib->getOptionType(i)) {
        case BOOLEAN:
            m_scriptTags[unitSyncLib->getOptionKey(i)] = unitSyncLib->getOptionBoolDef(i) ? "1" : "0";
            break;
        case LIST:
            m_scriptTags[unitSyncLib->getOptionKey(i)] = unitSyncLib->getOptionListDef(i);
            break;
        case FLOAT:
            m_scriptTags[unitSyncLib->getOptionKey(i)] = QString::number(unitSyncLib->getOptionNumberDef(i));
            break;
        case STRING:
            m_scriptTags[unitSyncLib->getOptionKey(i)] = unitSyncLib->getOptionStringDef(i);
            break;
        case SECTION:
        case UNDEFINED:
            break;
        }
    }
    //Game options
    m_scriptTags["diminishingmms"] = "0";
    m_scriptTags["disablemapdamage"] = "0";
    m_scriptTags["fixedallies"] = "1";
    m_scriptTags["ghostedbuildings"] = "1";
    m_scriptTags["limitdgun"] = "0";
    m_scriptTags["deathmode"] = "0";
    m_scriptTags["gamemode"] = "0";
    m_scriptTags["startpostype"] = "0";
}

void BattleHost::broadcastScriptTags() {
    //SETSCRIPTTAGS GAME/StartMetal=1000[TAB]GAME/StartEnergy=1000
    UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();
    QString command = "SETSCRIPTTAGS ";
    for(QMap<QString,QString>::const_iterator i = m_scriptTags.begin(); i != m_scriptTags.end(); i++) {
        if(unitSyncLib->isGameOption(i.key())) {
            command += "game/" + i.key() + "=" + i.value() + "\t";
        } else {
            command += "game/modoptions/" + i.key() + "=" + i.value() + "\t";
        }
    }
    command.chop(1);
    qDebug() << command;
    emit sendCommand(Command(command));
}

QStringList BattleHost::getScriptTagKeys() {
    return m_scriptTags.values();
}

bool BattleHost::isScriptTagValueValid(QString key, QString value) {
    if(key == "diminishingmms" ||
       key == "fixedallies" ||
       key == "disablemapdamage" ||
       key == "ghostedbuildings" ||
       key == "limitdgun"
       )
        return value == "0" || value == "1";
    else if(key ==  "startpostype")
        return value == "0" || value == "1" || value == "2" || value == "3";
    else if(key ==  "startpostype" || key == "gamemode")
        return value == "0" || value == "1" || value == "2";
    UnitSyncLib* unitSyncLib = UnitSyncLib::getInstance();
    int num_options = unitSyncLib->getModOptionCount();
    for (int i = 0; i < num_options; i++) {
        if (unitSyncLib->getOptionKey(i) == key) {
            float min;
            float max;
            float step;
            float val;
            switch (unitSyncLib->getOptionType(i)) {
            case BOOLEAN:
                return value == "0" || value == "1";
            case LIST:
                qDebug() << unitSyncLib->getOptionListItems(i);
                qDebug() << value;
                return unitSyncLib->getOptionListItems(i).contains(value);
            case FLOAT:
                min = unitSyncLib->getOptionNumberMin(i);
                max = unitSyncLib->getOptionNumberMax(i);
                step = unitSyncLib->getOptionNumberStep(i);
                val = value.toFloat();
                //FIXME needs some improvements concerning step checking
                //this check phails
                return abs(val - qRound(val/step)*step) < 1e-9 && (val >= min && val <= max);
            case STRING:
                return value.length() <= unitSyncLib->getOptionStringMaxLen(i);
            case SECTION:
            case UNDEFINED:
                break;
            }
            break;
        }
    }
    return false;
}

void BattleHost::setScriptTag(QString key, QString value) {
    m_scriptTags[key] = value;
    broadcastScriptTags();
}
