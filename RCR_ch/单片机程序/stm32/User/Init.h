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

void Init_StepMotor(void);  // ��ʼ������������°棩
void Init_SerialPort(void); // ����
void Init_Claw(void);       // ����
void Init_Orders(void);     // ��ʼ��ָ�
void Clear_Orders(void);    // ���ָ����ݣ���ֹ�ڴ�й©

#endif // INIT_H
