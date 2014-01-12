#include "device.h"
#include "utils.h"
#include "system_timer.h"
#include "errno.h"
#include <string.h>
#include "dev_leds.h"
#include "dev_relays.h"

//----------------------------------------------------------------------------

#define DEV_REFRESH_DELAY	500 //! refresh delay in us

//--------------------------		module variables    --------------------------

uint8_t		bInit = 0;

//----------------------------------------------------------------------------

TDevice device[DEV_TAB_LEN] = 
{ 
	[0] = 
	{
		.bType 				= DEV_TYPE_OUT,			//! specify if tehe device is input or output	
		.bState 			= 0,								//! the actual state of device
		.bNewState 		= 0,								//!	the new state of device
		.bDevAddress 	= DEV_ADDR_RL0,			//!	device address
		.dwTimeCounter= 0,								//!	timer count
	},
	[1] =  
	{
		.bType 				= DEV_TYPE_OUT,			//! specify if tehe device is input or output	
		.bState 			= 0,								//! the actual state of device
		.bNewState 		= 0,								//!	the new state of device
		.bDevAddress 	= DEV_ADDR_RL1,			//!	device address
		.dwTimeCounter= 0,								//!	timer count
	},
	[2] =  
	{
		.bType 				= DEV_TYPE_OUT,			//! specify if tehe device is input or output	
		.bState 			= 0,								//! the actual state of device
		.bNewState 		= 0,								//!	the new state of device
		.bDevAddress 	= DEV_ADDR_RL2,			//!	device address
		.dwTimeCounter= 0,								//!	timer count
	},
	[3] =  
	{
		.bType 				= DEV_TYPE_OUT,			//! specify if tehe device is input or output	
		.bState 			= 0,								//! the actual state of device
		.bNewState 		= 0,								//!	the new state of device
		.bDevAddress 	= DEV_ADDR_RL3,			//!	device address
		.dwTimeCounter= 0,								//!	timer count
	},
	[4] =  
	{
		.bType 				= DEV_TYPE_OUT,			//! specify if tehe device is input or output	
		.bState 			= 0,								//! the actual state of device
		.bNewState 		= 0,								//!	the new state of device
		.bDevAddress 	= DEV_LED_0,			//!	device address
		.dwTimeCounter= 0,								//!	timer count
	},
	[5] =  
	{
		.bType 				= DEV_TYPE_OUT,			//! specify if tehe device is input or output	
		.bState 			= 0,								//! the actual state of device
		.bNewState 		= 0,								//!	the new state of device
		.bDevAddress 	= DEV_LED_1,			//!	device address
		.dwTimeCounter= 0,								//!	timer count
	},
	[6] =  
	{
		.bType 				= DEV_TYPE_OUT,			//! specify if tehe device is input or output	
		.bState 			= 0,								//! the actual state of device
		.bNewState 		= 0,								//!	the new state of device
		.bDevAddress 	= DEV_LED_2,			//!	device address
		.dwTimeCounter= 0,								//!	timer count
	},
	[7] =  
	{
		.bType 				= DEV_TYPE_OUT,			//! specify if tehe device is input or output	
		.bState 			= 0,								//! the actual state of device
		.bNewState 		= 0,								//!	the new state of device
		.bDevAddress 	= DEV_LED_3,			//!	device address
		.dwTimeCounter= 0,								//!	timer count
	},	
};

//----------------------------------------------------------------------------

uint32_t	DevProcess( void* lParam );
uint32_t 	DevChangeStatus( TDevice * ptrDev );
uint8_t		VerifyAddress( uint16_t addr );

//----------------------------------------------------------------------------

uint8_t DevInit( void )
{
	uint32_t dwRet;
	if ( bInit == 0 ) 
	{
		// register a callback function
		dwRet = SystemTimerRegisterCallBack( DEV_REFRESH_DELAY, systemTimerRetriggerable, DevProcess, NULL, NULL );
		if ( dwRet ) 
		{	// error
			return dwRet;
		}
		
		bInit = 1;
	}
	return 0;
}

//----------------------------------------------------------------------------

uint32_t	DevSetValue( uint16_t addr, uint8_t bVal, uint32_t delay )
{
	TDevice * ptrDev;
	uint32_t	iCount;
	
	if ( !VerifyAddress( addr ) )
	{
		return EINVAL;
	}

	for ( ptrDev = &device[0], iCount = 0 ; iCount < VECTOR_LEN( device ) ; iCount++, ptrDev++ )
	{
		if ( ptrDev->bDevAddress == addr )
		{
			ptrDev->bNewState 		= bVal;
			ptrDev->dwTimeCounter = delay;
			break;
		}
	}

	return 0;
}

//----------------------------------------------------------------------------

uint8_t	DevGetValue( uint16_t addr, uint8_t *bVal )
{	
	TDevice * ptrDev;
	uint32_t	iCount;
	
	if ( !VerifyAddress( addr )  )
	{
		return EINVAL;
	}

	for ( ptrDev = &device[0], iCount = 0 ; iCount < VECTOR_LEN( device ) ; iCount++, ptrDev++ )
	{
		if ( ptrDev->bDevAddress == addr )
		{
			*bVal = ptrDev->bState;
			break;
		}
	}

	return 0;
}

//----------------------------------------------------------------------------

uint8_t	VerifyAddress( uint16_t addr )
{
	switch( addr )
	{
		case DEV_ADDR_RL0:
		case DEV_ADDR_RL1:
		case DEV_ADDR_RL2:
		case DEV_ADDR_RL3:
		case DEV_LED_0:								
		case DEV_LED_1:
		case DEV_LED_2:
		case DEV_LED_3:
			return 1;
		default:
			return 0;
	}
}

//----------------------------------------------------------------------------

uint32_t 		DevProcess( void* lParam )
{
	TDevice * ptrDev;
	uint32_t	iCount;
	
	if ( bInit == 0 )
	{
		return EINVAL;
	}
	
	for ( ptrDev = &device[0], iCount = 0 ; iCount < VECTOR_LEN( device ) ; iCount++, ptrDev++ )
	{
		if ( ptrDev->bNewState != ptrDev->bState  )
		{
			if ( ptrDev->dwTimeCounter <= 0 )
			{
				DevChangeStatus( ptrDev );
			}
		}
	}
	return 0;
}

//----------------------------------------------------------------------------

/**
 *	@brief	function that is responsable to change the peripheral status
 */
uint32_t DevChangeStatus( TDevice * ptrDev )
{
	uint32_t dwRet;
	
	dwRet = EFAULT;
	switch( ptrDev->bDevAddress )
	{
		case DEV_ADDR_RL0:
		case DEV_ADDR_RL1:
		case DEV_ADDR_RL2:
		case DEV_ADDR_RL3:
			dwRet = Relay_ChangeStatus( ptrDev->bDevAddress, ptrDev->bNewState );
			break;
			
		case DEV_LED_0:								
		case DEV_LED_1:
		case DEV_LED_2:
		case DEV_LED_3:
			dwRet =	LED_ChangeStatus( ptrDev->bDevAddress, ptrDev->bNewState );
			break;
	}
	
	if ( !dwRet )
	{
		ptrDev->bState = ptrDev->bNewState;
	}
	
	return	dwRet;
}

//----------------------------------------------------------------------------
