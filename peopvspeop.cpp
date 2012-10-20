#include "peopvspeop.h"
#include <QtGui>

#define TO_UP 87 //棋局到图片顶端的像素距离
#define TO_LEFT 73 //棋局到图片左端的像素距离
#define DISTANCE 30 //两个棋子之间的距离为30
#define CENTER_X 283 //第8行第8列的x坐标
#define CENTER_Y 297 //第8行第8列的y坐标

#define SERVER '1'
#define CLIENT '2'

PeopVSPeop::PeopVSPeop( QWidget *parent ) : QDialog( parent )
{
    itemClickCnt = 0 ;
    pushBuild = false ;
    color = "YELLOW" ;
    game_start = false ;
    nodeStack.top = -1 ;

    memset( drawed , 0 , sizeof( drawed ) ) ;
    for( int i = 0 ; i < 15 ; ++ i )
    {
        for( int j = 0 ; j < 15 ; ++ j )
        {
            chessBoard[ i ][ j ] = '0' ;
        }
    }
    //设置背景图片
    QPalette palette ;
    QPixmap pixmap( ":/resource/image/chessboard.bmp" ) ;
    palette.setBrush( QPalette::Background , QBrush( pixmap ) ) ;
    setPalette( palette ) ;
    setFixedSize( pixmap.size() ) ;

    //设置图片
    blackImage = new QImage( ":/resource/image/black.bmp" ) ;
    whiteImage = new QImage( ":/resource/image/white.bmp" ) ;

    tcpServer = new TcpServer ;
    tcpClient = new QTcpSocket ;

    buildServerButton = new QPushButton( QString::fromUtf8( "建立主机" ) , this ) ;
    buildServerButton->setGeometry( 600 , 120 , 100 , 50  ) ;
    findServerButton = new QPushButton( QString::fromUtf8( "寻找主机" ) , this ) ;
    findServerButton->setGeometry( 600 , 220 , 100 , 50 ) ;
    cutConnectButton = new QPushButton( QString::fromUtf8( "断开连接")  , this ) ;
    cutConnectButton->setGeometry( 600 , 320 , 100 , 50 );
    exitButton = new QPushButton( QString::fromUtf8( "退出") , this ) ;
    exitButton->setGeometry( 600 , 420 , 100 , 50 );

    connect( buildServerButton , SIGNAL( clicked() ) , this , SLOT( build_slot() ) ) ;
    connect( findServerButton , SIGNAL( clicked() ) , this , SLOT( find_slot() ) ) ;
    connect( cutConnectButton , SIGNAL( clicked() ) , this , SLOT( disconnect_slot() ) ) ;
    connect( exitButton , SIGNAL( clicked() ) , this , SLOT( exitSlot() ) ) ;
}

void PeopVSPeop::build_slot()
{
    pushBuild = true ;
    me = SERVER ;
    color = "BLACK" ;

    aBuildServer = new BuildServer() ;
    aBuildServer->exec() ;
}

void PeopVSPeop::find_slot()
{
    pushBuild = false ;
    me = CLIENT ;
    color = "WHITE" ;

    aFindServer = new FindServer() ;
    aFindServer->exec() ;
}

//建立tcp连接,作为主机
void PeopVSPeop::create_server()
{
    //定义一个HostInfo的对象然后对这个对象的fromName函数进行初始化
	QHostInfo info = QHostInfo::fromName( QHostInfo::localHostName() ) ;
    QHostAddress address = info.addresses().value( 3 ) ; //info.addresses() 返回一个地址表，我们取第一个
    qDebug() << info.addresses() ;
    qDebug() << address.toString() ;
    if( ( itemClickCnt ++ == 0 ) && tcpServer->listen( address , 9786 ) )
    {
        qDebug() << "PeopVSPeop resived toBuildTCP() signal and listen sucess, emit listenSuccess()" ;
        connect( tcpServer , SIGNAL( msgcome() ) , this , SLOT( readFromClient() ) ) ;
        connect( this , SIGNAL( listenSuccess() ) , aBuildServer , SLOT( onListenSuccess() ) ) ;
        emit listenSuccess() ; 
    }
    else
    {
        qDebug() << "PeopVSPeop listen failed";
    }
}

