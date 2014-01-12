#include "dev_leds.h"
#include "device.h"
#include "errno.h"
#include <stm32f4xx_gpio.h>
#include <string.h>

//----------------------------------------------------------------------------

static GPIO_TypeDef * LED_GPIO[DEV_LED_3 - DEV_LED_0 + 1] = 
{
	[DEV_LED_0 - DEV_LED_0] = GPIOA,
	[DEV_LED_1 - DEV_LED_0] = GPIOA,
	[DEV_LED_2 - DEV_LED_0] = GPIOA,
	[DEV_LED_3 - DEV_LED_0] = GPIOA
};	

static GPIO_TypeDef* Get_LED_GPIO( uint32_t dwID )
{
	dwID -= DEV_LED_0;
	if ( dwID > (DEV_LED_3 - DEV_LED_0))
	{
		return NULL;
	}
	return LED_GPIO[dwID];
}

static uint16_t LED_PIN [DEV_LED_3 - DEV_LED_0 + 1] = 
{
	[DEV_LED_0 - DEV_LED_0] = GPIO_Pin_12,
	[DEV_LED_1 - DEV_LED_0] = GPIO_Pin_13,
	[DEV_LED_2 - DEV_LED_0] = GPIO_Pin_14,
	[DEV_LED_3 - DEV_LED_0] = GPIO_Pin_15	
};

static uint16_t Get_LED_PIN( uint32_t dwID )
{
	dwID -= DEV_LED_0;
	if ( dwID > (DEV_LED_3 - DEV_LED_0))
	{
		return (uint16_t)-1;
	}
	return LED_PIN[dwID];
}

//----------------------------------------------------------------------------

uint8_t LED_Init( void )
{
	GPIO_InitTypeDef initGPIO;
	GPIO_TypeDef* ugpioTD;
	
	ugpioTD = Get_LED_GPIO( DEV_LED_0 );		// if the LED mapping change, will 
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
	initGPIO.GPIO_Pin		= Get_LED_PIN( DEV_LED_0 );
	initGPIO.GPIO_Pin		|= Get_LED_PIN( DEV_LED_1 );
	initGPIO.GPIO_Pin		|= Get_LED_PIN( DEV_LED_2 );
	initGPIO.GPIO_Pin		|= Get_LED_PIN( DEV_LED_3 );
	
	GPIO_Init( ugpioTD, &initGPIO );
	
	return 0;
}

//----------------------------------------------------------------------------

uint8_t LED_ChangeStatus( uint16_t wID, uint8_t val )
{
	GPIO_TypeDef* ugpioTD;
	uint16_t			ledPin;
	
	
	ugpioTD = Get_LED_GPIO( wID );
	ledPin	= Get_LED_PIN( wID );
	
	if (( ugpioTD == NULL ) || (ledPin == (uint16_t)-1 ))
	{
		return EINVAL;
	}
	
	switch( wID )
	{
		case DEV_LED_0:
		case DEV_LED_1:
		case DEV_LED_2:
		case DEV_LED_3:
			
			if ( val )
				GPIO_SetBits( ugpioTD, ledPin );
			else
				GPIO_ResetBits( ugpioTD, ledPin );
			
			return 0;
			
		default:
			return EINVAL;
	}
	
}

//----------------------------------------------------------------------------
