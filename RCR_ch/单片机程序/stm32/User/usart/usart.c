#include "usart/usart.h"

__HIDE_METHOD__ void Usart_NVIC_Config(const C_USART* usart) {
    NVIC_InitTypeDef n;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);          // Group2
    n.NVIC_IRQChannel                   = usart->USART_IRQn; // 配置USART为中断源
    n.NVIC_IRQChannelPreemptionPriority = 1;                 // 抢断优先级为1
    n.NVIC_IRQChannelSubPriority        = 3;                 // 子优先级为3
    n.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&n);
}

// DEBUG_USART GPIO配置, 工作模式配置. 115200 8-N-1, 中断接收模式
__HIDE_METHOD__ void Usart_Config(const C_USART* usart) {
    GPIO_InitTypeDef  g;
    USART_InitTypeDef u;

    RCC_AHB1PeriphClockCmd(usart->RX_GPIO_CLK | usart->TX_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(usart->USART_CLK, ENABLE);

    g.GPIO_OType = GPIO_OType_PP;
    g.GPIO_PuPd  = GPIO_PuPd_UP;
    g.GPIO_Speed = GPIO_Speed_50MHz;

    // 配置Tx引脚为复用功能
    /* 复用功能就是设置成芯片预设的模块功能，例如某个管脚芯片设置成串口，
       若想将这个管脚作为串口用，就设置成复用功能，
       如果只想作为GPIO用，就设置成OUT模式 */
    g.GPIO_Mode = GPIO_Mode_AF;
    g.GPIO_Pin  = usart->TX_Pin;
    GPIO_Init(usart->TX_GPIO, &g);

    // 配置Rx引脚为复用功能
    g.GPIO_Mode = GPIO_Mode_AF;
    g.GPIO_Pin  = usart->RX_Pin;
    GPIO_Init(usart->RX_GPIO, &g);

    GPIO_PinAFConfig(usart->RX_GPIO, usart->RX_PinSource, usart->GPIO_AF_USART);
    GPIO_PinAFConfig(usart->TX_GPIO, usart->TX_PinSource, usart->GPIO_AF_USART);

    // 配置串DEBUG_USART模式
    u.USART_BaudRate            = usart->Baudrate;                // 波特率设置：DEBUG_USART_BAUDRATE
    u.USART_WordLength          = USART_WordLength_8b;            // 字长(数据位+校验位)：8
    u.USART_StopBits            = USART_StopBits_1;               // 停止位：1个停止位
    u.USART_Parity              = USART_Parity_No;                // 校验位选择：不使用校验
    u.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制：不使用硬件流
    u.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  // USART模式控制：同时使能接收和发送
    USART_Init(usart->USART, &u);

    Usart_NVIC_Config(usart);

    USART_ITConfig(usart->USART, USART_IT_RXNE, ENABLE);          // 使能串口接收中断
    USART_Cmd(usart->USART, ENABLE);                              // 使能串口
}

// 重定向c库函数printf到串口，重定向后可使用printf函数
// https://blog.csdn.net/dragon12345666/article/details/24940395
__NOTICE__
__HIDE_METHOD__ int fputc(int ch, FILE* f) {
    C_USART* usart = &usart1;

    USART_SendData(usart->USART, (uint8_t)ch);                          // 发送一个字节数据到串口
    while (USART_GetFlagStatus(usart->USART, USART_FLAG_TXE) == RESET); // 等待发送完毕
    return ch;
}

// 重定向c库函数scanf到串口，重写向后可使用scanf, getchar等函数
__NOTICE__
__HIDE_METHOD__ int fgetc(FILE* f) {
    C_USART* usart = &usart1;

    while (USART_GetFlagStatus(usart->USART, USART_FLAG_RXNE) == RESET); // 等待串口输入数据
    return (int)USART_ReceiveData(usart->USART);
}

// 发送一个字符
void Usart_SendByte(USART_Type pUSART, uint8_t ch) {
    USART_SendData(pUSART, ch);                                   // 发送一个字节数据到USART
    while (USART_GetFlagStatus(pUSART, USART_FLAG_TXE) == RESET); // 等待发送数据寄存器为空
}

// 发送字符串
void Usart_SendString(USART_Type pUSART, char* str) {
    unsigned int k = 0;
    do {
        Usart_SendByte(pUSART, *(str + k));
        k++;
    }
    while (*(str + k) != '\0'); // '\0'表示一个字符串的结束

    while (USART_GetFlagStatus(pUSART, USART_FLAG_TC) == RESET);
}

// 发送一个16位数
void Usart_SendHalfWord(USART_Type pUSART, uint16_t ch) {
    uint8_t temp_H, temp_L;

    temp_H = (ch & 0XFF00) >> 8; // 取出高八位
    temp_L = ch & 0XFF;          // 取出低八位

    USART_SendData(pUSART, temp_H); // 发送高八位
    while (USART_GetFlagStatus(pUSART, USART_FLAG_TXE) == RESET);

    USART_SendData(pUSART, temp_L); // 发送低八位
    while (USART_GetFlagStatus(pUSART, USART_FLAG_TXE) == RESET);
}

// 清空发送缓冲
void Usart_FlushRxBuffer(C_USART* usart) {
    usart->RxPtr = 0;
    usart->RxBuffer[usart->RxPtr] = 0;
}

void InitUsart(const C_USART* usart) {
    Usart_Config(usart);
}
