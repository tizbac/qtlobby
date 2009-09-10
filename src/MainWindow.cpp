// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "MainWindow.h"
#include "ToolBarWidget.h"
#include "PathManager.h"
#include "ServerProfilesModel.h"
#include "History.h"
#include <QInputDialog>
#include <QDir>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ) {
    //   QTime *timer = new QTime();
    //   timer->start();
    //   setAttribute(Qt::WA_DeleteOnClose);

    setUnifiedTitleAndToolBarOnMac( true );
    settings = Settings::Instance();
    /* it's important to first init the preferences */
    preference          = new Preference();
    preference->setWindowFlags( Qt::Window );
    if ( settings->value("unitsync").toString().isEmpty() )
        preference->exec();
    QDir overlay(settings->value("spring_user_dir").toString());
    if(overlay.exists()) {
        overlay.mkdir("qtlobby");
        overlay.cd("qtlobby");
        PathManager::getInstance()->setOverlayPath(overlay.absolutePath());
    }
    setupUi( this );
    setupIcons();
    preference->onResetFormToSettings();
    battles->setUsers( users );
    setWindowTitle(QString( "QtLobby v%1" ).arg( QTLOBBY_VERSION ));
    serverContextState  = new ServerContextState( this );
    connectionWidget    = new ConnectionWidget( serverContextState );
    connectionWidget->setWindowFlags(Qt::Window);
    setupToolbar();
    lobbyTabs           = new LobbyTabs( this, battles, UnitSyncLib::getInstance(), tabBar, lobbyStackedWidget );
    commandAssigner     = new CommandAssigner( this );
    //statusTracker       = new StatusTracker( statusbar );
    //    mapSelector         = new MapSelector();
    stylesheetDialog    = new StylesheetDialog();
    stylesheetDialog->setWindowFlags(Qt::Window);
    userGroupsDialog    = new UserGroupsDialog();
    scriptingDialog     = new ScriptingDialog(&scriptingEngine, this);
    scriptingDialog->setWindowFlags(Qt::Window);
    battleHostingDialog = new BattleHostingDialog(&qpSpring, commandAssigner, lobbyTabs, this);
    battleHostingDialog->setWindowFlags(Qt::Window);
    downloadsDialog     = new DownloadsDialog(this);
    downloadsDialog->setWindowFlags(Qt::Window);
    historyDialog       = new HistoryDialog(this);
    historyDialog->setWindowFlags(Qt::Window);

    scriptingEngine.globalObject().setProperty("battles", scriptingEngine.newQObject(battles));
    scriptingEngine.globalObject().setProperty("users", scriptingEngine.newQObject(users));
    scriptingEngine.globalObject().setProperty("lobbyTabs", scriptingEngine.newQObject(lobbyTabs));
    scriptingEngine.globalObject().setProperty("serverContextState", scriptingEngine.newQObject(serverContextState));
    scriptingEngine.setProcessEventsInterval(500);

    battlesOnline = new QLabel("Battles online: 0");
    usersOnline = new QLabel("Users online: 0");
    moderatorsOnline = new QLabel("Moderators online: 0");
    statusBar()->addPermanentWidget(moderatorsOnline);
    statusBar()->addPermanentWidget(usersOnline);
    statusBar()->addPermanentWidget(battlesOnline);
    statusBar()->addPermanentWidget(new QLabel());
    createTrayIcon();
    trayIcon->show();

    Notification* notify = Notification::getInstance();
#ifdef Q_WS_X11
    notify->NotificationBackend = new DBusVisualNotificationBackend();
#else // Use system tray icon on none x11 systems
    TrayIconNotificationBackend* trayIconBackend = new TrayIconNotificationBackend();
    trayIconBackend->SystemTrayIcon = trayIcon;
    notify->NotificationBackend = trayIconBackend;
