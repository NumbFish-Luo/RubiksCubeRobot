#ifndef CYLINDER_H
#define CYLINDER_H

#include "stm32f4xx.h"
#include "typeDefine.h"

typedef struct C_Cylinder
{
    GPIO_Type GPIO;     // GPIOx
    Pin_Type  Pin;      // Pin_x
    CLK_Type  GPIO_CLK; // RCC_AHB1Periph_GPIOx
} C_Cylinder;

extern C_Cylinder cylinder_L;
extern C_Cylinder cylinder_D;

void InitCylinder(const C_Cylinder*);
void Cylinder_ON(const C_Cylinder*);
void Cylinder_OFF(const C_Cylinder*);
void Cylinder_Toogle(const C_Cylinder*);

#endif // CYLINDER_H
