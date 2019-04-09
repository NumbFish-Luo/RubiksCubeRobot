#ifndef MICRO_STEP_DRIVER_H
#define MICRO_STEP_DRIVER_H

#include "stm32f4xx.h"
#include "typeDefine.h"

typedef enum E_Direction { // ����ö��
    CW, // ˳ʱ��
    CCW // ��ʱ��
}E_Direction;

typedef enum E_Bool {
    FALSE = 0,
    TRUE  = 1
}E_Bool;

// ϵͳ״̬
typedef struct C_MSD_Status {
    unsigned char running; // �����������������ʱ, ֵΪTRUE, Ĭ��ΪFALSE
    unsigned char cmd    ; // �����ڽ��յ�����ʱ, ֵΪTURE
    unsigned char out_ena; // ���������������ʱ, ֵΪTRUE
} C_MSD_Status;

typedef struct C_MicroStepDriver {
    TIM_Type      Pulse_TIM;     // ��ʱ��
    void(*Pulse_APB_CLK_FUN)();  // ʱ�Ӻ���
    CLK_Type      Pulse_TIM_CLK; // ʱ��

    void(*Pulse_OC_Init)();
    void(*Pulse_OC_Preload_Config)(TIM_Type TIMx, Preload_Type TIM_OCPreload);
    void(*TIM_SetCompare)(TIM_Type TIMx, PWM_Type Compare2);

    IRQn_Type     Pulse_TIM_IRQn;  // �ж�
    void(*Pulse_TIM_IRQHandler)(); // �жϺ���

    void(*Pulse_AHB_CLK_FUN)();
    CLK_Type      Pulse_GPIO_CLK;
    GPIO_Type     Pulse_GPIO;
    Pin_Type      Pulse_Pin;
    AF_Type       Pulse_AF_TIM;
    Source_Type   Pulse_Pin_Source;

    void(*Dir_AHB_CLK_FUN)(CLK_Type RCC_AHB1Periph, FunctionalState NewState);
    CLK_Type      Dir_GPIO_CLK;
    GPIO_Type     Dir_GPIO;
    Pin_Type      Dir_Pin;

    void(*Ena_AHB_CLK_FUN)(CLK_Type RCC_AHB1Periph, FunctionalState NewState);
    CLK_Type      Ena_GPIO_CLK;
    GPIO_Type     Ena_GPIO;
    Pin_Type      Ena_Pin;

    E_Direction   dir;
    C_MSD_Status  status;
    int           steps;
    int           setAngle;
    int           angle;
} C_MSD;

// PWM �źŵ�Ƶ�� F = TIM_CLK / {(ARR + 1) * (PSC + 1)}
#define MSD_PULSE_TIM_PERIOD (10 - 1)
#define MSD_PULSE_TIM_PSC    (180 - 1)

void InitMSD(const C_MSD*);            // ��ʼ�����
void MSD_ENA(C_MSD*, FunctionalState); // ʹ��/ʧ�ܵ��
void MSD_Move(C_MSD*);                 // ����ƶ����� ��
void MSD_IRQHandler(C_MSD*);           // ����жϺ��� ��
void MSD_waitStop(const C_MSD*);       // �ȴ����ֹͣ

#endif /* MICRO_STEP_DRIVER_H */