//建立tcp连接,作为客户端
void PeopVSPeop::create_client()
{
    tcpClient->connectToHost( QHostAddress( aFindServer->F_IP ) , 9786 ) ;
    connect( tcpClient , SIGNAL( readyRead() ) , this , SLOT( readFromServer() ) ) ;

    if( tcpClient->ConnectedState == QAbstractSocket::ConnectedState )
    {
        qDebug() << "PeopVSPeop recieved pleaseConnectTcp() signal, connect success";
        connect( this ,SIGNAL( tcpConnectSuccess() ) , aFindServer , SLOT( onTcpConnectSuccess() ) ) ;
        emit tcpConnectSuccess() ;
    }

}

void PeopVSPeop::disconnect_slot()
{
    if( pushBuild )
    {
        if( tcpServer->isListening() )
        {
            tcpServer->disconnect() ;
            tcpServer->close() ;
            QMessageBox::warning( this , "" , QString::fromUtf8( "已经断开连接" ) ) ;
        }
        else if( tcpServer->isListening() == false )
        {
            QMessageBox::warning( this , "" , QString::fromUtf8( "没有建立连接" ) ) ;
        }
    }
    else
    {
        if( tcpClient->state() == QAbstractSocket::ConnectingState )
        {
            tcpClient->disconnect() ;
            tcpClient->close() ;
            QMessageBox::warning( this , "" , QString::fromUtf8( "已经断开连接" ) ) ;
        }
        else if( tcpClient->state() == QAbstractSocket::UnconnectedState )
        {
            QMessageBox::warning( this , "" , QString::fromUtf8( "没有建立连接" ) ) ;
        }
    }
}

//主机从客户端度读写
void PeopVSPeop::readFromClient()
{
    QDataStream in( tcpServer->tcpSocket );
    in.setVersion(QDataStream::Qt_4_3) ;
    in >> x >> y ;

    RenjuNode aRenjuNode ;
    aRenjuNode.x = x * DISTANCE + TO_LEFT - 13.5 ;
    aRenjuNode.y = y * DISTANCE + TO_UP - 14 ;
    aRenjuNode.color = "WHITE" ;
    nodeStack.nodes[ ++nodeStack.top ] = aRenjuNode ;

    drawed[ y ][ x ] = 1 ;
    chessBoard[ y ][ x ] = CLIENT ;
    repaint() ;

    if( win( chessBoard , y , x , CLIENT ) )
    {
        QMessageBox::StandardButton reply ;
        reply = QMessageBox::question( this , "" , "对不起，你输了，再接再厉！" ) ;
        if ( reply == QMessageBox::Ok )
        {
            delete this ;
        }
    }
    else if( nodeStack.top == 224 )
    {
        QMessageBox::StandardButton reply ;
        reply = QMessageBox::question( this , "" , QString::fromUtf8( "旗鼓相当，此局平局！" ) ) ;
        if ( reply == QMessageBox::Ok )
        {
            delete this ;
        }
    }
}

//客户端从主机读写
void PeopVSPeop::readFromServer()
{
    QDataStream in( tcpClient );
    in.setVersion(QDataStream::Qt_4_3) ;
    in >> x >> y ;

    RenjuNode aRenjuNode ;
    aRenjuNode.x = x * DISTANCE + TO_LEFT - 13.5 ;
    aRenjuNode.y = y * DISTANCE + TO_UP - 14 ;
    aRenjuNode.color = "BLACK" ;
    nodeStack.nodes[ ++nodeStack.top ] = aRenjuNode ;

    drawed[ y ][ x ] = 1 ;
    chessBoard[ y ][ x ] = SERVER ;

    repaint() ;

    if( win( chessBoard , y , x , SERVER ) )
    {
        QMessageBox::StandardButton reply ;
        reply = QMessageBox::question( this , "" , QString::fromUtf8( "对不起，你输了，再接再厉！" ) ) ;
        if ( reply == QMessageBox::Ok )
        {
            delete this ;
        }
    }
    else if( nodeStack.top == 224 )
    {
        QMessageBox::StandardButton reply ;
        reply = QMessageBox::question( this , "" , QString::fromUtf8( "旗鼓相当，此局平局！" ) ) ;
        if ( reply == QMessageBox::Ok )
        {
            delete this ;
        }
    }
}

