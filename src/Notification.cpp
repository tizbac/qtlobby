#include "Notification.h"

void Notification::showMessage(QString title, QString message, int timeout_ms)
{
        emit showMessageSignal(title, message, timeout_ms);
}

void Notification::showMessage(QString title, QString message, QString icon_path, int timeout_ms)
{
        emit showMessageSignal(title, message, icon_path, timeout_ms);
}
