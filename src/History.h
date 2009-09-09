#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>
#include <QtSql>
#include <QSet>
#include "Singleton.h"

class History : public QObject, public Singleton<History> {
    Q_OBJECT
    friend class Singleton<History>;
public:
    void play(QDate from, QDate to);
public slots:
    void receiveMessage(QString message);
signals:
    void historyMessage(QDateTime timestamp, QString message);
private:
    QSqlDatabase m_db;
    QSqlQuery m_insertQuery;
    QSqlQuery m_fetchQuery;
    bool m_initialized;
    QSet<QString> m_filter;

    History(QObject* parent = 0);
    void initialize();
};

#endif // HISTORY_H