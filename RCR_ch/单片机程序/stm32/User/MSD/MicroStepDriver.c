#include <math.h>
#include <stdio.h>
#include "config/config.h"
#include "MSD/MicroStepDriver.h"
#include "order/order.h"

unsigned int delayTime_180[TRUN_180] = {
3634,  954,  673,  539,  457,  401,  360,  329,  303,  283,  265,  251,  238, 
 227,  217,  208,  200,  193,  187,  181,  175,  170,  166,  161,  157,  154, 
 150,  147,  144,  141,  138,  136,  133,  131,  129,  127,  125,  123,  121, 
 119,  118,  116,  114,  113,  112,  110,  109,  108,  106,  105,  104,  103, 
 102,  101,  100,   99,   98,   97,   96,   95,   95,   94,   93,   92,   92, 
  91,   90,   90,   89,   88,   88,   87,   86,   86,   85,   85,   84,   84, 
  83,   83,   82,   82,   81,   81,   80,   80,   80,   79,   79,   78,   78, 
  78,   77,   77,   76,   76,   76,   75,   75,   75,   75,   74,   74,   74, 
  73,   73,   73,   73,   72,   72,   72,   72,   71,   71,   71,   71,   70, 
  70,   70,   70,   70,   69,   69,   69,   69,   69,   68,   68,   68,   68, 
  68,   68,   67,   67,   67,   67,   67,   67,   66,   66,   66,   66,   66, 
  66,   66,   66,   65,   65,   65,   65,   65,   65,   65,   65,   65,   64, 
  64,   64,   64,   64,   64,   64,   64,   64,   64,   64,   64,   64,   63, 
  63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63, 
  63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63, 
  63,   63,   63,   63,   62,   62,   63,   63,   63,   63,   63,   63,   63, 
  63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63, 
  63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   64,   64, 
  64,   64,   64,   64,   64,   64,   64,   64,   64,   64,   64,   65,   65, 
  65,   65,   65,   65,   65,   65,   65,   66,   66,   66,   66,   66,   66, 
  66,   66,   67,   67,   67,   67,   67,   67,   68,   68,   68,   68,   68, 
  68,   69,   69,   69,   69,   69,   70,   70,   70,   70,   70,   71,   71, 
  71,   71,   72,   72,   72,   72,   73,   73,   73,   73,   74,   74,   74, 
  75,   75,   75,   75,   76,   76,   76,   77,   77,   78,   78,   78,   79, 
  79,   80,   80,   80,   81,   81,   82,   82,   83,   83,   84,   84,   85, 
  85,   86,   86,   87,   88,   88,   89,   90,   90,   91,   92,   92,   93, 
  94,   95,   95,   96,   97,   98,   99,  100,  101,  102,  103,  104,  105, 
 106,  108,  109,  110,  112,  113,  114,  116,  118,  119,  121,  123,  125, 
 127,  129,  131,  133,  136,  138,  141,  144,  147,  150,  154,  157,  161, 
 166,  170,  175,  181,  187,  193,  200,  208,  217,  227,  238,  251,  265, 
 283,  303,  329,  360,  401,  457,  539,  673,  954, 3634
};
unsigned int delayTime_180_slow[TRUN_180] = {
4361, 1145,  808,  646,  548,  481,  432,  394,  364,  339,  318,  301,  285, 
 272,  260,  250,  240,  232,  224,  217,  210,  204,  199,  194,  189,  184, 
 180,  176,  173,  169,  166,  163,  160,  157,  155,  152,  150,  147,  145, 
 143,  141,  139,  137,  136,  134,  132,  131,  129,  128,  126,  125,  124, 
 122,  121,  120,  119,  118,  117,  116,  115,  114,  113,  112,  111,  110, 
 109,  108,  107,  107,  106,  105,  104,  104,  103,  102,  102,  101,  100, 
 100,   99,   99,   98,   98,   97,   96,   96,   95,   95,   94,   94,   94, 
  93,   93,   92,   92,   91,   91,   91,   90,   90,   89,   89,   89,   88, 
  88,   88,   87,   87,   87,   86,   86,   86,   86,   85,   85,   85,   84, 
  84,   84,   84,   83,   83,   83,   83,   82,   82,   82,   82,   82,   81, 
  81,   81,   81,   81,   80,   80,   80,   80,   80,   80,   79,   79,   79, 
  79,   79,   79,   79,   78,   78,   78,   78,   78,   78,   78,   78,   77, 
  77,   77,   77,   77,   77,   77,   77,   77,   76,   76,   76,   76,   76, 
  76,   76,   76,   76,   76,   76,   76,   76,   76,   76,   75,   75,   75, 
  75,   75,   75,   75,   75,   75,   75,   75,   75,   75,   75,   75,   75, 
  75,   75,   75,   75,   75,   75,   75,   75,   75,   75,   75,   75,   75, 
  75,   75,   75,   75,   75,   75,   75,   75,   75,   75,   75,   75,   75, 
  76,   76,   76,   76,   76,   76,   76,   76,   76,   76,   76,   76,   76, 
  76,   76,   77,   77,   77,   77,   77,   77,   77,   77,   77,   78,   78, 
  78,   78,   78,   78,   78,   78,   79,   79,   79,   79,   79,   79,   79, 
  80,   80,   80,   80,   80,   80,   81,   81,   81,   81,   81,   82,   82, 
  82,   82,   82,   83,   83,   83,   83,   84,   84,   84,   84,   85,   85, 
  85,   86,   86,   86,   86,   87,   87,   87,   88,   88,   88,   89,   89, 
  89,   90,   90,   91,   91,   91,   92,   92,   93,   93,   94,   94,   94, 
  95,   95,   96,   96,   97,   98,   98,   99,   99,  100,  100,  101,  102, 
 102,  103,  104,  104,  105,  106,  107,  107,  108,  109,  110,  111,  112, 
 113,  114,  115,  116,  117,  118,  119,  120,  121,  122,  124,  125,  126, 
 128,  129,  131,  132,  134,  136,  137,  139,  141,  143,  145,  147,  150, 
 152,  155,  157,  160,  163,  166,  169,  173,  176,  180,  184,  189,  194, 
 199,  204,  210,  217,  224,  232,  240,  250,  260,  272,  285,  301,  318, 
 339,  364,  394,  432,  481,  548,  646,  808, 1145, 4361
};
unsigned int delayTime_90[TRUN_90] = {
2294,  606,  429,  344,  293,  258,  232,  212,  197,  184,  173,  164,  156, 
 149,  142,  137,  132,  128,  124,  120,  117,  114,  111,  108,  106,  104, 
 101,   99,   98,   96,   94,   93,   91,   90,   89,   87,   86,   85,   84, 
  83,   82,   81,   80,   79,   79,   78,   77,   76,   76,   75,   74,   74, 
  73,   73,   72,   72,   71,   71,   70,   70,   69,   69,   69,   68,   68, 
  68,   67,   67,   67,   66,   66,   66,   66,   65,   65,   65,   65,   65, 
  64,   64,   64,   64,   64,   64,   63,   63,   63,   63,   63,   63,   63, 
  63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63, 
  63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   64, 
  64,   64,   64,   64,   64,   65,   65,   65,   65,   65,   66,   66,   66, 
  66,   67,   67,   67,   68,   68,   68,   69,   69,   69,   70,   70,   71, 
  71,   72,   72,   73,   73,   74,   74,   75,   76,   76,   77,   78,   79, 
  79,   80,   81,   82,   83,   84,   85,   86,   87,   89,   90,   91,   93, 
  94,   96,   98,   99,  101,  104,  106,  108,  111,  114,  117,  120,  124, 
 128,  132,  137,  142,  149,  156,  164,  173,  184,  197,  212,  232,  258, 
 293,  344,  429,  606, 2294
};
unsigned int delayTime_90_slow[TRUN_90] = {
2753,  727,  515,  413,  352,  310,  279,  255,  236,  220,  207,  196,  187, 
 178,  171,  164,  159,  153,  148,  144,  140,  136,  133,  130,  127,  124, 
 122,  119,  117,  115,  113,  111,  109,  108,  106,  105,  103,  102,  101, 
 100,   98,   97,   96,   95,   94,   93,   92,   92,   91,   90,   89,   89, 
  88,   87,   87,   86,   85,   85,   84,   84,   83,   83,   82,   82,   82, 
  81,   81,   80,   80,   80,   79,   79,   79,   78,   78,   78,   78,   77, 
  77,   77,   77,   77,   76,   76,   76,   76,   76,   76,   76,   76,   75, 
  75,   75,   75,   75,   75,   75,   75,   75,   75,   75,   75,   75,   75, 
  75,   75,   75,   75,   75,   75,   76,   76,   76,   76,   76,   76,   76, 
  76,   77,   77,   77,   77,   77,   78,   78,   78,   78,   79,   79,   79, 
  80,   80,   80,   81,   81,   82,   82,   82,   83,   83,   84,   84,   85, 
  85,   86,   87,   87,   88,   89,   89,   90,   91,   92,   92,   93,   94, 
  95,   96,   97,   98,  100,  101,  102,  103,  105,  106,  108,  109,  111, 
 113,  115,  117,  119,  122,  124,  127,  130,  133,  136,  140,  144,  148, 
 153,  159,  164,  171,  178,  187,  196,  207,  220,  236,  255,  279,  310, 
 352,  413,  515,  727, 2753
};
unsigned int delayTime_trim[TRUN_TRIM] = {
    1000
};
unsigned int delayTime_Idx = 0;

