#ifndef TRAYICONNOTIFICATIONBACKEND_H
#define TRAYICONNOTIFICATIONBACKEND_H

#include "AbstractNotificationBackend.h"

#include <QObject>
#include <QSystemTrayIcon>

class TrayIconNotificationBackend : public AbstractNotificationBackend
{
public:
    void showMessage(QString& title, QString& message, int timeout_ms = -2);

    void showMessage(QString& title, QString& message, QString& icon_path, int timeout_ms = -2);

    QSystemTrayIcon* SystemTrayIcon;
};

#endif // TRAYICONNOTIFICATIONBACKEND_H
