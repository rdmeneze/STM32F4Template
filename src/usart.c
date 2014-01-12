/**
 *	@file 	usart.c
 *	@author	Rafael Dias
 *	@date	Nov/2013
 * 
 * 	@
 */


#include "usart.h"
#include "errno.h"
#include <string.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_gpio.h>

//----------------------------------------------------------------------------

uint32_t USART_TX_WorkFunction( void* lParam );

//----------------------------------------------------------------------------

static TUsart usart[COMPORTS] = 
{
	[COM1] = 
	{
		.bInit				=	0,
		.comPort			= COM1,
		.baud				= BR_9600,
		.parity				=	PR_NONE,
		.stopBits			=	ST_ONE,
		.pSendBuffer		= NULL, 
		.pRcvBuffer			=	NULL, 
		.dwSendBufferLen 	= 0,
		.dwRcvBufferLen 	= 0,
		.pSendBufferHead	= NULL,
		.pSendBufferTail	= NULL,
		.pRcvBufferHead		= NULL,
		.pRcvBufferTail		= NULL,
		.callbackfunc 		= {0},
		.dwFlags			= 0,
		.uFlowControl		= FC_None
	},
	[COM2] = 
	{
		.bInit				=	0,
		.comPort 			= COM2,
		.baud				= BR_9600,
		.parity				=	PR_NONE,
		.stopBits			=	ST_ONE,
		.pSendBuffer		= NULL, 
		.pRcvBuffer			=	NULL, 
		.dwSendBufferLen 	= 0,
		.dwRcvBufferLen 	= 0,
		.pSendBufferHead	= NULL,
		.pSendBufferTail	= NULL,
		.pRcvBufferHead		= NULL,
		.pRcvBufferTail		= NULL,
		.callbackfunc 		= {0},
		.dwFlags			= 0,
		.uFlowControl		= FC_None
	},
	[COM3] = 
	{
		.bInit				=	0,
		.comPort 			= COM3,
		.baud				= BR_9600,
		.parity				=	PR_NONE,
		.stopBits			=	ST_ONE,
		.pSendBuffer		= NULL, 
		.pRcvBuffer			=	NULL, 
		.dwSendBufferLen 	= 0,
		.dwRcvBufferLen 	= 0,
		.pSendBufferHead	= NULL,
		.pSendBufferTail	= NULL,
		.pRcvBufferHead		= NULL,
		.pRcvBufferTail		= NULL,
		.callbackfunc 		= {0},
		.dwFlags			= 0,
		.uFlowControl		= FC_None
    },
	[COM4] = 
	{
		.bInit				=	0,
		.comPort 			= COM4,
		.baud				= BR_9600,
		.parity				=	PR_NONE,
		.stopBits			=	ST_ONE,
		.pSendBuffer		= NULL, 
		.pRcvBuffer			=	NULL, 
		.dwSendBufferLen 	= 0,
		.dwRcvBufferLen 	= 0,
		.pSendBufferHead	= NULL,
		.pSendBufferTail	= NULL,
		.pRcvBufferHead		= NULL,
		.pRcvBufferTail		= NULL,
		.callbackfunc 		= {0},
		.dwFlags			= 0,
		.uFlowControl		= FC_None
    },

	[COM5] = 
	{
		.bInit				=	0,
		.comPort 			= COM5,
		.baud				= BR_9600,
		.parity				=	PR_NONE,
		.stopBits			=	ST_ONE,
		.pSendBuffer		= NULL, 
		.pRcvBuffer			=	NULL, 
		.dwSendBufferLen 	= 0,
		.dwRcvBufferLen 	= 0,
		.pSendBufferHead	= NULL,
		.pSendBufferTail	= NULL,
		.pRcvBufferHead		= NULL,
		.pRcvBufferTail		= NULL,
		.callbackfunc 		= {0},
		.dwFlags			= 0,
		.uFlowControl		= FC_None
    },

	[COM6] = 
	{
		.bInit				=	0,
		.comPort 			= COM6,
		.baud				= BR_9600,
		.parity				=	PR_NONE,
		.stopBits			=	ST_ONE,
		.pSendBuffer		= NULL, 
		.pRcvBuffer			=	NULL, 
		.dwSendBufferLen 	= 0,
		.dwRcvBufferLen 	= 0,
		.pSendBufferHead	= NULL,
		.pSendBufferTail	= NULL,
		.pRcvBufferHead		= NULL,
		.pRcvBufferTail		= NULL,
		.callbackfunc 		= {0},
		.dwFlags			= 0,
		.uFlowControl		= FC_None
    },

	[COM7] = 
	{
		.bInit				=	0,
		.comPort 			= COM7,
		.baud				= BR_9600,
		.parity				=	PR_NONE,
		.stopBits			=	ST_ONE,
		.pSendBuffer		= NULL, 
		.pRcvBuffer			=	NULL, 
		.dwSendBufferLen 	= 0,
		.dwRcvBufferLen 	= 0,
		.pSendBufferHead	= NULL,
		.pSendBufferTail	= NULL,
		.pRcvBufferHead		= NULL,
		.pRcvBufferTail		= NULL,
		.callbackfunc 		= {0},
		.dwFlags			= 0,
		.uFlowControl		= FC_None
    },

	[COM8] = 
	{
		.bInit				=	0,
		.comPort 			= COM8,
		.baud				= BR_9600,
		.parity				=	PR_NONE,
		.stopBits			=	ST_ONE,
		.pSendBuffer		= NULL, 
		.pRcvBuffer			=	NULL, 
		.dwSendBufferLen 	= 0,
		.dwRcvBufferLen 	= 0,
		.pSendBufferHead	= NULL,
		.pSendBufferTail	= NULL,
		.pRcvBufferHead		= NULL,
		.pRcvBufferTail		= NULL,
		.callbackfunc 		= {0},
		.dwFlags			= 0,
		.uFlowControl		= FC_None
	}
};


//! COM_USART: configure the usart ports
static USART_TypeDef* COM_USART[COMPORTS] = 
{
	[COM1] = USART1	, 
	[COM2] = USART2	,
	[COM3] = USART3	, 
	[COM4] = UART4	, 
	[COM5] = UART5	, 
	[COM6] = USART6	, 
	[COM7] = UART7	, 
	[COM8] = UART8
};

/** 
 *	@brief	get a pointer to a USART_TypeDef structure
 * 	@param[in]	com	COM port to get a pointer
 * 	@return	pointer to a USART_TypeDef structure
 */
static USART_TypeDef* GetUsartTD( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return NULL;
	}
	return COM_USART[com];
}

/**
 *	@brief	COM_USART_IRQ: get the IRQs for each U(S)ART 
 */