#endif
    notify->NotificationBackend->applicationName = "QtLobby";

    regexpColor = QColor( 0xFFE4B5 );

    /** incoming data **/
    // serverContextState -> QString -> commandAssigner
    connect( serverContextState, SIGNAL( incommingMessage( QString ) ),
             commandAssigner, SLOT( receiveMessage( QString ) ) );
    connect( serverContextState, SIGNAL( incommingMessage( QString ) ),
             History::getInstance(), SLOT( receiveMessage( QString ) ) );
    // commandAssigner -> Command -> serverContextState
    connect( commandAssigner, SIGNAL( serverContextStateCommand( Command ) ),
             serverContextState, SLOT( receiveCommand( Command ) ) );
    connect( commandAssigner, SIGNAL( lobbyTabCommand( Command ) ),
             lobbyTabs, SLOT( receiveCommand( Command ) ) );
    connect( commandAssigner, SIGNAL( userCommand( Command ) ),
             users, SLOT( receiveCommand( Command ) ) );
    connect( commandAssigner, SIGNAL( battleCommand( Command ) ),
             battles, SLOT( receiveCommand( Command ) ) );

    /** outgoing data **/
    // commandAssigner -> QString -> serverContextState
    connect( commandAssigner, SIGNAL( sendMessage( QString ) ),
             serverContextState, SLOT( sendMessage( QString ) ) );
    // lobbyTabs -> Command -> commandAssigner
    connect( lobbyTabs, SIGNAL( sendCommand( Command ) ),
             commandAssigner, SLOT( sendCommand( Command ) ) );
    connect( battles, SIGNAL( sendCommand( Command ) ),
             commandAssigner, SLOT( sendCommand( Command ) ) );
    connect( users, SIGNAL( sendCommand( Command ) ),
             commandAssigner, SLOT( sendCommand( Command ) ) );

    /** other connections **/
    //Maps/mods reload
    connect(actionReload_maps_and_mods, SIGNAL(triggered()),
            UnitSyncLib::getInstance(), SLOT(reboot()));
    // connectionWidget
    connect( action_Connect, SIGNAL( triggered() ),
             connectionWidget, SLOT( show() ) );
    connect( actionStart_Spring, SIGNAL( triggered() ),
             this, SLOT( startSpring() ) );
    connect( action_Spring_Settings, SIGNAL( triggered() ),
             this, SLOT( startSpringSettings() ) );

    connect( serverContextState, SIGNAL( connectionStateChanged( ConnectionState ) ),
             connectionWidget, SLOT( connectionStatusChanged( ConnectionState ) ) );

    connect( serverContextState, SIGNAL( connectionStateChanged( ConnectionState ) ),
             this, SLOT( connectionStatusChanged( ConnectionState ) ) );

    // disconnect
    connect( action_Disconnect, SIGNAL( triggered() ),
             serverContextState, SLOT( forceDisconnect() ) );
    // chat menue
    connect( actionLeave, SIGNAL( triggered() ),
             lobbyTabs, SLOT( closeTab() ) );
    // lobbyTabs to open the default channel, when connected
    connect( serverContextState, SIGNAL( connectionStateChanged( ConnectionState ) ),
             lobbyTabs, SLOT( connectionStateChanged( ConnectionState ) ) );
    // users to reset stats
    connect( serverContextState, SIGNAL( connectionStateChanged( ConnectionState ) ),
             users, SLOT( connectionStateChanged( ConnectionState ) ) );
    // battles to reset stats
    connect( serverContextState, SIGNAL( connectionStateChanged( ConnectionState ) ),
             battles, SLOT( connectionStateChanged( ConnectionState ) ) );
    // lobbyTabs to open private channel, when requested in users or battles list
    connect( users, SIGNAL( sendInput( QString ) ),
             lobbyTabs, SLOT( receiveInput( QString ) ) );
    connect( users, SIGNAL( sendInputAndFocus( QString ) ),
             lobbyTabs, SLOT( receiveInputAndFocus( QString ) ) );
    connect( battleInfoTreeView, SIGNAL( sendInputAndFocus( QString ) ),
             lobbyTabs, SLOT( receiveInputAndFocus( QString ) ) );
    connect( battles, SIGNAL( sendInput( QString ) ),
             lobbyTabs, SLOT( receiveInput( QString ) ) );
    connect( users, SIGNAL(teamPlayerSpecCountChanged(QString)),
             this, SLOT(onTeamPlayerSpecCountChanged(QString)));
    //Stats updates for status bar
    connect( users, SIGNAL(statsChange(int,int)),
             this, SLOT(onStatsChange(int,int)));
    connect( users, SIGNAL(statsChange(int,int)),
             this, SLOT(onCurrentTabChanged()));
    connect( battles, SIGNAL(statsChange(int)),
             this, SLOT(onStatsChange(int)));

    // users, for the userlist to show
    connect( lobbyTabs, SIGNAL( currentTabChanged( QString, QString ) ),
             users, SLOT( currentTabChanged( QString, QString ) ) );
    connect( lobbyTabs, SIGNAL(changedToBattleTab()),
             this, SLOT( onChangedToBattleTab()) );
    connect( lobbyTabs, SIGNAL(changedFromBattleTab()),
             this, SLOT( onChangedFromBattleTab()) );
    connect( tabBar, SIGNAL(tabMoved(int,int)),
             lobbyTabs, SLOT(onTabMoved(int,int)));
    connect( tabBar, SIGNAL(currentChanged(int)),
             this, SLOT(onCurrentTabChanged()));
    closeTab = new QShortcut(QKeySequence(QKeySequence::Close), this);
    connect( closeTab, SIGNAL( activated() ),
             lobbyTabs, SLOT( closeTab() ) );

    // aboutDialog
    connect( action_About, SIGNAL( triggered() ),
             this, SLOT( about() ) );
    connect( action_Host_a_Battle, SIGNAL( triggered() ),
             this, SLOT( on_hostPushButton_clicked() ) );
    connect( joinPushButton, SIGNAL( clicked( bool ) ),
             battles, SLOT( onJoin() ) );
    connect( battles, SIGNAL( battleSelected( bool ) ),
             joinPushButton, SLOT( setEnabled( bool ) ) );
    // preferences
    connect( actionPreferences, SIGNAL( triggered() ),
             preference, SLOT( show() ) );
    connect( actionPreferences, SIGNAL( triggered() ),
             preference, SLOT( onResetFormToSettings() ) );
    // maplist
    //    connect( actionMap_list, SIGNAL( triggered() ),
    //             mapSelector, SLOT( show() ) );

    // inputLine
    inputLineEdit->setUsers(users);

    connect( inputLineEdit, SIGNAL( sendInput( QString ) ),
             lobbyTabs, SLOT( receiveInput( QString ) ) );
    connect (tabBar, SIGNAL(currentChanged(int)),
             inputLineEdit, SLOT(onTabChanged(int)));

    // regular expresion line edits
    connect( userListLineEdit, SIGNAL( textChanged( QString ) ),
             users, SLOT( setRegExp( QString ) ) );
    connect( battleListLineEdit, SIGNAL( textChanged( QString ) ),
             battles, SLOT( setRegExp( QString ) ) );
    connect( userListLineEdit, SIGNAL( textChanged( QString ) ),
             this, SLOT( setColorInducatorUsers( QString ) ) );
    connect( battleListLineEdit, SIGNAL( textChanged( QString ) ),
             this, SLOT( setColorInducatorBattles( QString ) ) );
    battleListLineEdit->setText(settings->value("battleListFilterString").toString());
    userListLineEdit->setText(settings->value("userListFilterString").toString());
    // tray icon stuff
