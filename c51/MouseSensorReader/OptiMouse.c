#include "OptiMouse.h"
#include "ADNS2610_DRV.h"
#include "types.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/

/* Following 2 pins must be defined in a client code */
extern bit _SCLK_PIN; /* points to the pin connected to SCLK optical sensor port */
extern bit _SDIO_PIN; /* points to the pin connected to SDIO optical sensor port */

#define OPTI_MOUSE_PROC_RAW_IMAGE 1

#if OPTI_MOUSE_PROC_RAW_IMAGE == 1
/* Raw image data storage */
UINT8 _RAW_IMAGE_DATA[RAW_IMAGE_SIZE*RAW_IMAGE_SIZE];
#endif

static UINT8 opti_mouse__read_register(UINT8 address);
static void opti_mouse__write_register(UINT8 address, UINT8 byte);

static void delay_ms(UINT16 delay);

#define LOW 	0
#define HIGH	1

#define DELTA_X 0x03
#define DELTA_Y 0x02


/******************************************************************************
 * Mouse optical sensor user API implementation
 ******************************************************************************/

void opti_mouse__start( void )
{
	/* Re-sync (see datasheet §5.4):
	   Toggle the SLCK line from high to low to high.... */
	_SCLK_PIN = HIGH;
	delay_ms(5);
	_SCLK_PIN = LOW;
	delay_ms(1);
	_SCLK_PIN = HIGH;
	
	/* Wait at least tSIWTT (0.9 second?) for the
	   OptiMouse serial transaction timer to time out. */
	delay_ms(1000);
}


INT8 opti_mouse__get_dx( void )
{
	return opti_mouse__read_register(DELTA_X);
}


INT8 opti_mouse__get_dy( void )
{
	return opti_mouse__read_register(DELTA_Y);
}


#if OPTI_MOUSE_PROC_RAW_IMAGE == 1
UINT8* opti_mouse__get_raw_image( void )
{
	UINT8 i = 0;

	opti_mouse__raw_image_mode_on();

	/* read raw image data byte by byte */
	for (; i < RAW_IMAGE_SIZE*RAW_IMAGE_SIZE; i++) {
		_RAW_IMAGE_DATA[i] = opti_mouse__read_register(i + _RAW_IMAGE_DATA_OFFSET);									   	
	}

	opti_mouse__raw_image_mode_off();

	return _RAW_IMAGE_DATA;
}
#endif


/******************************************************************************
 * Private methods implementation
 ******************************************************************************/

static UINT8 opti_mouse__read_register(UINT8 address)
{
	INT8  i 	= 7;
	UINT8 byte 	= 0;
	
	/* write the address of the register we want to read */
	for (; i >= 0; i--)
	{
		/* todo: may be some delay will be useful here... */ 

		_SCLK_PIN = LOW;
		_SDIO_PIN = address & (1 << i);
		_SCLK_PIN = HIGH;
	}
	
	/* wait a bit... */
	delay_ms(100);
	
	/* fetch the data */
	for (i = 7; i >=0; i--)
	{                      
		/* todo: May be delays will be useful here... */ 
		      
		_SCLK_PIN = LOW;

   		/* set SDIO pin into 'read' mode */
		_SDIO_PIN = 1;
		byte |= ((UINT8) _SDIO_PIN) << i;

		_SCLK_PIN = HIGH;
	}
	delay_ms(100);
	
	return byte;
}


static void opti_mouse__write_register(UINT8 address, UINT8 byte)
{
	INT8 i = 7;
	
	/* set MSB high, to indicate write operation */
	address |= 0x80;
	
	/* write the address */
	for (; i >= 0; i--)
	{
		/* todo: May be some delay will be useful here... */ 

		_SCLK_PIN = LOW;
		_SDIO_PIN = address & (1 << i);
		_SCLK_PIN = HIGH;
	}
	
	/* write the data */
	for (i = 7; i >= 0; i--)
	{
		/* todo: May be some delay will be useful here... */ 

		_SCLK_PIN = LOW;
		_SDIO_PIN = byte & (1 << i);
		_SCLK_PIN = HIGH;
	}
}
