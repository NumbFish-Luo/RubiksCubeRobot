#ifndef INIT_ALL_H
#define INIT_ALL_H

#include "config/config.h"
#include "key/key.h"
#include "led/led.h"
#include "MSD/MicroStepDriver.h"
#include "usart/usart.h"
#include "cylinder/cylinder.h"

void InitAll_LED(void);      // LED
void InitAll_KEY(void);      // 按键
void InitAll_USART(void);    // 串口
void InitAll_Cylinder(void); // 气缸
void InitAll_MSD(void);      // 步进电机
void InitAll_Order(void);    // 初始化指令集
void ClearAll_Order(void);   // 清除指令集数据，防止内存泄漏

#endif // INIT_ALL_H
