#ifndef USART_H
#define USART_H

#include <stdio.h>
#include "stm32f4xx.h"
#include "typeDefine.h"
#include "IO.h"

// 串口接收缓冲数组大小
#define RX_SIZE 1024

typedef struct SerialPort SerialPort;
struct SerialPort {
    Baudrate_Type baudrate;
    USART_Type    usart;
    GPIO_Type     rxGpio;
    Pin_Type      rxPin;
    GPIO_Type     txGpio;
    Pin_Type      txPin;
    unsigned char buffer[RX_SIZE];
    unsigned int  index;
    Bool          cmdOK;
    void(*SendByte)(SerialPort* self, uint8_t byte);
    void(*Flush)(SerialPort* self);
    void(*IRQHandler)();
};
SerialPort NewSerialPort(
    USART_Type usart, AF_Type af, CLK_Type usartClk, Baudrate_Type baudrate,
    IRQn_Type irqn, void(*IRQHandler)(void),
    GPIO_Type rxGpio, Pin_Type rxPin, CLK_Type rxClk, Source_Type rxSource,
    GPIO_Type txGpio, Pin_Type txPin, CLK_Type txClk, Source_Type txSource
);

extern SerialPort serialPort;

#endif // USART_H
