//
// C++ Implementation: ConnectionWidget
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ConnectionWidget.h"

ConnectionWidget::ConnectionWidget( ServerContextState* serverContextState,
                                    QWidget *parent ) : QDialog( parent ) {
  setupUi( this );
  this->serverContextState = serverContextState;

  // FIXME FIXME FIXME!!! (js)
  // - in the login tab -> while Authenticated we need to store the settings of the current
  //   connection maybe with a global QUrl currentsettings;
  // - we need a interface to get/set this global setting struct
  // - when set is called, the client is automatically disconnected, new config is set and
  //   the TASServer is called

  setWindowTitle( "status: logged in; user : *** @ foohost.com : 4002" );

  settings = Settings::Instance();
  addDefaultServers();

  tabWidget->setCurrentIndex( 0 );
  // limit the text blocks to 500
  QTextDocument * connectionLogTextDocument = new QTextDocument( this );
  connectionLogTextDocument->setMaximumBlockCount( 500 );
  connectionLogTextBrowser->setDocument( connectionLogTextDocument );

  /* backend connections for network connectivity */
  connect( loginButton, SIGNAL( clicked() ),
           this, SLOT( establishConnection() ) );
  connect( logoutButton, SIGNAL( clicked() ),
           serverContextState, SLOT( forceDisconnect() ) );
  connect( this, SIGNAL( establishConnection_() ),
           serverContextState, SLOT( establishConnection() ) );
//   connect( this, SIGNAL( emitConfiguration( QUrl ) ),
//            serverContextState, SLOT( emitConfiguration( QUrl ) ) );

  /* logging */
  connect( serverContextState, SIGNAL( logWrite( QString ) ),
           this, SLOT( logWrite( QString ) ) );

  /* for the radio button state tracking */
  connect( singleSession, SIGNAL( toggled( bool ) ),
           this, SLOT( radioButtonValueChanged( bool ) ) );
  connect( manualMultiSession, SIGNAL( toggled( bool ) ),
           this, SLOT( radioButtonValueChanged( bool ) ) );
  connect( autoMultiSession, SIGNAL( toggled( bool ) ),
           this, SLOT( radioButtonValueChanged( bool ) ) );

  /* for the checkbox in the logintab */
  connect( closeDialogCheckBox, SIGNAL( stateChanged( int ) ),
           this, SLOT( closeDialogCheckBoxValueChanged( int ) ) );
  connect( openDialogCheckBox, SIGNAL( stateChanged( int ) ),
           this, SLOT( openDialogCheckBoxValueChanged( int ) ) );

  connect( profilesServerProfileComboBox, SIGNAL( currentIndexChanged( int ) ),
           this, SLOT( comboBoxCurrentIndexChanged( int ) ) );

  connect( saveProfileButton, SIGNAL( clicked() ),
           this , SLOT( saveModifiedProfile() ) );

  connect( registerUserPushButton, SIGNAL( clicked( ) ),
           this, SLOT( registerNewAccount( ) ) );

  /* buttons from the profiles dialog */
  connect( newProfileButton, SIGNAL( clicked() ),
           this , SLOT( openProfileWizzard() ) );
  connect( delProfileButton, SIGNAL( clicked() ),
           this , SLOT( delSelectedProfile() ) );

  connect( connectionWizzardButton, SIGNAL( clicked() ),
           this , SLOT( openProfileEditor() ) );

  /* rename & change password buttons */
  connect( renameButton, SIGNAL( clicked() ),
           this, SLOT( renameLoginName() ) );
  connect( changePasswordButton, SIGNAL( clicked() ),
           this, SLOT( changePassword() ) );

  /* rename feedback */
  connect( serverContextState, SIGNAL( renameLoginNameSuccess( QString ) ),
           this, SLOT( renameLoginNameFeedbackSuccess( QString ) ) );
  connect( serverContextState, SIGNAL( renameLoginNameFailure( QString ) ),
           this, SLOT( renameLoginNameFeedbackFailure( QString ) ) );

  /* password change feedback */
  connect( serverContextState, SIGNAL( changePasswordFailure( QString ) ),
           this, SLOT( changePasswordFailure( QString ) ) );
  connect( serverContextState, SIGNAL( changePasswordSuccess( QString ) ),
           this, SLOT( changePasswordSuccess( QString ) ) );

  updateComboBoxes();
  if ( settings->value( "singleSession", true ).toBool() )
    singleSession->setChecked( true );
  if ( settings->value( "manualMultiSession", false ).toBool() )
    manualMultiSession->setChecked( true );
  if ( settings->value( "autoMultiSession", false ).toBool() )
    autoMultiSession->setChecked( true );

  if ( settings->value( "closeDialogCheckBox" ).toBool() )
    closeDialogCheckBox->setChecked( Qt::Checked );
  else
    closeDialogCheckBox->setChecked( Qt::Unchecked );

  if ( settings->value( "openDialogCheckBox" ).toBool() )
    openDialogCheckBox->setChecked( Qt::Checked );
  else
    openDialogCheckBox->setChecked( Qt::Unchecked );


  tabWidget->removeTab( 2 );
  QWizard* w = new QWizard( this );

  QWizardPage *page = new QWizardPage;
  page->setTitle( "Introduction" );

  QLabel *label = new QLabel( "This wizard will help you to register a new account for playing spring online or in a local network." );
  label->setWordWrap( true );

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget( label );
  page->setLayout( layout );
  w->addPage( page );

  tabWidget->insertTab( 2, w, "Profile wizard" );
//   tabWidget->setCurrentIndex( 2 );
}

