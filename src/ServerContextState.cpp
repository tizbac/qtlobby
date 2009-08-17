// $Id$
// QtLobby released under the GPLv3, see COPYING for details.

#include "ServerContextState.h"
#include "Settings.h"

ServerContextState::ServerContextState( QObject * parent ) : NetworkInterface( parent ) {
    keepaliveping = false;
    agreementWidget = new AgreementWidget();

    connect( tcpSocket, SIGNAL( error( QAbstractSocket::SocketError ) ),
             this, SLOT( displayError( QAbstractSocket::SocketError ) ) );
    connect( tcpSocket, SIGNAL( stateChanged( QAbstractSocket::SocketState ) ),
             this, SLOT( stateChanged( QAbstractSocket::SocketState ) ) );

    connect( agreementWidget->acceptPushButton, SIGNAL( clicked() ),
             this, SLOT( acceptAgreement() ) );
    connect( agreementWidget->denyPushButton, SIGNAL( clicked() ),
             this, SLOT( denyAgreement() ) );
    registration = false;
}

ServerContextState::~ServerContextState() {
    delete agreementWidget;
}

void ServerContextState::stateChanged( QAbstractSocket::SocketState state ) {
    switch ( state ) {
    case QAbstractSocket::UnconnectedState:
        emit connectionStateChanged( ServerContextState::UnconnectedState );
        break;
    case QAbstractSocket::HostLookupState:
        emit connectionStateChanged( ServerContextState::HostLookupState );
        break;
    case QAbstractSocket::ConnectingState:
        emit connectionStateChanged( ServerContextState::ConnectingState );
        break;
    case QAbstractSocket::ConnectedState:
        emit connectionStateChanged( ServerContextState::ConnectedState );
        break;
    case QAbstractSocket::BoundState:
        emit connectionStateChanged( ServerContextState::BoundState );
        break;
    case QAbstractSocket::ClosingState:
        emit connectionStateChanged( ServerContextState::ClosingState );
        break;
    case QAbstractSocket::ListeningState:
        emit connectionStateChanged( ServerContextState::ListeningState );
        break;
    }
}

void ServerContextState::displayError( QAbstractSocket::SocketError a ) {
    switch ( a ) {
    case QAbstractSocket::ConnectionRefusedError:
        emit connectionStateChanged( ServerContextState::ConnectionRefusedError );
        break;
    case QAbstractSocket::RemoteHostClosedError:
        emit connectionStateChanged( ServerContextState::RemoteHostClosedError );
        break;
    case QAbstractSocket::HostNotFoundError:
        emit connectionStateChanged( ServerContextState::HostNotFoundError );
        break;
    case QAbstractSocket::SocketAccessError:
        emit connectionStateChanged( ServerContextState::SocketAccessError );
        break;
    case QAbstractSocket::SocketResourceError:
        emit connectionStateChanged( ServerContextState::SocketResourceError );
        break;
    case QAbstractSocket::SocketTimeoutError:
        emit connectionStateChanged( ServerContextState::SocketTimeoutError );
        break;
    case QAbstractSocket::DatagramTooLargeError:
        emit connectionStateChanged( ServerContextState::DatagramTooLargeError );
        break;
    case QAbstractSocket::NetworkError:
        emit connectionStateChanged( ServerContextState::NetworkError );
        break;
    case QAbstractSocket::AddressInUseError:
        emit connectionStateChanged( ServerContextState::AddressInUseError );
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        emit connectionStateChanged( ServerContextState::SocketAddressNotAvailableError );
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        emit connectionStateChanged( ServerContextState::UnsupportedSocketOperationError );
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        emit connectionStateChanged( ServerContextState::ProxyAuthenticationRequiredError );
        break;
    case QAbstractSocket::UnknownSocketError:
        emit connectionStateChanged( ServerContextState::UnknownSocketError );
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        emit connectionStateChanged( ServerContextState::UnfinishedSocketOperationError );
        break;
    default:;
    }
}

