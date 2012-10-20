#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#define LOW 1
#define MID 2
#define HIGH 3

#include <QDialog>

class QDialog ;
class QHBoxLayout ;
class QVBoxLayout ;
class QButtonGroup ;
class QCheckBox ;
class QRadioButton ;
class QLabel ;
class QPushButton ;
/*
 *玩前的设置
 */
class Configuration : public QDialog
{
    Q_OBJECT
public:
    Configuration(  bool *_audio , bool *_p_move_first , int *_difficulty_degree , QWidget *parent = 0 ) ;//构造函数
private slots:
    void conf() ;//当点击确定键的时候获得参数

private:
    bool *audio ;//是否开启音效
    bool *p_move_first ;//是否玩家先走
    int *difficulty_degree ;//难度系数
    QVBoxLayout *mainLayout ;
    QHBoxLayout *allDiffLayout ;
    QVBoxLayout *partDiffLayout ;
    QHBoxLayout *OKCancelLayout ;
    QCheckBox *audioCheckBox ;
    QCheckBox *pFirstCheckBox ;
    QLabel *diffLabel ;
    QRadioButton *lowRadioButton ;
    QRadioButton *midRadioButton ;
    QRadioButton *highRadioButton ;
    QPushButton *okButton ;
    QPushButton *cancelButton ;
};

#endif // CONFIGURATION_H
