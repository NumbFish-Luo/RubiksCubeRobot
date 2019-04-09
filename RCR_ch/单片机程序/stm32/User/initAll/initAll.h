#ifndef INIT_ALL_H
#define INIT_ALL_H

#include "config/config.h"
#include "key/key.h"
#include "led/led.h"
#include "MSD/MicroStepDriver.h"
#include "usart/usart.h"
#include "cylinder/cylinder.h"

void InitAll_LED(void);      // LED
void InitAll_KEY(void);      // ����
void InitAll_USART(void);    // ����
void InitAll_Cylinder(void); // ����
void InitAll_MSD(void);      // �������
void InitAll_Order(void);    // ��ʼ��ָ�
void ClearAll_Order(void);   // ���ָ����ݣ���ֹ�ڴ�й©

#endif // INIT_ALL_H
