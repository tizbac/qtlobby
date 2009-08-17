// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#include "ConnectionWidget.h"
#include <QProgressBar>

ConnectionWidget::ConnectionWidget( ServerContextState* serverContextState,
                                    QWidget *parent ) : QDialog( parent ) {
    setupUi( this );
    profilesListView->setModel(ServerProfilesModel::getInstance());
    this->serverContextState = serverContextState;
    connected = false;

    setWindowTitle( "Connection Settings" );

    settings = Settings::Instance();
    //addDefaultServers();

    splitter->restoreState(settings->value("connectionWidgetSplitter").toByteArray());

    tabWidget->setCurrentIndex( 0 );
    // limit the text blocks to 500
    QTextDocument * connectionLogTextDocument = new QTextDocument( this );
    connectionLogTextDocument->setMaximumBlockCount( 500 );
    connectionLogTextBrowser->setDocument( connectionLogTextDocument );

    showSimple = settings->value("simpleloginform", 0).toBool();
    if(showSimple)
        simpleViewStackedWidget->setCurrentIndex(1);
    else
        simpleViewStackedWidget->setCurrentIndex(0);
    simpleDetailedSwitch->setChecked(showSimple);
    simpleUrl = ServerProfilesModel::parseUrl(settings->value("simpleLogin").toString());
    simpleUsernameEdit->setText(simpleUrl.userName());
    simplePasswdEdit->setText(simpleUrl.password());
    simpleAutologin = settings->value("simpleAutologin", false).toBool();
    simpleAutologinChechbox->setChecked(simpleAutologin);
    simpleRememberPasswdChechbox->setChecked(!simpleUrl.password().isEmpty());

    /* backend connections for network connectivity */
    connect(simpleDetailedSwitch, SIGNAL(clicked()), this, SLOT(simpleViewChanged()));
    connect(simpleLoginButton, SIGNAL(clicked()), this, SLOT(establishSimpleConnection()));

    connect( loginButton, SIGNAL( clicked() ),
             this, SLOT( onLogin() ) );

    connect( logoutButton, SIGNAL( clicked() ),
             serverContextState, SLOT( forceDisconnect() ) );
    connect( simpleLogoutButton, SIGNAL( clicked() ),
             serverContextState, SLOT( forceDisconnect() ) );
    connect( this, SIGNAL( establishConnection_() ),
             serverContextState, SLOT( establishConnection() ) );

    /* logging */
    connect( serverContextState, SIGNAL( logWrite( QString ) ),
             this, SLOT( logWrite( QString ) ) );


    connect(autoLoginCheckBox,SIGNAL(toggled(bool)),
            this, SLOT(toggleAutoLogin(bool) ) );
    connect(rememberPassCheckBox,SIGNAL(toggled(bool)),
            this, SLOT(toggleRememberPassword(bool) ) );

    connect( profilesListView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)),
             this, SLOT(comboBoxCurrentIndexChanged(QModelIndex,QModelIndex)) );

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

    logoutButton->setEnabled(false);
    profilesListView->setCurrentIndex(ServerProfilesModel::getInstance()->index(0,0));
    tabWidget->setTabEnabled(2, false);
}

ConnectionWidget::~ConnectionWidget() { }

void ConnectionWidget::establishConnection() {
    QModelIndex index = profilesListView->currentIndex();

    if ( !index.isValid() ) {
        QMessageBox::critical( this, tr("toProfile selected"),
                               tr("You have to add a profile first before you can connect to a server.") );
        return;
    }
    QUrl url = ServerProfilesModel::getInstance()->data(index, Qt::UserRole).toUrl();
    if ( !rememberPassCheckBox->isChecked() ) {
        url.setPassword("");
        ServerProfilesModel::getInstance()->modifyProfile( index, url );
    }

    if ( passwordLineEdit->text() == "" ) {
        if ( url.password() == "" ) {
            QMessageBox::critical( this, tr("No password"),
                                   tr("Type in the password and try again!") );
            return;
        } else {
            passwordLineEdit->setText( url.password() );
        }
    } else {
        url.setPassword( passwordLineEdit->text() );
    }

    if ( rememberPassCheckBox->isChecked() ){
        ServerProfilesModel::getInstance()->modifyProfile( index, url );
    }

    emit emitConfiguration( url );
    emit establishConnection_();
    emit usernameChanged(url.userName());
}

