/**
 *	@file 	timer functions header file
 *	@author	Rafael Dias Menezes	<rafael.menezes@voxage.com.br>	
  * @date		Mai/2013
 */

#include "defs.h"


#ifndef __TIMER_H__
#define __TIMER_H__

typedef DWORD (*CallBackFunc)(PVOID);

typedef struct _CTimerCallBack
{
	PVOID 				param;
	CallBackFunc	callbackfunc;
} CTimerCallBack;


/**
 * @brief	initialize the timer functions callback
 */
BYTE	Timer_Init( void );

/**
 *	@brief get a timer slot
 */
DWORD	Timer_Get( void );

/**
 *	@brief verify if a timer had timeout	
 */
BYTE	Timer_TimeOut( DWORD dwTimerID );

/**
 *	
 */
BYTE	Timer_SetCallBackFunction( CTimerCallBack c ); 

#endif // ~__TIMER_H__
