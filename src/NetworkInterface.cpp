#include "NetworkInterface.h"

NetworkInterface::NetworkInterface( QObject * parent ) : QObject( parent ) {
  tcpSocket = new QTcpSocket( this );
  connect( tcpSocket, SIGNAL( readyRead() ), this, SLOT( socketRead() ) );
}

NetworkInterface::~NetworkInterface() {
  delete tcpSocket;
}

void NetworkInterface::socketInit( QUrl url ) {
  tcpSocket->abort();
  tcpSocket->connectToHost( url.host(), url.port() );
}

/**
 * This method called when new data is available on the socket.
 * If there is at least one whole line in the buffer we read
 * all lines and send them out.
 */
void NetworkInterface::socketRead() {
  while ( tcpSocket->canReadLine() )
    emit incommingMessage( QString::fromUtf8( tcpSocket->readLine() ) );
}

void NetworkInterface::socketWrite( QString out ) {
#ifdef NETWORKDEBUG
  qDebug() << " >> " << out.trimmed();
#endif
  tcpSocket->write( out.toUtf8() );
}