void ConnectionWidget::simpleViewChanged() {
    bool showSimple = !settings->value("simpleloginform", 0).toBool();
    settings->setValue( "simpleloginform", showSimple);
    if(showSimple)
        simpleViewStackedWidget->setCurrentIndex(1);
    else
        simpleViewStackedWidget->setCurrentIndex(0);
}

/* simple login for lulz */
void ConnectionWidget::establishSimpleConnection() {
    if ( simpleUsernameEdit->text().isEmpty() ) {
        QMessageBox::critical( this, tr("No username"),
                               tr("Type in the username and try again!") );
        return;
    }

    if ( simplePasswdEdit->text().isEmpty() ) {
        QMessageBox::critical( this, tr("No password"),
                               tr("Type in the password and try again!") );
        return;
    }

    simpleUrl.setHost( "springrts.com" );
    simpleUrl.setPort( 8200 );
    simpleUrl.setUserName( simpleUsernameEdit->text() );
    simpleUrl.setPassword( simplePasswdEdit->text() );
    simpleUrl.setScheme("tas");

    emit emitConfiguration( simpleUrl );
    emit establishConnection_();
    emit usernameChanged(simpleUrl.userName());
    if(!simpleRememberPasswdChechbox->isChecked())
        simpleUrl.setPassword(QString::null);
    settings->setValue("simpleLogin", simpleUrl.toString());
}

void ConnectionWidget::logWrite( QString l ) {
    QString time = QTime().currentTime().toString( "hh:mm:ss " );
    connectionLogTextBrowser->append( time + l );
}

//// once the new profile wizzard is working, we can remove this code (js)
//void ConnectionWidget::addDefaultServers() {
//    QList<QVariant> list = settings->value( "ServerProfiles" ).toList();
//    if ( list.size() > 0 )
//        return;
//
//    QUrl url;
//    url.setHost( "taspring.clan-sy.com" );
//    url.setPort( 8200 );
//    url.setUserName( "qtlobby" );
//
//    list.append( url );
//    settings->setValue( "ServerProfiles", list );
//}

// after a modification of a server profile we have to reload the
// combobox with the new entries. in the profile editor we also need
// to update the selected profile's settings
// we need to disconnect/connect the signals to avoid a infinite loop
void ConnectionWidget::comboBoxCurrentIndexChanged(QModelIndex current, QModelIndex /*previous*/) {
    QUrl url = ServerProfilesModel::getInstance()->data(current, Qt::UserRole).toUrl();
    profileUserNameLineEdit->blockSignals(true);
    profileUserNameLineEdit->setText( url.userName() );
    profileUserNameLineEdit->blockSignals(false);
    profileServerAddressLineEdit->blockSignals(true);
    profileServerAddressLineEdit->setText( url.host() );
    profileServerAddressLineEdit->blockSignals(false);
    profilePortSpinBox->blockSignals(true);
    profilePortSpinBox->setValue( url.port() );
    profilePortSpinBox->blockSignals(false);
    passwordLineEdit->blockSignals(true);
    passwordLineEdit->setText(url.password());
    passwordLineEdit->blockSignals(false);
    rememberPassCheckBox->blockSignals(true);
    rememberPassCheckBox->setChecked(!url.password().isEmpty());
    rememberPassCheckBox->blockSignals(false);
    autoLoginCheckBox->blockSignals(true);
    autoLoginCheckBox->setChecked(ServerProfilesModel::getInstance()->isAutologin(profilesListView->currentIndex()));
    autoLoginCheckBox->blockSignals(false);
}

void ConnectionWidget::createNewProfile() {
    ServerProfilesModel::getInstance()->createProfile();
}

void ConnectionWidget::delSelectedProfile() {
    ServerProfilesModel::getInstance()->removeProfile(profilesListView->currentIndex());
}

