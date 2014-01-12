#include <system_timer.h>
#include <string.h>
#include "errno.h"
#include <utils.h>
#include <uniquecount.h>

//----------------------------------------------------------------------------

#define SYSTIMER_ENTRY_CB 				64
#define SYSTIMER_ENTRY_NULL				0
#define SYSTIMER_MAGIC						0xCCAACCAA

#define SYSTIMER_TIMEBASE_us			(SystemCoreClock / 1000000.)
#define SYSTIMER_TIMEBASE_ms			(SystemCoreClock / 1000.)

typedef struct _STSYSTEMTIMERCB
{
	int32_t 							magic;						//! magic number
	uint32_t 							handle;						//!	handle to timer structure 
	int32_t 							count;						//!	actual count
	int32_t 							countTime;				//! the reload time
	TSysTimerType					type;							//! type of timer: 	systemTimerOneShot or systemTimerRetriggerable 
	void*									lpParam;					//! param passed in register function
	callbacktimer_func 		callback_func;		//! callback function address
}STSystemTimerCB;

//----------------------------------------------------------------------------

STSystemTimerCB vecSystemTimerCB[SYSTIMER_ENTRY_CB];

static uint32_t iSysTimerTimeBase = 10;	//!	time base in us

//----------------------------------------------------------------------------

uint32_t SysTimerInit( uint32_t dwTimeMicro )
{
	int i;
	uint32_t returnCode;
	STSystemTimerCB 			*ptrTimerCB;

	memset( (char*)&vecSystemTimerCB,	(uint8_t)-1, sizeof( vecSystemTimerCB ));
	for ( i = 0, ptrTimerCB = &vecSystemTimerCB[0]; i < VECTOR_LEN( vecSystemTimerCB ); i++, ptrTimerCB++)
	{
		ptrTimerCB->magic		= SYSTIMER_MAGIC;
		ptrTimerCB->handle	= SYSTIMER_ENTRY_NULL;
	}
	
	if ( dwTimeMicro < 10 ) 
	{
		dwTimeMicro = 10;
	}
	else if ( dwTimeMicro == (uint32_t)-1 )
	{
		dwTimeMicro = SysTick_LOAD_RELOAD_Msk/SYSTIMER_TIMEBASE_us;
	}
	
	//! interrupção para dwTimeMicro
	returnCode = SysTick_Config( SYSTIMER_TIMEBASE_us * dwTimeMicro ); 
	if ( returnCode )
	{
		return 1;
	}
	
	iSysTimerTimeBase = dwTimeMicro;
	
	return 0;
}

//----------------------------------------------------------------------------

uint32_t SystemTimerRegisterCallBack( int32_t delay, TSysTimerType type, callbacktimer_func callback_func, void* lpParam, uint32_t* cbHandle )
{
	int i;
	uint32_t count;
	uint32_t reminder;
	STSystemTimerCB *ptrTimerCB;
	
	if ( vecSystemTimerCB[0].magic != SYSTIMER_MAGIC )
	{
		return EPERM;
	}
	
	if (delay < iSysTimerTimeBase )
		delay = iSysTimerTimeBase;
	
	count = delay / iSysTimerTimeBase;
	reminder = delay % iSysTimerTimeBase;
	if ( ((10*reminder) / iSysTimerTimeBase ) > 5 )
		count++;
	
	for ( ptrTimerCB = &vecSystemTimerCB[0], i = 0; i < VECTOR_LEN( vecSystemTimerCB ); i++, ptrTimerCB++)
	{
		if ( ptrTimerCB->handle == SYSTIMER_ENTRY_NULL )
		{
			ptrTimerCB->handle						= GetUniqueCount();
			ptrTimerCB->countTime					=	count;
			ptrTimerCB->count							= ptrTimerCB->countTime;
			ptrTimerCB->callback_func			=	callback_func;
			ptrTimerCB->lpParam						= lpParam;
			ptrTimerCB->type							= type;
			break;
		}
	}
	
	if ( i == VECTOR_LEN( vecSystemTimerCB ) )
	{
		return EFAULT;
	}
	
	if ( cbHandle != NULL )
	{
		*cbHandle = ptrTimerCB->handle;
	}
	
	return 0; 
}

//----------------------------------------------------------------------------

uint32_t	SystemTimerUnregisterCallBack( uint32_t handle )
{
	STSystemTimerCB *ptrTimerCB;
	uint32_t i;
	
	if ( vecSystemTimerCB[0].magic != SYSTIMER_MAGIC )
	{
		return EPERM;
	}	
	
	for ( ptrTimerCB = &vecSystemTimerCB[0], i = 0; i < VECTOR_LEN( vecSystemTimerCB ); i++, ptrTimerCB++)
	{
		if ( ptrTimerCB->handle == handle )
		{
			ptrTimerCB->handle						= SYSTIMER_ENTRY_NULL;
			ptrTimerCB->countTime					=	-1;
			ptrTimerCB->count							= ptrTimerCB->countTime;
			ptrTimerCB->lpParam 					= NULL;
			ptrTimerCB->type							= systemTimerOneShot;
			ptrTimerCB->callback_func			=	NULL;
			break;
		}
	}
		
	if ( i == VECTOR_LEN( vecSystemTimerCB ) )
	{
		return EFAULT;
	}
	
	return 0;
}

//----------------------------------------------------------------------------

/**
 *	@brief 	SysTick_Handler System Tick timer ISR
 *					decrement the count fielt in the structure vecSystemTimerCB. 
 *					Dependending on value of count fielt, call the callback function and perform some operations. 
 *					
 * 					Pay Attention!  Only one callback function is called for ISR each execution
 */
void SysTick_Handler (void)  
{
	int	i;
	STSystemTimerCB*	ptrVecTimerCB;
	uint32_t	dwExecuteCBFlag = 0;
	
	for ( i = 0, ptrVecTimerCB = vecSystemTimerCB; i < VECTOR_LEN( vecSystemTimerCB ); i++, ptrVecTimerCB++  )
	{
		if ( ptrVecTimerCB->handle != SYSTIMER_ENTRY_NULL )
		{
			ptrVecTimerCB->count--;
			if ( !dwExecuteCBFlag )
			{
				if ( ptrVecTimerCB->count < 0 )
				{
					if ( ptrVecTimerCB->callback_func != NULL )
					{
						dwExecuteCBFlag = 1;
						(ptrVecTimerCB->callback_func)( ptrVecTimerCB->lpParam );	// execute the callback code
						if ( ptrVecTimerCB->type  ==  systemTimerOneShot )
						{
							SystemTimerUnregisterCallBack( ptrVecTimerCB->handle );
						}
						else
						{
							ptrVecTimerCB->count = ptrVecTimerCB->countTime;
						}
					}
				}
			}
		}
	}
	return;
}

//----------------------------------------------------------------------------
