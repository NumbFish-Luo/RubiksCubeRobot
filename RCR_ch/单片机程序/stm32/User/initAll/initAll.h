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

void InitAll_SteppingMotor(void); // ��ʼ������������°棩
void InitAll_USART(void);     // ����
void InitAll_Cylinder(void);  // ����
void InitAll_Order(void);     // ��ʼ��ָ�
void ClearAll_Order(void);    // ���ָ����ݣ���ֹ�ڴ�й©

#endif // INIT_ALL_H
