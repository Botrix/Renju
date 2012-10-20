#include "configuration.h"
#include <QtGui>

Configuration::Configuration( bool *_audio , bool *_p_move_first , int *_difficulty_degree , QWidget *parent )
    :QDialog( parent )
{
    audio = _audio ;
    p_move_first = _p_move_first ;
    difficulty_degree = _difficulty_degree ;
    mainLayout = new QVBoxLayout ;
    allDiffLayout = new QHBoxLayout ;
    partDiffLayout = new QVBoxLayout ;
    OKCancelLayout = new QHBoxLayout ;

    audioCheckBox = new QCheckBox( QString::fromUtf8( "音效" ) , this ) ;
    pFirstCheckBox = new QCheckBox( QString::fromUtf8( "玩家先走" ) , this ) ;
    diffLabel = new QLabel( QString::fromUtf8( "难度系数:" ) , this  ) ;
    lowRadioButton = new QRadioButton( QString::fromUtf8( "低" ) , this ) ;
    lowRadioButton->setAutoExclusive( true ) ;
    midRadioButton = new QRadioButton( QString::fromUtf8( "中" ) , this ) ;
    midRadioButton->setAutoExclusive( true ) ;
    midRadioButton->setChecked( true ) ;
    highRadioButton = new QRadioButton( QString::fromUtf8( "高" ) , this ) ;
    highRadioButton->setAutoExclusive( true ) ;
    okButton = new QPushButton( QString::fromUtf8( "确定" ) , this ) ;
    cancelButton = new QPushButton( QString::fromUtf8( "取消" ) , this ) ;

    partDiffLayout->addWidget( lowRadioButton ) ;
    partDiffLayout->addWidget( midRadioButton ) ;
    partDiffLayout->addWidget( highRadioButton ) ;
    allDiffLayout->addWidget( diffLabel ) ;
    allDiffLayout->addLayout( partDiffLayout ) ;
    OKCancelLayout->addWidget( okButton ) ;
    OKCancelLayout->addStretch() ;
    OKCancelLayout->addWidget( cancelButton ) ;

    mainLayout->addWidget( audioCheckBox ) ;
    mainLayout->addWidget( pFirstCheckBox ) ;
    mainLayout->addLayout( allDiffLayout ) ;
    mainLayout->addStretch() ;
    mainLayout->addLayout( OKCancelLayout ) ;

    setLayout( mainLayout ) ;
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    connect( okButton , SIGNAL( clicked() ) , this , SLOT( conf() ) ) ;
    connect( cancelButton ,  SIGNAL( clicked()) , this , SLOT( close()) ) ;
}

void Configuration::conf()
{
    if( audioCheckBox->isChecked() )
        *audio = true ;
    else
        *audio = false ;
    if( pFirstCheckBox->isChecked() )
        *p_move_first = true ;
    else
        *p_move_first = false ;
    if( lowRadioButton->isChecked() )
        *difficulty_degree = LOW ;
    else if( midRadioButton->isChecked() )
        *difficulty_degree = MID ;
    else if( highRadioButton->isChecked() )
        *difficulty_degree = HIGH ;
    this->close() ;
}

