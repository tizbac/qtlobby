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

    setWindowTitle( "Connection Settings" );

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


    connect(autoLoginCheckBox,SIGNAL(toggled(bool)),
            this, SLOT(toggleAutoLogin() ) );
    connect(rememberPassCheckBox,SIGNAL(toggled(bool)),
            this, SLOT(toggleRememberPassword() ) );

    connect( profileComboBox, SIGNAL( currentIndexChanged( int ) ),
             this, SLOT( comboBoxCurrentIndexChanged( int ) ) );

    connect( saveProfileButton, SIGNAL( clicked() ),
             this , SLOT( saveModifiedProfile() ) );

    connect( registerUserPushButton, SIGNAL( clicked( ) ),
             this, SLOT( registerNewAccount( ) ) );

    /* buttons from the profiles dialog */
    connect( newProfileButton, SIGNAL( clicked() ),
             this , SLOT( createNewProfile() ) );

    connect( delProfileButton, SIGNAL( clicked() ),
             this , SLOT( delSelectedProfile() ) );

    //connect( connectionWizzardButton, SIGNAL( clicked() ),
    //        this , SLOT( openProfileEditor() ) );

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

    if ( settings->value( "autologin", false ).toBool() )
        autoLoginCheckBox->setChecked( true );
    if ( settings->value( "rememberpasswd", false).toBool() )
        rememberPassCheckBox->setChecked( true );

    logoutButton->setEnabled(false);
    registerUserPushButton->setEnabled(false); // Remove when registering is working correctly

    updateComboBoxes();

}

ConnectionWidget::~ConnectionWidget() { }

