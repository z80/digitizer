
#include "calibration_wnd.h"
#include "main_wnd.h"
#include "oscilloscope_wnd.h"

#include "matrix2.hpp"

const QString CalibrationWnd::VOLT_FILE = "./volt.txt";
const QString CalibrationWnd::CURR_FILE = "./curr.txt";

CalibrationWnd::CalibrationWnd( QWidget * parent )
: QWidget( parent )
{
    ui.setupUi( this );
    ui.panel->setEnabled( false );

    connect( ui.mode,    SIGNAL(clicked()), this, SLOT(slotEnable()) );
    connect( ui.addVolt, SIGNAL(clicked()), this, SLOT(slotAddVolt()) );
    connect( ui.addCurr, SIGNAL(clicked()), this, SLOT(slotAddCurr()) );

    connect( ui.clearFileBtn, SIGNAL(clicked()), this, SLOT(slotClearFiles()) );

    // DEBUG
    /*
    adcAux.append( 2000 );
    volt.append( -10.0 );

    adcAux.append( 4000 );
    volt.append( 10.0 );

    adcAux.append( 3000 );
    volt.append( 0.0 );

    calcAdcAux2Volt();
    */
}

CalibrationWnd::~CalibrationWnd()
{

}

void CalibrationWnd::setIo( VoltampIo * io, MainWnd * mainWnd, OscilloscopeWnd * osc )
{
    this->io      = io;
    this->mainWnd = mainWnd;
    this->osc     = osc;
}

void CalibrationWnd::showEvent( QShowEvent & e )
{
    ui.panel->setEnabled( false );
    ui.mode->setChecked( false );
}

void CalibrationWnd::closeEvent( QCloseEvent & e )
{
    if ( volt.size() >= 3 )
    {
        calcDac2Volt();
        mainWnd->setCalibrationDac( aDacLow, aDacHigh, bDac );
    }
    if ( volt.size() >= 2 )
    {
        calcAdcAux2Volt();
        calcAdcRef2Volt();
        mainWnd->setCalibrationAdcVolt( aAdcAux, bAdcAux, aAdcRef, bAdcRef );
    }
    if ( curr.size() >= 2 )
    {
        calcAdcI2Curr();
        mainWnd->setCalibrationAdcCurr( aAdcI, bAdcI );
    }
}

void CalibrationWnd::slotEnable()
{
    bool en = ui.mode->isChecked();
    ui.panel->setEnabled( en );
    ui.clearFileBtn->setEnabled( !en );
    if ( en )
    {
        openVoltCalibrationFile();
        openCurrCalibrationFile();

        QTime t = QTime::currentTime();
        int seed = t.msec() + (t.second() + (t.minute() + t.hour() * 24) * 60) * 1000;
        qsrand( seed );

        setRandomVolt();
    }
    else
    {
        if ( volt.size() >= 3 )
        {
            calcDac2Volt();
            mainWnd->setCalibrationDac( aDacLow, aDacHigh, bDac );
        }
        if ( volt.size() >= 2 )
        {
            calcAdcAux2Volt();
            calcAdcRef2Volt();
            mainWnd->setCalibrationAdcVolt( aAdcAux, bAdcAux, aAdcRef, bAdcRef );
        }
        if ( curr.size() >= 2 )
        {
            calcAdcI2Curr();
            mainWnd->setCalibrationAdcCurr( aAdcI, bAdcI );
        }
        closeCalibrationFiles();
    }
}

void CalibrationWnd::slotAddVolt()
{
    qreal v = ui.voltValue->value();
    volt.append( v );

    int eaux, eref, iaux;
    osc->mostRecentValsRaw( eaux, eref, iaux );

    adcAux.append( eaux );
    adcRef.append( eref );

    dacLowV.append( dacLow );
    dacHighV.append( dacHigh );

    QString stri = QString( "%1 %2 %3 %4 %5\n" ).arg( dacLow ).arg( dacHigh ).arg( eaux ).arg( eref ).arg( v );
    voltStream << stri;

    setRandomVolt();
}

void CalibrationWnd::slotAddCurr()
{
    qreal c = ui.currValue->value();
    curr.append( c );

    int eaux, eref, iaux;
    osc->mostRecentValsRaw( eaux, eref, iaux );
    adcI.append( iaux );

    QString stri = QString( "%1 %2\n" ).arg( iaux ).arg( c );
    voltStream << stri;

    setRandomVolt();
}

void CalibrationWnd::slotClearFiles()
{
    QFile::remove( VOLT_FILE );
    QFile::remove( CURR_FILE );
}