ConnectionWidget::~ConnectionWidget() { }

void ConnectionWidget::establishConnection() {
  int index = loginServerProfileComboBox->property( "currentIndex" ).toInt();
  settings->setValue( "SelectedServerProfile", index );

  if ( index == -1 ) {
    QMessageBox::critical( this, "No Profile selected",
                           "You have to add a profile in the 'Profile' tab first before you can connect to a server." );
    return;
  }
  QUrl url = loginServerProfileComboBox->itemData(
               index,
               Qt::UserRole ).toUrl();
  if ( singleSession->isChecked() ) {
    url.setPassword( "" );
    modifyServerProfile( index, url );
  }
  if ( passwordLineEdit->text() == "" ) {
    if ( url.password() == "" ) {
      QMessageBox::critical( this, "No password",
                             "Type in the password and try again!" );
      return;
    } else {
      passwordLineEdit->setText( url.password() );
    }
  } else {
    url.setPassword( passwordLineEdit->text() );
  }

  if ( autoMultiSession->isChecked() || manualMultiSession->isChecked() )
    modifyServerProfile( index, url );
  emit emitConfiguration( url );
  emit establishConnection_();
  emit usernameChanged(url.userName());
}

void ConnectionWidget::logWrite( QString l ) {
  if ( this->isVisible() ) {
    QString time = QTime().currentTime().toString( "hh:mm:ss " );
    connectionLogTextBrowser->append( time + l );
  }
}

void ConnectionWidget::closeDialogCheckBoxValueChanged( int ) {
  settings->setValue( "closeDialogCheckBox", closeDialogCheckBox->isChecked() );
}

void ConnectionWidget::openDialogCheckBoxValueChanged( int ) {
  settings->setValue( "openDialogCheckBox", openDialogCheckBox->isChecked() );
}

// if any radiobutton status changes, we want to remember after program restart
void ConnectionWidget::radioButtonValueChanged( bool ) {
  settings->setValue( "singleSession", singleSession->isChecked() );
  settings->setValue( "manualMultiSession", manualMultiSession->isChecked() );
  settings->setValue( "autoMultiSession", autoMultiSession->isChecked() );
}

void ConnectionWidget::modifyServerProfile( signed int index, QUrl url ) {
  if ( index == -1 ) {
//     qDebug() << "not modifying server profile";
    return ;
  }
  QList<QVariant> list = settings->value( "ServerProfiles" ).toList();
  list[index] = url;
//   qDebug() << "modifying server profile";
  settings->setValue( "ServerProfiles", list );
  updateComboBoxes();
}

// once the new profile wizzard is working, we can remove this code (js)
void ConnectionWidget::addDefaultServers() {
  QList<QVariant> list = settings->value( "ServerProfiles" ).toList();
  if ( list.size() > 0 )
    return;

  QUrl url;
  url.setHost( "taspring.clan-sy.com" );
  url.setPort( 8200 );
  url.setUserName( "qtlobby" );
  QUrl url2;
  url2.setHost( "127.0.0.1" );
  url2.setPort( 8200 );
  url2.setUserName( "qtlobby" );
  QUrl url3;
  url3.setHost( "127.0.0.1" );
  url3.setPort( 1111 );
  url3.setUserName( "qtlobby" );

  list.append( url );
  list.append( url2 );
  list.append( url3 );

  settings->setValue( "ServerProfiles", list );
}

