
#include "dac_ctrl.h"
#include "sweep_ctrl.h"
#include "hal.h"






#define DAC_QUEUE_SZ (24)

OutputQueue  dac_queue;
uint8_t      dac_queue_buffer[DAC_QUEUE_SZ];

#define DAC_CS_0     9
#define DAC_CS_1     10
#define DAC_CS_2     11
#define DAC_CS_3     12

uint8_t dac_tx_buffer[2];

int newValues[ 4 ] = {-1, -1, -1, -1};
int dacValues[ 4 ] = {-1, -1, -1, -1};

static void onDacSpiComplete( SPIDriver * spid );

/*
 * Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0, MSb first).
 */
static SPIConfig hs_spicfg =
{
	onDacSpiComplete,
    GPIOB,
    DAC_CS_0,
    SPI_CR1_BR_1
};




















void initDac( void )
{
	//palSetPadMode( GPIOB, 5, PAL_MODE_OUTPUT_PUSHPULL );

	chOQInit( &dac_queue, dac_queue_buffer, DAC_QUEUE_SZ, 0 );

	palSetPadMode( GPIOB, 13, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     // SCK.
    palSetPadMode( GPIOB, 15, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     // MOSI

    palSetPad( GPIOB, DAC_CS_0 );
    palSetPad( GPIOB, DAC_CS_1 );
    palSetPad( GPIOB, DAC_CS_2 );
    palSetPad( GPIOB, DAC_CS_3 );

    palSetPadMode( GPIOB, DAC_CS_0, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( GPIOB, DAC_CS_1, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( GPIOB, DAC_CS_2, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( GPIOB, DAC_CS_3, PAL_MODE_OUTPUT_PUSHPULL );

    spiStart( &SPID2, &hs_spicfg );

    setDac( 0, 32767 );
    setDac( 1, 32767 );
    setDac( 2, 32767 );
    setDac( 3, 32767 );
}

void setDacI( int index, int value )
{
	newValues[ index % 4 ] = value;
}

int setDac( int index, int value )
{
	int res;
	chSysLock();
		int empty =chOQGetEmptyI( &dac_queue );
	chSysUnlock();
	if ( empty >= 3 )
	{
		chOQPut( &dac_queue, (uint8_t)(index & 0xFF) );
		chOQPut( &dac_queue, (uint8_t)(value & 0xFF) );
		chOQPut( &dac_queue, (uint8_t)((value >> 8) & 0xFF) );
		res = 0;
	}
	else
		res = 1;
	return res;
}

int  setDac1( int valueL, int valueH )
{
	int res;
	res = setDac( 0, valueL );
	res += setDac( 1, valueH );
	return res;
}

int  setDac2( int valueL, int valueH )
{
	int res;
	res = setDac( 2, valueL );
	res += setDac( 3, valueH );
	return res;
}

void processDacI( void )
{
	static uint8_t i = 0;

	// Get next DAC data from output queue.
	int queueSize = chOQGetFullI( &dac_queue );
	if ( queueSize >= 3 )
	{
		msg_t res;
		res = chOQGetI( &dac_queue );
		i = (int)res;

		int val;
		res = chOQGetI( &dac_queue );
		val = (int)res;
		res = chOQGetI( &dac_queue );
		val += (((int)res) << 8);

		i = i % 4;

		newValues[i] = val;
	}


	if ( newValues[i] != dacValues[i] )
	{
		switch ( i )
		{
		case 0:
			hs_spicfg.sspad = DAC_CS_0;
			//palClearPad( GPIOB, DAC_CS_0 );
			break;
		case 1:
			hs_spicfg.sspad = DAC_CS_1;
			//palClearPad( GPIOB, DAC_CS_1 );
			break;
		case 2:
			hs_spicfg.sspad = DAC_CS_2;
			//palClearPad( GPIOB, DAC_CS_2 );
			break;
		case 3:
			hs_spicfg.sspad = DAC_CS_3;
			//palClearPad( GPIOB, DAC_CS_3 );
			break;
		}
		dacValues[i] = newValues[i];
		dac_tx_buffer[0] = (uint8_t)((newValues[i] >> 8) & 0xFF);
		dac_tx_buffer[1] = (uint8_t)(newValues[i] & 0xFF);

		spiSelectI( &SPID2 );
		//palClearPad( GPIOB, hs_spicfg.sspad );
		spiStartSendI( &SPID2, 2, dac_tx_buffer );
		// Only one DAC could be set at once due to they all are
		// connected to one and the same SPI bus.
	}

	processSweepI( i );

	i += 1;
	i %= 4;
}

static void onDacSpiComplete( SPIDriver * spid )
{
	(void)spid;
									//palSetPad( GPIOB, 5 );
	chSysLockFromIsr();
		spiUnselectI( &SPID2 );
		//palSetPad( GPIOB, DAC_CS_0 );
		//palSetPad( GPIOB, DAC_CS_1 );
		//palSetPad( GPIOB, DAC_CS_2 );
		//palSetPad( GPIOB, DAC_CS_3 );
	chSysUnlockFromIsr();
									//palClearPad( GPIOB, 5 );
}

void currentDacs( int * dacs )
{
	chSysLock();
		dacs[0] = dacValues[0];
		dacs[1] = dacValues[1];
		dacs[2] = dacValues[2];
		dacs[3] = dacValues[3];
	chSysUnlock();
}









