#ifndef HISTORY_H
#define HISTORY_H

#include <QThread>
#include <QtSql>
#include <QSet>
#include "Singleton.h"

class History : public QThread, public Singleton<History> {
    Q_OBJECT
    friend class Singleton<History>;
public slots:
    void play(QDate from, QDate to);
    void receiveMessage(QString message);
signals:
    void historyMessage(QDateTime timestamp, QString message);
protected:
    virtual void run();
private:
    QSqlDatabase m_db;
    QSqlQuery m_insertQuery;
    QSqlQuery m_fetchQuery;
    bool m_initialized;
    QSet<QString> m_filter;

    History();
    void initialize();
};

#endif // HISTORY_H