void CalibrationWnd::setRandomVolt()
{
    qreal range = static_cast<qreal>( ui.voltRange->value() * 4095 / 100 );
    qreal a = static_cast<qreal>( qrand() ) / static_cast<qreal>( RAND_MAX ) - 0.5;
    int dacLow  = static_cast<int>( a * range ) + 2047;
    a = static_cast<qreal>( qrand() ) / static_cast<qreal>( RAND_MAX ) - 0.5;
    int dacHigh = static_cast<int>( a * range ) + 2047;

    bool res = io->set_dac_raw( dacLow, dacHigh );
    if ( !res )
    {
        QMessageBox::critical( this, "Error", "Failed to set voltage!" );
        return;
    }

    this->dacLow  = dacLow;
    this->dacHigh = dacHigh;
}

void CalibrationWnd::calcDac2Volt()
{
    // Make linear assumption, e.i.
    //V = a*dacLow + b*dacHigh + c*1;
    Math::Matrix<> XtX;
    Math::Vector<> XtY;
    int sz = dacLowV.size();
    for ( int i=0; i<3; i++ )
    {
        QVector<int> * a;
        if ( i==0 )
            a = &dacLowV;
        else if ( i==1 )
            a = &dacHighV;
        else
            a = 0;
        for ( int j=0; j<3; j++ )
        {
            QVector<int> * b;
            if ( j==0 )
                b = &dacLowV;
            else if ( j==1 )
                b = &dacHighV;
            else
                b = 0;
            qreal v = 0.0;
            for ( int k=0; k<sz; k++ )
            {
                qreal va = ( a ) ? a->at( k ) : 1.0;
                qreal vb = ( b ) ? b->at( k ) : 1.0;
                v += va * vb;
            }
            XtX[i][j] = v;
        }

        qreal v = 0.0;
        for ( int k=0; k<sz; k++ )
        {
            qreal va = ( a ) ? a->at( k ) : 1.0;
            qreal vy = volt.at( k );
            v += va * vy;
        }
        XtY[i] = v;
    }
    // A = (XtX)^-1 * XtY;
    Math::Matrix<> invXtX;
    invXtX = XtX.inv();
    Math::Vector<> A;
    for ( int i=0; i<3; i++ )
    {
        qreal v = 0.0;
        for ( int j=0; j<3; j++ )
        {
            v += invXtX[i][j] * XtY[j];
        }
        A[i] = v;
    }
    aDacLow  = A[0];
    aDacHigh = A[1];
    bDac     = A[2];
}

void CalibrationWnd::calcAdcAux2Volt()
{
    // Make linear assumption, e.i.
    //V = a*adc + b*1;
    Math::Matrix<2> XtX;
    Math::Vector<2> XtY;
    int sz = adcAux.size();
    for ( int i=0; i<2; i++ )
    {
        QVector<int> * a;
        if ( i==0 )
            a = &adcAux;
        else
            a = 0;
        for ( int j=0; j<2; j++ )
        {
            QVector<int> * b;
            if ( j==0 )
                b = &adcAux;
            else
                b = 0;
            qreal v = 0.0;
            for ( int k=0; k<sz; k++ )
            {
                qreal va = ( a ) ? a->at( k ) : 1.0;
                qreal vb = ( b ) ? b->at( k ) : 1.0;
                v += va * vb;
            }
            XtX[i][j] = v;
        }

        qreal v = 0.0;
        for ( int k=0; k<sz; k++ )
        {
            qreal va = ( a ) ? a->at( k ) : 1.0;
            qreal vy = volt.at( k );
            v += va * vy;
        }
        XtY[i] = v;
    }
    // A = (XtX)^-1 * XtY;
    Math::Matrix<2> invXtX;
    invXtX = XtX.inv();
    Math::Vector<2> A;
    for ( int i=0; i<2; i++ )
    {
        qreal v = 0.0;
        for ( int j=0; j<2; j++ )
        {
            v += invXtX[i][j] * XtY[j];
        }
        A[i] = v;
    }
    aAdcAux = A[0];
    bAdcAux = A[1];
}

void CalibrationWnd::calcAdcRef2Volt()
{
    // Make linear assumption, e.i.
    //V = a*adc + b*1;
    Math::Matrix<2> XtX;
    Math::Vector<2> XtY;
    int sz = adcRef.size();
    for ( int i=0; i<2; i++ )
    {
        QVector<int> * a;
        if ( i==0 )
            a = &adcRef;
        else
            a = 0;
        for ( int j=0; j<2; j++ )
        {
            QVector<int> * b;
            if ( j==0 )
                b = &adcRef;
            else
                b = 0;
            qreal v = 0.0;
            for ( int k=0; k<sz; k++ )
            {
                qreal va = ( a ) ? a->at( k ) : 1.0;
                qreal vb = ( b ) ? b->at( k ) : 1.0;
                v += va * vb;
            }
            XtX[i][j] = v;
        }

        qreal v = 0.0;
        for ( int k=0; k<sz; k++ )
        {
            qreal va = ( a ) ? a->at( k ) : 1.0;
            qreal vy = volt.at( k );
            v += va * vy;
        }
        XtY[i] = v;
    }
    // A = (XtX)^-1 * XtY;
    Math::Matrix<2> invXtX;
    invXtX = XtX.inv();
    Math::Vector<2> A;
    for ( int i=0; i<2; i++ )
    {
        qreal v = 0.0;
        for ( int j=0; j<2; j++ )
        {
            v += invXtX[i][j] * XtY[j];
        }
        A[i] = v;
    }
    aAdcRef = A[0];
    bAdcRef = A[1];
}

