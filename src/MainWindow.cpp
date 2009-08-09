// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "MainWindow.h"
#include "ToolBarWidget.h"
#include <QInputDialog>

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ) {
    //   QTime *timer = new QTime();
    //   timer->start();
    //   setAttribute(Qt::WA_DeleteOnClose);
    setUnifiedTitleAndToolBarOnMac( true );
    setupUi( this );
    setWindowTitle(QString( "QtLobby 0.0.%1-svn" ).arg( SVN_REV ));

    settings = Settings::Instance();
    /* it's important to first init the preferences */

    battles->setUsers( users );
    preference          = new UserPreference();
    preference->setWindowFlags(Qt::Window);
    if (settings->value("unitsync").toString().isEmpty())
        preference->exec();
    UnitSyncLib::getInstance();
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

    // setting the configuration
    connect( connectionWidget, SIGNAL( emitConfiguration( QUrl ) ),
             users, SLOT( setConfiguration( QUrl ) ) );
    connect( connectionWidget, SIGNAL( emitConfiguration( QUrl ) ),
             battles, SLOT( setConfiguration( QUrl ) ) );
    connect( connectionWidget, SIGNAL( emitConfiguration( QUrl ) ),
             serverContextState, SLOT( setConfiguration( QUrl ) ) );
    connect( connectionWidget, SIGNAL( usernameChanged(QString ) ),
             battles, SLOT( setCurrentUsername( QString ) ) );
    connect( connectionWidget, SIGNAL( usernameChanged(QString ) ),
             this, SLOT( setCurrentUsername( QString ) ) );

    // disconnect
    connect( action_Disconnect, SIGNAL( triggered() ),
             serverContextState, SLOT( forceDisconnect() ) );
    // view menue
    //connect( action_Battle_list, SIGNAL( triggered() ),
    //         gameTreeVisibleCheckBox, SLOT( toggle() ) );
    //connect( gameTreeVisibleCheckBox, SIGNAL( toggled( bool ) ),
    //         action_Battle_list, SLOT( setChecked( bool ) ) );
    //connect( action_User_list, SIGNAL( triggered() ),
    //         this, SLOT( toggleUserListVisible() ) );
    //connect( userListDockWidget, SIGNAL( visibilityChanged( bool ) ),
    //         action_User_list, SLOT( setChecked( bool ) ) );
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
    connect( battles, SIGNAL( sendInput( QString ) ),
             lobbyTabs, SLOT( receiveInput( QString ) ) );

    connect( users, SIGNAL(teamPlayerSpecCountChanged(QString)),
             this, SLOT(onTeamPlayerSpecCountChanged(QString)));
    //Group action in users
    connect( users, SIGNAL(openGroupsDialog()),
             userGroupsDialog, SLOT(show()));
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
    connect (tabBar, SIGNAL(tabMoved(int,int)),
             lobbyTabs, SLOT(onTabMoved(int,int)));
    connect (tabBar, SIGNAL(currentChanged(int)),
             this, SLOT(onCurrentTabChanged()));
    //connect( lobbyTabs, SIGNAL( hideBattleList( bool ) ),
    //         this, SLOT( hideBattleList( bool ) ) );
    // aboutDialog
    connect( action_About, SIGNAL( triggered() ),
             this, SLOT( about() ) );
    connect( action_Host_a_Battle, SIGNAL( triggered() ),
             this, SLOT( on_hostPushButton_clicked() ) );
    // preferences
    connect( actionPreferences, SIGNAL( triggered() ),
             preference, SLOT( show() ) );
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
    connect( trayIcon, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ),
             this, SLOT( toggleShowHideMainWindow( QSystemTrayIcon::ActivationReason ) ) );

    connect( this, SIGNAL( newTrayMessage( QString ) ),
             this, SLOT( sendTrayMessage( QString ) ) );
    connect( battles, SIGNAL( start() ),
             this, SLOT( startSpring() ) );

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

    //Spring stopped signal
    connect(&qpSpring, SIGNAL(finished (int, QProcess::ExitStatus)),
            users, SLOT(onSpringStopped()));
    // this is the trigger for the login dialog popup
    QTimer::singleShot( 0, connectionWidget, SLOT( show_if_wanted() ) );
    //   qDebug() << timer->msec() << "ms elapsed";
    QSettings* s = Settings::Instance();
    if (s->contains("mainwindow/geometry")) {
        restoreGeometry(s->value("mainwindow/geometry").toByteArray());
        lastState = s->value("mainwindow/state").toByteArray();
        lastBattleState = s->value("mainwindow/battlestate").toByteArray();
        restoreState(lastState);
    }
    for (int i = 0; i < 4; i++)
        users->resizeColumnToContents(i);
    inBattle = false;
}

void MainWindow::setupToolbar() {
    ToolBarWidget* toolBarWidget = new ToolBarWidget(this);
    tabBar = toolBarWidget->ui->tabBar;
    newTabButton = toolBarWidget->ui->joinToolButton;
    connect(newTabButton, SIGNAL(clicked()), this, SLOT(onJoinRequested()));
    QAction* a = tabsToolBar->addWidget(toolBarWidget);
    a->setVisible(true);
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
    about->label->setText(about->label->text().replace("#version#", SVN_REV));
    a->show();
}

void MainWindow::showConnectionWidget( bool ) {
    connectionWidget->show();
}

void MainWindow::createTrayIcon() {
    trayIcon = new QSystemTrayIcon( this );
    QIcon icon = QIcon( ":/icons/logo.xpm" );

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
    qpSpring.start( settings->value( "spring_executable_with_abs_path_to_it" ).toString(),
                    QStringList( QString( "%1/%2" )
                                 .arg( settings->value( "spring_user_dir" ).toString() )
                                 .arg( "script_qtlobby.txt" ) ) );
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

void MainWindow::setCurrentUsername( QString username ) {
    AbstractChannel::setCurrentUsername(username);
    battleHostingDialog->setCurrentUsername(username);
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
    usersInCurrentChannel->setText(
            tr("Users: %1")
            .arg(QString::number(users->usersCountInCurrentChannel()))
            .append(ratio));
}

void MainWindow::on_hostPushButton_clicked() {
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

