#include "config/config.h"
#include "stm32f4xx_it.h"

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

void KEY_MSD_Enable_Handler() {
    // ȷ���Ƿ������EXTI Line�ж�
    if(EXTI_GetITStatus(key_MSD_Enable.EXTI_Line) != RESET) {
        if(MSD_D.status.out_ena != FALSE) {
            LED_Toggle(&g_LED_R);
            MSD_ENA(&MSD_D, ENABLE);
        }
        if(MSD_L.status.out_ena != FALSE) {
            LED_Toggle(&g_LED_R);
            MSD_ENA(&MSD_L, ENABLE);
        }
        // ����жϱ�־λ
        EXTI_ClearITPendingBit(key_MSD_Enable.EXTI_Line);
    }
}

void KEY_MSD_Disable_Handler() {
    // ȷ���Ƿ������EXTI Line�ж�
    if(EXTI_GetITStatus(key_MSD_Disable.EXTI_Line) != RESET) {
        if(MSD_D.status.out_ena != TRUE) {
            LED_Toggle(&g_LED_G);
            MSD_ENA(&MSD_D, DISABLE);
        }
        if(MSD_L.status.out_ena != TRUE) {
            LED_Toggle(&g_LED_G);
            MSD_ENA(&MSD_L, DISABLE);
        }
        // ����жϱ�־λ
        EXTI_ClearITPendingBit(key_MSD_Disable.EXTI_Line);
    }
}

// �����жϷ�����
void USART1_Handler() {
    unsigned char data;
    if (USART_GetITStatus(usart1.USART, USART_IT_RXNE) != RESET) {
        data = USART_ReceiveData(usart1.USART);
        if (MSD_D.status.running == FALSE && MSD_D.status.out_ena == TRUE
            && MSD_L.status.running == FALSE && MSD_L.status.out_ena == TRUE) {
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
                    MSD_D.status.cmd = TRUE;
                    MSD_L.status.cmd = TRUE;
                } else {
                    Usart_SendByte(usart1.USART, data);
                }
            }
        }
    }
}

// �������嶨ʱ�����ж���Ӧ����ÿ��һ����������˶�״̬
void MSD_D_Handler() {
    MSD_IRQHandler(&MSD_D);
}

void MSD_L_Handler() {
    MSD_IRQHandler(&MSD_L);
}
