
#include "calibration_wnd.h"
#include "main_wnd.h"
#include "oscilloscope_wnd.h"

#include "matrix2.hpp"

CalibrationWnd::CalibrationWnd( QWidget * parent )
: QWidget( parent )
{
    ui.setupUi( this );
    ui.panel->setEnabled( false );
    ui.clear->setEnabled( false );

    connect( ui.mode,         SIGNAL(clicked()), this, SLOT(slotEnable()) );
    connect( ui.addWorkVolt,  SIGNAL(clicked()), this, SLOT(slotAddWorkVolt()) );
    connect( ui.addProbeVolt, SIGNAL(clicked()), this, SLOT(slotAddProbeVolt()) );
    connect( ui.addAdc,       SIGNAL(clicked()), this, SLOT(slotAddAdc()) );

    connect( ui.clear, SIGNAL(clicked()), this, SLOT(slotClearFiles()) );

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

void CalibrationWnd::setIo( Bipot * io )
{
    this->io      = io;
}

void CalibrationWnd::showEvent( QShowEvent & e )
{
    ui.panel->setEnabled( false );
    ui.mode->setChecked( false );
}

void CalibrationWnd::closeEvent( QCloseEvent & e )
{
    saveCalibrationFiles();
}

void CalibrationWnd::slotEnable()
{
    if ( !io->isOpen() )
        return;

    bool en = ui.mode->isChecked();
    ui.panel->setEnabled( en );
    ui.clear->setEnabled( !en );
    if ( en )
    {
        openCalibrationFiles();

        QTime t = QTime::currentTime();
        int seed = t.msec() + (t.second() + (t.minute() + t.hour() * 24) * 60) * 1000;
        qsrand( seed );

        setRandomVolt();
    }
    else
    {
        saveCalibrationFiles();
    }
}

void CalibrationWnd::slotAddWorkVolt()
{
    if ( !io->isOpen() )
        return;

    qreal v = ui.voltDacWork->value();
    bool res = io->addCalibrationWorkDac( this->dacA, this->dacB, this->dacC, this->dacD, v );
    if ( !res )
    {
        QMessageBox::critical( this, "Error", "Failed to obtain temperature!" );
        return;
    }

    setRandomVolt();
}

void CalibrationWnd::slotAddProbeVolt()
{
    if ( !io->isOpen() )
        return;

    qreal v = ui.voltDacProbe->value();
    bool res = io->addCalibrationProbeDac( this->dacA, this->dacB, this->dacC, this->dacD, v );
    if ( !res )
    {
        QMessageBox::critical( this, "Error", "Failed to retrieve temperature!" );
        return;
    }

    setRandomVolt();
}

void CalibrationWnd::slotAddAdc()
{
    if ( !io->isOpen() )
        return;

    qreal vA = ui.voltAdcWork->value();
    qreal vB = ui.voltAdcProbe->value();
    qreal vC = ui.currAdcWork->value();
    qreal vD = ui.currAdcProbe->value();
    bool res = io->addCalibrationAdc( vA, vB, vC, vD );
    if ( !res )
       QMessageBox::critical( this, "Error", "Failed to retrieve ADC (or temperature)!" ); 
    // Don't set voltage here.
    // Voltage is set by rotating knobs.
    //setRandomVolt();
}

void CalibrationWnd::slotClearFiles()
{
    io->clearCalibrationWorkDac();
    io->clearCalibrationProbeDac();
    io->clearCalibrationAdc();
}

void CalibrationWnd::setRandomVolt()
{
    if ( !io->isOpen() )
        return;

    qreal range = static_cast<qreal>( ui.voltRange->value() * 65535 / 100 );

    qreal a = static_cast<qreal>( qrand() ) / static_cast<qreal>( RAND_MAX ) - 0.5;
    int dacA  = static_cast<int>( a * range ) + 32767;
    a = static_cast<qreal>( qrand() ) / static_cast<qreal>( RAND_MAX ) - 0.5;
    int dacB = static_cast<int>( a * range ) + 32767;
    bool res = io->setWorkRaw( dacA, dacB );
    if ( !res )
    {
        QMessageBox::critical( this, "Error", "Failed to set voltage!" );
        return;
    }
    this->dacA = dacA;
    this->dacB = dacB;

    a = static_cast<qreal>( qrand() ) / static_cast<qreal>( RAND_MAX ) - 0.5;
    int dacC  = static_cast<int>( a * range ) + 32767;
    a = static_cast<qreal>( qrand() ) / static_cast<qreal>( RAND_MAX ) - 0.5;
    int dacD = static_cast<int>( a * range ) + 32767;
    res = io->setProbeRaw( dacC, dacD );
    if ( !res )
    {
        QMessageBox::critical( this, "Error", "Failed to set voltage!" );
        return;
    }

    this->dacC = dacC;
    this->dacD = dacD;
}

void CalibrationWnd::openCalibrationFiles()
{
    io->loadCalibrationWorkDac();
    io->loadCalibrationProbeDac();
    io->loadCalibrationAdc();
}

void CalibrationWnd::saveCalibrationFiles()
{
    io->saveCalibrationWorkDac();
    io->saveCalibrationProbeDac();
    io->saveCalibrationAdc();
}

void CalibrationWnd::clearCalibrationFiles()
{
    io->clearCalibrationWorkDac();
    io->clearCalibrationProbeDac();
    io->clearCalibrationAdc();
}












