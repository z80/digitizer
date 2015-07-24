
#include <QtGui>
#include "main_wnd.h"

int main( int argc, char * argv[])
{
    QApplication app( argc, argv );

    MainWnd * mw = new MainWnd( 0 );
    mw->show();

    int res = app.exec();
    return res;
}




