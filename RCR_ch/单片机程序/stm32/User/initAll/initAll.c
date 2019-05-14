#include "initAll/initAll.h"
#include "order/order.h"
#include "order/Orders.h"

// �������
IO            motor_L_Plu;
IO            motor_L_Dir;
IO            motor_L_Ena;
IO            motor_D_Plu;
IO            motor_D_Dir;
IO            motor_D_Ena;
SteppingMotor motor_L;
SteppingMotor motor_D;

void InitAll_SteppingMotor() {
    motor_L_Plu = NewIO(GPIOC, GPIO_Pin_9,  RCC_AHB1Periph_GPIOC, GPIO_Speed_50MHz);
    motor_L_Dir = NewIO(GPIOC, GPIO_Pin_6,  RCC_AHB1Periph_GPIOC, GPIO_Speed_50MHz);
    motor_L_Ena = NewIO(GPIOB, GPIO_Pin_15, RCC_AHB1Periph_GPIOB, GPIO_Speed_50MHz);
    motor_D_Plu = NewIO(GPIOA, GPIO_Pin_3,  RCC_AHB1Periph_GPIOA, GPIO_Speed_50MHz);
    motor_D_Dir = NewIO(GPIOC, GPIO_Pin_0,  RCC_AHB1Periph_GPIOC, GPIO_Speed_50MHz);
    motor_D_Ena = NewIO(GPIOB, GPIO_Pin_14, RCC_AHB1Periph_GPIOB, GPIO_Speed_50MHz);
    motor_L     = NewSteppingMotor(motor_L_Plu, motor_L_Ena, motor_L_Dir);
    motor_D     = NewSteppingMotor(motor_L_Plu, motor_L_Ena, motor_L_Dir);
}

void InitAll_USART() {
    InitUsart(&usart1);
}

// ��ʼ������(צ��)
void InitAll_Cylinder() {
    InitCylinder(&cylinder_D);
    InitCylinder(&cylinder_L);
}

// ��ʼ�����
void InitAll_Order() {
    #define OPB(_) g_orders.PushBack(&g_orders, NewOrder##_().base)
    
    g_orders = NewVectorOrder();
    // MPB(ERR); // �������ERRָ��
    OPB(LMA); OPB(LMS); OPB(LMF); OPB(LMR); OPB(LM2); OPB(LMT);
    OPB(DMA); OPB(DMS); OPB(DMF); OPB(DMR); OPB(DM2); OPB(DMT);
    OPB(LCA); OPB(LCS); OPB(DCA); OPB(DCS); OPB(ACA); OPB(ACS);
}

// ���ָ����ݣ���ֹ�ڴ�й©
void ClearAll_Order() {
    g_orders.Clear(&g_orders);
}
