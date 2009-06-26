#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <QObject>
#include <QTcpSocket>
#include <QUrl>

class NetworkInterface : public QObject
{
  Q_OBJECT
public:
  NetworkInterface( QObject * parent = 0 );
  ~NetworkInterface();
  void login();
  void socketInit( QUrl );

signals:
  void incommingMessage( QString message );

public slots:
  void socketWrite( QString out );
  void socketRead();

protected:
  QTcpSocket *tcpSocket;
};

#endif
