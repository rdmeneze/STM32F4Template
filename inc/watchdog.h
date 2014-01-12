/**
 *	@file		watchdog.h
 *	@brief	implementation of watchdog timer interface
 *	@author	Rafael Dias Menezes
 *	@date		Dez/2013
 */


#ifndef __WATCHDOG__H_
#define __WATCHDOG__H_

#include "processor.h"

//!	callback to watchdog timer
typedef uint32_t (*callbackwdt_func)( void* );

/**
 *	@brief	initialize the watchdog system
 *	@return	success or fail state
 */
uint32_t	WatchDogInit( void );

/**
 *	@brief	reset the watchdog system
 *	@return	success or fail state
 */
uint32_t	WatchDogReset( void );

/**
 *	@brief	initialize callback watchdog timer function
 *					This function is called when the watchdog timer counter is 0x40
 *	@return	success or fail state
 */
uint32_t	WatchDogRegisterCallBack( callbackwdt_func cb );

#endif
