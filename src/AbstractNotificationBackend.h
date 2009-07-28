#ifndef ABSTRACTNOTIFICATIONBACKEND_H
#define ABSTRACTNOTIFICATIONBACKEND_H

#include <QObject>
#include <QString>

class AbstractNotificationBackend : public QObject
{
public:
    bool isUsable() const;
    QString applicationName;

    // Uses the application name for the title
    virtual void showMessage(QString& message)
    {
        if(applicationName == NULL)
        {
            QString app_name = QString("Application");
            showMessage(app_name, message, -2);
        }
        else
        {
            showMessage(applicationName, message, -2);
        }
    }
    virtual void showMessage(QString& title, QString& message, int timeout_ms = -2);
    // timeout in milliseconds, 0 displays the message endless, -1 is system based, -2 is message length based
    virtual void showMessage(QString& title, QString& message, QString& icon_path, int timeout_ms = -2);
    // TODO: display and hide window with actions
};
#endif // ABSTRACTNOTIFICATIONBACKEND_H