void ConnectionWidget::connectionStatusChanged( ConnectionState state ) {
    switch ( state ) {
    case DISCONNECTED:
        connected = false;
        simpleLoginButton->setEnabled(true);
        simpleLogoutButton->setEnabled( false );
        logoutButton->setEnabled( false );
        loginButton->setEnabled( true );
        statusLabel->setText( "unconnected" );
        //unlockInterface();
        lockRenameAndChangePassword();
        break;
    case CONNECTING:
        loginButton->setText(tr("&Login"));
        loginButton->setEnabled( false );
        logoutButton->setEnabled( true );
        simpleLogoutButton->setEnabled( true );
        statusLabel->setText( tr("connecting") );
        //lockInterface();
        break;
    case CONNECTED:
        connected = true;
        statusLabel->setText( tr("connected") );
        break;
    case AUTHENTICATING:
        statusLabel->setText( tr("authenticating") );
        break;
    case AUTHENTICATED:
        simpleLoginButton->setEnabled(false);
        loginButton->setEnabled(true);
        unlockRenameAndChangePassword();
        statusLabel->setText( tr("authenticated (logged in)") );
        hide();
        break;
    }
}

void ConnectionWidget::lockInterface() {
    tabWidget->widget(0)->setEnabled(false);
}

void ConnectionWidget::unlockInterface() {
    tabWidget->widget(0)->setEnabled(true);
}

void ConnectionWidget::unlockRenameAndChangePassword() {
    tabWidget->setTabEnabled(2, true);
}

void ConnectionWidget::lockRenameAndChangePassword() {
    tabWidget->setTabEnabled(2, false);
}

void ConnectionWidget::renameLoginName() {
    QString newUsername = newUsernameLineEdit->text();

    if ( newUsername == "" )
        return;
    serverContextState->sendMessage( QString( "RENAMEACCOUNT %1" ).arg( newUsername ) ) ;
    emit usernameChanged(newUsername);
}

void ConnectionWidget::renameLoginNameFeedbackSuccess( QString newName ) {
    QString renameString = QString( tr("Please change your new name to %1 and reconnect to the server.") )
                           .arg( newName );
    QMessageBox::information( this, tr("Rename success"), renameString );
}

void ConnectionWidget::renameLoginNameFeedbackFailure( QString failureMsg ) {
    QMessageBox::critical( this, tr("Rename failure"), failureMsg );
}

void ConnectionWidget::changePassword() {

    QString oldPassword = oldPasswordLineEdit->text();
    if(newPasswordTry1LineEdit->text() != newPasswordTry2LineEdit->text()) {
        QMessageBox::critical( this, tr("Different password"), tr("Passwords you entered are different! Try again please.") );
        return;
    }
    QString newPassword = newPasswordTry1LineEdit->text();
    if (( newPassword == "" ) || ( newPassword == oldPassword ) )
        return;
    QString a = serverContextState->encodePassword( oldPassword );
    QString b = serverContextState->encodePassword( newPassword );
    serverContextState->sendMessage( QString( "CHANGEPASSWORD %1 %2" )
                                     .arg( a )
                                     .arg( b ) ) ;
}

void ConnectionWidget::changePasswordSuccess( QString pwString ) {
    QMessageBox::information( this, tr("Password change success"), pwString );
    //FIXME add direct modificator to profiles
}

void ConnectionWidget::changePasswordFailure( QString pwString ) {
    QMessageBox::critical( this, tr("Password change error"), pwString );
}

void ConnectionWidget::toggleRememberPassword(bool /*checked*/) {
    saveModifiedProfile();
}

void ConnectionWidget::toggleAutoLogin(bool checked) {
    //qDebug() << "set autologin: "<<  autoLoginCheckBox->isChecked();
    if(checked)
        ServerProfilesModel::getInstance()->setAutologin(profilesListView->currentIndex());
    else
        ServerProfilesModel::getInstance()->setAutologin(QModelIndex());
}

