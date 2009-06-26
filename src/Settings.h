//
// C++ Interface: Settings
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

/**
	@author Joachim Schiele <js@lastlog.de>
*/

/*
** can be used this way:
  QSettings* settings = Settings::Instance();
  settings->setValue("abc", 500);
  qDebug() << settings->value("abc").toInt();
*/

class Settings : public QObject
{
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
