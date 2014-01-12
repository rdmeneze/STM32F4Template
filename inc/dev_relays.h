#ifndef __DEV_RELAYS__
#define __DEV_RELAYS__

#include "processor.h"

/**
 *	@brief 	    initialize the led hardware
 *	@return	    success or fault status
 */
uint8_t Relay_Init( void );

/**
 *	@brief		change the status of signaling leds
 *	@param[in]	ID led address
 *	@param[in]	val new value
 *	@return		success  or fault
 */
uint8_t Relay_ChangeStatus( uint16_t wID, uint8_t val );

#endif //~__DEV_RELAYS__
