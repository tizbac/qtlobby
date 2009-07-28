#include "Notification.h"

void Notification::showMessage(QString& title, QString& message, int timeout_ms)
{
    if(NotificationBackend == NULL)
    {
        // TODO: error handling for none notification backend
    }
    else
        NotificationBackend->showMessage(title, message, timeout_ms);
}

void Notification::showMessage(QString& title, QString& message, QString& icon_path, int timeout_ms)
{
    if(NotificationBackend == NULL)
    {
        // TODO: error handling for none notification backend
    }
    else
        NotificationBackend->showMessage(title, message, icon_path, timeout_ms);
}
