#ifndef TRAYICONNOTIFICATIONBACKEND_H
#define TRAYICONNOTIFICATIONBACKEND_H

#include "AbstractNotificationBackend.h"

#include <QObject>
#include <QSystemTrayIcon>

class TrayIconNotificationBackend : public AbstractNotificationBackend
{
    Q_OBJECT
public:
    bool isUsable() const;

    QSystemTrayIcon* SystemTrayIcon;
public slots:
    void showMessage(QString title, QString message, int timeout_ms = -2);
    /**
      * Shows the system tray icon bubble message.
      * icon_path icon possibilities: Information, Warning, Critical and empty for no icon
      **/
    void showMessage(QString title, QString message, QString icon_path, int timeout_ms = -2);

};

#endif // TRAYICONNOTIFICATIONBACKEND_H
