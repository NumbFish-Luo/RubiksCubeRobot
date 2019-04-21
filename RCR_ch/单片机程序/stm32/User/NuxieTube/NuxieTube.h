#ifndef NUXIE_TUBE_H
#define NUXIE_TUBE_H

#include "stm32f4xx.h"
#include "typeDefine.h"
#include "led/led.h"

typedef C_LED C_Bit;

extern const unsigned char SHOW[25];

typedef struct C_NuxieTube { // �����
    C_LED led[8]; // a b c d e f g dp
    C_Bit bit[3]; // A B C
    int  data[9];
    int    dpPos;
} C_NuxieTube;

void InitNuxieTube(const C_NuxieTube* nt);                    // ��ʼ�������
void SetNuxieTube(C_NuxieTube* nt, int number[8], int dpPos); // �����������ʾ���ֺ�С����λ��
void ShowNuxieTube(const C_NuxieTube* nt);                    // ��ʾ�����

#endif // NUXIE_TUBE_H
