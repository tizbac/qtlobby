#include "History.h"
#include "PathManager.h"

History::History() : QThread(0) {
    m_initialized = false;
    m_filter << "SAYPRIVATE"
            << "SAIDPRIVATE"
            //<< "SAIDBATTLE"
            //<< "SAIDBATTLEEX"
            << "SAID"
            << "SAIDEX"
            //<< "JOINEDBATTLE"
            //<< "LEFTBATTLE"
            << "JOINED"
            << "LEFT"
            ;
    initialize();
    start();
    moveToThread(this);
}

void History::run() {
    qDebug() << "History thread started";
    exec();
}

void History::initialize() {
    if(m_initialized) return;
    if(!PathManager::getInstance()->isOverlayValid()) return;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(PathManager::getInstance()->resolveOverlayPath("history.sqlite"));
    m_initialized = m_db.open();
    if(!m_initialized) return;
    QSqlQuery q;
    q.exec("create table history(timestamp integer, command text)");
    qDebug() << q.lastError().text();
    m_insertQuery = QSqlQuery();
    m_fetchQuery = QSqlQuery();
    m_insertQuery.prepare("insert into history values(?, ?)");
    m_fetchQuery.prepare("select * from history where timestamp>=? and timestamp<=?");
}

void History::receiveMessage(QString message) {
    if(!m_enabled) return;
    initialize();
    if(!m_initialized) return;
    if(!m_filter.contains(message.split(" ").takeFirst())) return;
    m_insertQuery.bindValue(0, QDateTime::currentDateTime().toTime_t());
    m_insertQuery.bindValue(1, message);
    m_insertQuery.exec();
}

void History::play(QDate from, QDate to) {
    if(!m_enabled) return;
    initialize();
    if(!m_initialized) return;
    m_fetchQuery.bindValue(0, QDateTime(from).toTime_t());
    m_fetchQuery.bindValue(1, QDateTime(to).toTime_t());
    m_fetchQuery.exec();
    while(m_fetchQuery.next()) {
        emit historyMessage(QDateTime::fromTime_t(m_fetchQuery.value(0).toUInt()), m_fetchQuery.value(1).toString());
    }
    disconnect(this, SIGNAL(historyMessage(QDateTime,QString)), 0, 0);
    emit finished();
    disconnect(this, SIGNAL(finished()), 0, 0);
}
