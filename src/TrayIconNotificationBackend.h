#ifndef TRAYICONNOTIFICATIONBACKEND_H
#define TRAYICONNOTIFICATIONBACKEND_H

#include "AbstractNotificationBackend.h"

#include <QObject>
#include <QSystemTrayIcon>

class TrayIconNotificationBackend : public AbstractNotificationBackend
{
public:
    TrayIconNotificationBackend(QSystemTrayIcon* trayIcon);

    void showMessage(QString& title, QString& message, int timeout_ms = -2);

    void showMessage(QString& title, QString& message, QString& icon_path, int timeout_ms = -2);

    QSystemTrayIcon* systemTrayIcon;
};

#endif // TRAYICONNOTIFICATIONBACKEND_H
