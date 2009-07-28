#include "TrayIconNotificationBackend.h"

bool TrayIconNotificationBackend::isUsable() const
{
    if(SystemTrayIcon == NULL)
        return false;
    else
        return SystemTrayIcon->isSystemTrayAvailable();
}

void TrayIconNotificationBackend::showMessage(QString& title, QString& message, int timeout_ms)
{
    QString icon_path = QString("");
    showMessage(title, message, icon_path, timeout_ms);
}

void TrayIconNotificationBackend::showMessage(QString& title, QString& message, QString& icon_path, int timeout_ms)
{
    QSystemTrayIcon::MessageIcon icon;

    if(icon_path == "Information")
        icon = QSystemTrayIcon::Information;
    else if (icon_path == "Warning")
        icon = QSystemTrayIcon::Warning;
    else if (icon_path == "Critical")
        icon = QSystemTrayIcon::Critical;
    else
        icon = QSystemTrayIcon::NoIcon;

    if(timeout_ms == -2)
    {
        timeout_ms = (title.length() + message.length()) * 100 + 500; //500 extra for icon
    }
    QSystemTrayIcon::MessageIcon messageIcon = QSystemTrayIcon::Information;

    SystemTrayIcon->showMessage(title, message, messageIcon, timeout_ms);
}
