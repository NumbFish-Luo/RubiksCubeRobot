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

// �����жϷ�����
void USART1_Handler() {
    unsigned char data;
    if (USART_GetITStatus(usart1.USART, USART_IT_RXNE) != RESET) {
        data = USART_ReceiveData(usart1.USART);
        // ���Ϊ�˸��
        if (data == '\b') {
            // ���ָ�벻������Ŀ�ʼλ��
            if(usart1.RxPtr) {
                Usart_SendByte(usart1.USART, '\b');
                Usart_SendByte(usart1.USART, ' ');
                Usart_SendByte(usart1.USART, '\b');
                usart1.RxPtr--;
                usart1.RxBuffer[usart1.RxPtr] = 0x00;
            }
        } else { // ��������˸��
            // ��������������usart1.RxBuffer
            // ���ҽ������һ��Ԫ�������������������д�����һ��Ԫ��Ϊֹ
            if (usart1.RxPtr < (UART_RX_BUFFER_SIZE - 1)) {
                usart1.RxBuffer[usart1.RxPtr] = data;
                usart1.RxBuffer[usart1.RxPtr + 1] = 0x00;
                usart1.RxPtr++;
            } else {
                usart1.RxBuffer[usart1.RxPtr - 1] = data;
                Usart_SendByte(usart1.USART, '\b');
            }
            // ���Ϊ�س�������ʼ����������
            if (data == '\r') {
                // ...
            } else {
                Usart_SendByte(usart1.USART, data);
            }
        }
    }
}
