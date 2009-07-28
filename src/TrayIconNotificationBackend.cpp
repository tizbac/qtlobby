#include "TrayIconNotificationBackend.h"

TrayIconNotificationBackend::TrayIconNotificationBackend(QSystemTrayIcon* trayIcon)
{
    if(trayIcon->isSystemTrayAvailable())
    {
        SystemTrayIcon = trayIcon;
    }
    else
    {
        // TODO: report error to fall back
    }
}

void TrayIconNotificationBackend::showMessage(QString& title, QString& message, int timeout_ms)
{
    if(timeout_ms == -2)
    {
        timeout_ms = (title.length() + message.length()) * 100 + 500; //500 extra for icon
    }
    QSystemTrayIcon::MessageIcon messageIcon = QSystemTrayIcon::Information;

    SystemTrayIcon->showMessage(title, message, messageIcon, timeout_ms);
}

void TrayIconNotificationBackend::showMessage(QString& title, QString& message, QString& icon_path, int timeout_ms)
{
    // TODO: parse icon_path
    showMessage(title, message, timeout_ms);
}