static uint8_t COM_USART_IRQ[COMPORTS] = 
{
	[COM1] = USART1_IRQn	, 
	[COM2] = USART2_IRQn	,
	[COM3] = USART3_IRQn	, 
	[COM4] = UART4_IRQn		, 
	[COM5] = UART5_IRQn		, 
	[COM6] = USART6_IRQn	, 
	[COM7] = (uint8_t)-100  , 
	[COM8] = (uint8_t)-100
};


/** 
 *	@brief	get the IRQs for each U(S)ART
 * 	@param[in]	com	COM port to get the IRQ
 * 	@return	IRQ value 
 */
static uint8_t GetCOM_USART_IRQ( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint8_t)-100;
	}
	return COM_USART_IRQ[com];
}

/**
 *	@brief RCC_APBPeriph:	get the APB Bus that the peripheral is connected
 */  
static uint32_t RCC_APBPeriph[COMPORTS]= 
{
	[COM1] = RCC_APB2Periph_USART1  ,
	[COM2] = RCC_APB1Periph_USART2  ,
	[COM3] = RCC_APB1Periph_USART3  ,
	[COM4] = RCC_APB1Periph_UART4   ,
	[COM5] = RCC_APB1Periph_UART5   ,
	[COM6] = RCC_APB2Periph_USART6  ,
	[COM7] = 0,
	[COM8] = 0
};

/**
 *	@brief	get the APB periph associated with the USART port
 * 	@param[in]	com	COM port to get the IRQ
 * 	@return	IRQ value 
 */
static uint32_t GetRCC_APBPeriph( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return 0;
	}
	return RCC_APBPeriph[com];
}

/**
 *	@brief RCC_AHBPeriph:	get the AHB Bus that the peripheral is connected
 */  
static uint32_t RCC_AHBPeriph[COMPORTS]= 
{
	[COM1] = RCC_AHB1Periph_GPIOA,
	[COM2] = RCC_AHB1Periph_GPIOA,
	[COM3] = RCC_AHB1Periph_GPIOB,
	[COM4] = RCC_AHB1Periph_GPIOC,
	[COM5] = RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD,
	[COM6] = RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOG,
	[COM7] = 0,
	[COM8] = 0
};


/**
 *	@brief	get the AHB periph associated with the USART port
 * 	@param[in]	com	COM port to get the IRQ
 * 	@return	IRQ value 
 */
static uint32_t GetRCC_AHBPeriph( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return 0;
	}
	return RCC_AHBPeriph[com];
}

/**
 *	@brief COM_TX_PORT:	GPIO port for each hardware communication port
 */  
static GPIO_TypeDef * COM_TX_PORT[COMPORTS] = 
{
	[COM1] = GPIOA,
	[COM2] = GPIOA,
	[COM3] = GPIOB,
	[COM4] = GPIOC,
	[COM5] = GPIOC,
	[COM6] = GPIOC,
	[COM7] = NULL,
	[COM8] = NULL
};

/** 
 *	@brief	get a pointer to a GPIO_TypeDef structure used in the configuration of TX port
 * 	@param[in]	com	COM port to get a pointer GPIO_TypeDef
 * 	@return	pointer to a GPIO_TypeDef structure
 */
static GPIO_TypeDef* GetCOM_TX_PORT( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return NULL;
	}
	return COM_TX_PORT[com];
}

/**
 *	@brief COM_RX_PORT:	GPIO port for each hardware communication port
 */
static GPIO_TypeDef* COM_RX_PORT[COMPORTS] = 
{
	[COM1] = GPIOA,
	[COM2] = GPIOA,
	[COM3] = GPIOB,
	[COM4] = GPIOC,
	[COM5] = GPIOD,
	[COM6] = GPIOC,
	[COM7] = NULL,
	[COM8] = NULL
};

/** 
 *	@brief	get a pointer to a GPIO_TypeDef structure used in the configuration of RX port
 * 	@param[in]	com	COM port to get a pointer GPIO_TypeDef
 * 	@return	pointer to a GPIO_TypeDef structure
 */
static GPIO_TypeDef* GetCOM_RX_PORT( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return NULL;
	}
	return COM_RX_PORT[com];
}

/**
 *	@brief COM_RTS_PORT:	GPIO port for each hardware communication port
 */
static GPIO_TypeDef * COM_RTS_PORT[COMPORTS] = 
{
	[COM1] = GPIOA,
	[COM2] = GPIOA,
	[COM3] = GPIOB,
	[COM4] = NULL,
	[COM5] = NULL,
	[COM6] = GPIOG,
	[COM7] = NULL,
	[COM8] = NULL
};

/** 
 *	@brief	get a pointer to a GPIO_TypeDef structure used in the configuration of RTS port
 * 	@param[in]	com	COM port to get a pointer GPIO_TypeDef
 * 	@return	pointer to a GPIO_TypeDef structure
 */
static GPIO_TypeDef* GetCOM_RTS_PORT( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return NULL;
	}
	return COM_RTS_PORT[com];
}

/**
 *	@brief COM_CTS_PORT:	GPIO port for each hardware communication port
 */
static GPIO_TypeDef * COM_CTS_PORT[COMPORTS] = 
{
	[COM1] = GPIOA,
	[COM2] = GPIOA,
	[COM3] = GPIOB,
	[COM4] = NULL,
	[COM5] = NULL,
	[COM6] = GPIOG,
	[COM7] = NULL,
	[COM8] = NULL
};

/** 
 *	@brief	get a pointer to a GPIO_TypeDef structure used in the configuration of CTS port
 * 	@param[in]	com	COM port to get a pointer GPIO_TypeDef
 * 	@return	pointer to a GPIO_TypeDef structure
 */
static GPIO_TypeDef* GetCOM_CTS_PORT( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return NULL;
	}
	return COM_CTS_PORT[com];
}

/**
 *	@brief COM_CLK_PORT:	GPIO port for each hardware communication port
 */
static GPIO_TypeDef * COM_CLK_PORT[COMPORTS] = 
{
	[COM1] = GPIOA,
	[COM2] = GPIOA,
	[COM3] = GPIOB,
	[COM4] = NULL,
	[COM5] = NULL,
	[COM6] = GPIOG,
	[COM7] = NULL,
	[COM8] = NULL
};

/** 
 *	@brief	get a pointer to a GPIO_TypeDef structure used in the configuration of CLK port
 * 	@param[in]	com	COM port to get a pointer GPIO_TypeDef
 * 	@return	pointer to a GPIO_TypeDef structure
 */
static GPIO_TypeDef* GetCOM_CLK_PORT( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return NULL;
	}
	return COM_CLK_PORT[com];
}

/**
 *	@brief COM_USART_TX_PIN:	GPIO port for each hardware communication port
 */
