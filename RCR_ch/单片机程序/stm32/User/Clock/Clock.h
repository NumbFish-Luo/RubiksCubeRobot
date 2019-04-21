#ifndef CLOCK_H
#define CLOCK_H

#include "NuxieTube/NuxieTube.h"

typedef struct C_Clock {
    C_NuxieTube   nuxieTube;    // �����
    TIM_Type      TIM;          // ��ʱ��
    CLK_Type      TIM_CLK;      // ʱ��
    IRQn_Type     TIM_IRQn;     // �жϴ���
    void(*Pulse_APB_CLK_FUN)(); // ʱ�Ӻ���
    void(*IRQHandler)();        // �жϺ���
    int           count;        // ����, ��λΪ�ٷ�֮һ��
    Bool          Start;        // ��ʼ��־
} C_Clock;

void InitClock(C_Clock*);          // ��ʼ��ʱ��
void StartClock(C_Clock*);         // ����
void EndClock(C_Clock*);           // ��������ͣ��
void ResetClock(C_Clock*);         // ���ã����㣩
void Clock_IRQHandler(C_Clock* c); // ʱ���жϺ���

#endif // CLOCK_H
