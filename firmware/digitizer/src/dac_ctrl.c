
#include "dac.h"
#include "hal.h"

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
static const SPIConfig hs_spicfg = {
    onSpiComplete,
    GPIOB,
    DAC_CS_0,
    0
};


void initDac( void )
{
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

void processDacI( void )
{
	int i;
	for ( i=0; i<4; i++ )
	{
		if ( newValues[i] != dacValues[i] )
		{
			switch ( i )
			{
			case 0:
				casehs_spicfg.sspad = DAC_CS_0;
				dacValues[newIndex] = newValue;
				break;
			case 1:
				casehs_spicfg.sspad = DAC_CS_1;
				dacValues[newIndex] = newValue;
				break;
			case 2:
				casehs_spicfg.sspad = DAC_CS_2;
				dacValues[newIndex] = newValue;
				break;
			case 3:
				casehs_spicfg.sspad = DAC_CS_3;
				dacValues[newIndex] = newValue;
				break;
			}
			dac_tx_buffer[0] = (uint8_t)((newValue >> 8) & 0xFF);
			dac_tx_buffer[1] = (uint8_t)(newValue & 0xFF);
			spiSelectI( SPID2 );
			spiStartSendI( SPID2, 2, dac_tx_buffer );
			// Only one DAC could be set at once due to they all are
			// connected to one and the same SPI bus.
			return;
		}
	}
}

static void onSpiComplete( SPIDriver * spid )
{
	spiUnselectI( SPID2 );
}