void ConnectionWidget::registerNewAccount() {
    QString username = desiredUsernamelineEdit->text();
    if(username.isEmpty()) {
        QMessageBox::critical( this, tr("Empty username"), tr("Please enter a username!") );
        return;
    }
    //qDebug() << "register new account "<< username;
    if(passwordTry1lineEdit->text() != passwordTry2lineEdit->text()) {
        QMessageBox::critical( this, tr("Different password"), tr("Passwords you entered are different! Try again please.") );
        return;
    }
    if(passwordTry1lineEdit->text().isEmpty()) {
        QMessageBox::critical( this, tr("Empty password"), tr("Please enter a password!") );
        return;
    }
    registrationServerContextState = new ServerContextState(this);
    /*registration feedback*/
    connect( registrationServerContextState, SIGNAL( incommingMessage(QString) ),
             this, SLOT( serverContextStateLoopback(QString)) );
    connect( registrationServerContextState, SIGNAL( registrationFailure(QString) ),
             this, SLOT( registrationFailFeedback(QString)) );
    connect( registrationServerContextState, SIGNAL( registrationSuccess(QString) ),
             this, SLOT( registrationSuccessFeedback(QString)) );
    configuration.setHost(newServerLineEdit->text());
    configuration.setPort(newPortSpinBox->value());
    registrationServerContextState->setConfiguration(configuration);
    registrationServerContextState->registerNewAccount(username, passwordTry1lineEdit->text());
    configuration.setUserName(username);
    configuration.setPassword(passwordTry1lineEdit->text());
    registerUserPushButton->setEnabled(false);
}

void ConnectionWidget::show_if_wanted() {
    if(showSimple) {
        if(simpleAutologin) {
            establishSimpleConnection();
        } else show();
    } else {
        if ( ServerProfilesModel::getInstance()->hasAutologin() ) {
            profilesListView->setCurrentIndex(ServerProfilesModel::getInstance()->getAutologinIndex());
            establishConnection();
        } else show();
    }
}

void ConnectionWidget::onLogin() {
    saveModifiedProfile();
    if(connected) {
        serverContextState->forceDisconnect();
        countdownTimer = new QTimer(this);
        countdownDialog = new QProgressDialog(tr("Reconnecting..."),tr("Cancel"),0,0,this);
        countdownDialog->setValue(0);
        countdownDialog->setWindowModality(Qt::WindowModal);
        countdownDialog->setLabelText(tr("Wait 5 seconds to reconnect"));
        countdownDialog->show();
        QTimer::singleShot(5000, this, SLOT(establishConnection()));
        QTimer::singleShot(5000, countdownDialog, SLOT(hide()));
        return;
    }
    establishConnection();
}


/**
  Handle pressing enter so that it triggers loggin in
  */
void ConnectionWidget::keyPressEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        onLogin();
    }
    QWidget::keyPressEvent(event);
}

void ConnectionWidget::saveModifiedProfile() {
    QModelIndex index = profilesListView->currentIndex();
    if(!index.isValid()) return;

    QUrl url;
    url.setUserName( profileUserNameLineEdit->text() );
    url.setHost( profileServerAddressLineEdit->text() );
    url.setPort( profilePortSpinBox->value() );
    if(rememberPassCheckBox->isChecked())
        url.setPassword( passwordLineEdit->text() );

    ServerProfilesModel::getInstance()->modifyProfile( index, url );
}

void ConnectionWidget::on_profileUserNameLineEdit_textChanged(QString ) {
    saveModifiedProfile();
}

void ConnectionWidget::on_passwordLineEdit_textChanged(QString ) {
    if(rememberPassCheckBox->isChecked())
        saveModifiedProfile();
}

void ConnectionWidget::on_profileServerAddressLineEdit_textChanged(QString ) {
    saveModifiedProfile();
}

void ConnectionWidget::on_profilePortSpinBox_valueChanged(int ) {
    saveModifiedProfile();
}

void ConnectionWidget::hideEvent(QHideEvent* /*event*/) {
    settings->setValue("connectionWidgetSplitter", splitter->saveState());
}

void ConnectionWidget::on_simpleAutologinChechbox_toggled(bool checked) {
    settings->setValue("simpleAutologin", checked);
}

void ConnectionWidget::registrationSuccessFeedback(QString info) {
    QMessageBox::information( this, tr("Registration was successful"), info );
    registrationServerContextState->deleteLater();
    registerUserPushButton->setEnabled(true);
    if(createProfileCheckBox->isChecked()) {
        ServerProfilesModel::getInstance()->createProfile(configuration);
    }
}

void ConnectionWidget::registrationFailFeedback(QString info) {
    QMessageBox::information( this, tr("Registration failed"), info );
    registrationServerContextState->deleteLater();
    registerUserPushButton->setEnabled(true);
}


void ConnectionWidget::serverContextStateLoopback(QString message) {
    registrationServerContextState->receiveCommand(Command(message));
}
