//
// C++ Interface: ServerContextState
//
// Description:
//
//
// Author: Joachim Schiele <js@lastlog.de>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SERVERCONTEXTSTATE_H
#define SERVERCONTEXTSTATE_H

#include <QRegExp>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QTime>
#include <QTimer>
#include <QUrl>

#include "NetworkInterface.h"
#include "AbstractStateClient.h"
#include "Command.h"
#include "AgreementWidget.h"

/**
 @author Joachim Schiele <js@lastlog.de>
*/
class ServerContextState : public NetworkInterface
{
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
  void registerNewAccount( QString );
  QString encodePassword( QString password );
private:
  QUrl url; // server:port username:password for the TASServer is saved here
  bool keepaliveping;
  AgreementWidget* agreementWidget;
  QString agreement;

signals:
  void connectionStateChanged( ConnectionState );
  void serverContextConnected();
  void serverContextReConnected();
  void logWrite( QString );
  void renameLoginNameSuccess( QString );
  void renameLoginNameFailure( QString );
  void changePasswordSuccess( QString );
  void changePasswordFailure( QString );
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
