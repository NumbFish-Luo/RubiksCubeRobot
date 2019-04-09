#include "led/led.h"

void InitLED(const C_LED* led) {
    RCC_AHB1PeriphClockCmd(led->GPIO_CLK, ENABLE);
    GPIO_InitTypeDef g;
    g.GPIO_Pin   = led->Pin;
    g.GPIO_Mode  = GPIO_Mode_OUT;
    g.GPIO_OType = GPIO_OType_PP;
    g.GPIO_PuPd  = GPIO_PuPd_UP; // 上拉, 默认高电平, LED亮
    g.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(led->GPIO, &g);
}

void LED_RGB_OFF() {
    LED_OFF(&g_LED_R);
    LED_OFF(&g_LED_G);
    LED_OFF(&g_LED_B);
}

__HIDE_METHOD__ inline void DigitalHi(GPIO_Type g, Pin_Type p) { g->BSRRL = p; }
__HIDE_METHOD__ inline void DigitalLo(GPIO_Type g, Pin_Type p) { g->BSRRH = p; }
__HIDE_METHOD__ inline void DigitalToggle(GPIO_Type g, Pin_Type p) { g->ODR ^= p; }

void LED_ON(const C_LED* led) {
    DigitalHi(led->GPIO, led->Pin);
}

void LED_OFF(const C_LED* led) {
    DigitalLo(led->GPIO, led->Pin);
}

void LED_Toggle(const C_LED* led) {
    DigitalToggle(led->GPIO, led->Pin);
}
