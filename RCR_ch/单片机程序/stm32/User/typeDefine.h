#ifndef TYPE_DEFINE_H
#define TYPE_DEFINE_H

#define __NOTICE__                        // ��Ҫ�ر�ע��ĺ���
#define __HIDE_METHOD__  // inline        // ���ṩ���û�ʹ�õĺ���
#define __REDEFINITION__ // extern        // Ϊ�˱�֤������, ���ദextern�ı���/����

#define GPIO_Type     GPIO_TypeDef*       // GPIO
#define TIM_Type      TIM_TypeDef*        // ��ʱ��
#define USART_Type    USART_TypeDef*      // ����

#define CLK_Type      uint32_t            // ʱ��
#define Line_Type     uint32_t            // �ⲿ�ж���
#define PWM_Type      uint32_t            // PWM
#define Baudrate_Type uint32_t            // ������
#define MSD_Type      uint32_t            // �������

#define Pin_Type      uint16_t            // Pin��
#define Preload_Type  uint16_t            // ��ʱ��Ԥװ��

#define Source_Type   uint8_t             // PinSource
#define AF_Type       uint8_t             // ����

#define IRQn_Type     int                 // �жϴ���

#define Trigger_Type  EXTITrigger_TypeDef // �����жϴ�������

#define Bool          unsigned char

#endif // TYPE_DEFINE_H
