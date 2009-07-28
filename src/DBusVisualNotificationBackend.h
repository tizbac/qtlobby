#ifndef KNOTIFYBACKEND_H
#define KNOTIFYBACKEND_H

#include <QObject>
#include <QtDBus>

#include "AbstractNotificationBackend.h"

class DBusVisualNotificationBackend : public AbstractNotificationBackend
{
public:
    void showMessage(QString& title, QString& message, int timeout_ms = -2);
    void showMessage(QString& title, QString& message, QString& icon_path, int timeout_ms = -2);
private:
    QDBusInterface* m_notify;
};

#endif // KNOTIFYBACKEND_H
