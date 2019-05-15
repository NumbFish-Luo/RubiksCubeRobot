#include "stm32f4xx_it.h"
#include "SerialPort.h"

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
    SerialPort* s = &serialPort;
    
    if (USART_GetITStatus(s->usart, USART_IT_RXNE) != RESET) {
        data = USART_ReceiveData(s->usart);
        // 如果为退格键
        if (data == '\b') {
            // 如果指针不在数组的开始位置
            if(s->index) {
                s->SendByte(s, '\b');
                s->SendByte(s, ' ');
                s->SendByte(s, '\b');
                --(s->index);
                s->buffer[s->index] = 0;
            }
        } else { // 如果不是退格键
            // 将数据填入数组usart1.RxBuffer
            // 并且将后面的一个元素清零如果数组满了则写入最后一个元素为止
            if (s->index < (RX_SIZE - 1)) {
                s->buffer[s->index] = data;
                s->buffer[s->index + 1] = 0x00;
                ++(s->index);
            } else {
                s->buffer[s->index - 1] = data;
                s->SendByte(s, '\b');
            }
            // 如果为回车键，则开始处理串口数据
            if (data == '\r') {
                s->cmdOK = 1;
            } else {
                s->SendByte(s, data);
            }
        }
    }
}