__HIDE_METHOD__ void MSD_NVIC_Config(const C_MSD* m) { // 定时器中断优先级配置
    NVIC_InitTypeDef n;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); // Group 0
    n.NVIC_IRQChannel = m->Pulse_TIM_IRQn;          // 设置中断来源
    n.NVIC_IRQChannelPreemptionPriority = 1;        // 设置抢占优先级
    n.NVIC_IRQChannelSubPriority = 1;               // 设置子优先级
    n.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&n);
}

__HIDE_METHOD__ void MSD_GPIO_Config(const C_MSD* m) {
    GPIO_InitTypeDef g;

    // 电机脉冲输出 GPIO 初始化
    m->Pulse_AHB_CLK_FUN(m->Pulse_GPIO_CLK, ENABLE);
    g.GPIO_Pin   = m->Pulse_Pin;
    g.GPIO_Mode  = GPIO_Mode_OUT;
    g.GPIO_OType = GPIO_OType_PP;
    g.GPIO_PuPd  = GPIO_PuPd_UP;
    g.GPIO_Mode  = GPIO_Mode_AF;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(m->Pulse_GPIO, &g);

    GPIO_PinAFConfig(m->Pulse_GPIO, m->Pulse_Pin_Source, m->Pulse_AF_TIM); // 这里连接定时器

    // 电机方向输出GPIO初始化
    m->Dir_AHB_CLK_FUN(m->Dir_GPIO_CLK, ENABLE);
    g.GPIO_Pin   = m->Dir_Pin;
    g.GPIO_Mode  = GPIO_Mode_OUT;
    g.GPIO_OType = GPIO_OType_PP;
    g.GPIO_PuPd  = GPIO_PuPd_UP;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(m->Dir_GPIO, &g);

    GPIO_ResetBits(m->Dir_GPIO, m->Dir_Pin);

    //电机使能输出 GPIO 初始化
    m->Ena_AHB_CLK_FUN(m->Ena_GPIO_CLK, ENABLE);
    g.GPIO_Pin   = m->Ena_Pin;
    g.GPIO_Mode  = GPIO_Mode_OUT;
    g.GPIO_OType = GPIO_OType_PP;
    g.GPIO_PuPd  = GPIO_PuPd_UP;
    g.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(m->Ena_GPIO, &g);

    GPIO_ResetBits(m->Ena_GPIO, m->Ena_Pin);
}

