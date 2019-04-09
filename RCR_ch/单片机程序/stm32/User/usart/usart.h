#ifndef USART_H
#define USART_H

#include <stdio.h>
#include "stm32f4xx.h"
#include "typeDefine.h"

// 串口接收缓冲数组大小
#define UART_RX_BUFFER_SIZE 1024

typedef struct C_USART
{
    USART_Type    USART;         // USARTx
    AF_Type       GPIO_AF_USART; // GPIO_AF_USARTx
    CLK_Type      USART_CLK;     // RCC_APB2Periph_USARTx
    Baudrate_Type Baudrate;

    GPIO_Type     RX_GPIO;       // GPIOx
    Pin_Type      RX_Pin;        // GPIO_Pin_x
    CLK_Type      RX_GPIO_CLK;   // RCC_AHB1Periph_GPIOx
    Source_Type   RX_PinSource;  // GPIO_PinSource_x

    GPIO_Type     TX_GPIO;       // GPIOx
    Pin_Type      TX_Pin;        // GPIO_Pin_x
    CLK_Type      TX_GPIO_CLK;   // RCC_AHB1Periph_GPIOx
    Source_Type   TX_PinSource;  // GPIO_PinSource_x

    IRQn_Type     USART_IRQn;    // USARTx_IRQn
    void(*USART_IRQHandler)();   // USARTx_IRQHandler

    unsigned char RxBuffer[UART_RX_BUFFER_SIZE]; // 串口接收缓冲数组
    unsigned int  RxPtr;                         // 串口接收缓冲数组下标
} C_USART;

extern C_USART usart1;

void Usart_SendByte(USART_Type pUSARTx, uint8_t ch);
void Usart_SendString(USART_Type pUSARTx, char *str);

void Usart_SendHalfWord(USART_Type pUSARTx, uint16_t ch);
void Usart_FlushRxBuffer(C_USART*);

void InitUsart(const C_USART*);

#endif // USART_H

// ################################ End of file ################################
