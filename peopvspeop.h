#ifndef PEOPVSPEOP_H
#define PEOPVSPEOP_H

#include "buildserver.h"
#include "findserver.h"
#include "tcpserver.h"
#include "my_stl.h"

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostInfo>

class BuildServer ;
class FindServer ;
class QPushButton ;
class QImage ;
class QPainter ;
class BuildServer ;
class FindServer ;


class PeopVSPeop : public QDialog
{
    Q_OBJECT
public:
    PeopVSPeop( QWidget *parent = 0 ) ;
signals:
    void listenSuccess() ;
    void tcpConnectSuccess() ;
protected:
    void mousePressEvent ( QMouseEvent  * e ) ;
    void paintEvent( QPaintEvent *e ) ;

private slots:
    void build_slot() ;
    void find_slot() ;
    void disconnect_slot() ;
    void create_server() ; //建立tcp连接,作为主机
    void create_client() ; //建立tcp连接,作为客户端
    void readFromClient() ;//主机从客户端度读、写
    void readFromServer() ; //客户端从主机读、写
    void exitSlot() ;

private:
    TcpServer *tcpServer ;
    QTcpSocket *tcpClient ;

    QPushButton *buildServerButton ;
    QPushButton *findServerButton ;
    QPushButton *cutConnectButton ;
    QPushButton *exitButton ;
    FindServer *aFindServer ;
    BuildServer *aBuildServer ;

    QImage *blackImage ;
    QImage *whiteImage ;

    int itemClickCnt ; //记录itemClicked()的次数，当为0时有效
    bool pushBuild ;//判断是否按的是建立主机
    bool game_start ;//判断是否能够重绘

    //下面是传回的值分别是x坐标，y坐标，不要和第几行第几列弄混了
    int x ;//x坐标，横向
    int y ;//y坐标，纵向

    double pos_x ;
    double pos_y ;
    QString color ;
    int drawed[ 15 ][ 15 ] ;//记录每个位置是否被绘制
    char chessBoard[ 15 ][ 15 ] ;
    char me ; //标记我是谁 主机是'1'，客户端为'2'
    RenjuNodeStack nodeStack ;
};

#endif // PEOPVSPEOP_H