// after a modification of a server profile we have to reload the
// combobox with the new entries. in the profile editor we also need
// to update the selected profile's settings
// we need to disconnect/connect the signals to avoid a infinite loop
void ConnectionWidget::comboBoxCurrentIndexChanged( int index ) {
  QUrl url = loginServerProfileComboBox->itemData(
               index,
               Qt::UserRole ).toUrl();

  profileUserNameLineEdit->setText( url.userName() );
  profileServerAddressLineEdit->setText( url.host() );
  profilePortSpinBox->setValue( url.port() );
}

// profile edit tab: when a profile was altered the values need to be
// a) written into the 'modified' profile and b) the combobox needs a reset
// so that it can reflect the changes 'on change'
void ConnectionWidget::saveModifiedProfile() {
  int index = profilesServerProfileComboBox->property( "currentIndex" ).toInt();
  settings->setValue( "SelectedServerProfile", index );

  QUrl url;
  url.setUserName( profileUserNameLineEdit->text() );
  url.setHost( profileServerAddressLineEdit->text() );
  url.setPort( profilePortSpinBox->value() );
  modifyServerProfile( index, url );
}

// this function handles the change of combobox entries which happen
// when a profile gets modified or deleted. it will also select the
// previously selected 'server profile' in both 'login' and 'server' tab
void ConnectionWidget::updateComboBoxes() {
  QList<QVariant> list = settings->value( "ServerProfiles" ).toList();

  if ( list.size() == 0 ) {
    profileUserNameLineEdit->setEnabled( false );
    profileServerAddressLineEdit->setEnabled( false );
    profilePortSpinBox->setEnabled( false );
  } else {
    profileUserNameLineEdit->setEnabled( true );
    profileServerAddressLineEdit->setEnabled( true );
    profilePortSpinBox->setEnabled( true );
  }

  loginServerProfileComboBox->clear();
  profilesServerProfileComboBox->clear();
  int index = settings->value( "SelectedServerProfile" ).toInt();

  for ( int i = 0 ; i < list.size(); ++i ) {
    QUrl url = list[i].toUrl();
    QString urldescription = QString( "%1 : %2 @ %3 : %4 " )
                             .arg( url.userName() )
                             .arg( QString( url.password().size(), QChar( '*' ) ) )
                             .arg( url.host() )
                             .arg( url.port() );
    if ( i == index )
      if ( url.password() != "" )
        passwordLineEdit->setText( url.password() );
    loginServerProfileComboBox->insertItem( i, QString( urldescription ), url );
    profilesServerProfileComboBox->insertItem( i, QString( urldescription ), url );
  }

  loginServerProfileComboBox->setCurrentIndex( index );
  profilesServerProfileComboBox->setCurrentIndex( index );
}

void ConnectionWidget::openProfileWizzard() {
  tabWidget->setCurrentIndex( 2 );
}

void ConnectionWidget::openProfileEditor() {
  tabWidget->setCurrentIndex( 1 );
}

void ConnectionWidget::delSelectedProfile() {
  int index = profilesServerProfileComboBox->property( "currentIndex" ).toInt();
  if ( index >= 0 ) {
    QList<QVariant> list = settings->value( "ServerProfiles" ).toList();
    list.removeAt( index );
    settings->setValue( "ServerProfiles", list );
    settings->setValue( "SelectedServerProfile", 0 );
    updateComboBoxes();
  }
}

void ConnectionWidget::connectionStatusChanged( ConnectionState state ) {
  switch ( state ) {
    case DISCONNECTED:
      logoutButton->setEnabled( false );
      loginButton->setEnabled( true );
      statusLabel->setText( "unconnected" );
      unlockInterface();
      lockRenameAndChangePassword();
      break;
    case CONNECTING:
      loginButton->setEnabled( false );
      logoutButton->setEnabled( true );
      statusLabel->setText( "connecting" );
      lockInterface();
      break;
    case CONNECTED:
      statusLabel->setText( "connected" );
      break;
    case AUTHENTICATING:
      statusLabel->setText( "authenticating" );
      break;
    case AUTHENTICATED:
      unlockRenameAndChangePassword();
      statusLabel->setText( "authenticated (logged in)" );
      if ( closeDialogCheckBox->isChecked() )
        hide();
      break;
  }
}

