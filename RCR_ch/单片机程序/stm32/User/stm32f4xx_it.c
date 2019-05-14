#include "config/config.h"
#include "stm32f4xx_it.h"
#include "Order/Order.h"

void NMI_Handler       () { }
void HardFault_Handler () {
    while (1);    /* Go to infinite loop when Hard Fault exception occurs */
}
void MemManage_Handler () {
    while (1);    /* Go to infinite loop when Memory Manage exception occurs */
}
void BusFault_Handler  () {
    while (1);    /* Go to infinite loop when Bus Fault exception occurs */
}
void UsageFault_Handler() {
    while (1);    /* Go to infinite loop when Usage Fault exception occurs */
}
void DebugMon_Handler  () { }
void SVC_Handler       () { }
void PendSV_Handler    () { }
void SysTick_Handler   () { }

// 串口中断服务函数
void USART1_Handler() {
    unsigned char data;
    if (USART_GetITStatus(usart1.USART, USART_IT_RXNE) != RESET) {
        data = USART_ReceiveData(usart1.USART);
        // 如果为退格键
        if (data == '\b') {
            // 如果指针不在数组的开始位置
            if(usart1.RxPtr) {
                Usart_SendByte(usart1.USART, '\b');
                Usart_SendByte(usart1.USART, ' ');
                Usart_SendByte(usart1.USART, '\b');
                usart1.RxPtr--;
                usart1.RxBuffer[usart1.RxPtr] = 0x00;
            }
        } else { // 如果不是退格键
            // 将数据填入数组usart1.RxBuffer
            // 并且将后面的一个元素清零如果数组满了则写入最后一个元素为止
            if (usart1.RxPtr < (UART_RX_BUFFER_SIZE - 1)) {
                usart1.RxBuffer[usart1.RxPtr] = data;
                usart1.RxBuffer[usart1.RxPtr + 1] = 0x00;
                usart1.RxPtr++;
            } else {
                usart1.RxBuffer[usart1.RxPtr - 1] = data;
                Usart_SendByte(usart1.USART, '\b');
            }
            // 如果为回车键，则开始处理串口数据
            if (data == '\r') {
                // ...
            } else {
                Usart_SendByte(usart1.USART, data);
            }
        }
    }
}
