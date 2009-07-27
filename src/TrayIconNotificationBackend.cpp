#include "TrayIconNotificationBackend.h"

TrayIconNotificationBackend::TrayIconNotificationBackend(QSystemTrayIcon* trayIcon)
{
    if(trayIcon->isSystemTrayAvailable())
    {
        systemTrayIcon = trayIcon;
    }
    else
    {
        // TODO: report error to fall back
    }
}

void TrayIconNotificationBackend::showMessage(QString& title, QString& message, int timeout_ms)
{

}

void TrayIconNotificationBackend::showMessage(QString& title, QString& message, QString& icon_path, int timeout_ms)
{
    QSystemTrayIcon::MessageIcon messageIcon = QSystemTrayIcon::Information;

    systemTrayIcon->showMessage(title, message, messageIcon, timeout_ms);
}
