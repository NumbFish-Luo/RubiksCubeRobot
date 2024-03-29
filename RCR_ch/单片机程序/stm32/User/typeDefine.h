#ifndef TYPE_DEFINE_H
#define TYPE_DEFINE_H

#include "stm32f4xx.h"

#define GPIO_Type     GPIO_TypeDef*       // GPIO
#define TIM_Type      TIM_TypeDef*        // 定时器
#define USART_Type    USART_TypeDef*      // 串口

#define CLK_Type      uint32_t            // 时钟
#define Line_Type     uint32_t            // 外部中断线
#define PWM_Type      uint32_t            // PWM
#define Baudrate_Type uint32_t            // 波特率
#define MSD_Type      uint32_t            // 步进电机

#define Pin_Type      uint16_t            // Pin口
#define Preload_Type  uint16_t            // 定时器预装载

#define Source_Type   uint8_t             // PinSource
#define AF_Type       uint8_t             // 功能

#define IRQn_Type     int                 // 中断次数

#define Trigger_Type  EXTITrigger_TypeDef // 按键中断触发类型

#define Bool          unsigned char

#endif // TYPE_DEFINE_H
