#include "findserver.h"
#include <QtGui>

extern QWidget *peopVSpeop ;
FindServer::FindServer( QWidget *parent ) : QDialog( parent )
{
    tcpSocket = new QTcpSocket( this) ;
    label = new QLabel( QString::fromUtf8( "正在寻找..." ) ) ;
    tableWidget = new QTableWidget( this ) ;
    tableWidget->setColumnCount( 2 ) ;
    tableWidget->setColumnWidth( 0 , 140 );
    tableWidget->setColumnWidth( 1 , 140 ) ;
    tableWidget->setRowCount( 0 ) ;

    QStringList headers ;
    headers << "HostName" << "IP" ;
    tableWidget->setHorizontalHeaderLabels( headers ) ;

    tableWidget->setEditTriggers( QTableWidget::NoEditTriggers ) ;
    tableWidget->setSelectionBehavior( QTableWidget::SelectRows ) ;
    tableWidget->setSelectionMode( QTableWidget::SingleSelection ) ;
    cancelButton = new QPushButton( QString::fromUtf8( "取消" ) , this ) ;
    buttonLayout = new QHBoxLayout ;
    mainLayout = new QVBoxLayout ;
    buttonLayout->addStretch() ;
    buttonLayout->addWidget( cancelButton ) ;
    buttonLayout->addStretch() ;
    mainLayout->addWidget( label ) ;
    mainLayout->addWidget( tableWidget ) ;
    mainLayout->addLayout( buttonLayout ) ;
    setLayout( mainLayout ) ;
    setFixedSize( 300 , 300) ;

    find_server() ;
    connect( cancelButton , SIGNAL( clicked() ) , this , SLOT( cancelSlot() ) )  ;
}

void FindServer::find_server()
{
    serverFinderSocket = new QUdpSocket( this ) ;
    serverFinderSocket->bind( 7758 , QUdpSocket::ShareAddress ) ; //先建立监听，准备读server返回值
    connect( serverFinderSocket , SIGNAL( readyRead() ) , this , SLOT( readFromUdpServer() ) ) ;
    QString str = "12ssYYhh" + QHostInfo::localHostName() ;
    QByteArray datagram = str.toUtf8() ;
    serverFinderSocket->writeDatagram( datagram.data() , datagram.size() , QHostAddress::Broadcast , 2089 ) ;
}

void FindServer::readFromUdpServer()
{
    QHostAddress aQHost ;
    quint16 port = 0 ;
    QByteArray datagram ;
    do
    {
        datagram.resize( serverFinderSocket->pendingDatagramSize() ) ;
        serverFinderSocket->readDatagram( datagram.data() , datagram.size() , &aQHost , &port ) ;
    }while( serverFinderSocket->hasPendingDatagrams() ) ;

    if( datagram[ 0 ] == '1' && datagram[ 1 ] == '2' && datagram[ 2 ] == 's' && datagram[ 3 ] == 's'
            &&  datagram[ 4 ] == 'Y' && datagram[ 5 ] == 'Y' &&  datagram[ 6 ] == 'h' && datagram[ 7 ] == 'h' )
    {
        QString hostNameStr ;
        QRegExp regexp( "12ssYYhh(.*)" ) ;
        QString( datagram ).indexOf( regexp ) ;
        hostNameStr = regexp.cap( 1 ) ;

        int rowCnt = tableWidget->rowCount() ;
        tableWidget->insertRow( rowCnt ) ;
        tableWidget->setItem( rowCnt , 0 , new QTableWidgetItem( QString::fromUtf8( hostNameStr.toAscii() ) ) ) ;
        tableWidget->setItem( rowCnt , 1 , new QTableWidgetItem( aQHost.toString() ) ) ;
    }
    else if( datagram == "PleaseConnect" )
    {
        F_IP = aQHost.toString() ;
        connect( this ,SIGNAL( pleaseConnectTcp() ) , peopVSpeop , SLOT( create_client() ) ) ;
        emit pleaseConnectTcp() ;
        qDebug() << "FindServer recieved PleaseConnect, emit pleaseConnectTcp()";
    }
}

void FindServer::onTcpConnectSuccess()
{
    label->setText( QString::fromUtf8( "连接成功" ) ) ;
    QByteArray datagram = "TcpConnectSucess" ;
    serverFinderSocket->writeDatagram( datagram.data() , datagram.size() , QHostAddress( F_IP ) , 2089 ) ;

    QMessageBox::StandardButton reply ;
    reply = QMessageBox::question( this , "" , QString::fromUtf8( "已经建立连接" ) ) ;
    if ( reply == QMessageBox::Ok )
    {
        serverFinderSocket->close() ;
        this->close() ;
    }
}

void FindServer::cancelSlot()
{
    serverFinderSocket->close();
    this->close() ;
}
