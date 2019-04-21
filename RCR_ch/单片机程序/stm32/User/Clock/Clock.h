#ifndef CLOCK_H
#define CLOCK_H

#include "NuxieTube/NuxieTube.h"

typedef struct C_Clock {
    C_NuxieTube   nuxieTube;    // 数码管
    TIM_Type      TIM;          // 定时器
    CLK_Type      TIM_CLK;      // 时钟
    IRQn_Type     TIM_IRQn;     // 中断次数
    void(*Pulse_APB_CLK_FUN)(); // 时钟函数
    void(*IRQHandler)();        // 中断函数
    int           count;        // 计数, 单位为百分之一秒
    Bool          Start;        // 开始标志
} C_Clock;

void InitClock(C_Clock*);          // 初始化时钟
void StartClock(C_Clock*);         // 启动
void EndClock(C_Clock*);           // 结束（暂停）
void ResetClock(C_Clock*);         // 重置（清零）
void Clock_IRQHandler(C_Clock* c); // 时钟中断函数

#endif // CLOCK_H
