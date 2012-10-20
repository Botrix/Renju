#ifndef FINDSERVER_H
#define FINDSERVER_H

#include <QDialog>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QHostInfo>

class QPushButton ;
class QTableWidget ;
class QLabel ;
class QHBoxLayout ;
class QVBoxLayout ;

class FindServer : public QDialog
{
    Q_OBJECT
public:
    FindServer( QWidget *parent = 0 ) ;
    QString F_IP ;
signals:
    void pleaseConnectTcp() ;//接收到BuildServer的tcp请求连接时，触发该信号，让PeopVSPeop连接tcp服务器端
private slots:
    void readFromUdpServer() ;
    void onTcpConnectSuccess() ;//受到PeopVSPeop的tcpConnectSuccess()信号时触发该槽
    void cancelSlot() ;
private:
    QHBoxLayout *buttonLayout ;
    QVBoxLayout *mainLayout ;

    QLabel *label ;
    QTableWidget *tableWidget ;
    QPushButton *cancelButton ;
    QUdpSocket *serverFinderSocket ;
    QTcpSocket *tcpSocket ;

    void find_server() ;
};

#endif // FINDSERVER_H