#ifndef Q_WS_MAC
    connect( trayIcon, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ),
             this, SLOT( toggleShowHideMainWindow( QSystemTrayIcon::ActivationReason ) ) );
#endif
    connect( this, SIGNAL( newTrayMessage( QString ) ),
             this, SLOT( sendTrayMessage( QString ) ) );
    connect( battles, SIGNAL( start() ),
             this, SLOT( startSpring() ) );
    connect( battles->battleCloseFirstWidget, SIGNAL( wantHostBattle( bool ) ),
             this, SLOT( on_hostPushButton_clicked( bool ) ) );
    //Groups dialog
    connect(userGroupsDialog, SIGNAL(groupsChanged()), battles, SLOT(invalidateModel()));
    connect(userGroupsDialog, SIGNAL(groupsChanged()), users, SLOT(invalidateModel()));
    //    connect(userGroupsDialog, SIGNAL(groupsChanged()), battles, SLOT(reset()));
    //    connect(userGroupsDialog, SIGNAL(groupsChanged()), users, SLOT(reset()));

    //Battle info view
    connect(battles->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(onCurrentChanged(QModelIndex,QModelIndex)));

    //input blocking signal
    connect(lobbyTabs, SIGNAL(blockInput(bool)),
            this, SLOT(onBlockInput(bool)));

    //Spring version signal
    connect(serverContextState, SIGNAL(serverSpringVersion(QString)),
            battles, SLOT(onServerSpringVersion(QString)));
    connect(serverContextState, SIGNAL(serverSpringVersion(QString)),
            this, SLOT(onServerSpringVersion(QString)));

    //user menu builder stuff
    UserMenuBuilder* b = UserMenuBuilder::getInstance();
    connect(b, SIGNAL(joinSameBattle(User)), users, SLOT(joinSameBattle(User)));
    connect(b, SIGNAL(sendInput(QString)), lobbyTabs, SLOT(receiveInput(QString)));
    connect(b, SIGNAL(invalidateModel()), users, SLOT(invalidateModel()));
    connect(b, SIGNAL(openGroupsDialog()), userGroupsDialog, SLOT(show()));
    connect(b, SIGNAL(sendCommand(Command)), commandAssigner, SLOT(sendCommand(Command)));


    //Spring stopped signal
    connect(&qpSpring, SIGNAL(finished (int, QProcess::ExitStatus)),
            users, SLOT(onSpringStopped()));
    // this is the trigger for the login dialog popup
    QTimer::singleShot( 0, connectionWidget, SLOT( show_if_wanted() ) );
    //   qDebug() << timer->msec() << "ms elapsed";
    QSettings* s = Settings::Instance();
    //Default states for main window and docks
    if (!s->contains("mainwindow/geometry")) {
        s->setValue("mainwindow/geometry", QVariant("\\x1\\xd9\\xd0\\xcb\\0\\x1\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\x4\\xff\\0\\0\\x3\\xd6\\0\\0\\0\\0\\0\\0\\0\\0\\xff\\xff\\xff\\xfe\\xff\\xff\\xff\\xfe\\0\\0\\0\\0\\x2\\0"));
        s->setValue("mainwindow/state", QVariant("\\0\\0\\0\\xff\\0\\0\\0\\0\\xfd\\0\\0\\0\\x3\\0\\0\\0\\x1\\0\\0\\x1\\v\\0\\0\\x2T\\xfc\\x2\\0\\0\\0\\x1\\xfb\\0\\0\\0$\\0u\\0s\\0\\x65\\0r\\0L\\0i\\0s\\0t\\0\\x44\\0o\\0\\x63\\0k\\0W\\0i\\0\\x64\\0g\\0\\x65\\0t\\x1\\0\\0\\0H\\0\\0\\x2T\\0\\0\\0\\x96\\x1\\0\\0\\x2\\0\\0\\0\\x2\\0\\0\\x5\\0\\0\\0\\0+\\xfc\\x1\\0\\0\\0\\x2\\xfb\\0\\0\\0\\x1c\\0t\\0\\x61\\0\\x62\\0s\\0\\x44\\0o\\0\\x63\\0k\\0W\\0i\\0\\x64\\0g\\0\\x65\\0t\\x1\\0\\0\\0\\0\\0\\0\\x5\\0\\0\\0\\x4\\x3\\x1\\0\\0\\x5\\xfb\\0\\0\\0\\x14\\0\\x64\\0o\\0\\x63\\0k\\0W\\0i\\0\\x64\\0g\\0\\x65\\0t\\x1\\0\\0\\0\\0\\0\\0\\x3p\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\x3\\0\\0\\x5\\0\\0\\0\\x1\\v\\xfc\\x1\\0\\0\\0\\x2\\xfb\\0\\0\\0(\\0\\x62\\0\\x61\\0t\\0t\\0l\\0\\x65\\0L\\0i\\0s\\0t\\0\\x44\\0o\\0\\x63\\0k\\0W\\0i\\0\\x64\\0g\\0\\x65\\0t\\x1\\0\\0\\0\\0\\0\\0\\x3\\xf6\\0\\0\\x1v\\x1\\0\\0\\x5\\xfb\\0\\0\\0(\\0\\x62\\0\\x61\\0t\\0t\\0l\\0\\x65\\0I\\0n\\0\\x66\\0o\\0\\x44\\0o\\0\\x63\\0k\\0W\\0i\\0\\x64\\0g\\0\\x65\\0t\\x1\\0\\0\\x3\\xf9\\0\\0\\x1\\a\\0\\0\\0N\\x1\\0\\0\\x5\\0\\0\\x3\\xf2\\0\\0\\x2T\\0\\0\\0\\x4\\0\\0\\0\\x4\\0\\0\\0\\b\\0\\0\\0\\b\\xfc\\0\\0\\0\\x1\\0\\0\\0\\x2\\0\\0\\0\\0"));
        s->setValue("mainwindow/battlestate", QVariant("\\0\\0\\0\\xff\\0\\0\\0\\0\\xfd\\0\\0\\0\\x3\\0\\0\\0\\0\\0\\0\\x1\\xd2\\0\\0\\x2?\\xfc\\x2\\0\\0\\0\\x1\\xfb\\0\\0\\0$\\0u\\0s\\0\\x65\\0r\\0L\\0i\\0s\\0t\\0\\x44\\0o\\0\\x63\\0k\\0W\\0i\\0\\x64\\0g\\0\\x65\\0t\\x1\\0\\0\\0\\x38\\0\\0\\x2?\\0\\0\\0\\x96\\x1\\0\\0\\x2\\0\\0\\0\\x2\\0\\0\\x5\\0\\0\\0\\0\\xa6\\xfc\\x1\\0\\0\\0\\x1\\xfb\\0\\0\\0\\x14\\0\\x64\\0o\\0\\x63\\0k\\0W\\0i\\0\\x64\\0g\\0\\x65\\0t\\x1\\0\\0\\0\\0\\0\\0\\x3p\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\0\\x3\\0\\0\\x5\\0\\0\\0\\x1\\x30\\xfc\\x1\\0\\0\\0\\x2\\xfb\\0\\0\\0(\\0\\x62\\0\\x61\\0t\\0t\\0l\\0\\x65\\0L\\0i\\0s\\0t\\0\\x44\\0o\\0\\x63\\0k\\0W\\0i\\0\\x64\\0g\\0\\x65\\0t\\x1\\0\\0\\0\\0\\0\\0\\x3\\x93\\0\\0\\x1v\\x1\\0\\0\\x5\\xfb\\0\\0\\0(\\0\\x62\\0\\x61\\0t\\0t\\0l\\0\\x65\\0I\\0n\\0\\x66\\0o\\0\\x44\\0o\\0\\x63\\0k\\0W\\0i\\0\\x64\\0g\\0\\x65\\0t\\x1\\0\\0\\x3\\x96\\0\\0\\x1j\\0\\0\\0N\\x1\\0\\0\\x5\\0\\0\\x3+\\0\\0\\x2?\\0\\0\\0\\x4\\0\\0\\0\\x4\\0\\0\\0\\b\\0\\0\\0\\b\\xfc\\0\\0\\0\\x1\\0\\0\\0\\x2\\0\\0\\0\\x1\\0\\0\\0\\x16\\0t\\0\\x61\\0\\x62\\0s\\0T\\0o\\0o\\0l\\0\\x42\\0\\x61\\0r\\x1\\0\\0\\0\\0\\xff\\xff\\xff\\xff\\0\\0\\0\\0\\0\\0\\0\\0"));
        move(100,50);
    }
    restoreGeometry(s->value("mainwindow/geometry").toByteArray());
    lastState = s->value("mainwindow/state").toByteArray();
    lastBattleState = s->value("mainwindow/battlestate").toByteArray();
    restoreState(lastState);
    for (int i = 0; i < 4; i++)
        users->resizeColumnToContents(i);
    inBattle = false;
}

