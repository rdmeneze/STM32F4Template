#include "dev_relays.h"
#include "device.h"
#include "errno.h"
#include <stm32f4xx_gpio.h>
#include <string.h>

//----------------------------------------------------------------------------

static GPIO_TypeDef * Relays_GPIO[DEV_ADDR_RL3 - DEV_ADDR_RL0 + 1] = 
{
	[DEV_ADDR_RL0 - DEV_ADDR_RL0] = NULL,
	[DEV_ADDR_RL1 - DEV_ADDR_RL0] = NULL,
	[DEV_ADDR_RL2 - DEV_ADDR_RL0] = NULL,
	[DEV_ADDR_RL3 - DEV_ADDR_RL0] = NULL
};	

static GPIO_TypeDef* Get_Relays_GPIO( uint32_t dwID )
{
	dwID -= DEV_ADDR_RL0;
	if ( dwID > (DEV_ADDR_RL3 - DEV_ADDR_RL0))
	{
		return NULL;
	}
	return Relays_GPIO[dwID];
}

static uint16_t Relays_PIN [DEV_ADDR_RL3 - DEV_ADDR_RL0 + 1] = 
{
	[DEV_ADDR_RL0 - DEV_ADDR_RL0] = (uint16_t)-1,
	[DEV_ADDR_RL1 - DEV_ADDR_RL0] = (uint16_t)-1,
	[DEV_ADDR_RL2 - DEV_ADDR_RL0] = (uint16_t)-1,
	[DEV_ADDR_RL3 - DEV_ADDR_RL0] = (uint16_t)-1	
};

static uint16_t Get_Relays_PIN( uint32_t dwID )
{
	dwID -= DEV_ADDR_RL0;
	if ( dwID > (DEV_ADDR_RL3 - DEV_ADDR_RL0))
	{
		return (uint16_t)-1;
	}
	return Relays_PIN[dwID];
}

//----------------------------------------------------------------------------

uint8_t Relay_Init( void )
{
	GPIO_InitTypeDef initGPIO;
	GPIO_TypeDef* ugpioTD;
	
	ugpioTD =    Get_Relays_GPIO( DEV_ADDR_RL0 );		// if the LED mapping change, will 
																					//    be necessary change the initialization code 

	if ( ugpioTD == NULL )
	{
		return EINVAL;
	}		
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	
	GPIO_StructInit( &initGPIO );
	initGPIO.GPIO_Mode	= GPIO_Mode_OUT;
	initGPIO.GPIO_OType	= GPIO_OType_PP;
	initGPIO.GPIO_PuPd	=	GPIO_PuPd_UP;
	initGPIO.GPIO_Speed	=	GPIO_Speed_25MHz;
	initGPIO.GPIO_Pin		= Get_Relays_PIN( DEV_ADDR_RL0 );
	initGPIO.GPIO_Pin		|= Get_Relays_PIN( DEV_ADDR_RL1 );
	initGPIO.GPIO_Pin		|= Get_Relays_PIN( DEV_ADDR_RL2 );
	initGPIO.GPIO_Pin		|= Get_Relays_PIN( DEV_ADDR_RL3 );
	
	GPIO_Init( ugpioTD, &initGPIO );
	
	return 0;
}

//----------------------------------------------------------------------------

uint8_t Relay_ChangeStatus( uint16_t wID, uint8_t val )
{
	GPIO_TypeDef* ugpioTD;
	uint16_t			pin;
	
	
	ugpioTD = Get_Relays_GPIO( wID );
	pin			= Get_Relays_PIN( wID );
	
	if (( ugpioTD == NULL ) || (pin == (uint16_t)-1 ))
	{
		return EINVAL;
	}
	
	switch( wID )
	{
		case DEV_ADDR_RL0:
		case DEV_ADDR_RL1:
		case DEV_ADDR_RL2:
		case DEV_ADDR_RL3:
			
			if ( val )
				GPIO_SetBits( ugpioTD, pin );
			else
				GPIO_ResetBits( ugpioTD, pin );
			return 0;
			
		default:
			return EINVAL;
	}
	
}

//----------------------------------------------------------------------------
