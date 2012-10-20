#ifndef MY_STL_H
#define MY_STL_H

#define MAX_LINE 15

struct RenjuNode
{
    double x ;
    double y ;
    QString color ;
};

struct RenjuNodeStack
{
    int top ;
    RenjuNode nodes[ 225 ] ;
};


//判断能否赢
static bool win( char chessBoard[ 15 ][ 15 ] , int x , int y , char player )
{
    int count = 1 ; //记录相连同色个数
    //先搜索水平方向
    bool search_left = true , search_right = false ; //一开始先向左搜索
    int tmp_left_y = y , tmp_right_y = y ; //分别替换向左向右搜索
    while( search_left || search_right )
    {
        if( search_left )
        {
            if( --tmp_left_y < 0 || chessBoard[ x ][ tmp_left_y ] != player )
            {
                search_left = false ;
                search_right = true ;
            }
            else
            {
                if( ++ count == 5 )
                    return true ;
            }
        }
        else
        {
            if( ++ tmp_right_y >= MAX_LINE || chessBoard[ x ][ tmp_right_y ] != player )
                search_right = false ;
            else
            {
                if( ++ count == 5 )
                {
                    return true ;
                }
            }
        }
    }


    //然后搜索垂直方向
    count = 1 ;
    bool search_up = true , search_down = false ;//首先向上搜索
    int tmp_up_x = x , tmp_down_x = x ; //分别代替上下时的搜索
    while( search_up || search_down )
    {
        if( search_up )
        {
            if( --tmp_up_x < 0 || chessBoard[ tmp_up_x ][ y ] != player )
            {
                search_up = false ;
                search_down = true ;
            }
            else
            {
                if( ++count == 5 )
                {
                    return true ;
                }
            }
        }
        else
        {
            if( ++tmp_down_x >= MAX_LINE || chessBoard[ tmp_down_x ][ y ] != player )
            {
                search_down = false ;
            }
            else
            {
                if( ++ count == 5 )
                {
                    return true ;
                }
            }
        }
    }


    //左上-右下搜索
    count = 1 ;
    bool search_left_up = true , search_right_down = false ; //先搜索左上
    int tmp_x1 = x , tmp_y1 = y ;//左上搜索时
    int tmp_x2 = x , tmp_y2 = y ;//右下搜索时
    if( abs( x - y ) <= 10 ) //只有线上的格的个数不小于5时才进行搜索
    {
        while( search_left_up || search_right_down )
        {
            if( search_left_up )
            {
                if( --tmp_x1 < 0 || --tmp_y1 < 0 || chessBoard[ tmp_x1 ][ tmp_y1 ] != player )
                {
                    search_left_up = false ;
                    search_right_down = true ;
                }
                else
                {
                    if( ++count == 5 )
                    {
                        return true ;
                    }
                }
            }
            else
            {
                if( ++tmp_x2 >= MAX_LINE || ++tmp_y2 >= MAX_LINE || chessBoard[ tmp_x2 ][ tmp_y2 ] != player )
                {
                    search_right_down = false ;
                }
                else
                {
                    if( ++ count == 5 )
                    {
                        return true ;
                    }
                }
            }
        }
    }


    //左下-右上搜索
    count = 1 ;
    bool search_left_down = true , search_right_up = false ; //先搜索左下
    tmp_x1 = tmp_x2 = x ;
    tmp_y1 = tmp_y2 = y ;
    if( x + y >= 4 && x + y <= 24 ) //线上的格的个数不小于5才搜索
    {
        while( search_left_down || search_right_up )
        {
            if( search_left_down )
            {
                if( ++tmp_x1 >= MAX_LINE || --tmp_y1 < 0 || chessBoard[ tmp_x1 ][ tmp_y1 ] != player )
                {
                    search_left_down = false ;
                    search_right_up = true ;
                }
                else
                {
                    if( ++count == 5 )
                    {
                        return true ;
                    }
                }
            }
            else
            {
                if( --tmp_x2 < 0 || ++ tmp_y2 >= MAX_LINE || chessBoard[ tmp_x2 ][ tmp_y2 ] != player )
                {
                    search_right_up = false ;
                }
                else
                {
                    if( ++ count == 5 )
                    {
                        return true ;
                    }
                }
            }
        }
    }

    return false ; //当上面四种情况都不满足时，返回false
}


#endif // MY_STL_H
