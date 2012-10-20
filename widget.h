#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "peopvscomp.h"
#include "peopvspeop.h"

class QPushButton ;
class PeopVSPeop ;
class QVBoxLayout ;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);

private slots:
    void pushPeoBttn() ;
    void pushComBttn() ;
    void full_screen_slot() ;
    void exit_full_scen_slot() ;
private:
    QPushButton *peoVSpeoButton ;
    QPushButton *comVSpeoButton ;
    PeopVSPeop *aPeopVSPeop ;
    QPushButton *fullScreenButton ;
    QPushButton *exitFullScreenButton ;
};

#endif // WIDGET_H