__HIDE_METHOD__ void MSD_Mode_Config(const C_MSD* m) {
    TIM_TimeBaseInitTypeDef t;
    TIM_OCInitTypeDef       o;

    m->Pulse_APB_CLK_FUN(m->Pulse_TIM_CLK, ENABLE);  // 内部时钟CK_INT = 180M

    // TimeBase
    TIM_TimeBaseStructInit(&t);
    t.TIM_Period        = MSD_PULSE_TIM_PERIOD; // 自动重装载寄存器的值, 累计TIM_Period+1个周期后产生一个更新或者中断
    t.TIM_Prescaler     = MSD_PULSE_TIM_PSC;    // 驱动CNT计数器的时钟 = Fck_int / (psc + 1)
    t.TIM_ClockDivision = TIM_CKD_DIV1;         // 时钟分频因子, 配置死区时间时需要用到
    t.TIM_CounterMode   = TIM_CounterMode_Up;   // 计数器计数模式, 设置为向上计数
    // TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // 重复计数器的值, 最大值为255由于用的是TIM2, 所以没有TIM_RepetitionCounter
    TIM_TimeBaseInit(m->Pulse_TIM, &t);

    o.TIM_OCMode       = TIM_OCMode_PWM2;
    o.TIM_OutputState  = TIM_OutputState_Enable;
    o.TIM_OutputNState = TIM_OutputNState_Disable;  // 互补输出禁能
    o.TIM_Pulse        = MSD_PULSE_TIM_PERIOD >> 1; // 设置占空比大小
    o.TIM_OCPolarity   = TIM_OCPolarity_Low;        // 输出通道电平极性配置
    o.TIM_OCIdleState  = TIM_OCIdleState_Reset;     // 输出通道空闲电平极性配置

    m->Pulse_OC_Init(m->Pulse_TIM, &o);
    m->Pulse_OC_Preload_Config(m->Pulse_TIM, TIM_OCPreload_Enable);
 
    TIM_ARRPreloadConfig(m->Pulse_TIM, ENABLE);

    TIM_UpdateRequestConfig(m->Pulse_TIM, TIM_UpdateSource_Regular); // 设置中断源，只有溢出时才中断
    TIM_ClearITPendingBit(m->Pulse_TIM, TIM_IT_Update);              // 清除中断标志位
    TIM_ITConfig(m->Pulse_TIM, TIM_IT_Update, ENABLE);               // 使能中断
    TIM_Cmd(m->Pulse_TIM, DISABLE);                                  // 使能计数器
}

