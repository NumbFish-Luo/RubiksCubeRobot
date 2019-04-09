#ifndef KEY_H
#define KEY_H

#include "stm32f4xx.h"
#include "typeDefine.h"

typedef struct C_Key
{
    GPIO_Type    GPIO;            // GPIOx
    Pin_Type     Pin;             // Pin_x
    CLK_Type     GPIO_CLK;        // RCC_AHB1Periph_GPIOx

    Source_Type  EXTI_GPIOSource; // EXTI_PortSourceGPIOx
    Source_Type  EXTI_PinSource;  // EXTI_PinSource_x
    Line_Type    EXTI_Line;       // EXTI_Line_x

    IRQn_Type    EXTI_IRQn;       // EXTIx_IRQn
    void(*EXTI_IRQHandler)();     // EXTIx_IRQHandler

    Trigger_Type trigger;
} C_Key;

void InitKEY(const C_Key*);

#endif // KEY_H