void MainWindow::setupIcons() {
    action_Connect->setIcon(QIcon(P("icons/Connect_creating.png")));
    action_Disconnect->setIcon(QIcon(P("icons/Connect_no.png")));
    action_Exit->setIcon(QIcon(P("icons/Exit.png")));
    actionDownloads->setIcon(QIcon(P("icons/download_map.xpm")));
}

void MainWindow::setupToolbar() {
    tabBar = toolBarWidget->ui->tabBar;
    newTabButton = toolBarWidget->ui->joinToolButton;
    connect(newTabButton, SIGNAL(clicked()), this, SLOT(onJoinRequested()));
    nextTab = new QShortcut(QKeySequence(QKeySequence::NextChild), this);
    connect( nextTab, SIGNAL( activated() ),
             toolBarWidget, SLOT( onNextTab() ) );
    previousTab = new QShortcut(QKeySequence(QKeySequence::PreviousChild), this);
    connect( previousTab, SIGNAL( activated() ),
             toolBarWidget, SLOT( onPreviousTab() ) );
    openNewTab = new QShortcut(QKeySequence(QKeySequence::Open), this);
    connect( openNewTab, SIGNAL( activated() ),
             this, SLOT( onJoinRequested() ) );
    openNewTab2 = new QShortcut(QKeySequence(QKeySequence::New), this);
    connect( openNewTab2, SIGNAL( activated() ),
             this, SLOT( onJoinRequested() ) );
}

