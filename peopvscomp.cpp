/*
 *Copyright (C) 2011 @HIT FooToo Lab
 *Author: 时彦辉
 *Create Date: 2011-4-15
 */
#include "peopvscomp.h"
#include <QtGui>

#define TO_UP 87 //棋局到图片顶端的像素距离
#define TO_LEFT 73 //棋局到图片左端的像素距离
#define DISTANCE 30 //两个棋子之间的距离为30
#define CENTER_X 283 //第8行第8列的x坐标
#define CENTER_Y 297 //第8行第8列的y坐标

PeopVSComp::PeopVSComp( QWidget *parent ) : QDialog( parent )
{
    //设置背景图片
    QPalette palette ;
    QPixmap pixmap( ":/resource/image/chessboard.bmp" ) ;
    palette.setBrush( QPalette::Background , QBrush( pixmap ) ) ;
    setPalette( palette ) ;
    setFixedSize( pixmap.size() ) ;

    //用正则表达式获得项目的绝对路径
    QRegExp regexp( "(.*)Renju-build-desktop" ) ;
    QDir::currentPath().indexOf( regexp ) ;
    projectPath = regexp.cap( 1 ) + "Renju/" ;

    //设置声音
    qDebug() << projectPath ;
    moveSound = new QSound( projectPath + "resource/sound/move.wav" ) ;
    winSound = new QSound( projectPath + "resource/sound/win.wav" ) ;
    loseSound = new QSound( projectPath + "resource/sound/lose.wav " ) ;

    //设置图片
    blackImage = new QImage( ":/resource/image/black.bmp" ) ;
    whiteImage = new QImage( ":/resource/image/white.bmp" ) ;

    //设置默认值
    audio = new bool ;
    p_move_first = new bool ;
    difficulty_degree = new int ;
    *audio = false ;
    *p_move_first = false ;
    *difficulty_degree = MID ;
    player_win = 0 ;

    //按钮布局
    configPushButton = new QPushButton( QString::fromUtf8( "设置" ) , this ) ;
    configPushButton->setGeometry( 600 , 120 , 100 , 50 ) ;
    startPushButton = new QPushButton( QString::fromUtf8( "开始" ) , this ) ;
    startPushButton->setGeometry( 600 , 220 , 100, 50 ) ;
    backStepPushButton = new QPushButton( QString::fromUtf8( "悔棋" ) , this ) ;
    backStepPushButton->setGeometry( 600 , 320 , 100 , 50 ) ;
    exitPushButton = new QPushButton( QString::fromUtf8( "退出" ) , this ) ;
    exitPushButton->setGeometry( 600 , 420 , 100 , 50 ) ;

    //建立信号-槽
    connect( configPushButton , SIGNAL( clicked() ) , this , SLOT( config() ) ) ;
    connect( startPushButton , SIGNAL( clicked() ) , this , SLOT( startGame() ) ) ;
    connect( backStepPushButton , SIGNAL( clicked()) , this , SLOT( backOneStep()) ) ;
    connect( exitPushButton , SIGNAL( clicked()) , this , SLOT( exitGame()) ) ;

    //设置重绘标记
    black_move_flag = true ;//先走的用黑棋

    //设置重绘位置默认值
    pos_x = CENTER_X ;
    pos_y = CENTER_Y ;

    //游戏开始之前不能重绘
    game_start = false ;

    //设置栈顶
    nodeStack.top = -1 ;

    //开始时是空白的
    memset( drawed , 0 , sizeof( drawed ) ) ;
    for( int i = 0 ; i < MAX_LINE ; i ++ )
    {
        for( int j = 0 ; j < MAX_LINE ; j ++ )
        {
            chessBoard[ i ][ j ] = EMPTY ;
        }
    }
}

void PeopVSComp::config()
{
    Configuration *aConfiguration = new Configuration( audio , p_move_first , difficulty_degree , this ) ;
    aConfiguration->exec() ;
}


void PeopVSComp::startGame()
{
    configPushButton->setDisabled( true ) ;
    startPushButton->setDisabled( true ) ;
    game_start = true ;

    if( *difficulty_degree == LOW )
    {
        search_depth = 2 ;
        search_width = 2 ;
    }
    else if( *difficulty_degree == MID )
    {
        search_depth = 2 ;
        search_width = 4 ;
    }
    else
    {
        search_depth = 3 ;
        search_width = 6 ;
    }


    if( !(*p_move_first) )
    {
        RenjuNode aNode ;
        aNode.x = CENTER_X - 13.5 ;
        aNode.y = CENTER_Y - 14 ;
        aNode.color = "BLACK" ;
        nodeStack.nodes[ ++nodeStack.top ] = aNode ;
        drawed[ 7 ][ 7 ] = 1 ;
        chessBoard[ 7 ][ 7 ] = MYSELF ;
        if( *audio )
            moveSound->play() ;
        repaint() ;
    }
}

