
#include "temp_ctrl.h"
#include "hal.h"

#define ADDR 0X48

#define REG_CONF 0x03
#define REG_DATA 0x00

static Mutex mutex;
static uint16_t temperature = 0;

/*
static const I2CConfig i2cfg1 =
{
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_2,
};
*/

static const I2CConfig i2cfg1 =
{
	OPMODE_I2C,
	100000,
	STD_DUTY_CYCLE,
};


void initTemp( void )
{
	chMtxInit( &mutex );

    i2cInit();

    i2cStart( &I2CD1, &i2cfg1 );

    /* tune ports for I2C1*/
    palSetPadMode(IOPORT2, 6, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
    palSetPadMode(IOPORT2, 7, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);

    // startups. Pauses added just to be safe
    chThdSleepMilliseconds(100);

    // Write to config continuous convertion and 16 bit mode.
    //msg_t res;
    uint8_t tx[2] = { REG_CONF, (1<<7) };
    systime_t tmo = MS2ST(100);
    /*res =*/ i2cMasterTransmitTimeout ( &I2CD1, ADDR, tx, 2, 0, 0, tmo );
}


void processTemp( void )
{
    msg_t res;
    uint8_t tx[3] = { REG_DATA, 0, 0 };
    uint8_t rx[2];
    systime_t tmo = MS2ST(100);
    res = i2cMasterTransmitTimeout ( &I2CD1, ADDR, tx, 1, rx, 2, tmo );
    if ( res != RDY_OK )
    {
        chMtxLock( &mutex );
            temperature = 0xFFFF;
        chMtxUnlock();
        return;
    }
    uint16_t t = (((uint16_t)rx[0]) << 8) + ((uint16_t)rx[1]);
    chMtxLock( &mutex );
    	temperature = t;
    chMtxUnlock();

}

uint16_t lastTemp( void )
{
    chMtxLock( &mutex );
    	uint16_t t = temperature;
    chMtxUnlock();
    return t;
}





