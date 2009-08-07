// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QDialog>
#include <QSystemTrayIcon>
#include <QScriptEngine>

#include "ui_MainWidget.h"
#include "ui_AboutWidget.h"
#include "Settings.h"
#include "ConnectionWidget.h"
#include "CommandAssigner.h"
#include "Command.h"
#include "LobbyTabs.h"
#include "Users.h"
#include "Battles.h"
#include "InputLine.h"
#include "AudioBackend.h"
#include "UnitSyncLib.h"
#include "Preference.h"
//#include "MapSelector.h"
#include "StylesheetDialog.h"
#include "UserGroupsDialog.h"
#include "ScriptingDialog.h"
#include "BattleHost.h"
#include "BattleHostingDialog.h"
#include "DownloadsDialog.h"
#include "Notification.h"
#ifdef Q_WS_X11
    #include "DBusVisualNotificationBackend.h"
#else
    #include "TrayIconNotificationBackend.h"
#endif


class MainWindow : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT
public:
    MainWindow( QWidget* parent = 0 );
    ~MainWindow();
    QProcess qpSpring;
    QProcess qpSpringSetting;

private:
    void setupToolbar();
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
//    MapSelector* mapSelector;
    StylesheetDialog* stylesheetDialog;
    UserGroupsDialog* userGroupsDialog;
    ScriptingDialog* scriptingDialog;
    BattleHostingDialog *battleHostingDialog;
    DownloadsDialog* downloadsDialog;

    QTabBar* tabBar;
    QToolButton* newTabButton;
    QLabel* battlesOnline;
    QLabel* usersOnline;
    QLabel* moderatorsOnline;
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
    void on_actionDownloads_triggered();
    void on_hostPushButton_clicked();
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
    void onCurrentTabChanged();
    void onTeamPlayerSpecCountChanged(QString ratio);
    void onStatsChange(int users, int moderators);
    void onStatsChange(int battles);
    void connectionStatusChanged(ConnectionState);
    void onBlockInput(bool b);
    void onJoinRequested();

    void onChangedToBattleTab();
    void onChangedFromBattleTab();
public slots:
    void playSample( SampleCollection sample );
    void sendTrayMessage( QString message, int milliseconds = 2000);
};

#endif