void PeopVSComp::backOneStep()
{
}

void PeopVSComp::exitGame()
{
    QMessageBox::StandardButton reply ;
    reply = QMessageBox::question( this , "Warning" , QString::fromUtf8("真的想退出吗?") ,
                                   QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes ) ;
    if ( reply == QMessageBox::Yes )
        this->close() ;
}

void PeopVSComp::mousePressEvent( QMouseEvent  * e )
{
    if( game_start )
    {
        int x = e->x() ;
        int y = e->y() ;
        if( ( x - TO_LEFT ) % DISTANCE < DISTANCE / 2 )
            pos_x = TO_LEFT + ( x - TO_LEFT ) / DISTANCE * DISTANCE ;
        else
            pos_x = TO_LEFT + ( ( x - TO_LEFT ) / DISTANCE + 1 ) * DISTANCE ;
        if( ( y - TO_UP ) % DISTANCE < DISTANCE / 2 )
            pos_y = TO_UP + ( y - TO_UP ) / DISTANCE * DISTANCE ;
        else
            pos_y = TO_UP + ( ( y - TO_UP ) / DISTANCE + 1 ) * DISTANCE ;

        RenjuNode aNode ;
        aNode.x = pos_x - 13.5;
        aNode.y = pos_y - 14 ;
        if( !(*p_move_first) )
            aNode.color = "WHITE" ;
        else
            aNode.color = "BLACK" ;


        int chessboard_x = ( pos_x - TO_LEFT ) / DISTANCE ;
        int chessboard_y = ( pos_y - TO_UP ) / DISTANCE ;
        qDebug() << "not saved" << chessboard_x << chessboard_y ;
        if( pos_x >= 73 && pos_x <= 493 && pos_y >= 87 && pos_y <= 507 &&
                drawed[ chessboard_y ][ chessboard_x ] == 0 )
        {
            qDebug() << chessboard_x << chessboard_y ;
            nodeStack.nodes[ ++nodeStack.top ] = aNode ;
            if( *audio )
                moveSound->play() ;
            repaint() ;
            drawed[ chessboard_y ][ chessboard_x ] = 1 ;
            chessBoard[ chessboard_y ][ chessboard_x ] =  RIVAL ;
            if( win( chessBoard ,chessboard_y , chessboard_x , RIVAL ) )
            {
                winSound->play() ;
                QMessageBox::StandardButton reply ;
                reply = QMessageBox::question( this , "" , QString::fromUtf8( "你太厉害了，膜拜一下！" ) ) ;
                if ( reply == QMessageBox::Ok )
                {
                    this->close() ;
                }
            }
            computer_move();
        }
    }
}

void PeopVSComp::paintEvent( QPaintEvent *e )
{
    QPainter painter( this ) ;
    painter.setRenderHint( QPainter::Antialiasing , true ) ;
    if( game_start )
    {
        for( int i = 0 ; i <= nodeStack.top ; ++ i )
        {
            if( nodeStack.nodes[ i ].color == "BLACK" )
                painter.drawImage( nodeStack.nodes[ i ].x , nodeStack.nodes[ i ].y , *blackImage ) ;
            else
                painter.drawImage( nodeStack.nodes[ i ].x , nodeStack.nodes[ i ].y , *whiteImage ) ;
        }
        painter.end() ;
    }
}

void PeopVSComp::computer_move()
{
    Search search( search_depth , search_width , chessBoard ) ;
    search.alphaBetaSearch( 1 , -INIT_MAX , INIT_MAX , MYSELF ) ;
    pos_x = TO_LEFT + search.bestMove.y * DISTANCE ;
    pos_y = TO_UP + search.bestMove.x * DISTANCE ;

    RenjuNode aNode ;
    aNode.x = pos_x - 13.5 ;
    aNode.y = pos_y - 14 ;
    if( !(*p_move_first) )
        aNode.color = "BLACK" ;
    else
        aNode.color = "WHITE" ;
    nodeStack.nodes[ ++nodeStack.top ] = aNode ;

    chessBoard[ search.bestMove.x ][ search.bestMove.y ] = MYSELF ;
    drawed[ search.bestMove.x ][ search.bestMove.y ] = 1 ;
    if( *audio )
        moveSound->play() ;
    repaint() ;

    if( win( chessBoard , search.bestMove.x , search.bestMove.y , MYSELF ) )
    {
        loseSound->play();
        QMessageBox::StandardButton reply ;
        reply = QMessageBox::question( this , "" , QString::fromUtf8( "对不起，你输了，再接再厉！" ) ) ;
        if ( reply == QMessageBox::Ok )
        {
            this->close() ;
        }
    }
}