MainWindow::~MainWindow() {
    settings->setValue("battleListFilterString", battleListLineEdit->text());
    settings->setValue("userListFilterString", userListLineEdit->text());
    serverContextState->forceDisconnect();
    //delete statusTracker;
    delete connectionWidget;
    delete lobbyTabs;
    delete commandAssigner;
    delete serverContextState;
    delete preference;
    //    delete mapSelector;
}

void MainWindow::newUserTextInput() {
    //allow at most one leading space and no trailing spaces
    QString input = inputLineEdit->text().trimmed();
    if ( input.size() == 0 ) {
        inputLineEdit->clear();
        return;
    }
    lobbyTabs->receiveInput( input.prepend( input.at( 0 ).isSpace() ? " " : "" ) );
    inputLineEdit->clear();
}

void MainWindow::toggleUserListVisible() {
    this->userListDockWidget->setVisible( !userListDockWidget->isVisible() );
}

void MainWindow::toggleShowHideMainWindow( QSystemTrayIcon::ActivationReason reason ) {
    if ( reason == QSystemTrayIcon::Trigger ) {
        if ( isVisible() ) {
            //       QTimer::singleShot( 0, connectionWidget, SLOT( hide() ) );
            hide();
        } else {
            show();
            restoreState(state);
            userListDockWidget->blockSignals(false);
            battleListDockWidget->blockSignals(false);
            //       QTimer::singleShot( 0, connectionWidget, SLOT( show() ) );
        }
    }
}

