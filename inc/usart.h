/**
 *	@file	usart.h
 *	@brief	universal sincronous assincronous receiver transmiter (USART) module 
 *	@author	Rafael Dias <rdmeneze@gmail.com>
 *	@date 	2013/11
 */

#ifndef __USART_H__
#define __USART_H__

#include <processor.h>
#include <string.h>

//!	callback for usart operations
typedef uint32_t (*callbackusart_func)( void* ) ;

//!	com port definitions
typedef enum 
{
	COM1 = 0,
	COM2,
	COM3,
	COM4,
	COM5,
	COM6, 
	COM7, 
	COM8 
} COMPORT;

//!	number of  communications  ports channels
#define COMPORTS (COM8+1)

//!	baud rate
typedef enum 
{
	BR_300 = 0, 
	BR_600, 
	BR_1200,
	BR_2400, 
	BR_4800,
	BR_9600,
	BR_19200, 
	BR_38400, 
	BR_57600,
	BR_115200
} BaudRate;

//!	parity configuration
typedef enum 
{
	PR_NONE = 0, 
	PR_EVEN, 
	PR_ODD
} Parity;

//!	stop bits
typedef enum
{
	ST_ONE = 0,
	ST_TWO,
	ST_ONE_HALF
} StopBits;

typedef enum 
{
	FC_None = 0, 
	FC_RTS, 
	FC_CTS, 
	FC_RTS_CTS
} FlowControl;

//!	type of events generated by usart module
typedef enum 
{
	F_CB_OVR = (0),	//! overrun
	F_CB_RXD = (1),	//!	data received
	F_CB_TXD = (2)	//!	data transmited
} UsartEvents;

#define NUSARTEVENTS 3

//!	number of usart events
#define USARTCALLBACKENTRYPOINTS (NUSARTEVENTS)

//!	usart control struct
typedef struct TUSART
{
	uint16_t	bInit;			//!	initialization flag
	COMPORT 	comPort;		//! com port designator
	BaudRate 	baud;			//! baud rate configuration
	Parity		parity;			//!	parity configuration
	FlowControl	uFlowControl;	//! flow control
	StopBits	stopBits;		//!	stop bits
	char * pSendBuffer;		//! pointer to send buffer
	char * pRcvBuffer;		//! pointer to receiver buffer
	uint32_t	dwSendBufferLen;	//! length of send buffer
	uint32_t	dwRcvBufferLen;		//! length of receive buffer
	char * 		pSendBufferHead;	//! pointer to send circular buffer head
	char * 		pSendBufferTail;	//! pointer to send circular buffer tail
	char * 		pRcvBufferHead;		//! pointer to receive circular buffer head
	char * 		pRcvBufferTail;		//! pointer to receive circular buffer tail
	callbackusart_func	callbackfunc[USARTCALLBACKENTRYPOINTS];	//! pointer to a callback receiver function
	uint32_t	dwFlags;
} TUsart;

//----------------------------------------------------------------------------

/**
 *	@brief			get a comport struct pointer 
 *	@param[in]	com port address
 *	@return			pointer to a com port structure
 */
TUsart*	GetUsart( COMPORT com );

/**
 *	@brief	
 *	@param[in]	usart pointer to a usart struct
 *	@return		success or fail status
 */
uint32_t USARTInit(	TUsart * usart );

/**
 *	@brief		configure the baudrate
 *	@param[in]	usart pointer to a usart struct
 * 	@param[in]	baud baud rate
 *	@return		success or fail status
 */
uint32_t USARTSetBaudRate( TUsart * usart, BaudRate baud );

/**
 *	@brief		configure the baudrate
 *	@param[in]	usart pointer to a usart struct
 *	@return		baud rate value
 */
BaudRate USARTGetBaudRate( TUsart * usart );

/**
 *	@brief		configure the baudrate
 *	@param[in]	usart pointer to a usart struct
 * 	@param[in]	pr parity
 *	@return		success or fail status
 */
uint32_t USARTSetParity( TUsart * usart, Parity pr );

/**
 *	@brief		configure the parity
 *	@param[in]	usart pointer to a usart struct
 *	@return		Parity value
 */
Parity USARTGetParity( TUsart * usart );

/**
 *	@brief		configure the Stop Bits
 *	@param[in]	usart pointer to a usart struct
 * 	@param[in]	st StopBits
 *	@return		success or fail status
 */
uint32_t USARTSetStopBits( TUsart * usart, StopBits st );

/**
 *	@brief		configure the baudrate
 *	@param[in]	usart pointer to a usart struct
 *	@return		StopBits value
 */
StopBits USARTGetStopBits( TUsart * usart );

/**
 *	@brief		configure the Stop Bits
 *	@param[in]	usart pointer to a usart struct
 * 	@param[in]	rcv receive buffer
 *	@param[in]	rcvLen length of the receiver buffer
 * 	@param[in]	send send buffer
 *	@param[in]	sendLen length of the send buffer
 *	@return		success or fail status
 */
uint32_t USARTSetBuffers( TUsart * usart, const char* rcv, const uint32_t rcvLen, const char * send, const uint32_t sendLen );

/**
 *	@brief		send a buffer across the serial interface
 *	@param[in]	usart pointer to a usart struct
 *	@param[in]	buffer	pointer to buffer to send
 * 	@param[in]	buflen	length of buffer to send	
 *	@return		success or fail status
 */
uint32_t USARTSend(	TUsart * usart, const char * buffer, const uint16_t buflen );

/**
 *	@brief	
 *	@param[in]	usart pointer to a usart structure
 *	@param[in]	callback_func	pointer to callback function
 *  @param[in]	callback function flag
 *	@return		success or fail status
 */
uint32_t USARTRegisterCallBack( TUsart* usart, callbackusart_func callback_func, UsartEvents flag );

/**
 *	@brief		unregister a callback function
 *	@param[in]	usart pointer to a usart structure
 *  @param[in]	callback function flag
 *	@return		success or error status
 */
uint32_t USARTUnregisterCallBack( TUsart* usart, UsartEvents flag );

#endif