static uint16_t COM_USART_TX_PIN [COMPORTS] = 
{
	[COM1] = GPIO_Pin_9,
	[COM2] = GPIO_Pin_2,
	[COM3] = GPIO_Pin_10,
	[COM4] = GPIO_Pin_10,
	[COM5] = GPIO_Pin_12,
	[COM6] = GPIO_Pin_6,
	[COM7] = (uint16_t)-1,
	[COM8] = (uint16_t)-1	
};

/** 
 *	@brief	get GPIO_Pin value used in the configuration of TX port
 * 	@param[in]	com	COM port to get a GPIO_Pin
 * 	@return	GPIO_Pin value or (-1)
 */
static uint16_t GetCOM_USART_TX_PIN( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint16_t)-1;
	}
	return COM_USART_TX_PIN[com];
}

/**
 *	@brief COM_CLK_PORT:	GPIO port for each hardware communication port
 */
static uint16_t COM_USART_RX_PIN [COMPORTS] = 
{
	[COM1] = GPIO_Pin_10,
	[COM2] = GPIO_Pin_3,
	[COM3] = GPIO_Pin_11,
	[COM4] = GPIO_Pin_11,
	[COM5] = GPIO_Pin_2,
	[COM6] = GPIO_Pin_9,
	[COM7] = (uint16_t)-1,
	[COM8] = (uint16_t)-1	
};

/** 
 *	@brief	get GPIO_Pin value used in the configuration of RX port
 * 	@param[in]	com	COM port to get a GPIO_Pin
 * 	@return	GPIO_Pin value or (-1)
 */
static uint16_t GetCOM_USART_RX_PIN( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint16_t)-1;
	}
	return COM_USART_RX_PIN[com];
}

/**
 *	@brief COM_USART_CTS_PIN:	GPIO port for each hardware communication port
 */
static uint16_t COM_USART_CTS_PIN [COMPORTS] = 
{
	[COM1] = GPIO_Pin_11,
	[COM2] = GPIO_Pin_0,
	[COM3] = GPIO_Pin_13,
	[COM4] = (uint16_t)-1,
	[COM5] = (uint16_t)-1,
	[COM6] = GPIO_Pin_15,
	[COM7] = (uint16_t)-1,
	[COM8] = (uint16_t)-1
};

/** 
 *	@brief	get GPIO_Pin value used in the configuration of CTS port
 * 	@param[in]	com	COM port to get a GPIO_Pin
 * 	@return	GPIO_Pin value or (-1)
 */
static uint16_t GetCOM_USART_CTS_PIN( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint16_t)-1;
	}
	return COM_USART_CTS_PIN[com];
}

/**
 *	@brief COM_USART_RTS_PIN:	GPIO port for each hardware communication port
 */
static uint16_t COM_USART_RTS_PIN [COMPORTS] = 
{
	[COM1] = GPIO_Pin_12,
	[COM2] = GPIO_Pin_1,
	[COM3] = GPIO_Pin_14,
	[COM4] = (uint16_t)-1,
	[COM5] = (uint16_t)-1,
	[COM6] = GPIO_Pin_8,
	[COM7] = (uint16_t)-1,
	[COM8] = (uint16_t)-1
};

/** 
 *	@brief	get GPIO_Pin value used in the configuration of RTS port
 * 	@param[in]	com	COM port to get a GPIO_Pin
 * 	@return	GPIO_Pin value or (-1)
 */
static uint16_t GetCOM_USART_RTS_PIN( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint16_t)-1;
	}
	return COM_USART_RTS_PIN[com];
}

/**
 *	@brief COM_USART_RTS_PIN:	GPIO port for each hardware communication port
 */
static uint16_t COM_USART_CLK_PIN [COMPORTS] = 
{
	[COM1] = GPIO_Pin_8,
	[COM2] = GPIO_Pin_4,
	[COM3] = GPIO_Pin_12,
	[COM4] = (uint16_t)-1,
	[COM5] = (uint16_t)-1,
	[COM6] = GPIO_Pin_7,
	[COM7] = (uint16_t)-1,
	[COM8] = (uint16_t)-1
};

/** 
 *	@brief	get GPIO_Pin value used in the configuration of CLK port
 * 	@param[in]	com	COM port to get a GPIO_Pin
 * 	@return	GPIO_Pin value or (-1)
 */
static uint16_t GetCOM_USART_CLK_PIN( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint16_t)-1;
	}
	return COM_USART_CLK_PIN[com];
}

/**
 *	@brief COM_USART_TX_SOURCE:	GPIO port for each hardware communication port
 */
static uint8_t COM_USART_TX_SOURCE [COMPORTS] = 
{
	[COM1] = GPIO_PinSource9,
	[COM2] = GPIO_PinSource2,
	[COM3] = GPIO_PinSource10,
	[COM4] = GPIO_PinSource10,
	[COM5] = GPIO_PinSource12,
	[COM6] = GPIO_PinSource6,
	[COM7] = (uint8_t)-1,
	[COM8] = (uint8_t)-1
};

/** 
 *	@brief	get GPIO_PinSource value used in the configuration of TX port
 * 	@param[in]	com	COM port to get a GPIO_PinSource value
 * 	@return	GPIO_PinSource value or (-1)
 */
static uint8_t GetCOM_USART_TX_SOURCE( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint8_t)-1;
	}
	return COM_USART_TX_SOURCE[com];
}

/**
 *	@brief COM_USART_RX_SOURCE:	GPIO port for each hardware communication port
 */
static uint8_t COM_USART_RX_SOURCE [COMPORTS] = 
{
	[COM1] = GPIO_PinSource10,
	[COM2] = GPIO_PinSource3,
	[COM3] = GPIO_PinSource11,
	[COM4] = GPIO_PinSource11,
	[COM5] = GPIO_PinSource2,
	[COM6] = GPIO_PinSource9,
	[COM7] = (uint8_t)-1,
	[COM8] = (uint8_t)-1
};

/** 
 *	@brief	get GPIO_PinSource value used in the configuration of RX port
 * 	@param[in]	com	COM port to get a GPIO_PinSource value
 * 	@return	GPIO_PinSource value or (-1)
 */
static uint8_t GetCOM_USART_RX_SOURCE( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint8_t)-1;
	}
	return COM_USART_RX_SOURCE[com];
}

/**
 *	@brief COM_USART_CTS_PIN:	GPIO port for each hardware communication port
 */
static uint8_t COM_USART_CTS_SOURCE [COMPORTS] = 
{
	[COM1] = GPIO_PinSource11,
	[COM2] = GPIO_PinSource0,
	[COM3] = GPIO_PinSource13,
	[COM4] = (uint8_t)-1,
	[COM5] = (uint8_t)-1,
	[COM6] = GPIO_PinSource15,
	[COM7] = (uint8_t)-1,
	[COM8] = (uint8_t)-1
};

