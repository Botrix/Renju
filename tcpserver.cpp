#include "tcpserver.h"
#include <iostream>

TcpServer::TcpServer( QObject *parent ) : QTcpServer( parent )
{
}

void TcpServer::incomingConnection( int socketDescriptor )
{
	tcpSocket = new QTcpSocket() ;
	if( !tcpSocket->setSocketDescriptor( socketDescriptor ) )
	{
		std::cout << "Fail to setSocketDescriprot!" << std::endl ;
		return;
	}
	else
	{
		std::cout << "connect tcp success!!" << std::endl ;
	}
	connect( tcpSocket , SIGNAL( readyRead() ) , this , SLOT( readFromTcpClient() ) ) ;
	connect( tcpSocket , SIGNAL( disconnected() ) , tcpSocket , SLOT( deleteLater() ) ) ;
}

void TcpServer::readFromTcpClient()
{
	emit msgcome() ;
}
