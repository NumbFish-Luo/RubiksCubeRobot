#ifndef MICRO_STEP_DRIVER_H
#define MICRO_STEP_DRIVER_H

#include "stm32f4xx.h"
#include "typeDefine.h"

typedef enum E_Direction { // 方向枚举
    CW, // 顺时针
    CCW // 逆时针
}E_Direction;

typedef enum E_Bool {
    FALSE = 0,
    TRUE  = 1
}E_Bool;

// 系统状态
typedef struct C_MSD_Status {
    unsigned char running; // 当步进电机正在运行时, 值为TRUE, 默认为FALSE
    unsigned char cmd    ; // 当串口接收到数据时, 值为TURE
    unsigned char out_ena; // 当驱动器正常输出时, 值为TRUE
} C_MSD_Status;

typedef struct C_MicroStepDriver {
    TIM_Type      Pulse_TIM;     // 定时器
    void(*Pulse_APB_CLK_FUN)();  // 时钟函数
    CLK_Type      Pulse_TIM_CLK; // 时钟

    void(*Pulse_OC_Init)();
    void(*Pulse_OC_Preload_Config)(TIM_Type TIMx, Preload_Type TIM_OCPreload);
    void(*TIM_SetCompare)(TIM_Type TIMx, PWM_Type Compare2);

    IRQn_Type     Pulse_TIM_IRQn;  // 中断
    void(*Pulse_TIM_IRQHandler)(); // 中断函数

    void(*Pulse_AHB_CLK_FUN)();
    CLK_Type      Pulse_GPIO_CLK;
    GPIO_Type     Pulse_GPIO;
    Pin_Type      Pulse_Pin;
    AF_Type       Pulse_AF_TIM;
    Source_Type   Pulse_Pin_Source;

    void(*Dir_AHB_CLK_FUN)(CLK_Type RCC_AHB1Periph, FunctionalState NewState);
    CLK_Type      Dir_GPIO_CLK;
    GPIO_Type     Dir_GPIO;
    Pin_Type      Dir_Pin;

    void(*Ena_AHB_CLK_FUN)(CLK_Type RCC_AHB1Periph, FunctionalState NewState);
    CLK_Type      Ena_GPIO_CLK;
    GPIO_Type     Ena_GPIO;
    Pin_Type      Ena_Pin;

    E_Direction   dir;
    C_MSD_Status  status;
    int           steps;
    int           setAngle;
    int           angle;
} C_MSD;

// PWM 信号的频率 F = TIM_CLK / {(ARR + 1) * (PSC + 1)}
#define MSD_PULSE_TIM_PERIOD (10 - 1)
#define MSD_PULSE_TIM_PSC    (180 - 1)

void InitMSD(const C_MSD*);            // 初始化电机
void MSD_ENA(C_MSD*, FunctionalState); // 使能/失能电机
void MSD_Move(C_MSD*);                 // 电机移动计算 ☆
void MSD_IRQHandler(C_MSD*);           // 电机中断函数 ☆
void MSD_waitStop(const C_MSD*);       // 等待电机停止

#endif /* MICRO_STEP_DRIVER_H */
