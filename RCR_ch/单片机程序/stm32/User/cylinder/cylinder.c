#include "cylinder/cylinder.h"

__HIDE_METHOD__ void Cylinder_GPIO_Config(const C_Cylinder* c)
{
    GPIO_InitTypeDef g;
    RCC_AHB1PeriphClockCmd(c->GPIO_CLK, ENABLE);
    g.GPIO_Pin   = c->Pin;
    g.GPIO_Mode  = GPIO_Mode_OUT;
    g.GPIO_OType = GPIO_OType_PP;
    g.GPIO_PuPd  = GPIO_PuPd_DOWN;
    g.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(c->GPIO, &g);
}

void InitCylinder(const C_Cylinder *c)
{
    Cylinder_GPIO_Config(c);
    Cylinder_ON(c);
}

__HIDE_METHOD__ inline void DigitalHi(GPIO_Type g, Pin_Type p) { g->BSRRL = p; }
__HIDE_METHOD__ inline void DigitalLo(GPIO_Type g, Pin_Type p) { g->BSRRH = p; }
__HIDE_METHOD__ inline void DigitalToggle(GPIO_Type g, Pin_Type p) { g->ODR ^= p; }

void Cylinder_ON(const C_Cylinder* c)
{
    DigitalHi(c->GPIO, c->Pin);
}

void Cylinder_OFF(const C_Cylinder* c)
{
    DigitalLo(c->GPIO, c->Pin);
}

void Cylinder_Toogle(const C_Cylinder* c)
{
    DigitalToggle(c->GPIO, c->Pin);
}