/** 
 *	@brief	get GPIO_PinSource value used in the configuration of CTS port
 * 	@param[in]	com	COM port to get a GPIO_PinSource value
 * 	@return	GPIO_PinSource value or (-1)
 */
static uint8_t GetCOM_USART_CTS_SOURCE( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint8_t)-1;
	}
	return COM_USART_CTS_SOURCE[com];
}

/**
 *	@brief COM_USART_RTS_SOURCE:	GPIO port for each hardware communication port
 */
static uint8_t COM_USART_RTS_SOURCE [COMPORTS] = 
{
	[COM1] = GPIO_PinSource12,
	[COM2] = GPIO_PinSource1,
	[COM3] = GPIO_PinSource14,
	[COM4] = (uint8_t)-1,
	[COM5] = (uint8_t)-1,
	[COM6] = GPIO_PinSource8,
	[COM7] = (uint8_t)-1,
	[COM8] = (uint8_t)-1
};

/** 
 *	@brief	get GPIO_PinSource value used in the configuration of RTS port
 * 	@param[in]	com	COM port to get a GPIO_PinSource value
 * 	@return	GPIO_PinSource value or (-1)
 */
static uint8_t GetCOM_USART_RTS_SOURCE( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint8_t)-1;
	}
	return COM_USART_RTS_SOURCE[com];
}

/**
 *	@brief COM_USART_CLK_SOURCE:	GPIO port for each hardware communication port
 */
static uint8_t COM_USART_CLK_SOURCE [COMPORTS] = 
{
	[COM1] = GPIO_PinSource8,
	[COM2] = GPIO_PinSource4,
	[COM3] = GPIO_PinSource12,
	[COM4] = (uint8_t)-1,
	[COM5] = (uint8_t)-1,
	[COM6] = GPIO_PinSource7,
	[COM7] = (uint8_t)-1,
	[COM8] = (uint8_t)-1
};

/** 
 *	@brief	get GPIO_PinSource value used in the configuration of CLK port
 * 	@param[in]	com	COM port to get a GPIO_PinSource value
 * 	@return	GPIO_PinSource value or (-1)
 */
static uint8_t GetCOM_USART_CLK_SOURCE( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint8_t)-1;
	}
	return COM_USART_CLK_SOURCE[com];
}

/**
 *	@brief COM_USART_TX_AF:	GPIO port for each hardware communication port
 */
static uint8_t COM_USART_TX_AF [COMPORTS] = 
{
	[COM1] = GPIO_AF_USART1,
	[COM2] = GPIO_AF_USART2,
	[COM3] = GPIO_AF_USART3,
	[COM4] = GPIO_AF_UART4,
	[COM5] = GPIO_AF_UART5,
	[COM6] = GPIO_AF_USART6,
	[COM7] = (uint8_t)-1,
	[COM8] = (uint8_t)-1
};

/** 
 *	@brief	get GPIO_AF value used in the configuration of TX port
 * 	@param[in]	com	COM port to get a GPIO_AF value
 * 	@return	GPIO_AF value or (-1)
 */
static uint8_t GetCOM_USART_TX_AF( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint8_t)-1;
	}
	return COM_USART_TX_AF[com];
}

/**
 *	@brief COM_USART_RX_AF:	GPIO port for each hardware communication port
 */
static uint8_t COM_USART_RX_AF [COMPORTS] = 
{
	[COM1] = GPIO_AF_USART1,
	[COM2] = GPIO_AF_USART2,
	[COM3] = GPIO_AF_USART3,
	[COM4] = GPIO_AF_UART4,
	[COM5] = GPIO_AF_UART5,
	[COM6] = GPIO_AF_USART6,
	[COM7] = (uint8_t)-1,
	[COM8] = (uint8_t)-1
};

/** 
 *	@brief	get GPIO_AF value used in the configuration of RX port
 * 	@param[in]	com	COM port to get a GPIO_AF value
 * 	@return	GPIO_AF value or (-1)
 */static uint8_t GetCOM_USART_RX_AF( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint8_t)-1;
	}
	return COM_USART_RX_AF[com];
}

/**
 *	@brief COM_USART_CTS_AF:	GPIO port for each hardware communication port
 */
static uint8_t COM_USART_CTS_AF [COMPORTS] = 
{
	[COM1] = GPIO_AF_USART1,
	[COM2] = GPIO_AF_USART2,
	[COM3] = GPIO_AF_USART3,
	[COM4] = (uint8_t)-1,
	[COM5] = (uint8_t)-1,
	[COM6] = GPIO_AF_USART6,
	[COM7] = (uint8_t)-1,
	[COM8] = (uint8_t)-1
};

/** 
 *	@brief	get GPIO_AF value used in the configuration of CTS port
 * 	@param[in]	com	COM port to get a GPIO_AF value
 * 	@return	GPIO_AF value or (-1)
 */
static uint8_t GetCOM_USART_CTS_AF( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint8_t)-1;
	}
	return COM_USART_CTS_AF[com];
}

/**
 *	@brief COM_USART_RTS_AF:	GPIO port for each hardware communication port
 */
static uint8_t COM_USART_RTS_AF [COMPORTS] = 
{
	[COM1] = GPIO_AF_USART1,
	[COM2] = GPIO_AF_USART2,
	[COM3] = GPIO_AF_USART3,
	[COM4] = (uint8_t)-1,
	[COM5] = (uint8_t)-1,
	[COM6] = GPIO_AF_USART6,
	[COM7] = (uint8_t)-1,
	[COM8] = (uint8_t)-1
};

/** 
 *	@brief	get GPIO_AF value used in the configuration of RTS port
 * 	@param[in]	com	COM port to get a GPIO_AF value
 * 	@return	GPIO_AF value or (-1)
 */
static uint8_t GetCOM_USART_RTS_AF( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint8_t)-1;
	}
	return COM_USART_RTS_AF[com];
}

/**
 *	@brief COM_USART_CK_AF:	GPIO port for each hardware communication port
 */
static uint8_t COM_USART_CLK_AF [COMPORTS] = 
{
	[COM1] = GPIO_AF_USART1,
	[COM2] = GPIO_AF_USART2,
	[COM3] = GPIO_AF_USART3,
	[COM4] = (uint8_t)-1,
	[COM5] = (uint8_t)-1,
	[COM6] = GPIO_AF_USART6,
	[COM7] = (uint8_t)-1,
	[COM8] = (uint8_t)-1
};

/** 
 *	@brief	get GPIO_AF value used in the configuration of CLK port
 * 	@param[in]	com	COM port to get a GPIO_AF value
 * 	@return	GPIO_AF value or (-1)
 */
static uint8_t GetCOM_USART_CLK_AF( COMPORT com )
{
	if ( (uint32_t)com > (uint32_t)COM8 )
	{
		return (uint8_t)-1;
	}
	return COM_USART_CLK_AF[com];
}

