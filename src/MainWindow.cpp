//
// C++ Implementation: MainWindow
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "MainWindow.h"

MainWindow::MainWindow( QWidget* parent ) : QMainWindow( parent ) {
//   QTime *timer = new QTime();
//   timer->start();
  //   setAttribute(Qt::WA_DeleteOnClose);
  setUnifiedTitleAndToolBarOnMac( true );
  setupUi( this );
  setWindowTitle(QString( "0.0.%1-svn" ).arg( SVN_REV ));

  settings = Settings::Instance();

  /* it's important to first init the preferences */

  UnitSyncLib::getInstance();
  battles->setUsers( users );
  preference          = new UserPreference();
  serverContextState  = new ServerContextState( this );
  connectionWidget    = new ConnectionWidget( serverContextState );
  lobbyTabs           = new LobbyTabs( this, battles, UnitSyncLib::getInstance(), lobbyTabWidget );
  commandAssigner     = new CommandAssigner( this );
  statusTracker       = new StatusTracker( statusbar );
  mapSelector         = new MapSelector();
  stylesheetDialog    = new StylesheetDialog();




  createTrayIcon();
  trayIcon->show();

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
  // connectionWidget
  connect( action_Connect, SIGNAL( triggered() ),
           connectionWidget, SLOT( show() ) );
  connect( actionStart_Spring, SIGNAL( triggered() ),
           this, SLOT( startSpring() ) );
  connect( action_Spring_Settings, SIGNAL( triggered() ),
           this, SLOT( startSpringSettings() ) );

  connect( serverContextState, SIGNAL( connectionStateChanged( ConnectionState ) ),
           connectionWidget, SLOT( connectionStatusChanged( ConnectionState ) ) );

  // setting the configuration
  connect( connectionWidget, SIGNAL( emitConfiguration( QUrl ) ),
           users, SLOT( setConfiguration( QUrl ) ) );
  connect( connectionWidget, SIGNAL( emitConfiguration( QUrl ) ),
           battles, SLOT( setConfiguration( QUrl ) ) );
  connect( connectionWidget, SIGNAL( emitConfiguration( QUrl ) ),
           serverContextState, SLOT( setConfiguration( QUrl ) ) );
  connect( connectionWidget, SIGNAL( usernameChanged(QString ) ),
           battles, SLOT( setCurrentUsername( QString ) ) );

  // disconnect
  connect( action_Disconnect, SIGNAL( triggered() ),
           serverContextState, SLOT( forceDisconnect() ) );
  // view menue
  connect( action_Battle_list, SIGNAL( triggered() ),
           gameTreeVisibleCheckBox, SLOT( toggle() ) );
  connect( gameTreeVisibleCheckBox, SIGNAL( toggled( bool ) ),
           action_Battle_list, SLOT( setChecked( bool ) ) );
  connect( action_User_list, SIGNAL( triggered() ),
           this, SLOT( toggleUserListVisible() ) );
  connect( userListDockWidget, SIGNAL( visibilityChanged( bool ) ),
           action_User_list, SLOT( setChecked( bool ) ) );
  // chat menue
  connect( actionLeave, SIGNAL( triggered() ),
           lobbyTabs, SLOT( closeTab() ) );
  // lobbyTabs to open the default channel, when connected
  connect( serverContextState, SIGNAL( connectionStateChanged( ConnectionState ) ),
           lobbyTabs, SLOT( connectionStateChanged( ConnectionState ) ) );
  // lobbyTabs to open private channel, when requested in user list
  connect( users, SIGNAL( sendInput( QString ) ),
           lobbyTabs, SLOT( receiveInput( QString ) ) );
  // lobbyTabs to send the ready state
  connect( lobbyTabs, SIGNAL( readyStateChanged( bool ) ),
           users, SLOT( onReadyStateChanged( bool ) ) );
  // lobbyTabs to send the spec state // NEW
  connect( lobbyTabs, SIGNAL(specStateChanged(bool)),
           users, SLOT( onSpecStateChanged(bool)));
  // lobbyTabs to send  side state changed // NEW
  connect( lobbyTabs, SIGNAL(sideChanged(int)),
           users, SLOT(onSideComboBoxChanged( int )));

  // users, for the userlist to show
  connect( lobbyTabs, SIGNAL( currentTabChanged( QString, QString ) ),
           users, SLOT( currentTabChanged( QString, QString ) ) );
  connect( lobbyTabs, SIGNAL( hideBattleList( bool ) ),
           this, SLOT( hideBattleList( bool ) ) );
  // aboutDialog
  connect( action_About, SIGNAL( triggered() ),
           this, SLOT( about() ) );
  // preferences
  connect( actionPreferences, SIGNAL( triggered() ),
           preference, SLOT( show() ) );
  // maplist
  connect( actionMap_list, SIGNAL( triggered() ),
           mapSelector, SLOT( show() ) );

  // inputLine
  connect( inputLineEdit, SIGNAL( sendInput( QString ) ),
           lobbyTabs, SLOT( receiveInput( QString ) ) );

  // regular expresion line edits
  connect( userListLineEdit, SIGNAL( textChanged( QString ) ),
           users, SLOT( setRegExp( QString ) ) );
  connect( battleListLineEdit, SIGNAL( textChanged( QString ) ),
           battles, SLOT( setRegExp( QString ) ) );
  connect( userListLineEdit, SIGNAL( textChanged( QString ) ),
           this, SLOT( setColorInducatorUsers( QString ) ) );
  connect( battleListLineEdit, SIGNAL( textChanged( QString ) ),
           this, SLOT( setColorInducatorBattles( QString ) ) );

  // tray icon stuff
  connect( trayIcon, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ),
           this, SLOT( toggleShowHideMainWindow( QSystemTrayIcon::ActivationReason ) ) );

  connect( this, SIGNAL( newTrayMessage( QString ) ),
           this, SLOT( sendTrayMessage( QString ) ) );
  connect( battles, SIGNAL( start() ),
           this, SLOT( startSpring() ) );

  // this is the trigger for the login dialog popup
  QTimer::singleShot( 0, connectionWidget, SLOT( show_if_wanted() ) );
