#include "buildserver.h"
#include <QtGui>

extern QWidget *peopVSpeop ;
BuildServer::BuildServer( QWidget *parent ) : QDialog( parent )
{
    label = new QLabel( QString::fromUtf8( "正在监听..." ) ) ;
    cancelPushButton = new QPushButton( QString::fromUtf8( "取消" ) , this ) ;
    connect( cancelPushButton , SIGNAL( clicked() ) , this , SLOT( cancelSlot() ) ) ;
    tableWidget = new QTableWidget( this ) ;
    tableWidget->setColumnCount( 2 ) ;
    tableWidget->setColumnWidth( 0 , 140 );
    tableWidget->setColumnWidth( 1 , 140 ) ;
    tableWidget->setRowCount( 0 ) ;
    QStringList headers;
    headers << "HostName" << "IP" ;
    tableWidget->setHorizontalHeaderLabels( headers ) ;

    tableWidget->setEditTriggers( QTableWidget::NoEditTriggers ) ;
    tableWidget->setSelectionBehavior( QTableWidget::SelectRows ) ;
    tableWidget->setSelectionMode( QTableWidget::SingleSelection ) ;
    buttonLayout = new QHBoxLayout ;
    mainLayout = new QVBoxLayout ;
    buttonLayout->addStretch() ;
    buttonLayout->addWidget( cancelPushButton ) ;
    buttonLayout->addStretch() ;
    mainLayout->addWidget( label ) ;
    mainLayout->addWidget( tableWidget ) ;
    mainLayout->addLayout( buttonLayout ) ;
    setLayout( mainLayout ) ;
    setFixedSize( 300 , 300) ;

    connect( tableWidget , SIGNAL( itemClicked( QTableWidgetItem * ) ) , this ,
                                   SLOT( onItemClicked ( QTableWidgetItem*) ) ) ;
    listen_udp() ;
}

void BuildServer::listen_udp()
{
    udpServer = new QUdpSocket( this ) ;
    udpServer->bind( 2089 , QUdpSocket::ShareAddress ) ;//udp端口号为2089
    connect( udpServer , SIGNAL( readyRead() ) , this , SLOT( readFromUdpClient() ) ) ;
}

void BuildServer::readFromUdpClient()
{
    QHostAddress aQHost ;
    quint16 port ;
    QByteArray datagram ;
    do
    {
        datagram.resize( udpServer->pendingDatagramSize() ) ;
        udpServer->readDatagram( datagram.data() , datagram.size() , &aQHost , &port ) ;
    }while( udpServer->hasPendingDatagrams() ) ;

    if( datagram[ 0 ] == '1' && datagram[ 1 ] == '2' && datagram[ 2 ] == 's' && datagram[ 3 ] == 's'
            &&  datagram[ 4 ] == 'Y' && datagram[ 5 ] == 'Y' &&  datagram[ 6 ] == 'h' && datagram[ 7 ] == 'h' ) //验证客户端的udp申请
    { 
        QString hostNameStr ;
        QRegExp regexp( "12ssYYhh(.*)" ) ;
        QString( datagram ).indexOf( regexp ) ;
        hostNameStr = regexp.cap( 1 ) ;

        int rowCnt = tableWidget->rowCount() ;
        tableWidget->insertRow( rowCnt ) ;
        tableWidget->setItem( rowCnt , 0 , new QTableWidgetItem( QString::fromUtf8( hostNameStr.toAscii() ) ) ) ;
        tableWidget->setItem( rowCnt , 1 , new QTableWidgetItem( aQHost.toString() ) ) ;
        datagram = ( "12ssYYhh" + QHostInfo::localHostName() ).toUtf8() ;
        udpServer->writeDatagram( datagram.data() , datagram.size() , aQHost , 7758 ) ;
    }
    else if( datagram == "TcpConnectSucess" )
    {
        label->setText( QString::fromUtf8( "已经建立连接" ) ) ;
        QMessageBox::StandardButton reply ;
        reply = QMessageBox::question( this , "" , QString::fromUtf8( "已经建立连接" ) ) ;
        if ( reply == QMessageBox::Ok )
        {
            udpServer->close() ;
            this->close() ;
        }
    }

}

void BuildServer::onItemClicked( QTableWidgetItem *item )
{
    qDebug() << "BuildServer: onItemClicked and emit toBuildTCP()" ;
    IP = tableWidget->item( item->row() , 1 )->text() ;
    connect( this , SIGNAL( toBuildTCP() ) , peopVSpeop , SLOT( create_server() ) ) ;
    emit toBuildTCP() ;   
}

void BuildServer::cancelSlot()
{
    udpServer->close() ;
    this->close() ;
}

//当PeopVSPeop tcp监听成功时，触发该槽
//向FindServer发送指令，让其连接TCP
void BuildServer::onListenSuccess()
{
    qDebug() << "BuildServer recieved listenSuccess() signal, command PleaseConnect to FindServer";
    QByteArray datagram = "PleaseConnect" ;
    udpServer->writeDatagram( datagram.data() , datagram.size() , QHostAddress( IP ) , 7758 ) ;
}
