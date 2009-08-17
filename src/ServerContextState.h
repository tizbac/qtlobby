// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef SERVERCONTEXTSTATE_H
#define SERVERCONTEXTSTATE_H

#include <QRegExp>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QTime>
#include <QTimer>
#include <QUrl>
#include <QFile>

#include <QProcess>

#include "NetworkInterface.h"
#include "AbstractStateClient.h"
#include "Command.h"
#include "AgreementWidget.h"

class ServerContextState : public NetworkInterface {
    Q_OBJECT

    enum State {
        /* qt internal states */
        UnconnectedState,
        HostLookupState,
        ConnectingState,
        ConnectedState,
        BoundState,
        ClosingState,
        ListeningState,
        ConnectionRefusedError,
        RemoteHostClosedError,
        HostNotFoundError,
        SocketAccessError,
        SocketResourceError,
        SocketTimeoutError,
        DatagramTooLargeError,
        NetworkError,
        AddressInUseError,
        SocketAddressNotAvailableError,
        UnsupportedSocketOperationError ,
        ProxyAuthenticationRequiredError  ,
        UnknownSocketError,
        UnfinishedSocketOperationError,
    };

public:
    ServerContextState( QObject * parent = 0 );
    ~ServerContextState();
    void requestRename( QString newUsername );
    void registerNewAccount( QString user, QString password );
    QString encodePassword( QString password );
private:
    QUrl url; // server:port username:password for the TASServer is saved here
    bool keepaliveping;
    AgreementWidget* agreementWidget;
    QString agreement;
    bool registration;

signals:
    void connectionStateChanged( ConnectionState );
    void serverContextConnected();
    void serverContextReConnected();
    void logWrite( QString );
    void renameLoginNameSuccess( QString );
    void renameLoginNameFailure( QString );
    void changePasswordSuccess( QString );
    void changePasswordFailure( QString );
    void registrationSuccess( QString );
    void registrationFailure( QString );
public slots:
    void authenticate();
    void sendMessage( QString message );
    void receiveCommand( Command command );

    void stateChanged( QAbstractSocket::SocketState );
    void displayError( QAbstractSocket::SocketError );
    void establishConnection();
    void forceDisconnect();
    void setConfiguration( QUrl );
    QUrl getConfiguration( );
    void connectionStateChanged( ServerContextState::State state );

    void acceptAgreement();
    void denyAgreement();

private slots:
    void ping();
};

#endif
