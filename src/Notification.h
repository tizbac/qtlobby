#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "Singleton.h"

#include "AbstractNotificationBackend.h"

#include <QtCore>
#include <QtDBus>
#include <QString>
#include <QSystemTrayIcon>

class Notification : public Singleton<Notification>
{
public:
    AbstractNotificationBackend* NotificationBackend;

    void showMessage(QString& title, QString& message, int timeout_ms = -2);

    // timeout in milliseconds, 0 displays the message endless, -1 is system based, -2 is message length based
    void showMessage(QString& title, QString& message, QString& icon_path, int timeout_ms = -2);

    // TODO: Error handling
private:
};

#endif // NOTIFICATION_H