static uint32_t BaudRateTable[ BR_115200 +1 ] = 
{
	[BR_300] 		= 300L, 
	[BR_600] 		= 600L,
	[BR_1200]		= 1200L,
	[BR_2400] 		= 2400L,
	[BR_4800]		= 4800L,
	[BR_9600]		= 9600L,
	[BR_19200] 		= 19200L,
	[BR_38400] 		= 38400L,
	[BR_57600]		= 57600L,
	[BR_115200]		= 115200L
};
/**
 *	@brief calcule the baudrate value
 */
static uint32_t CalcBaudRate( BaudRate br )
{
	if ( br > BR_115200 )
		return BaudRateTable[BR_9600];
	
	return BaudRateTable[br];
}

static uint16_t STOP_BIT_CONV[3] = 
{
	[ST_ONE] 		=	USART_StopBits_1,
	[ST_TWO]		=	USART_StopBits_2,
	[ST_ONE_HALF]	=	USART_StopBits_1_5
};

/**
 *	@brief	convert  from StopBits to USART_StopBits values
 */
static uint16_t GetStopBit( StopBits st )
{
	if ( st > ST_ONE_HALF )
	{
		return STOP_BIT_CONV[0];
	}
	
	return STOP_BIT_CONV[st];
}

static uint16_t FLOW_CONTROL[4] = 
{
	[FC_None] 		= 	USART_HardwareFlowControl_None	, 
	[FC_RTS] 		=	USART_HardwareFlowControl_RTS		, 
	[FC_CTS] 		= 	USART_HardwareFlowControl_CTS		, 
	[FC_RTS_CTS]	=	USART_HardwareFlowControl_RTS_CTS
};

/**
 *	@brief	convert  from FlowControl to USART_HardwareFlowControl values
 */
static uint16_t GetFlowControl( FlowControl f )
{
	if ( f > FC_RTS_CTS )
	{
		return FLOW_CONTROL[0];
	}
	
	return FLOW_CONTROL[f];
}

static uint16_t PARITY[3] = 
{
	[PR_NONE]		= USART_Parity_No	, 
	[PR_EVEN] 	=	USART_Parity_Even, 
	[PR_ODD] 		= USART_Parity_Odd		
};

/**
 *	@brief	convert  from Parity to USART_Parity values
 */
static uint16_t GetParity( Parity p )
{
	if ( p > PR_ODD )
	{
		return PARITY[0];
	}
	return PARITY[p];
}

static uint16_t EVENTS_TABLE[NUSARTEVENTS] = 
{
	[F_CB_OVR] = (USART_IT_ORE_ER)	,	//! overrun
	[F_CB_RXD] = (USART_IT_RXNE)		,	//!	data received
	[F_CB_TXD] = (USART_IT_TC)				//!	data transmited
};

/**
 *	@brief	convert  from Parity to USART_Parity values
 */
static uint16_t GetEventID( UsartEvents ev )
{
	if ( ev > F_CB_TXD )
	{
		return (uint16_t)-1;
	}
	
	return EVENTS_TABLE[ev];
}

//------------------------------------------------------------------

TUsart*	GetUsart( COMPORT com )
{
	if ( com > COM8 )
	{
		return NULL;
	}
	
	return &(usart[com]);
}

//------------------------------------------------------------------

uint32_t USARTInit(  TUsart * usart )
{
	GPIO_InitTypeDef GPIO_InitStruct;			// this is for the GPIO pins used as TX and RX
	USART_InitTypeDef USART_InitStruct; 	// this is for the USART1 initilization
	NVIC_InitTypeDef NVIC_InitStructure; 	// this is used to configure the NVIC (nested vector interrupt controller)

	if ( usart == NULL )
	{
		return EINVAL;
	}
		
	if( usart->bInit == 1 )
	{
		return 0;
	}
	
	if( usart->comPort > COM8 )
	{
		return EINVAL;
	}
   
	switch( usart->comPort )
	{
		case COM1:
		case COM6:
			RCC_APB2PeriphClockCmd( GetRCC_APBPeriph( usart->comPort ), ENABLE );
			break;
		case COM2:
		case COM3:
		case COM4:
		case COM5:
			RCC_APB1PeriphClockCmd( GetRCC_APBPeriph( usart->comPort ), ENABLE );
			break;
		default:
			break;
	}
	
	RCC_AHB1PeriphClockCmd(GetRCC_AHBPeriph( usart->comPort ), ENABLE);
	
	// TX pin configuration
	GPIO_StructInit( &GPIO_InitStruct );		
	GPIO_InitStruct.GPIO_Pin 		= GetCOM_USART_TX_PIN( usart->comPort );
	GPIO_InitStruct.GPIO_Mode 	=	GPIO_Mode_AF; 
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP;  
	GPIO_Init(	GetCOM_TX_PORT( usart->comPort ) , &GPIO_InitStruct); 
	GPIO_PinAFConfig(GetCOM_TX_PORT( usart->comPort ), GetCOM_USART_TX_SOURCE( usart->comPort ), GetCOM_USART_TX_AF(usart->comPort) );

	// RX pin configuration
	GPIO_StructInit( &GPIO_InitStruct );		
	GPIO_InitStruct.GPIO_Pin 		= GetCOM_USART_RX_PIN( usart->comPort );
	GPIO_InitStruct.GPIO_Mode 	=	GPIO_Mode_AF; 
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP;  
	GPIO_Init(	GetCOM_RX_PORT( usart->comPort ) , &GPIO_InitStruct); 
	GPIO_PinAFConfig(GetCOM_RX_PORT( usart->comPort ), GetCOM_USART_RX_SOURCE( usart->comPort ), GetCOM_USART_RX_AF(usart->comPort) );
	

	switch( usart->comPort )
	{
		case COM1:	// CTS RTS
		case COM6:
		{
			if ( usart->uFlowControl == FC_None )
				break;
			
			if ( usart->uFlowControl  == FC_RTS || usart->uFlowControl == FC_RTS_CTS )
			{
				// RTS pin configuration
				GPIO_StructInit( &GPIO_InitStruct );		
				GPIO_InitStruct.GPIO_Pin 		= GetCOM_USART_RTS_PIN( usart->comPort );
				GPIO_InitStruct.GPIO_Mode 	=	GPIO_Mode_AF; 
				GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
				GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP;
				GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP;  
				GPIO_Init(	GetCOM_RTS_PORT( usart->comPort ) , &GPIO_InitStruct); 
				GPIO_PinAFConfig(GetCOM_RTS_PORT( usart->comPort ), GetCOM_USART_RTS_SOURCE( usart->comPort ), GetCOM_USART_RTS_AF(usart->comPort) );
			}

			if ( usart->uFlowControl  == FC_CTS || usart->uFlowControl == FC_RTS_CTS )
			{
				// CTS pin configuration
				GPIO_StructInit( &GPIO_InitStruct );		
				GPIO_InitStruct.GPIO_Pin 		= GetCOM_USART_CTS_PIN( usart->comPort );
				GPIO_InitStruct.GPIO_Mode 	=	GPIO_Mode_AF; 
				GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
				GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP;
				GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP;  
				GPIO_Init(	GetCOM_CTS_PORT( usart->comPort ) , &GPIO_InitStruct); 
				GPIO_PinAFConfig(GetCOM_CTS_PORT( usart->comPort ), GetCOM_USART_CTS_SOURCE( usart->comPort ), GetCOM_USART_CTS_AF(usart->comPort) );
			}
		}
		break;
		
		default:
			break;
	}
	
	// initialize the usart structure
	USART_StructInit( &USART_InitStruct );
	USART_InitStruct.USART_BaudRate 	= CalcBaudRate( usart->baud );
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits 	= GetStopBit( usart->stopBits );
	USART_InitStruct.USART_Parity		 	= GetParity( usart->parity );
	USART_InitStruct.USART_HardwareFlowControl =  GetFlowControl( usart->uFlowControl );
	USART_InitStruct.USART_Mode 			= USART_Mode_Tx | USART_Mode_Rx;
	USART_Init( GetUsartTD( usart->comPort ), &USART_InitStruct);  
	
	if ( USARTRegisterCallBack(  usart, USART_TX_WorkFunction, F_CB_TXD ) )
	{
		return EFAULT;
	}
	
	USART_Cmd( GetUsartTD( usart->comPort ), ENABLE );
	
	NVIC_InitStructure.NVIC_IRQChannel 		= GetCOM_USART_IRQ( usart->comPort );	// we want to configure the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;										// this sets the priority group of the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 					// this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         		// the USART1 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);                                                         // the properties are passed to the NVIC_Init function which takes care of the low level stuff        

	USART_Cmd( GetUsartTD( usart->comPort ), ENABLE );
	usart->bInit = 1;
	
	return 0;
}

