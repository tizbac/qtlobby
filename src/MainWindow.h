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
#include <QScriptEngine>

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
#include "UserGroupsDialog.h"
#include "ScriptingDialog.h"

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
    UserGroupsDialog* userGroupsDialog;
    ScriptingDialog* scriptingDialog;

    QTabBar* tabBar;
    QLabel* usersInCurrentChannel;
    QLabel* battlesOnline;
    QLabel* usersOnline;
    QLabel* moderatorsOnline;

    //the status bar text
    StatusTracker* statusTracker;

    QSystemTrayIcon* trayIcon;
    void createTrayIcon();
    QMenu* trayIconMenu;
    QAction* restoreAction;
    QColor regexpColor;
    QSettings* settings;
    bool inBattle;
    QByteArray lastState;
    QByteArray lastBattleState;
    QByteArray state;
    QScriptEngine scriptingEngine;
signals:
    void newTrayMessage(QString);
protected:
    void closeEvent(QCloseEvent *event);
    void hideEvent(QHideEvent * event);
    void showEvent(QShowEvent * event);
private slots:
    void on_actionScripting_triggered();
    void about();
    void newUserTextInput();
    void toggleUserListVisible();
    void toggleShowHideMainWindow( QSystemTrayIcon::ActivationReason );
    void showConnectionWidget( bool );
    void showGroupsDialog();
    void setColorInducatorUsers( QString regExp );
    void setColorInducatorBattles( QString regExp );
    void startSpring();
    void startSpringSettings();
    void showStylesheetEditor();
    void setCurrentUsername( QString username );
    void onCurrentChanged(const QModelIndex & current, const QModelIndex & previous);
    void onStatsChange(int users, int moderators);
    void onStatsChange(int battles);

    void onChangedToBattleTab();
    void onChangedFromBattleTab();
public slots:
    void playSample( SampleCollection sample );
    void sendTrayMessage( QString message, int milliseconds = 2000);
};

#endif
