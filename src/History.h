#ifndef HISTORY_H
#define HISTORY_H

#include <QThread>
#include <QtSql>
#include <QSet>
#include <QTimerEvent>
#include "Singleton.h"

class History : public QThread, public Singleton<History> {
    Q_OBJECT
    friend class Singleton<History>;
public:
    void setEnabled(bool b);
public slots:
    void play(QDate from, QDate to);
    void receiveMessage(QString message);
signals:
    void historyMessage(QDateTime timestamp, QString message);
    void finished();
protected:
    virtual void run();
    void timerEvent(QTimerEvent* event);
private:
    QSqlDatabase m_db;
    QSqlQuery m_insertQuery;
    QSqlQuery m_fetchQuery;
    bool m_initialized;
    QSet<QString> m_filter;
    bool m_enabled;
    int m_requestsNumber;
    QDate m_from;
    QDate m_to;
    bool m_collecting;

    History();
    void initialize();
};

inline void History::setEnabled(bool b) {
    m_enabled = b;
}

#endif // HISTORY_H
