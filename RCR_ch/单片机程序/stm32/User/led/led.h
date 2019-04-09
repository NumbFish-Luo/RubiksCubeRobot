#ifndef LED_H
#define LED_H

#include "stm32f4xx.h"
#include "typeDefine.h"

typedef struct C_LED           // LED����
{
    GPIO_Type GPIO;            // GPIOx
    Pin_Type  Pin;             // Pin_x
    CLK_Type  GPIO_CLK;        // RCC_AHB1Periph_GPIOx
} C_LED;

extern C_LED g_LED_R;          // ���
extern C_LED g_LED_G;          // �̵�
extern C_LED g_LED_B;          // ����

void InitLED(const C_LED*);    // ��ʼ��
void LED_RGB_OFF(void);        // ȫ��Ϩ��
void LED_ON(const C_LED*);     // ����
void LED_OFF(const C_LED*);    // Ϩ��
void LED_Toggle(const C_LED*); // ��ת

#endif // LED_H