void CalibrationWnd::calcAdcI2Curr()
{
    // Make linear assumption, e.i.
    //I = a*adc + b*1;
    Math::Matrix<2> XtX;
    Math::Vector<2> XtY;
    int sz = adcI.size();
    for ( int i=0; i<2; i++ )
    {
        QVector<int> * a;
        if ( i==0 )
            a = &adcI;
        else
            a = 0;
        for ( int j=0; j<2; j++ )
        {
            QVector<int> * b;
            if ( j==0 )
                b = &adcI;
            else
                b = 0;
            qreal v = 0.0;
            for ( int k=0; k<sz; k++ )
            {
                qreal va = ( a ) ? a->at( k ) : 1.0;
                qreal vb = ( b ) ? b->at( k ) : 1.0;
                v += va * vb;
            }
            XtX[i][j] = v;
        }

        qreal v = 0.0;
        for ( int k=0; k<sz; k++ )
        {
            qreal va = ( a ) ? a->at( k ) : 1.0;
            qreal vy = curr.at( k );
            v += va * vy;
        }
        XtY[i] = v;
    }
    // A = (XtX)^-1 * XtY;
    Math::Matrix<2> invXtX;
    invXtX = XtX.inv();
    Math::Vector<2> A;
    for ( int i=0; i<2; i++ )
    {
        qreal v = 0.0;
        for ( int j=0; j<2; j++ )
        {
            v += invXtX[i][j] * XtY[j];
        }
        A[i] = v;
    }
    aAdcI = A[0];
    bAdcI = A[1];
}

void CalibrationWnd::openVoltCalibrationFile()
{
    dacLowV.clear();
    dacHighV.clear();
    adcAux.clear();
    adcRef.clear();
    volt.clear();

    voltFile.setFileName( VOLT_FILE );
    bool open = voltFile.open( QIODevice::ReadOnly );
    if ( open )
    {
        voltStream.setDevice( &voltFile );

        QString stri; 
        while ( !voltStream.atEnd() )
        {
            stri = voltStream.readLine();
            QRegExp ex( "(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)\\s+(\\w+)" );
            int index = ex.indexIn( stri );
            if ( index >= 0 )
            {
                QString m;
                m = ex.cap( 1 );
                int dl = m.toInt();
                dacLowV.append( dl );

                m = ex.cap( 2 );
                int dh = m.toInt();
                dacHighV.append( dh );

                m = ex.cap( 3 );
                int au = m.toInt();
                adcAux.append( au );

                m = ex.cap( 4 );
                int ar = m.toInt();
                adcRef.append( au );

                m = ex.cap( 5 );
                qreal v = m.toDouble();
                volt.append( v );
            }
        }
        voltFile.close();
    }

    quint64 sz = voltFile.size();
    open = voltFile.open( QIODevice::Append );
    if ( !open )
    {
        voltStream.setDevice( 0 );
        return;
    }
    voltStream.setDevice( &voltFile );
}

void CalibrationWnd::openCurrCalibrationFile()
{
    adcI.clear();
    curr.clear();

    currFile.setFileName( CURR_FILE );
    bool open = currFile.open( QIODevice::ReadOnly );
    if ( open )
    {
        currStream.setDevice( &currFile );

        QString stri; 
        while ( !currStream.atEnd() )
        {
            stri = currStream.readLine();
            QRegExp ex( "(\\w+)\\s+(\\w+)" );
            int index = ex.indexIn( stri );
            if ( index >= 0 )
            {
                QString m;
                m = ex.cap( 1 );
                int ai = m.toInt();
                adcI.append( ai );

                m = ex.cap( 2 );
                qreal dh = m.toDouble();
                curr.append( dh );
            }
        }
        currFile.close();
    }

    quint64 sz = currFile.size();
    open = currFile.open( QIODevice::Append );
    currFile.seek( sz );
    if ( !open )
    {
        currStream.setDevice( 0 );
        return;
    }
    currStream.setDevice( &currFile );
}

void CalibrationWnd::closeCalibrationFiles()
{
    voltFile.close();
    currFile.close();
}



