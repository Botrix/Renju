/*
 *Copyright (C) 2011 @HIT FooToo Lab
 *Author: 时彦辉
 *Create Date: 2011-4-15
 */
#ifndef EVALUATE_H
#define EVALUATE_H

#include <string>
#include <memory.h>

using namespace std ;

//棋盘的行列数
#define MAX_LINE 15

//每个棋位的状态，空为0，我为1，对手为2
#define EMPTY '0'
#define MYSELF '1'
#define RIVAL '2'

//初始化的最大值
#define INIT_MAX 2147483647

#define FIVE 1
#define FOUR 2
#define SLEEP_FOUR 3
#define THREE 4
#define SLEEP_THREE 5
#define TWO 6
#define SLEEP_TWO 7
#define UNANALYSED 0
#define ANALYSED 10

//赢、输、和局
#define  WIN 1
#define  LOSE -1
#define  TIE 0

class Evaluate
{
public:
    Evaluate() ;//构造函数
    int getValue(  char player , char board[ MAX_LINE ][ MAX_LINE ] , int *whether_win ) ;//得到估价值，whether_win表示当前选手能否赢
    void analyseLine( char player , string line ) ;//对某一行(水平，垂直或倾斜)进行分析
    void findStateNum( char board[ MAX_LINE ][ MAX_LINE ]) ;//找出各个状态的个数
private:
    int numOfState[ 2 ][ 8 ] ;//记录人机各个状态(五连，活四，冲四等)的个数，0为机器即MYSELF，1为人即RIVAL
};

#endif
