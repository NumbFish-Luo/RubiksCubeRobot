#include "Clock/Clock.h"

static void TIM_NVIC_Configuration(const C_Clock* c)
{
    NVIC_InitTypeDef n; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    n.NVIC_IRQChannel = c->TIM_IRQn;
    n.NVIC_IRQChannelPreemptionPriority = 0;
    n.NVIC_IRQChannelSubPriority = 3;
    n.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&n);
}

// 设置为千分之一秒中断一次
static void TIM_Mode_Config(const C_Clock* c)
{
    TIM_TimeBaseInitTypeDef t;
    RCC_APB2PeriphClockCmd(c->TIM_CLK, ENABLE);
    t.TIM_Period = 5 - 1;
    t.TIM_Prescaler = 18000 - 1;
    t.TIM_ClockDivision = TIM_CKD_DIV1;
    t.TIM_CounterMode = TIM_CounterMode_Up;
    t.TIM_RepetitionCounter = 1;
    TIM_TimeBaseInit(c->TIM, &t);
    TIM_ClearFlag(c->TIM, TIM_FLAG_Update);
    TIM_ITConfig(c->TIM,TIM_IT_Update,ENABLE);
    TIM_Cmd(c->TIM, ENABLE);
}

void InitClock(C_Clock* c) {
    int number[8] = {0};
    int dp = -1;
    TIM_NVIC_Configuration(c);
    TIM_Mode_Config(c);
    InitNuxieTube(&c->nuxieTube);
    SetNuxieTube(&c->nuxieTube, number, dp);
}

void StartClock(C_Clock* c) {
    c->Start = 1;
}

void EndClock(C_Clock* c) {
    c->Start = 0;
}

void ResetClock(C_Clock* c) {
    int number[8] = {0, 0, 0, 23, 23, 23, 23, 23};
    c->count = 0;
    c->Start = 0;
    SetNuxieTube(&c->nuxieTube, number, 2);
}

void Clock_IRQHandler(C_Clock* c) // 时钟中断
{
    static const int pow[8] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};
    static int time = 0;
    int number[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int dp = 2;
    int i = 0;
    
    if (TIM_GetITStatus(c->TIM, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(c->TIM, TIM_IT_Update);
    }
    
    if (c->Start == 1) {
        if (++time == 10) {
            time = 0;
            ++(c->count);
            for (i = 0; i < 8; ++i) {
                if (i < 3 || c->count >= pow[i]) {
                    number[i] = (c->count / pow[i]) % 10;
                } else {
                    number[i] = 23;
                }
            }
            SetNuxieTube(&c->nuxieTube, number, dp);
        }
    }
    ShowNuxieTube(&c->nuxieTube);
}