//   qDebug() << timer->msec() << "ms elapsed";
}

MainWindow::~MainWindow() {
  serverContextState->forceDisconnect();
  delete statusTracker;
  delete connectionWidget;
  delete lobbyTabs;
  delete commandAssigner;
  delete serverContextState;
  delete preference;
  delete mapSelector;
}

void MainWindow::newUserTextInput() {
  //allow at most one leading space and now trailing spaces
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
    }
    else {
      show();
//       QTimer::singleShot( 0, connectionWidget, SLOT( show() ) );
    }
  }
}

void MainWindow::about() {
  //FIXME this code is bad, and this can be made better (js)
  // HINT in general prefere the "multiple inheritance approach" instead of this
  //      but since this widget doesn't have any input it doesn't matter anyway
  QDialog* a = new QDialog;
  Ui::aboutWidget* about = new Ui::aboutWidget;
  about->setupUi( a );
  a->show();
}

void MainWindow::showConnectionWidget( bool ) {
//FIXME this code might be unneeded, please remove it if you are (js)
//   connectionWidget->hide();
  connectionWidget->show();
}

void MainWindow::hideBattleList( bool isBattleTab ) {
  if ( gameTreeVisibleCheckBox->isChecked() && isBattleTab )
    gameTreeVisibleCheckBox->toggle();
  if ( !gameTreeVisibleCheckBox->isChecked() && !isBattleTab )
    gameTreeVisibleCheckBox->toggle();
}

void MainWindow::createTrayIcon()
{
  trayIcon = new QSystemTrayIcon( this );
  QIcon icon = QIcon( ":/icons/heart.svg" );

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
  emit newTrayMessage( "spring instance started" );
  qp.start( settings->value( "spring_executable_with_abs_path_to_it" ).toString(),
            QStringList( QString( "%1/%2" )
                         .arg( settings->value( "spring_user_dir" ).toString() )
                         .arg( "script_qtlobby.txt" ) ) );
}

void MainWindow::sendTrayMessage( QString message, int milliseconds ) {
  trayIcon->showMessage( message, message, QSystemTrayIcon::Information, milliseconds );
}

void MainWindow::startSpringSettings() {
  emit newTrayMessage( "spring settings dialog started" );
  qp.start( settings->value( "springsettings+" ).toString() );
}

void MainWindow::playSample( SampleCollection sample ) {
  audioBackend.playSample( sample );
}

void MainWindow::showStylesheetEditor() {
    stylesheetDialog->exec();
}

