/**
 *  @brief  IMplementation of WatchDog functionalities
 *  @Author Rafael Dias Menezes <rdmeneze@gmail.com>
 *  @date   December/2013
 */

#include "errno.h"
#include "watchdog.h"
#include <string.h>
#include "utils.h"
#include <stm32f4xx_wwdg.h>
#include <uniquecount.h>

//----------------------------------------------------------------------------

#define WDT_ENTRY_CB    1
#define WATCHDOG_MAGIC  0xDDAACCAA

//----------------------------------------------------------------------------

typedef struct _STWATCHDOGCB
{
	int32_t         magic;						//! magic number
	void*           lpParam;					//! param passed in register function
  uint32_t        handle;           
	callbackwdt_func  callback_func;		//! callback function address
}STWatchDogCB;

STWatchDogCB vecWatchDogCB[WDT_ENTRY_CB];

//----------------------------------------------------------------------------

uint32_t	WatchDogInit( void )
{
  int i;
  STWatchDogCB * ptrSTWatchDogCB;
  for ( i = 0, ptrSTWatchDogCB = &vecWatchDogCB[0]; i < VECTOR_LEN( vecWatchDogCB ); i++ )
  {
    ptrSTWatchDogCB->magic          = WATCHDOG_MAGIC;
    ptrSTWatchDogCB->lpParam        = NULL;
    ptrSTWatchDogCB->callback_func  = NULL;
    ptrSTWatchDogCB->handle         = (uint32_t)-1;
  }

	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue( 0x7F );
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
uint32_t	WatchDogRegisterCallBack( callbackwdt_func cb, void* lParam, uint32_t* cbHandle )
{
  if ( vecWatchDogCB[0].magic != WATCHDOG_MAGIC )
  {
    return EPERM;
  }
  
  if ( cb == NULL )
  {
    return EPERM;
  }

  vecWatchDogCB[0].handle         = GetUniqueCount();
  vecWatchDogCB[0].callback_func  = cb;
  vecWatchDogCB[0].lpParam        = lParam;
  
  if ( cbHandle != NULL )
{
    *cbHandle = vecWatchDogCB[0].handle;
  }
  
	WWDG_EnableIT();
	return 0;
}

//----------------------------------------------------------------------------

void WWDG_IRQHandler( void )    //! interrupt function
{
  vecWatchDogCB[0].callback_func( vecWatchDogCB[0].lpParam );
  
	return;
}

//----------------------------------------------------------------------------