void PeopVSPeop::mousePressEvent ( QMouseEvent  * e )
{
    int x_point = e->x() ;
    int y_point = e->y() ;
    if( ( x_point - TO_LEFT ) % DISTANCE < DISTANCE / 2 )
        pos_x = TO_LEFT + ( x_point - TO_LEFT ) / DISTANCE * DISTANCE ;
    else
        pos_x = TO_LEFT + ( ( x_point - TO_LEFT ) / DISTANCE + 1 ) * DISTANCE ;
    if( ( y_point - TO_UP ) % DISTANCE < DISTANCE / 2 )
        pos_y = TO_UP + ( y_point - TO_UP ) / DISTANCE * DISTANCE ;
    else
        pos_y = TO_UP + ( ( y_point - TO_UP ) / DISTANCE + 1 ) * DISTANCE ;


    x = ( pos_x - TO_LEFT ) / DISTANCE ;
    y = ( pos_y - TO_UP ) / DISTANCE ;
    if( pos_x >= 73 && pos_x <= 493 && pos_y >= 87 && pos_y <= 507 &&
            drawed[ y ][ x ] == 0 )
    {
        if( ( nodeStack.top == -1 ) || ( nodeStack.top >= 0 && color != nodeStack.nodes[ nodeStack.top ].color ) )
        {
            RenjuNode aRenjuNode ;
            aRenjuNode.x = x * DISTANCE + TO_LEFT - 13.5 ;
            aRenjuNode.y = y * DISTANCE + TO_UP - 14 ;
            aRenjuNode.color = color ;
            nodeStack.nodes[ ++nodeStack.top ] = aRenjuNode ;

            repaint() ;

            drawed[ y ][ x ] = 1 ;
            chessBoard[ y ][ x ] =  me ;

            QByteArray block ;
            QDataStream out( &block , QIODevice::WriteOnly ) ;
            out.setVersion( QDataStream::Qt_4_3 ) ;
            out << x << y ;
            if( pushBuild )
            {
               tcpServer->tcpSocket->write( block ) ;
            }
            else
            {
                tcpClient->write( block ) ;
            }

            if( win( chessBoard , y , x , me ) )
            {
                QMessageBox::StandardButton reply ;
                reply = QMessageBox::question( this , "" , QString::fromUtf8( "你太厉害了，膜拜一下！" ) ) ;
                if ( reply == QMessageBox::Ok )
                {
                    delete this ;
                }
            }
            else if( nodeStack.top == 224 )
            {
                QMessageBox::StandardButton reply ;
                reply = QMessageBox::question( this , "" , QString::fromUtf8( "旗鼓相当，此局平局！" ) ) ;
                if ( reply == QMessageBox::Ok )
                {
                    delete this ;
                }
            }
        }
    }
}

void PeopVSPeop::paintEvent( QPaintEvent *e )
{
    QPainter painter( this ) ;
    painter.setRenderHint( QPainter::Antialiasing , true ) ;
    for( int i = 0 ; i <= nodeStack.top ; ++ i )
    {
        if( nodeStack.nodes[ i ].color == "BLACK" )
            painter.drawImage( nodeStack.nodes[ i ].x , nodeStack.nodes[ i ].y , *blackImage ) ;
        else
            painter.drawImage( nodeStack.nodes[ i ].x , nodeStack.nodes[ i ].y , *whiteImage ) ;
    }
    painter.end() ;
}

void PeopVSPeop::exitSlot()
{
    delete this ;
}
