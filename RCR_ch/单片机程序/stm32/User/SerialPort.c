#include "SerialPort.h"

int fputc(int ch, FILE* f) {
    SerialPort* s = &serialPort;

    USART_SendData(s->usart, (uint8_t)ch);
    while (USART_GetFlagStatus(s->usart, USART_FLAG_TXE) == RESET);
    return ch;
}

int fgetc(FILE* f) {
    SerialPort* s = &serialPort;

    while (USART_GetFlagStatus(s->usart, USART_FLAG_RXNE) == RESET);
    return (int)USART_ReceiveData(s->usart);
}

void SendByte(SerialPort* self, uint8_t byte) {
    USART_SendData(self->usart, byte);
    while (USART_GetFlagStatus(self->usart, USART_FLAG_TXE) == RESET); 
}

void Flush(SerialPort* self) {
    self->index = 0;
    self->buffer[0] = 0;
}

SerialPort NewSerialPort(
    USART_Type usart, AF_Type af, CLK_Type usartClk, Baudrate_Type baudrate,
    IRQn_Type irqn, void(*IRQHandler)(void),
    GPIO_Type rxGpio, Pin_Type rxPin, CLK_Type rxClk, Source_Type rxSource,
    GPIO_Type txGpio, Pin_Type txPin, CLK_Type txClk, Source_Type txSource) {

    SerialPort        s;
    GPIO_InitTypeDef  g;
    USART_InitTypeDef u;
    NVIC_InitTypeDef  n;

    s.usart      = usart;
    s.baudrate   = baudrate;
    s.rxGpio     = rxGpio;
    s.rxPin      = rxPin;
    s.txGpio     = txGpio;
    s.txPin      = txPin;
    s.cmdOK      = 0;
    s.SendByte   = SendByte;
    s.Flush      = Flush;
    s.IRQHandler = IRQHandler;

    RCC_AHB1PeriphClockCmd(rxClk | txClk, ENABLE);
    RCC_APB2PeriphClockCmd(usartClk, ENABLE);

    g.GPIO_OType = GPIO_OType_PP;
    g.GPIO_PuPd  = GPIO_PuPd_UP;
    g.GPIO_Speed = GPIO_Speed_50MHz;

    g.GPIO_Mode = GPIO_Mode_AF;
    g.GPIO_Pin  = txPin;
    GPIO_Init(txGpio, &g);

    g.GPIO_Mode = GPIO_Mode_AF;
    g.GPIO_Pin  = rxPin;
    GPIO_Init(rxGpio, &g);

    GPIO_PinAFConfig(rxGpio, rxSource, af);
    GPIO_PinAFConfig(txGpio, txSource, af);

    u.USART_BaudRate            = baudrate;
    u.USART_WordLength          = USART_WordLength_8b;
    u.USART_StopBits            = USART_StopBits_1;
    u.USART_Parity              = USART_Parity_No;
    u.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    u.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(usart, &u);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    n.NVIC_IRQChannel                   = irqn;
    n.NVIC_IRQChannelPreemptionPriority = 1;
    n.NVIC_IRQChannelSubPriority        = 3;
    n.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&n);

    USART_ITConfig(usart, USART_IT_RXNE, ENABLE);
    USART_Cmd(usart, ENABLE);
    
    s.Flush(&s);
    
    return s;
}
