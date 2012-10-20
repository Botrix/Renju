#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    TcpServer( QObject *parent = 0 ) ;
    QTcpSocket *tcpSocket ;
signals:
    void msgcome() ;
protected:
    void incomingConnection( int socketDescriptor ) ;
private slots:
    void readFromTcpClient() ; 
} ;

#endif // TCPSERVER_H
