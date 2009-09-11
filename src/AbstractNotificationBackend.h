#ifndef ABSTRACTNOTIFICATIONBACKEND_H
#define ABSTRACTNOTIFICATIONBACKEND_H

#include <QThread>
#include <QString>
#include <QDebug>

class AbstractNotificationBackend : public QThread
{
    Q_OBJECT
public:
    AbstractNotificationBackend();
    bool isUsable() const;
    QString applicationName;
public slots:

    // Uses the application name for the title
    virtual void showMessage(QString message);
    virtual void showMessage(QString title, QString message, int timeout_ms = -2) = 0;
    // timeout in milliseconds, 0 displays the message endless, -1 is system based, -2 is message length based
    virtual void showMessage(QString title, QString message, QString icon_path, int timeout_ms = -2) = 0;
    // TODO: display and hide window with actions
protected:
    virtual void run();
};

inline AbstractNotificationBackend::AbstractNotificationBackend() : QThread(0) {}

inline void AbstractNotificationBackend::run() { qDebug() << "NotificationBackend thread started"; exec(); }

inline void AbstractNotificationBackend::showMessage(QString message)
{
    if(applicationName.isEmpty())
    {
        QString app_name = QString("Application");
        showMessage(app_name, message, -2);
    }
    else
    {
        showMessage(applicationName, message, -2);
    }
}

#endif // ABSTRACTNOTIFICATIONBACKEND_H
