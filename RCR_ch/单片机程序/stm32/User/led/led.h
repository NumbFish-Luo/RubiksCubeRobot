#ifndef LED_H
#define LED_H

#include "stm32f4xx.h"
#include "typeDefine.h"

typedef struct C_LED           // LED类型
{
    GPIO_Type GPIO;            // GPIOx
    Pin_Type  Pin;             // Pin_x
    CLK_Type  GPIO_CLK;        // RCC_AHB1Periph_GPIOx
} C_LED;

extern C_LED g_LED_R;          // 红灯
extern C_LED g_LED_G;          // 绿灯
extern C_LED g_LED_B;          // 蓝灯

void InitLED(const C_LED*);    // 初始化
void LED_RGB_OFF(void);        // 全部熄灭
void LED_ON(const C_LED*);     // 点亮
void LED_OFF(const C_LED*);    // 熄灭
void LED_Toggle(const C_LED*); // 反转

#endif // LED_H