void ServerContextState::connectionStateChanged( State state ) {
    if(registration) return;
    switch ( state ) {
    case ConnectionRefusedError:
    case RemoteHostClosedError:
    case HostNotFoundError:
    case SocketAccessError:
    case SocketResourceError:
    case SocketTimeoutError:
    case DatagramTooLargeError:
    case NetworkError:
    case AddressInUseError:
    case SocketAddressNotAvailableError:
    case UnsupportedSocketOperationError:
    case ProxyAuthenticationRequiredError:
    case UnknownSocketError:
    case UnfinishedSocketOperationError:
        emit logWrite( QString( "<> %2 %3" )
                       .arg( "ServerContextState::State" )
                       .arg( tcpSocket->errorString() ) );
        return;
    case ConnectedState:
        emit connectionStateChanged( CONNECTED );
        QTimer::singleShot( 0, this, SLOT( ping() ) );
        keepaliveping = true;
        authenticate();
        return;
    case ConnectingState:
        emit connectionStateChanged( CONNECTING );
        break;
    case UnconnectedState:
        emit connectionStateChanged( DISCONNECTED );
        agreementWidget->hide();
    case HostLookupState:
        keepaliveping = false;
        return;
        // these 3 are not used
    case BoundState:
    case ClosingState:
    case ListeningState:
        return;
    }
}

void ServerContextState::ping() {
    if ( !keepaliveping ) {
        //        qDebug() << "no more sending of keepaliveping";
        return;
    }
    sendMessage( "PING 1" );
    QTimer::singleShot( 20000, this, SLOT( ping() ) );
}

void ServerContextState::setConfiguration( QUrl url ) {
    this->url = url;
}

void ServerContextState::establishConnection() {
    emit logWrite( QString( ">< %2 -> %3::%4" )
                   .arg( "address::port" )
                   .arg( url.host() )
                   .arg( url.port() ) );
    socketInit( url );
}

