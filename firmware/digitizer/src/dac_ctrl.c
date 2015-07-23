
#include "dac_ctrl.h"
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

static void onSpiComplete( SPIDriver * spid );

/*
 * Maximum speed SPI configuration (18MHz, CPHA=0, CPOL=0, MSb first).
 */
static SPIConfig hs_spicfg =
{
    onSpiComplete,
    GPIOB,
    DAC_CS_0,
    0
};


void initDac( void )
{
	chOQInit( &dac_queue, dac_queue_buffer, DAC_QUEUE_SZ, 0 );

	palSetPadMode( GPIOB, 13, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     /* SCK. */
    palSetPadMode( GPIOB, 14, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     /* MISO.*/
    palSetPadMode( GPIOB, 15, PAL_MODE_STM32_ALTERNATE_PUSHPULL );     /* MOSI.*/

    palSetPad( GPIOB, DAC_CS_0 );
    palSetPad( GPIOB, DAC_CS_1 );
    palSetPad( GPIOB, DAC_CS_2 );
    palSetPad( GPIOB, DAC_CS_3 );

    palSetPadMode( GPIOB, DAC_CS_0, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( GPIOB, DAC_CS_1, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( GPIOB, DAC_CS_2, PAL_MODE_OUTPUT_PUSHPULL );
    palSetPadMode( GPIOB, DAC_CS_3, PAL_MODE_OUTPUT_PUSHPULL );

    spiStart( &SPID2, &hs_spicfg );
}

void setDacI( int index, int value )
{
	newValues[ index % 4 ] = value;
}

int setDac( int index, int value )
{
	int res;
	chSysLock();
		if ( chOQGetEmptyI( &dac_queue ) >= 3 )
		{
			chOQPut( &dac_queue, (uint8_t)(index & 0xFF) );
			chOQPut( &dac_queue, (uint8_t)(value & 0xFF) );
			chOQPut( &dac_queue, (uint8_t)((value >> 8) & 0xFF) );
			res = 0;
		}
		else
			res = 1;
	chSysUnlock();
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
	int i;
	chSysLockFromIsr();
		// Get All data from output queue.
		while ( chOQGetFullI( &dac_queue ) >= 3 )
		{
			msg_t res;
			res = chOQGetI( &dac_queue );
			i = res;

			int val;
			res = chOQGetI( &dac_queue );
			val = res;
			res = chOQGetI( &dac_queue );
			val += (res << 8);

			i = i % 4;
			newValues[i] = val;
		}
	chSysUnlockFromIsr();

	for ( i=0; i<4; i++ )
	{
		if ( newValues[i] != dacValues[i] )
		{
			switch ( i )
			{
			case 0:
				hs_spicfg.sspad = DAC_CS_0;
				dacValues[i] = newValues[i];
				break;
			case 1:
				hs_spicfg.sspad = DAC_CS_1;
				dacValues[i] = newValues[i];
				break;
			case 2:
				hs_spicfg.sspad = DAC_CS_2;
				dacValues[i] = newValues[i];
				break;
			case 3:
				hs_spicfg.sspad = DAC_CS_3;
				dacValues[i] = newValues[i];
				break;
			}
			dac_tx_buffer[0] = (uint8_t)((newValues[i] >> 8) & 0xFF);
			dac_tx_buffer[1] = (uint8_t)(newValues[i] & 0xFF);
			chSysLockFromIsr();
				spiSelectI( &SPID2 );
				spiStartSendI( &SPID2, 2, dac_tx_buffer );
			chSysUnlockFromIsr();
			// Only one DAC could be set at once due to they all are
			// connected to one and the same SPI bus.
			return;
		}
	}
}

static void onSpiComplete( SPIDriver * spid )
{
	(void)spid;
	chSysLockFromIsr();
		spiUnselectI( &SPID2 );
	chSysUnlockFromIsr();
}








