
#include "osc_main.h"

int main( int argc, char * argv[] )
{
    QApplication app( argc, argv );
    COscMain * om = new COscMain();
    om->show();

    int res = app.exec();
    return res;
}