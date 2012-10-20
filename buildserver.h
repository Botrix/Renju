#ifndef BUILDSERVER_H
#define BUILDSERVER_H

#include <QDialog>
#include <QUdpSocket>
#include <QHostInfo>
#include "tcpserver.h"
#include "peopvspeop.h"

class QPushButton ;
class QTableWidget ;
class QLabel ;
class QHBoxLayout ;
class QVBoxLayout ;
class QTableWidget ;
class QTableWidgetItem ;

class BuildServer : public QDialog
{
    Q_OBJECT
public:
    BuildServer( QWidget *parent = 0 ) ;
    QString IP ;//用于向PeopVSPeop类传递IP值
signals:
    void toBuildTCP() ;
private slots:
    void cancelSlot() ;
    void readFromUdpClient() ;
    void onItemClicked( QTableWidgetItem * ) ;//当一个IP被选中时触发该槽
    void onListenSuccess() ;//当PeopVSPeop tcp监听成功时，触发该槽
private:
    QHBoxLayout *buttonLayout ;
    QVBoxLayout *mainLayout ;
    QLabel *label ;
    QTableWidget *tableWidget ;
    QPushButton *cancelPushButton ;

    QUdpSocket *udpServer ;
    TcpServer *tcpServer ;
    void listen_udp() ;
};

#endif // BUILDSERVER_H
