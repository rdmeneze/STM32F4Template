#ifndef __SYSTEM_TIMER_H__
#define __SYSTEM_TIMER_H__

#include <processor.h>

typedef uint32_t (*callbacktimer_func)( void* ) ;

typedef enum 
{ 
	systemTimerOneShot = 0,
	systemTimerRetriggerable 
} TSysTimerType;

/**
 *	@brief			Initialize the system timer
 *	@param[in]	dwTimeMicro system tick interval in microseconds
 *	@return			success or fail
 */
uint32_t SysTimerInit( uint32_t dwTimeMicro );

/**
 *	@brief			register a function callback timer 
 *	@param[in]	delay					delay to call the callback funtion. Delay units us
 *	@param[in]	type					function timer type
 *	@param[in]	callback_func	function timer callback
 *	@param[in]	lpParam				function timer param
 *	@param[out]	cbHandle			function timer handle
 *	@return			callback function creation success
 */
uint32_t SystemTimerRegisterCallBack( int32_t delay, TSysTimerType type, callbacktimer_func callback_func, void* lpParam, uint32_t* cbHandle );

/**
 *	@brief			unregister a function timer callback
 *	@param[in]	handle handle to callback functiion to unregister
 */
uint32_t	SystemTimerUnregisterCallBack( uint32_t handle );


#endif	//~