void MainWindow::about() {
    QDialog* a = new QDialog(this);
    Ui::AboutWidget* about = new Ui::AboutWidget();
    about->setupUi( a );
    about->label->setText(about->label->text().replace("#version#", QString(QTLOBBY_VERSION)).replace("#revision#", QString(SVN_REV)).replace("#logo_icon#", P("icons/qtlobby-logo-64x64.png")));
    a->show();
}

void MainWindow::showConnectionWidget( bool ) {
    connectionWidget->show();
}

void MainWindow::createTrayIcon() {
    trayIcon = new QSystemTrayIcon( this );
    QIcon icon = QIcon( P("icons/qtlobby-logo.svg") );

    trayIconMenu = new QMenu( this );

    restoreAction = new QAction( tr( "&Restore" ), this );
    connect( restoreAction, SIGNAL( triggered() ), this, SLOT( showNormal() ) );

    trayIconMenu->addAction( action_Connect );
    trayIconMenu->addAction( action_Disconnect );
    trayIconMenu->addSeparator();
    trayIconMenu->addAction( action_Exit );
    trayIcon->setContextMenu( trayIconMenu );

    trayIcon->setIcon( icon );
    setWindowIcon( icon );

    // set icon for macos dock
    QApplication::setWindowIcon(icon);
}

void MainWindow::setColorInducatorUsers( QString regExp ) {
    if ( regExp != "" ) {
        QPalette qp( QPalette::Window );
        qp.setColor( QPalette::Normal, QPalette::Base, regexpColor );
        qp.setColor( QPalette::Normal, QPalette::Text, Qt::black );
        userListLineEdit->setPalette( qp );
    } else {
        // reset to default palette
        userListLineEdit->setPalette( QPalette() );
    }
}

void MainWindow::setColorInducatorBattles( QString regExp ) {
    if ( regExp != "" ) {
        QPalette qp( QPalette::Window );
        qp.setColor( QPalette::Normal, QPalette::Base, regexpColor );
        qp.setColor( QPalette::Normal, QPalette::Text, Qt::black );
        battleListLineEdit->setPalette( qp );
    } else {
        // reset to default palette
        battleListLineEdit->setPalette( QPalette() );
    }
}

