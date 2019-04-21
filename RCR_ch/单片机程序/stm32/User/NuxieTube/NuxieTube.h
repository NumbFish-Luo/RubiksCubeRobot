#ifndef NUXIE_TUBE_H
#define NUXIE_TUBE_H

#include "stm32f4xx.h"
#include "typeDefine.h"
#include "led/led.h"

typedef C_LED C_Bit;

extern const unsigned char SHOW[25];

typedef struct C_NuxieTube { // 数码管
    C_LED led[8]; // a b c d e f g dp
    C_Bit bit[3]; // A B C
    int  data[9];
    int    dpPos;
} C_NuxieTube;

void InitNuxieTube(const C_NuxieTube* nt);                    // 初始化数码管
void SetNuxieTube(C_NuxieTube* nt, int number[8], int dpPos); // 设置数码管显示数字和小数点位置
void ShowNuxieTube(const C_NuxieTube* nt);                    // 显示数码管

#endif // NUXIE_TUBE_H