void InitMSD(const C_MSD* m) {
    MSD_GPIO_Config(m);
    MSD_NVIC_Config(m);
    MSD_Mode_Config(m);
}

// 驱动器紧急停止或开启
void MSD_ENA(C_MSD* m, FunctionalState NewState) { // NewState 是一个枚举类型, 有ENABLE = 1和DISABLE = 0两值
    if (NewState) {
        GPIO_SetBits(m->Ena_GPIO, m->Ena_Pin); // ENA失能, 禁止驱动器输出
        m->status.out_ena = FALSE;             // 紧急停止标志位为真
        printf("\nDrive stop output, Now you can rotate the motor manually\n");
    } else {
        GPIO_ResetBits(m->Ena_GPIO, m->Ena_Pin); // ENA使能
        m->status.out_ena = TRUE;                // 紧急停止标志位为假
        printf("\nDrive Keep Output\n");
    }
}

void MSD_Move(C_MSD* m) {
    m->dir = m->steps < 0 ? CCW : CW;
    m->dir == CW ?
        GPIO_ResetBits(m->Dir_GPIO, m->Dir_Pin) :
        GPIO_SetBits(m->Dir_GPIO, m->Dir_Pin);
    m->status.out_ena = TRUE;
    m->status.running = TRUE;
    delayTime_Idx = 0;
    TIM_SetAutoreload(m->Pulse_TIM, 20); // 默认放置20周期
    m->TIM_SetCompare(m->Pulse_TIM, 10);
    TIM_Cmd(m->Pulse_TIM, ENABLE);
}

void MSD_IRQHandler(C_MSD* m) {
    unsigned int delayTime = 0;
    int step = m->steps;
    
    if (step < 0) { step = -step; }
    if (TIM_GetITStatus(m->Pulse_TIM, TIM_IT_Update) == RESET) {
        return; // 若定时器溢出, 则return
    }
    TIM_ClearITPendingBit(m->Pulse_TIM, TIM_IT_Update);
    
    switch (step) {
        case TRUN_180:
            if (g_robotState.speed == Slow) {
                delayTime = delayTime_180_slow[delayTime_Idx++];
            } else {
                delayTime = delayTime_180[delayTime_Idx++];
            }
            break;
        case TRUN_90:
            if (g_robotState.speed == Slow) {
                delayTime = delayTime_90_slow[delayTime_Idx++];
            } else {
                delayTime = delayTime_90[delayTime_Idx++];
            }
            break;
        case TRUN_TRIM:
            delayTime = delayTime_trim[delayTime_Idx++];
            break;
        default:
            delayTime_Idx = step + 1; // 直接结束
    }
    // 防止延时值过小导致的电机转矩过小，这会导致丢步
    if (delayTime < MIN_DELAY_TIME) {
        delayTime = MIN_DELAY_TIME;
    }
    m->Pulse_TIM->CCR4 = delayTime;
    m->Pulse_TIM->ARR  = delayTime << 1; // 50%脉冲
    
    if (m->status.out_ena == FALSE || delayTime_Idx > step) {
        m->Pulse_TIM->CCER &= ~(1 << 12); // 禁止输出
        TIM_Cmd(m->Pulse_TIM, DISABLE);
        m->status.running = FALSE;
        delayTime_Idx = 0;
    } else {
        m->Pulse_TIM->CCER |= 1 << 12;    // 使能输出
    }
}

void MSD_waitStop(const C_MSD* m) {
    while (m->status.running == TRUE && m->status.out_ena == TRUE);
}