void MainWindow::startSpring() {
    //emit newTrayMessage( "spring instance started" );
    qpSpring.setWorkingDirectory( settings->value( "spring_user_dir" ).toString() );
    qpSpring.start( settings->value( "spring_executable_with_abs_path_to_it" ).toString(),
                    QStringList( QDir::toNativeSeparators(QString( "%1/%2" )
                                                          .arg( settings->value( "spring_user_dir" ).toString() )
                                                          .arg( "script_qtlobby.txt" ) ) ) );
}

void MainWindow::sendTrayMessage( QString message, int milliseconds ) {
    trayIcon->showMessage( message, message, QSystemTrayIcon::Information, milliseconds );
}

void MainWindow::startSpringSettings() {
    if (settings->value( "springsettings+" ).toString().isEmpty()) return;
    qpSpringSetting.start( settings->value( "springsettings+" ).toString() );
}

void MainWindow::playSample( SampleCollection sample ) {
    audioBackend.playSample( sample );
}

void MainWindow::showStylesheetEditor() {
    stylesheetDialog->exec();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    QSettings* s = Settings::Instance();
    s->setValue("channels", lobbyTabs->getChannelList());
    s->setValue("mainwindow/geometry", saveGeometry());
    if (inBattle)
        lastBattleState = saveState();
    else
        lastState = saveState();
    s->setValue("mainwindow/state", lastState);
    s->setValue("mainwindow/battlestate", lastBattleState);
    event->accept();
    QApplication::exit(0);
}

void MainWindow::showGroupsDialog() {
    userGroupsDialog->show();
    userGroupsDialog->activateWindow();
    userGroupsDialog->raise();
}

void MainWindow::showHistoryDialog() {
    historyDialog->show();
    historyDialog->activateWindow();
    historyDialog->raise();
}

void MainWindow::onChangedToBattleTab() {
    lastState = saveState();
    restoreState(lastBattleState);
    //FIXME: make this working. It breaks TreeView when restoring state after some columns have moved
    /*s->setValue("mainwindow/usersHeaderViewState", users->header()->saveState());
    if(s->contains("mainwindow/usersHeaderViewBattleState")) {
        users->header()->restoreState(s->value("mainwindow/usersHeaderViewBattleState").toByteArray());
    }*/
    for (int i = 0; i < 9; i++)
        users->resizeColumnToContents(i);
    inBattle = true;
}

void MainWindow::onChangedFromBattleTab() {
    lastBattleState = saveState();
    restoreState(lastState);
    /*s->setValue("mainwindow/usersHeaderViewBattleState", users->header()->saveState());
    if(s->contains("mainwindow/usersHeaderViewState")) {
        users->header()->restoreState(s->value("mainwindow/usersHeaderViewState").toByteArray());
    }*/
    for (int i = 0; i < 4; i++)
        users->resizeColumnToContents(i);
    inBattle = false;
}

void MainWindow::onCurrentChanged(const QModelIndex & current, const QModelIndex & /*previous*/) {
    QModelIndex sourceIndex = battles->battleManager->proxyModel()->mapToSource(current);
    QList<Battle> bl = battles->battleManager->model()->battleList();
    if( sourceIndex.isValid() && bl.size() ) {
        Battle b = bl.at(sourceIndex.row());
        battleInfoTreeView->setModel((UserTreeModel*)users->getUserModel(b.id));
    }
    for (int i = 0; i < 9; i++)
        battleInfoTreeView->resizeColumnToContents(i);
    for (int i = 4; i <= 9; i++)
        battleInfoTreeView->hideColumn(i);
}

void MainWindow::hideEvent(QHideEvent * /*event*/) {
    userListDockWidget->blockSignals(true);
    battleListDockWidget->blockSignals(true);
    state = saveState();
}

void MainWindow::showEvent(QShowEvent * /*event*/) {
    userListDockWidget->blockSignals(false);
    battleListDockWidget->blockSignals(false);
    restoreState(state);
}


void MainWindow::onStatsChange(int users, int moderators) {
    usersOnline->setText(tr("Users online: %1").arg(QString::number(users)));
    moderatorsOnline->setText(tr("Moderators online: %1").arg(QString::number(moderators)));
}

