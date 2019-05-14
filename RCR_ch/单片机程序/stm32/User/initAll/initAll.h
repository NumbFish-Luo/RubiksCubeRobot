#ifndef INIT_ALL_H
#define INIT_ALL_H

#include "config/config.h"
#include "usart/usart.h"
#include "cylinder/cylinder.h"
#include "IO/IO.h"
#include "SteppingMotor/SteppingMotor.h"

extern IO            motor_L_Plu;
extern IO            motor_L_Dir;
extern IO            motor_L_Ena;
extern IO            motor_D_Plu;
extern IO            motor_D_Dir;
extern IO            motor_D_Ena;
extern SteppingMotor motor_L;
extern SteppingMotor motor_D;

void InitAll_SteppingMotor(void); // 初始化步进电机（新版）
void InitAll_USART(void);     // 串口
void InitAll_Cylinder(void);  // 气缸
void InitAll_Order(void);     // 初始化指令集
void ClearAll_Order(void);    // 清除指令集数据，防止内存泄漏

#endif // INIT_ALL_H