//------------------------------------------------------------------

uint32_t USARTSetBaudRate( TUsart * usart, BaudRate baud )
{
	USART_InitTypeDef USART_InitStruct; 	// this is for the USART1 initilization
	if ( usart == NULL )
	{
		return EINVAL;
	}
	
	if( baud > BR_115200 )
	{
		return EINVAL;
	}
	
	usart->baud = baud;
	
	if ( usart->bInit == ENABLE )
	{
		// initialize the usart structure
		USART_StructInit( &USART_InitStruct );
		USART_InitStruct.USART_BaudRate 	= CalcBaudRate(usart->baud );
		USART_Init( GetUsartTD( usart->comPort ), &USART_InitStruct);  
	}
	
	return 0;
}

//------------------------------------------------------------------

BaudRate USARTGetBaudRate( TUsart * usart )
{
	if ( usart == NULL )
		return (BaudRate)-1;
		
	return usart->baud;
}

//------------------------------------------------------------------

uint32_t USARTSetParity( TUsart * usart, Parity pr )
{
	if ( usart == NULL )
	{
		return EINVAL;
	}
	
	if( pr > PR_ODD )
	{
		return EINVAL;
	}
	
	usart->parity = pr;

	// add the peripheral parity change code here: 
	if ( usart->bInit == ENABLE )
	{
		// initialize the usart structure
		USART_InitTypeDef USART_InitStruct; 	// this is for the USART1 initilization
		USART_StructInit( &USART_InitStruct );
		USART_InitStruct.USART_Parity	= GetParity( usart->parity );
		USART_Init( GetUsartTD( usart->comPort ), &USART_InitStruct);  	
	}
	
	return 0;
}

//------------------------------------------------------------------

Parity USARTGetParity( TUsart * usart )
{
	if ( usart != NULL )
		return usart->parity;
		
	return (Parity)-1;
}

//------------------------------------------------------------------

uint32_t USARTSetStopBits( TUsart * usart, StopBits st )
{
	if ( usart == NULL )
	{
		return EINVAL;
	}
	
	if( st > ST_TWO )
	{
		return EINVAL;
	}
	
	usart->stopBits = st;
    
	// add the peripheral stop bits change code here: 
	if ( usart->bInit == ENABLE )
	{
		// initialize the usart structure
		USART_InitTypeDef USART_InitStruct; 	// this is for the USART1 initilization
		USART_StructInit( &USART_InitStruct );
		USART_InitStruct.USART_StopBits	= GetStopBit( usart->stopBits );
		USART_Init( GetUsartTD( usart->comPort ), &USART_InitStruct);  	
	}
	
	return 0;
}

//------------------------------------------------------------------

StopBits USARTGetStopBits( TUsart * usart )
{
	if ( usart != NULL )
		return usart->stopBits;
	return (StopBits)-1;
}

//------------------------------------------------------------------

uint32_t USARTSetBuffers( TUsart * usart, const char* rcv, const uint32_t rcvLen, const char * send, const uint32_t sendLen )
{
	
	USART_TypeDef * utd;
	
	if ( usart == NULL )
	{
		return EINVAL;
	}
	
	if ( rcv == NULL || send == NULL )
	{
		return EINVAL;		
	}
	
	usart->pSendBuffer			= (char*)send;
	usart->pRcvBuffer				= (char*)rcv;
	usart->dwSendBufferLen	= sendLen;
	usart->dwRcvBufferLen	= rcvLen;
	usart->pSendBufferHead 	= usart->pSendBuffer;
	usart->pRcvBufferHead 	= usart->pRcvBuffer;
	usart->pSendBufferTail 	= usart->pSendBuffer + usart->dwSendBufferLen;
	usart->pRcvBufferTail 	= usart->pRcvBuffer + usart->dwRcvBufferLen;
    
  // add the peripheral interrupt function association here
  // enable the TXD interruption
	//F_CB_TXD
	utd = GetUsartTD( usart->comPort );
	if ( utd == NULL )
	{
		return EINVAL;
	}
	
	USART_ITConfig( utd, GetEventID( F_CB_TXD ), ENABLE );
	USART_ITConfig( utd, GetEventID( F_CB_RXD ), ENABLE );
	
	return 0;
}

//------------------------------------------------------------------

uint32_t USARTRegisterCallBack( TUsart* usart, callbackusart_func callback_func, UsartEvents flag )
{
	if ( usart == NULL )
	{
		return EINVAL;
	}
	
	if ( callback_func == NULL )
	{
		return EINVAL;
	}
	
	if ( flag > F_CB_TXD )
	{
		return EINVAL;
	}

	usart->callbackfunc[flag]	 = callback_func;
	usart->dwFlags				|= (1 << flag);
	
	// add the peripheral interrupt function association here
	if ( usart->bInit == ENABLE )
	{
		for ( int i = F_CB_OVR; i <= F_CB_TXD; i++ )
		{
			if ( flag & (1 << i))
			{
				USART_ITConfig(GetUsartTD( usart->comPort ), GetEventID( (UsartEvents)i ), ENABLE); // enable the USART receive interrupt
			}
		}
	}
	else
	{
		return EINVAL;
	}
	
	return 0;
}