void MainWindow::onStatsChange(int battles) {
    battlesOnline->setText(tr("Battles: %1").arg(QString::number(battles)));
}

void MainWindow::on_actionScripting_triggered() {
    scriptingDialog->show();
    scriptingDialog->activateWindow();
    scriptingDialog->raise();
}

void MainWindow::connectionStatusChanged(ConnectionState state) {
    switch ( state ) {
    case DISCONNECTED:
        statusBar()->showMessage(tr("Disconnected"));
        users->wipeModels();
        battles->wipeModels();
        hostPushButton->setEnabled(false);
        break;
    case CONNECTING:
        statusBar()->showMessage(tr("Connecting..."), 20000);
        break;
    case CONNECTED:
        statusBar()->showMessage(tr("Connected"), 20000);
        hostPushButton->setEnabled(true);
        break;
    case AUTHENTICATING:
        statusBar()->showMessage(tr("Authenticating..."), 20000);
        break;
    case AUTHENTICATED:
        statusBar()->showMessage(tr("Authenticated"));
        break;
    }
}

void MainWindow::onCurrentTabChanged() {
    onTeamPlayerSpecCountChanged(users->teamPlayerSpecCount());
}

void MainWindow::onTeamPlayerSpecCountChanged(QString ratio) {
    if( ratio.isEmpty() ) {
        usersInCurrentChannel->setText( tr("Users: %1").arg(QString::number(users->usersCountInCurrentChannel())));
        usersInCurrentChannel->setToolTip(tr("# Users"));
        availableSlots->setText("");
    } else {
        usersInCurrentChannel->setText(ratio);
        usersInCurrentChannel->setToolTip(tr("# Players + # Specs (# 1st Ally : # 2nd Ally :...)"));
        QUrl url = ServerProfilesModel::getInstance()->getActiveProfile();
        availableSlots->setText( tr("free slot(s): %1")
                                 .arg(QString::number(battles->battleManager->getBattle(users->getUser(
                                         url.userName()).joinedBattleId).maxPlayers
                                                      - users->usersInChanCount())));
        availableSlots->setToolTip( tr("# free slots in the current battle"));
    }
}

void MainWindow::on_hostPushButton_clicked( bool closeFirst ) {
    QUrl url = ServerProfilesModel::getInstance()->getActiveProfile();
    User me = users->getUser( url.userName() );
    if ( closeFirst ) {
        lobbyTabs->onCloseBattleChannel();
    } else if ( me.joinedBattleId >= 0 ) { // in battle
        if ( !Settings::Instance()->value( "Battle/autoCloseFirst", false ).toBool() ) {
            battles->battleCloseFirstWidget->setBattleId( -1 );
            battles->battleCloseFirstWidget->show();
            return;
        }
        lobbyTabs->onCloseBattleChannel();
    }
    battleHostingDialog->show();
    battleHostingDialog->activateWindow();
    battleHostingDialog->raise();
}

void MainWindow::onBlockInput(bool b) {
    inputLineEdit->setDisabled(b);
}

void MainWindow::onJoinRequested() {
    bool ok;
    QString channel = QInputDialog::getText(this,
                                            tr("Join channel"),
                                            tr("Specify a channel you want to join"),
                                            QLineEdit::Normal,
                                            "",
                                            &ok);
    if(!ok) return;
    QRegExp re("#?(\\w+)");
    if(!re.exactMatch(channel)) return;
    lobbyTabs->receiveInput("/j " + re.cap(1));
}

void MainWindow::on_actionDownloads_triggered() {
    downloadsDialog->show();
    downloadsDialog->activateWindow();
    downloadsDialog->raise();
}

void MainWindow::changeEvent(QEvent *e) {
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        this->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::onServerSpringVersion(QString version) {
    QString unitsyncSpringVersion = UnitSyncLib::getInstance()->getSpringVersion();
    if(version != unitsyncSpringVersion)
        QMessageBox::warning(this, "Wrong spring version",
                             "Lobby server reported spring version " + version +
                             "\nYour unitsync reported that you have " + unitsyncSpringVersion +
                             "\nYou can desync if you have wrong spring version!!!");
}
