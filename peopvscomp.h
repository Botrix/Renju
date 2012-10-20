#ifndef COMPVSPEOP_H
#define COMPVSPEOP_H

#include <QDialog>
#include "configuration.h"
#include "evaluate.h"
#include "search.h"
#include "my_stl.h"

/*
 *实现主要的功能，走棋
 */
class QPushButton ;
class QHBoxLayout ;
class QVBoxLayout ;
class QImage ;
class QMessageBox ;
class QSound ;
class QDir ;
class QRegExp ;
class QPainter ;

class PeopVSComp : public QDialog
{
    Q_OBJECT

public:
    PeopVSComp( QWidget *parent = 0 ) ;

protected:
    void mousePressEvent ( QMouseEvent  * e ) ;
    void paintEvent( QPaintEvent *e ) ;

private slots:
    void config() ;//当点击设置时，弹出对话框
    void startGame() ;//点击start按钮时开始游戏
    void backOneStep() ;//悔棋
    void exitGame() ;//退出游戏

private:
    bool *audio ;//是否开启音效
    bool *p_move_first ;//是否玩家先走
    int *difficulty_degree ;//难度系数

    bool game_start ;//判断游戏时候开始
    bool black_move_flag ;//重绘的时候区别是绘制黑棋还是白棋
    int pos_x ;//重绘位置的x坐标
    int pos_y ;//重绘位置的y坐标

    QPushButton *configPushButton ;
    QPushButton *startPushButton ;
    QPushButton *backStepPushButton ;
    QPushButton *exitPushButton ;
    QVBoxLayout *mainLayout ;
    QString projectPath ;
    QSound *moveSound ;
    QSound *winSound ;
    QSound *loseSound ;
    QImage *blackImage ;
    QImage *whiteImage ;

    RenjuNodeStack nodeStack ;
    int drawed[ 15 ][ 15 ] ;//记录每个位置是否被绘制
    int search_depth ; //搜索的深度
    int search_width ; //搜索的宽度
    int player_win ;//玩家赢 -1输，0平，1赢
    char chessBoard[ MAX_LINE ][ MAX_LINE ] ;
    void computer_move() ;
    //bool win( int x , int y , char player ) ;
};

#endif // COMPVSPEOP_H