//------------------------------------------------------------------

uint32_t USARTUnregisterCallBack( TUsart* usart, UsartEvents flag )
{
	if ( usart == NULL )
	{
		return EINVAL;
	}
	
	if ( flag > F_CB_TXD )
	{
		return EINVAL;
	}
	
	usart->dwFlags		&= ~(1 << flag);
	usart->callbackfunc[flag]	= NULL;


	if ( usart->bInit == ENABLE )
	{
		for ( int i = F_CB_OVR; i <= F_CB_TXD; i++ )
		{
			if ( flag & (1 << i))
			{
				USART_ITConfig(GetUsartTD( usart->comPort ), GetEventID( (UsartEvents)i ), DISABLE); // disable the USART receive interrupt
			}
		}
	}
	else 
	{
		return EINVAL;
	}

	return 0;
}

//------------------------------------------------------------------

uint32_t USARTSend(	TUsart * usart, const char * buffer, const uint16_t buflen )
{
	USART_TypeDef*	utd 	= 	NULL;
	uint16_t		data 	=	(uint16_t)-1;
	uint16_t		iBufferInUse = 0;
		
	// verify if buffer is NULL 
	if ( buffer == NULL )
	{
		return EFAULT;
	}
	
	// vefiry if length of buffer have a valid value
	if ( buflen == 0 )
	{
		return EFAULT;		
	}
	
	//	verify if the usart structure is not NULL  
    if ( usart == NULL )
  {
		return EFAULT;
	}
	
	//! if the serial ineterface isn't initialized, don't continue the operation 
	if ( usart->bInit != ENABLE )
	{
		return EFAULT;		
	}	

	// have the buffers initialized?
	if ( usart->pSendBuffer == NULL )
	{
		return EFAULT;
	}
		
	// determine the quantity of bytes in use
	iBufferInUse = usart->dwSendBufferLen - (uint16_t)( usart->pSendBufferTail - usart->pSendBufferHead ); 
	if (buflen <= iBufferInUse )
	{
		// copy the entire buffer to usart transmiter buffer 
		memcpy( (char*)usart->pSendBufferTail, (char*)buffer, buflen );
		usart->pSendBufferTail = usart->pSendBuffer + buflen;
	}
	else
	{
		uint32_t dwBytesToHead = 0;
		
		// if the buffer + total space in use 
		
		//! this algorithm have a bigger chance of 
		memcpy( (char*)usart->pSendBufferTail, (char*)buffer, iBufferInUse );
		usart->pSendBufferTail = usart->pSendBuffer + iBufferInUse;
		
		dwBytesToHead =  (uint32_t)(usart->pSendBufferHead - usart->pSendBuffer); 
		memcpy( (char*)usart->pSendBuffer, (char*)&buffer[iBufferInUse], dwBytesToHead );
		
		usart->pSendBufferHead += dwBytesToHead;
	}
	
	utd = GetUsartTD( usart->comPort );
	if ( utd == NULL )
	{
		return EFAULT;
	}
	
	if ( iBufferInUse == usart->dwSendBufferLen )
	{
		data = (uint16_t)*usart->pSendBufferHead;
		USART_SendData( utd, data );
		usart->pSendBufferHead++;
	}
	
	return 0;
}

//------------------------------------------------------------------

//! default transmiter function
//! called in every transmiter event, by the interrupt function
//! TODO: analyse the problem of interruption management because this function isn't blocking

uint32_t USART_TX_WorkFunction( void* lParam )
{
	TUsart* usart ;
	USART_TypeDef*	utd 	= 	NULL;
	uint16_t		data 	=	(uint16_t)-1;
	
	usart = (TUsart*)lParam;
	
	if ( usart == NULL )
	{
		return EFAULT;
	}
	
	utd = GetUsartTD( usart->comPort );
	if ( utd == NULL )
	{
		return EFAULT;
	}	
	
	data = (uint16_t)*usart->pSendBufferHead;
	usart->pSendBufferHead++;
	USART_SendData( utd, data );
	
	return 0;
}

//------------------------------------------------------------------

// ISR functions:
// put the interrupt code for each U(S)ART peripheral
void USART1_IRQHandler(void)
{
	// put verification to flag errors, input/output events and whatever fucking code
	
	USART_TypeDef * usartTD;
	TUsart  * usart;
	usartTD = GetUsartTD( COM1 );
	usart		= GetUsart( COM1 );
	
	if ( usartTD != NULL && usart != NULL )
	{
		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_RXD ) ) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_RXD] != NULL )
			{
				uint16_t data;
				
				data = USART_ReceiveData(usartTD);
				(usart->callbackfunc[F_CB_RXD])( (char*)&data );
			}
		}
		
		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_TXD )) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_TXD] != NULL )
			{
				// do something for transmite
				// there are a default function 'USART_TX_WorkFunction' for that
				(usart->callbackfunc[F_CB_TXD])( (TUsart*)usart );
			}
		}
		
 		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_OVR )) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_OVR] != NULL )
			{
				// put ssome logging functions
			}
		}   
		
		// put ISR verifications for anothers event handlers
		// 	USART_IT_CTS
		//	USART_IT_LBD
		//	USART_IT_TXE
		//	USART_IT_RXNE
		//	USART_IT_IDLE
		//	USART_IT_ORE_RX
		//	USART_IT_NE
		//	USART_IT_FE
		//	USART_IT_PE
	}
}

void USART2_IRQHandler(void)
{
		// put verification to flag errors, input/output events and whatever fucking code
	
	USART_TypeDef * usartTD;
	TUsart  * usart;
	usartTD = GetUsartTD( COM2 );
	usart		= GetUsart( COM2 );
	
	//[F_CB_OVR] = (USART_IT_ORE_ER)	,	//! overrun
	//[F_CB_RXD] = (USART_IT_RXNE)		,	//!	data received
	//[F_CB_TXD] = (USART_IT_TC)				//!	data transmited	
	
	if ( usartTD != NULL && usart != NULL )
	{
		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_RXD ) ) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_RXD] != NULL )
			{
				uint16_t data;
				
				data = USART_ReceiveData(usartTD);
				(usart->callbackfunc[F_CB_RXD])( (char*)&data );
			}
		}
		
		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_TXD )) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_TXD] != NULL )
			{
				// do something for transmite
				// there are a default function 'USART_TX_WorkFunction' for that
				(usart->callbackfunc[F_CB_TXD])( (TUsart*)usart );
			}
		}
		
 		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_OVR )) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_OVR] != NULL )
			{
				
			}
		}   
		
		// put ISR verifications for anothers event handlers
		// 	USART_IT_CTS
		//	USART_IT_LBD
		//	USART_IT_TXE
		//	USART_IT_RXNE
		//	USART_IT_IDLE
		//	USART_IT_ORE_RX
		//	USART_IT_NE
		//	USART_IT_FE
		//	USART_IT_PE 
	}
}