void ConnectionWidget::establishConnection() {
    int index = profileComboBox->property( "currentIndex" ).toInt();
    settings->setValue( "SelectedServerProfile", index );

    if ( index == -1 ) {
        QMessageBox::critical( this, "No Profile selected",
                               "You have to add a profile in the 'Profile' tab first before you can connect to a server." );
        return;
    }
    QUrl url = profileComboBox->itemData(index, Qt::UserRole ).toUrl();
    if ( !rememberPassCheckBox->isChecked() ) {
        qDebug("Connecting... Forget password");
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

    if ( rememberPassCheckBox->isChecked() ){
        modifyServerProfile( index, url );
        qDebug("Connecting... Remember password");
    }
    emit emitConfiguration( url );
    emit establishConnection_();
    emit usernameChanged(url.userName());
}

void ConnectionWidget::logWrite( QString l ) {
    //if ( this->isVisible() ) {
    QString time = QTime().currentTime().toString( "hh:mm:ss " );
    connectionLogTextBrowser->append( time + l );
    //}
}

void ConnectionWidget::modifyServerProfile( signed int index, QUrl url )
{

    if ( index == -1 ) {
        qDebug() << "not modifying server profile";
        return ;
    }
    QList<QVariant> list = settings->value( "ServerProfiles" ).toList();
    list[index] = url;
    qDebug() << "modifying server profile "<< index;
    settings->setValue( "ServerProfiles", list );
    updateComboBoxes();
}

// once the new profile wizzard is working, we can remove this code (js)
void ConnectionWidget::addDefaultServers()
{
    QList<QVariant> list = settings->value( "ServerProfiles" ).toList();
    if ( list.size() > 0 )
        return;

    QUrl url;
    url.setHost( "taspring.clan-sy.com" );
    url.setPort( 8200 );
    url.setUserName( "qtlobby" );

    list.append( url );
    settings->setValue( "ServerProfiles", list );
}

// after a modification of a server profile we have to reload the
// combobox with the new entries. in the profile editor we also need
// to update the selected profile's settings
// we need to disconnect/connect the signals to avoid a infinite loop
void ConnectionWidget::comboBoxCurrentIndexChanged( int index ) {
    QUrl url = profileComboBox->itemData(
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
    int index = profileComboBox->property( "currentIndex" ).toInt();
    qDebug()<< "Index of saved profile: "<< index;
    settings->setValue( "SelectedServerProfile", index );

    QUrl url;
    url.setUserName( profileUserNameLineEdit->text() );
    url.setHost( profileServerAddressLineEdit->text() );
    url.setPort( profilePortSpinBox->value() );
    qDebug()<<"Modify profile.";

    modifyServerProfile( index, url );
}

// this function handles the change of combobox entries which happen
// when a profile gets modified or deleted. it will also select the
// previously selected 'server profile' in both 'login' and 'server' tab
void ConnectionWidget::updateComboBoxes()
{
    qDebug() << "Update comboboxes";
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
    profileComboBox->clear();
    int index = settings->value( "SelectedServerProfile" ).toInt();

    for ( int i = 0 ; i < list.size(); ++i )
    {
        QUrl url = list[i].toUrl();
        QString urldescription = QString( "%1 : %2 @ %3 : %4 " )
                                 .arg( url.userName() )
                                 .arg( QString( url.password().size(), QChar( '*' ) ) )
                                 .arg( url.host() )
                                 .arg( url.port() );
        if ( i == index )
            if( rememberPassCheckBox->isChecked()) {
            qDebug() << "Laeta paekallee";
            passwordLineEdit->setText( url.password() );
        }

        profileComboBox->insertItem( i, QString( urldescription ), url );
    }
    profileComboBox->setCurrentIndex( index );
}

void ConnectionWidget::createNewProfile()
{
    qDebug()<<"New Server Profile";
    QList<QVariant> list = settings->value( "ServerProfiles" ).toList();

    QUrl url;
    url.setHost( "taspring.clan-sy.com" );
    url.setPort( 8200 );
    url.setUserName( "qtlobby" );

    //list.append( url );
    list.prepend( url);
    settings->setValue( "ServerProfiles", list );
    updateComboBoxes();
}

void ConnectionWidget::delSelectedProfile() {
    int index = profileComboBox->property( "currentIndex" ).toInt();
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
        registerUserPushButton->setEnabled(false); // change to true when registering is working correctly
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
        registerUserPushButton->setEnabled(false);
        hide();
        break;
    }
}

void ConnectionWidget::lockInterface()
{
    profileComboBox->setEnabled( false );
    passwordLineEdit->setEnabled( false );
    //autoLoginCheckBox->setEnabled(false);
    profileComboBox->setEnabled( false );
    profileUserNameLineEdit->setEnabled( false );
    profileServerAddressLineEdit->setEnabled( false );
    profilePortSpinBox->setEnabled( false );
    newProfileButton->setEnabled( false );
    delProfileButton->setEnabled( false );
    saveProfileButton->setEnabled( false );
}

void ConnectionWidget::unlockInterface()
{
    profileComboBox->setEnabled( true );
    passwordLineEdit->setEnabled( true );
    autoLoginCheckBox->setEnabled(true);
    profileComboBox->setEnabled( true );
    profileUserNameLineEdit->setEnabled( true );
    profileServerAddressLineEdit->setEnabled( true );
    profilePortSpinBox->setEnabled( true );
    newProfileButton->setEnabled( true );
    delProfileButton->setEnabled( true );
    saveProfileButton->setEnabled( true );
}

void ConnectionWidget::unlockRenameAndChangePassword()
{
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

void ConnectionWidget::lockRenameAndChangePassword()
{
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

void ConnectionWidget::renameLoginNameFeedbackSuccess( QString newName )
{
    QString renameString = QString( "Please change your new name to %1 and reconnect to the server." )
                           .arg( newName );
    QMessageBox::information( this, "Rename success", renameString );
}

void ConnectionWidget::renameLoginNameFeedbackFailure( QString failureMsg )
{
    QMessageBox::critical( this, "Rename failure", failureMsg );
}

void ConnectionWidget::changePassword()
{

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

void ConnectionWidget::toggleRememberPassword()
{
    qDebug() << "set rememberpasswd: "<< rememberPassCheckBox->isChecked();
    settings->setValue( "rememberpasswd", rememberPassCheckBox->isChecked() );
}

void ConnectionWidget::toggleAutoLogin(){
    qDebug() << "set autologin: "<<  autoLoginCheckBox->isChecked();
    settings->setValue( "autologin", autoLoginCheckBox->isChecked() );
}

void ConnectionWidget::registerNewAccount() {
    QString username = profileUserNameLineEdit->text();
    qDebug() << "register new account "<< username;
    serverContextState->registerNewAccount( username );
}

void ConnectionWidget::show_if_wanted() {
    if ( autoLoginCheckBox->isChecked() ) {
        qDebug("Autologin");
        establishConnection();
    } else show();
}
