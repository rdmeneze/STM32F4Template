#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <processor.h>


#define DEV_TYPE_IN		0x1
#define DEV_TYPE_OUT	0x0

//----------------------------------------------------------------------------

#define DEV_TABLE_START					(12)
#define DEV_ADDR_RL0						0x00		//! pay attention on this definitions at dev_relays.c
#define DEV_ADDR_RL1						0x01		//! pay attention on this definitions at dev_relays.c
#define DEV_ADDR_RL2						0x02		//! pay attention on this definitions at dev_relays.c
#define DEV_ADDR_RL3						0x03		//! pay attention on this definitions at dev_relays.c
#define DEV_LED_0								0x04		//! pay attention on this definitions at dev_leds.c
#define DEV_LED_1								0x05		//! pay attention on this definitions at dev_leds.c
#define DEV_LED_2								0x06		//! pay attention on this definitions at dev_leds.c
#define DEV_LED_3								0x07		//! pay attention on this definitions at dev_leds.c
#define DEV_TABLE_END						(21)
#define DEV_TAB_LEN (DEV_TABLE_END - DEV_TABLE_START-1)

//----------------------------------------------------------------------------

/**
 *	@brief	define the tdevice struct
 */
typedef struct TDEVICE 
{
	uint8_t		bType;					//! specify if tehe device is input or output	
	uint8_t		bState;					//! the actual state of device
	uint8_t		bNewState;			//!	the new state of device
	uint16_t	bDevAddress;		//!	device address
	uint32_t	dwTimeCounter;	//!	timer count
} TDevice;

//----------------------------------------------------------------------------

/**
 *	@brief Initialize the device engine
 */
uint8_t DevInit( void );

/**
 *	@brief			change the device state
 *	@param[in]	addr logical address of the device
 *	@param[in]	bVal	new value
 *	@return			success or fail of operation
 */
uint32_t	DevSetValue( uint16_t addr, uint8_t bVal, uint32_t delay );

/**
 *	@brief			get the device state
 *	@param[in]	addr logical address of the device
 *	@param[out]	bVal	new value
 *	@return			device value
 */
uint8_t		DevGetValue( uint16_t addr, uint8_t *bVal );

#endif	// ~__DEVICE_H__