void ConnectionWidget::lockInterface() {
  loginServerProfileComboBox->setEnabled( false );
  passwordLineEdit->setEnabled( false );
  singleSession->setEnabled( false );
  manualMultiSession->setEnabled( false );
  autoMultiSession->setEnabled( false );
  closeDialogCheckBox->setEnabled( false );

  profilesServerProfileComboBox->setEnabled( false );
  profileUserNameLineEdit->setEnabled( false );
  profileServerAddressLineEdit->setEnabled( false );
  profilePortSpinBox->setEnabled( false );
  newProfileButton->setEnabled( false );
  delProfileButton->setEnabled( false );
  saveProfileButton->setEnabled( false );
}

void ConnectionWidget::unlockInterface() {
  loginServerProfileComboBox->setEnabled( true );
  passwordLineEdit->setEnabled( true );
  singleSession->setEnabled( true );
  manualMultiSession->setEnabled( true );
  autoMultiSession->setEnabled( true );
  closeDialogCheckBox->setEnabled( true );

  profilesServerProfileComboBox->setEnabled( true );
  profileUserNameLineEdit->setEnabled( true );
  profileServerAddressLineEdit->setEnabled( true );
  profilePortSpinBox->setEnabled( true );
  newProfileButton->setEnabled( true );
  delProfileButton->setEnabled( true );
  saveProfileButton->setEnabled( true );
}

void ConnectionWidget::unlockRenameAndChangePassword() {
  // new username
  newUsernameLabel->setEnabled( true );
  newUsernameLineEdit->setEnabled( true );
  renameButton->setEnabled( true );

  // new password
  oldPasswordLineEdit->setEnabled( true );
  newPasswordLineEdit->setEnabled( true );

  oldPasswordLabel->setEnabled( true );
  newPasswordLabel->setEnabled( true );
  changePasswordButton->setEnabled( true );
}

void ConnectionWidget::lockRenameAndChangePassword() {
  // new username
  newUsernameLabel->setEnabled( false );
  newUsernameLineEdit->setEnabled( false );
  renameButton->setEnabled( false );

  // new password
  oldPasswordLineEdit->setEnabled( false );
  newPasswordLineEdit->setEnabled( false );
  oldPasswordLineEdit->setText( "" );
  newPasswordLineEdit->setText( "" );

  oldPasswordLabel->setEnabled( false );
  newPasswordLabel->setEnabled( false );
  changePasswordButton->setEnabled( false );
}

void ConnectionWidget::renameLoginName(){
  QString newUsername = newUsernameLineEdit->text();

  if ( newUsername == "" )
    return;
  serverContextState->sendMessage( QString( "RENAMEACCOUNT %1" ).arg( newUsername ) ) ;
  emit usernameChanged(newUsername);
}

void ConnectionWidget::renameLoginNameFeedbackSuccess( QString newName ) {
  QString renameString = QString( "Please change your new name to %1 and reconnect to the server." )
                         .arg( newName );
  QMessageBox::information( this, "Rename success", renameString );
}

void ConnectionWidget::renameLoginNameFeedbackFailure( QString failureMsg ) {
  QMessageBox::critical( this, "Rename failure", failureMsg );
}

void ConnectionWidget::changePassword(){

  QString oldPassword = oldPasswordLineEdit->text();
  QString newPassword = newPasswordLineEdit->text();
  if (( newPassword == "" ) || ( newPassword == oldPassword ) )
    return;
  QString a = serverContextState->encodePassword( oldPassword );
  QString b = serverContextState->encodePassword( newPassword );
  serverContextState->sendMessage( QString( "CHANGEPASSWORD %1 %2" )
                                   .arg( a )
                                   .arg( b ) ) ;
}

void ConnectionWidget::changePasswordSuccess( QString pwString ) {
  QMessageBox::information( this, "Password change success", pwString );
  //FIXME add direct modificator to profiles
}

void ConnectionWidget::changePasswordFailure( QString pwString ) {
  QMessageBox::critical( this, "Password change error", pwString );
}

void ConnectionWidget::registerNewAccount() {
  QString username = registerUsernameLineEdit->text();
  serverContextState->registerNewAccount( username );
}

void ConnectionWidget::show_if_wanted() {
  if ( autoMultiSession->isChecked() ) {
    establishConnection();
  }

  if (settings->value( "openDialogCheckBox", true ).toBool())
    show();
}
