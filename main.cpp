#include <QtGui/QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget my_widget ;
    my_widget.setWindowFlags( Qt::Widget ) ;
    my_widget.showFullScreen();

    return a.exec();
}
