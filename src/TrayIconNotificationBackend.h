#ifndef TRAYICONNOTIFICATIONBACKEND_H
#define TRAYICONNOTIFICATIONBACKEND_H

#include "AbstractNotificationBackend.h"

#include <QObject>
#include <QSystemTrayIcon>

class TrayIconNotificationBackend : public AbstractNotificationBackend
{
public:
    bool isUsable() const;
    void showMessage(QString& title, QString& message, int timeout_ms = -2);
    /**
      * Shows the system tray icon bubble message.
      * icon_path icon possibilities: Information, Warning, Critical and empty for no icon
      **/
    void showMessage(QString& title, QString& message, QString& icon_path, int timeout_ms = -2);

    QSystemTrayIcon* SystemTrayIcon;
};

#endif // TRAYICONNOTIFICATIONBACKEND_H
