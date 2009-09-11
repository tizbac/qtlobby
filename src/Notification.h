#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "Singleton.h"

#include "AbstractNotificationBackend.h"

#include <QString>

class Notification : public QObject, public Singleton<Notification>
{
    Q_OBJECT
public:
    AbstractNotificationBackend* NotificationBackend;

    void showMessage(QString title, QString message, int timeout_ms = -2);

    // timeout in milliseconds, 0 displays the message endless, -1 is system based, -2 is message length based
    void showMessage(QString title, QString message, QString icon_path, int timeout_ms = -2);
signals:
    void showMessageSignal(QString title, QString message, int timeout_ms);
    void showMessageSignal(QString title, QString message, QString icon_path, int timeout_ms);
    // TODO: Error handling
};

#endif // NOTIFICATION_H
