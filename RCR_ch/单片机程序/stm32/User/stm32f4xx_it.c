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

// �����жϷ�����
void USART1_Handler() {
    unsigned char data;
    SerialPort* s = &serialPort;
    
    if (USART_GetITStatus(s->usart, USART_IT_RXNE) != RESET) {
        data = USART_ReceiveData(s->usart);
        // ���Ϊ�˸��
        if (data == '\b') {
            // ���ָ�벻������Ŀ�ʼλ��
            if(s->index) {
                s->SendByte(s, '\b');
                s->SendByte(s, ' ');
                s->SendByte(s, '\b');
                --(s->index);
                s->buffer[s->index] = 0;
            }
        } else { // ��������˸��
            // ��������������usart1.RxBuffer
            // ���ҽ������һ��Ԫ�������������������д�����һ��Ԫ��Ϊֹ
            if (s->index < (RX_SIZE - 1)) {
                s->buffer[s->index] = data;
                s->buffer[s->index + 1] = 0x00;
                ++(s->index);
            } else {
                s->buffer[s->index - 1] = data;
                s->SendByte(s, '\b');
            }
            // ���Ϊ�س�������ʼ����������
            if (data == '\r') {
                s->cmdOK = 1;
            } else {
                s->SendByte(s, data);
            }
        }
    }
}