void ServerContextState::authenticate() {
    emit connectionStateChanged( AUTHENTICATING );
    QString user = url.userName();

    unsigned int cpu = 0;
#ifdef Q_WS_WIN
    QSettings settings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", QSettings::NativeFormat);
    cpu = settings.value("~MHz").toUInt();
#endif
#ifdef Q_WS_MAC
    QProcess *myProcess = new QProcess();
    myProcess->start("sysctl", QStringList() << "-n" << "hw.cpufrequency");
    myProcess->waitForFinished();

    QString result;
    while( myProcess->canReadLine())
        result= myProcess->readLine();
    cpu = result.toInt()/1000000;
    qDebug() << tr("Detected cpu frequency is: %1 MHz").arg(cpu);
#else
    QFile file("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << tr("Could not retreive CPU information.");
    QTextStream in(&file);
    cpu = in.readLine().toInt()/1000;
    file.close();
#endif
    QString lobbyclient = "qtlobby";
    QString version = QString( "0.0.%1-svn" ).arg( SVN_REV );
    QString loginstring = QString( "LOGIN %1 %2 %3 * %4 %5\n" )
                          .arg( url.userName() )
                          .arg( encodePassword( url.password() ) )
                          .arg( cpu )
                          .arg( lobbyclient )
                          .arg( version );
    sendMessage( loginstring );
}

void ServerContextState::forceDisconnect() {
    //   qDebug() << "forceDisconnect() called";
    tcpSocket->disconnectFromHost();
    keepaliveping = false;
    tcpSocket->close();
}

void ServerContextState::sendMessage( QString message ) {
    socketWrite( message.trimmed().append( "\n" ) );
    emit logWrite( message.prepend( ">> " ) );
}

void ServerContextState::receiveCommand( Command command ) {
    command.name = command.name.toUpper();
    emit logWrite( command.toQString().prepend( "<< " ) );
    if ( command.name == "ACCEPTED" ) {
        emit connectionStateChanged( AUTHENTICATED );
        //TODO add a QDate object to track how long the server connection lasts
    } else
        if ( command.name == "AGREEMENT" ) {
        agreement.append( command.attributes.join( " " ) ) ;
    } else
        if ( command.name == "AGREEMENTEND" ) {
        // dirty rtf strip hack (mw)
        agreement.replace( "\\pard", "" );
        agreement.replace( "\\par", " \n" );
        agreement.replace( QRegExp( "(\\\\[^\\\\ \\}\\{]+|[^\\\\;\\}\\{]+;)+" ), "" );
        agreement.replace( QRegExp( "\\{ ?| ?\\}" ), "" );
        agreement.replace( QRegExp( "\n ?" ), "\n" );
        agreement.replace( QRegExp( "^ " ), "" );
        agreementWidget->textBrowser->setText( agreement );
        agreement = "";
        agreementWidget->show();
    } else
        if ( command.name == "DENIED" ) {
        forceDisconnect();
        //TODO give a TASserver reason for as "Bad username/password"
    } else
        if ( command.name == "SERVERMSG" ) {
        if ( command.attributes.join( " " ).contains( QRegExp( "^RENAMEACCOUNT failed|Bad RENAMEACCOUNT" ) ) )
            emit renameLoginNameFailure( command.toQString() );
        if ( command.attributes.join( " " ).contains( QRegExp( "^Your account has been renamed to" ) ) ) {
            QString u = command.attributes[6];
            u.remove( 0, 1 );
            u.chop( 2 );
            emit renameLoginNameSuccess( u );
            // FIXME make change reflected in the profile automatically
            //   int index = loginServerProfileComboBox->property( "currentIndex" ).toInt();
            //   QUrl url = ServerContextState->getConfiguration;
            //   modifyServerProfile( index, url );
            //   show();
        }
        if ( command.attributes.join( " " ).contains( QRegExp( "^Bad CHANGEPASSWORD command" ) ) ) {
            // SERVERMSG Bad CHANGEPASSWORD command - too many or too few parameters
            QString u = command.attributes.join( " " );
            emit changePasswordFailure( u );
        }

        if ( command.attributes.join( " " ).contains( QRegExp( "^CHANGEPASSWORD failed:" ) ) ) {
            // SERVERMSG CHANGEPASSWORD failed: Old password is incorrect!
            QString u = command.attributes.join( " " );
            emit changePasswordFailure( u );
        }
        if ( command.attributes.join( " " ).contains( QRegExp( "^Your password has been successfully updated!" ) ) ) {
            // SERVERMSG Your password has been successfully updated!
            QString u = command.attributes.join( " " );
            emit changePasswordSuccess( u );
        }
    } else
        if ( command.name == "REGISTRATIONACCEPTED" ) {
        emit registrationSuccess(tr("Registration was successful. You can now log in with your account"));
    } else
        if ( command.name == "REGISTRATIONDENIED" ) {
        emit registrationFailure(command.attributes.join( " " ));
    }
}

QUrl ServerContextState::getConfiguration( ) {
    return url;
}

void ServerContextState::acceptAgreement() {
    sendMessage( "CONFIRMAGREEMENT" );
    emit logWrite( tr( ">> agreement accepted, trying to login now." ) );
    authenticate();
}

void ServerContextState::denyAgreement() {
    forceDisconnect();
    QString time = QTime().currentTime().toString( "hh:mm:ss" );
    emit logWrite( tr( ">> agreement denied, disconnecting from server." ) );
}

void ServerContextState::registerNewAccount( QString user, QString password ) {
    registration = true;
    establishConnection();
    qDebug() << QString("REGISTER %1 %2").arg(user).arg(password);
    sendMessage(QString("REGISTER %1 %2").arg(user).arg(password));
}

QString ServerContextState::encodePassword( QString password ) {
    QByteArray p( password.toAscii() );
    return ( QString ) QCryptographicHash::hash( p, QCryptographicHash::Md5 ).toBase64();
}
