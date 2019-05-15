#ifndef INIT_H
#define INIT_H

#include "SerialPort.h"
#include "IO.h"
#include "StepMotor.h"

typedef IO Claw;
#define NewClaw NewIO

extern SteppingMotor motor_L;
extern SteppingMotor motor_D;
extern Claw          claw_L;
extern Claw          claw_D;

void Init_StepMotor(void);  // 初始化步进电机（新版）
void Init_SerialPort(void); // 串口
void Init_Claw(void);       // 气缸
void Init_Orders(void);     // 初始化指令集
void Clear_Orders(void);    // 清除指令集数据，防止内存泄漏

#endif // INIT_H
