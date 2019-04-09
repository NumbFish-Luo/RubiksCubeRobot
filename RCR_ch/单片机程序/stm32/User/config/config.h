#ifndef DEFINE_H
#define DEFINE_H

#include "led/led.h"
#include "key/key.h"
#include "MSD/MicroStepDriver.h"
#include "cylinder/cylinder.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_tim.h"
#include "usart/usart.h"

// ############## Cylinder #############
#define CYLINDER_DELAY_TIME_CLAMP  (uint32_t)(3000000)
#define CYLINDER_DELAY_TIME_LOOSEN (uint32_t)(2500000)

// ################ MSD ################
#define TRUN_90      5000
#define TRUN_180     10000
#define TRUN_TRIM    25

#define TRUN_90_L    TRUN_90 // steps
#define TRUN_90_D    TRUN_90 // steps

#define TRUN_90S_L  -TRUN_90 // steps
#define TRUN_90S_D  -TRUN_90 // steps

#define ANGLE_L_MIN -270
#define ANGLE_L_MAX  90
#define ANGLE_D_MIN -90
#define ANGLE_D_MAX  270

__REDEFINITION__ extern C_LED g_LED_R;
__REDEFINITION__ extern C_LED g_LED_G;
__REDEFINITION__ extern C_LED g_LED_B;
extern C_LED g_LED_Flag;

extern const C_Key key_MSD_Enable;
extern const C_Key key_MSD_Disable;

__REDEFINITION__ extern C_USART usart1;

__REDEFINITION__ extern C_Cylinder cylinder_D;
__REDEFINITION__ extern C_Cylinder cylinder_L;

extern C_MSD MSD_D;
extern C_MSD MSD_L;

extern unsigned int delayTime_180[TRUN_180];
extern unsigned int delayTime_90[TRUN_90];
extern unsigned int delayTime_trim[TRUN_TRIM];
extern unsigned int delayTime_Idx;

#endif // DEFINE_H
