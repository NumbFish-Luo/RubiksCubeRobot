#ifndef DEFINE_H
#define DEFINE_H

#include "cylinder/cylinder.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_tim.h"
#include "usart/usart.h"
#include "IO/IO.h"
#include "SteppingMotor/SteppingMotor.h"

// ############## Delay Time #############
#define MSD_DELAY_TIME             (uint32_t)(      1)
#define CYLINDER_DELAY_TIME_CLAMP  (uint32_t)(4000000)
#define CYLINDER_DELAY_TIME_LOOSEN (uint32_t)(4500000)

extern C_USART usart1;

extern C_Cylinder cylinder_D;
extern C_Cylinder cylinder_L;

#endif // DEFINE_H
