#include "key/key.h"

// NVIC设置中断优先级
__HIDE_METHOD__ void KEY_NVIC_Config(const C_Key* key) {
    NVIC_InitTypeDef n;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);                 // 配置NVIC为优先级组1
    n.NVIC_IRQChannel = key->EXTI_IRQn;                             // 配置中断源
    n.NVIC_IRQChannelPreemptionPriority = key->preemptionPriority;  // 配置抢占优先级：1
    n.NVIC_IRQChannelSubPriority = key->subPriority;                // 配置子优先级：2
    n.NVIC_IRQChannelCmd = ENABLE;                                  // 使能中断通道
    NVIC_Init(&n);
}

__HIDE_METHOD__ void KEY_GPIO_Config(const C_Key* key) {
    GPIO_InitTypeDef g;
    RCC_AHB1PeriphClockCmd(key->GPIO_CLK, ENABLE);
    g.GPIO_Pin = key->Pin;
    g.GPIO_Mode = GPIO_Mode_IN;
    g.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(key->GPIO, &g);
}

__HIDE_METHOD__ void KEY_EXTI_Config(const C_Key* key) {
    EXTI_InitTypeDef e;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);           // 使能SYSCFG时钟，使用GPIO外部中断时必须使能SYSCFG时钟
    KEY_NVIC_Config(key);                                            // 配置NVIC, 设置优先级
    SYSCFG_EXTILineConfig(key->EXTI_PinSource, key->EXTI_PinSource); // 连接EXTI中断源到KEY1引脚
    e.EXTI_Line = key->EXTI_Line;                                    // 选择EXTI中断源
    e.EXTI_Mode = EXTI_Mode_Interrupt;                               // 中断模式
    e.EXTI_Trigger = key->trigger;                                   // xx沿触发
    e.EXTI_LineCmd = ENABLE;                                         // 使能中断/事件线
    EXTI_Init(&e);
}

void InitKEY(const C_Key* key) {
    KEY_GPIO_Config(key);
    KEY_EXTI_Config(key);
}
