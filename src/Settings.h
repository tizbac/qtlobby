// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

/**
 * Settings to be stored and recalled.
 * Can be used this way:
 * QSettings* settings = Settings::Instance();
 * settings->setValue("category/settingname", 500);
 * qDebug() << settings->value("category/settingname").toInt();
 * or 
 * Settings::Instance()->value("category/settingname").toInt();
 */
class Settings : public QObject {
    Q_OBJECT
public:
    static QSettings *Instance();
    ~Settings();
private:
    Settings(QObject *parent = 0);
    Settings(const Settings&);
    Settings& operator= (const Settings&);
};

#endif
