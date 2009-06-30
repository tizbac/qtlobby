//
// C++ Interface: MainWindow
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDialog>
#include <QSystemTrayIcon>
// #include <QTime>

#include "ui_mainWidget.h"
#include "ui_aboutWidget.h"
#include "Settings.h"
#include "ConnectionWidget.h"
#include "CommandAssigner.h"
#include "StatusTracker.h"
#include "Command.h"
#include "LobbyTabs.h"
#include "Users.h"
#include "Battles.h"
#include "InputLine.h"
#include "AudioBackend.h"
#include "UnitSyncLib.h"
#include "Preference.h"
#include "MapSelector.h"
#include "StylesheetDialog.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
  Q_OBJECT
public:
  MainWindow( QWidget* parent = 0 );
  ~MainWindow();
  QProcess qp;

private:
  //the connection dialog
  ConnectionWidget* connectionWidget;
  //handles the interaction with the server and the authentication
  ServerContextState* serverContextState;
  //receives all commands, parses and assigns it to its destination module
  CommandAssigner* commandAssigner;
  //the tabs at the right top, which contain the chat etc.
  LobbyTabs* lobbyTabs;

  AudioBackend audioBackend;
  UnitSyncLib* unitSyncLib;
  UserPreference* preference;
  MapSelector* mapSelector;
  StylesheetDialog* stylesheetDialog;

  //the status bar text
  StatusTracker* statusTracker;

  QSystemTrayIcon* trayIcon;
  void createTrayIcon();
  QMenu* trayIconMenu;
  QAction* restoreAction;
  QColor regexpColor;
  QSettings* settings;
signals:
  void newTrayMessage(QString);
protected:
  void closeEvent(QCloseEvent *event);
private slots:
  void about();
  void newUserTextInput();
  void toggleUserListVisible();
  void toggleShowHideMainWindow( QSystemTrayIcon::ActivationReason );
  void showConnectionWidget( bool );
  void setColorInducatorUsers( QString regExp );
  void setColorInducatorBattles( QString regExp );
  void startSpring();
  void startSpringSettings();
  void hideBattleList( bool isBattleTab );
  void showStylesheetEditor();
public slots:
  void playSample( SampleCollection sample );
  void sendTrayMessage( QString message, int milliseconds = 2000);
};

#endif
