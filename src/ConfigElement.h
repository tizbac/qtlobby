//
// C++ Interface:
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef CONFIGELEMENT_H
#define CONFIGELEMENT_H

#include <QWidget>
#include <QStringList>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>

#include "Settings.h"
#include "ui_configElement.h"

class ConfigElement : public QWidget, public Ui::configElement
{
  Q_OBJECT
public:
  ConfigElement( QStringList config, QWidget* parent = 0 );
  ~ConfigElement();

protected:
  QStringList m_config;
  QSettings* settings;
  bool exists;
  bool isDirectory;
  QStringList examples;

protected slots:
  void SaveElement();
  void ResetConfiguration();
  void openFileBrowser();
  void updateExistingState( QString a = "" );
};

#endif

