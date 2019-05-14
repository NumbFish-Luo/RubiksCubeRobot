#include "config/config.h"

// ################################### usart ###################################
C_USART usart1 =   // USART1, 115200, Rx: PA10, Tx: PA9
{
    .USART         = USART1,
    .GPIO_AF_USART = GPIO_AF_USART1,
    .USART_CLK     = RCC_APB2Periph_USART1,
    .Baudrate      = 115200,

    .RX_GPIO      = GPIOA,
    .RX_Pin       = GPIO_Pin_10,
    .RX_GPIO_CLK  = RCC_AHB1Periph_GPIOA,
    .RX_PinSource = GPIO_PinSource10,

    .TX_GPIO      = GPIOA,
    .TX_Pin       = GPIO_Pin_9,
    .TX_GPIO_CLK  = RCC_AHB1Periph_GPIOA,
    .TX_PinSource = GPIO_PinSource9,

    .USART_IRQn       = USART1_IRQn,
    .USART_IRQHandler = USART1_IRQHandler,

    .RxPtr = 0
};

// #################################### cylinder ###############################
C_Cylinder cylinder_D = { GPIOA, GPIO_Pin_4, RCC_AHB1Periph_GPIOD };
C_Cylinder cylinder_L = { GPIOD, GPIO_Pin_6, RCC_AHB1Periph_GPIOD };