void USART3_IRQHandler(void)
{
		// put verification to flag errors, input/output events and whatever fucking code
	
	USART_TypeDef * usartTD;
	TUsart  * usart;
	usartTD = GetUsartTD( COM3 );
	usart		= GetUsart( COM3 );
	
	//[F_CB_OVR] = (USART_IT_ORE_ER)	,	//! overrun
	//[F_CB_RXD] = (USART_IT_RXNE)		,	//!	data received
	//[F_CB_TXD] = (USART_IT_TC)				//!	data transmited	
	
	if ( usartTD != NULL && usart != NULL )
	{
		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_RXD ) ) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_RXD] != NULL )
			{
				uint16_t data;
				
				data = USART_ReceiveData(usartTD);
				(usart->callbackfunc[F_CB_RXD])( (char*)&data );
			}
		}
		
		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_TXD )) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_TXD] != NULL )
			{
				// do something for transmite
				// there are a default function 'USART_TX_WorkFunction' for that
				(usart->callbackfunc[F_CB_TXD])( (TUsart*)usart );
			}
		}
		
 		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_OVR )) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_OVR] != NULL )
			{
				
			}
		}   
		
		// put ISR verifications for anothers event handlers
		// 	USART_IT_CTS
		//	USART_IT_LBD
		//	USART_IT_TXE
		//	USART_IT_RXNE
		//	USART_IT_IDLE
		//	USART_IT_ORE_RX
		//	USART_IT_NE
		//	USART_IT_FE
		//	USART_IT_PE
		}
}

void UART4_IRQHandler( void )
{
		// put verification to flag errors, input/output events and whatever fucking code
	
	USART_TypeDef * usartTD;
	TUsart  * usart;
	usartTD = GetUsartTD( COM4 );
	usart		= GetUsart( COM4 );
	
	//[F_CB_OVR] = (USART_IT_ORE_ER)	,	//! overrun
	//[F_CB_RXD] = (USART_IT_RXNE)		,	//!	data received
	//[F_CB_TXD] = (USART_IT_TC)				//!	data transmited	
	
	if ( usartTD != NULL && usart != NULL )
	{
		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_RXD ) ) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_RXD] != NULL )
			{
				uint16_t data;
				
				data = USART_ReceiveData(usartTD);
				(usart->callbackfunc[F_CB_RXD])( (char*)&data );
			}
		}
		
		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_TXD )) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_TXD] != NULL )
			{
				// do something for transmite
				// there are a default function 'USART_TX_WorkFunction' for that
				(usart->callbackfunc[F_CB_TXD])( (TUsart*)usart );
			}
		}
		
 		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_OVR )) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_OVR] != NULL )
			{
				
			}
		}   
		
		// put ISR verifications for anothers event handlers
		// 	USART_IT_CTS
		//	USART_IT_LBD
		//	USART_IT_TXE
		//	USART_IT_RXNE
		//	USART_IT_IDLE
		//	USART_IT_ORE_RX
		//	USART_IT_NE
		//	USART_IT_FE
		//	USART_IT_PE
		}
}

void UART5_IRQHandler( void )
{
		// put verification to flag errors, input/output events and whatever fucking code
	
	USART_TypeDef * usartTD;
	TUsart  * usart;
	usartTD = GetUsartTD( COM5 );
	usart		= GetUsart( COM5 );
	
	//[F_CB_OVR] = (USART_IT_ORE_ER)	,	//! overrun
	//[F_CB_RXD] = (USART_IT_RXNE)		,	//!	data received
	//[F_CB_TXD] = (USART_IT_TC)				//!	data transmited	
	
	if ( usartTD != NULL && usart != NULL )
	{
		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_RXD ) ) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_RXD] != NULL )
			{
				uint16_t data;
				
				data = USART_ReceiveData(usartTD);
				(usart->callbackfunc[F_CB_RXD])( (char*)&data );
			}
		}
		
		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_TXD )) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_TXD] != NULL )
			{
				// do something for transmite
				// there are a default function 'USART_TX_WorkFunction' for that
				(usart->callbackfunc[F_CB_TXD])( (TUsart*)usart );
			}
		}
		
 		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_OVR )) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_OVR] != NULL )
			{
				
			}
		}   
		
		// put ISR verifications for anothers event handlers
		// 	USART_IT_CTS
		//	USART_IT_LBD
		//	USART_IT_TXE
		//	USART_IT_RXNE
		//	USART_IT_IDLE
		//	USART_IT_ORE_RX
		//	USART_IT_NE
		//	USART_IT_FE
		//	USART_IT_PE
	}
}

void USART6_IRQHandler( void )
{
		// put verification to flag errors, input/output events and whatever fucking code
	
	USART_TypeDef * usartTD;
	TUsart  * usart;
	usartTD = GetUsartTD( COM6 );
	usart		= GetUsart( COM6 );
	
	//[F_CB_OVR] = (USART_IT_ORE_ER)	,	//! overrun
	//[F_CB_RXD] = (USART_IT_RXNE)		,	//!	data received
	//[F_CB_TXD] = (USART_IT_TC)				//!	data transmited	
	
	if ( usartTD != NULL && usart != NULL )
	{
		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_RXD ) ) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_RXD] != NULL )
			{
				uint16_t data;
				
				data = USART_ReceiveData(usartTD);
				(usart->callbackfunc[F_CB_RXD])( (char*)&data );
			}
		}
		
		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_TXD )) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_TXD] != NULL )
			{
				// do something for transmite
				// there are a default function 'USART_TX_WorkFunction' for that
				(usart->callbackfunc[F_CB_TXD])( (TUsart*)usart );
			}
		}
		
 		if ( USART_GetITStatus( usartTD, GetEventID( F_CB_OVR )) != RESET ) 
		{
			if ( usart->callbackfunc[F_CB_OVR] != NULL )
			{
				
			}
		}   
		
		// put ISR verifications for anothers event handlers
		// 	USART_IT_CTS
		//	USART_IT_LBD
		//	USART_IT_TXE
		//	USART_IT_RXNE
		//	USART_IT_IDLE
		//	USART_IT_ORE_RX
		//	USART_IT_NE
		//	USART_IT_FE
		//	USART_IT_PE
	}
}

//------------------------------------------------------------------

