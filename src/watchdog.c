/**
 *  @brief  
 */

#include "errno.h"
#include "watchdog.h"
#include <string.h>
#include <stm32f4xx_wwdg.h>

//----------------------------------------------------------------------------

callbackwdt_func callback_watchdogtimer = NULL;

//----------------------------------------------------------------------------

uint32_t	WatchDogInit( void )
{
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue( 0x45 );
	WWDG_SetCounter( 0x7F );
	WWDG_Enable( 0x7F );


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	return 0;
}

//----------------------------------------------------------------------------

uint32_t	WatchDogReset( void )
{
	WWDG_SetCounter( 0x7F );
	return 0;
}

//----------------------------------------------------------------------------

uint32_t	WatchDogRegisterCallBack( callbackwdt_func cb )
{
	callback_watchdogtimer = cb;
	WWDG_EnableIT();
	return 0;
}

//----------------------------------------------------------------------------

void WWDG_IRQHandler( void )    //! interrupt function
{
	if ( callback_watchdogtimer != NULL )
	{
		callback_watchdogtimer( 0 );
	}
	return;
}

//----------------------------------------------------------------------------
