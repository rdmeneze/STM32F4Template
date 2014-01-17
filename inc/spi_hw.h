/**
 *	@file	spi_hw.h
 *	@brief	Master Serial Peripheral Interface module 
 *	@author	Rafael Dias <rdmeneze@gmail.com>
 *	@date 	2014/01
 */

#ifndef __SPI_HW__
#define __SPI_HW__

#include <processor.h>
#include <string.h>

//!	com port definitions
typedef enum 
{
	SPI1 = 0,
	SPI2,
	SPI3,
	SPI4,
	SPI5,
	SPI6, 
	SPI7, 
	SPI8 
} SPIPORT;


typedef enum 
{
    CM0 = 0,
    CM1, 
    CM2
} SPIMODE;

typedef struct TMASTERSPI
{
	uint16_t	bInit;			//!	initialization flag
	SPIPORT 	port;		//! com port designator
    SPIMODE     mode;
	char * pSendBuffer;		//! pointer to send buffer
	char * pRcvBuffer;		//! pointer to receiver buffer
	uint32_t	dwSendBufferLen;	//! length of send buffer
	uint32_t	dwRcvBufferLen;		//! length of receive buffer
	char * 		pSendBufferHead;	//! pointer to send circular buffer head
	char * 		pSendBufferTail;	//! pointer to send circular buffer tail
	char * 		pRcvBufferHead;		//! pointer to receive circular buffer head
	char * 		pRcvBufferTail;		//! pointer to receive circular buffer tail
	uint32_t	dwFlags;
} TMasterSPI;



/**
 *	@brief		get a comport struct pointer 
 *	@param[in]	com port address
 *	@return		pointer to a com port structure
 */
TMasterSPI*	GetSPI( SPIPORT port );

/**
 *	@brief	
 *	@param[in]	spi pointer to a usart struct
 *	@return		success or fail status
 */
uint32_t SPI_Init(	TMasterSPI * spi );


#endif //__SPI_HW__ 