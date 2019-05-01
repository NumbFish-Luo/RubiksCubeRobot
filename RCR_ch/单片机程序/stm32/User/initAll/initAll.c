#include "initAll/initAll.h"
#include "order/order.h"
#include "order/Orders.h"

void InitAll_LED() {
//    InitLED(&g_LED_R);
//    InitLED(&g_LED_G);
//    InitLED(&g_LED_B);

//    LED_RGB_OFF();
//    LED_ON(&g_LED_Flag); // ָʾ��Ĭ�Ͽ���
}

void InitAll_Clock() {
    // ��ʱ����ʼ������ܿ��� -- 190501
    // InitClock(&clock);
}

void InitAll_KEY() {
    InitKEY(&key_MSD_Enable);
    InitKEY(&key_MSD_Disable);
//    InitKEY(&key_LMF);
//    InitKEY(&key_LMR);
//    InitKEY(&key_DMF);
//    InitKEY(&key_DMR);
}

void InitAll_USART() {
    InitUsart(&usart1);
}

// ��ʼ������(צ��)
void InitAll_Cylinder() {
    InitCylinder(&cylinder_D);
    InitCylinder(&cylinder_L);
}

// ��ʼ���������
void InitAll_MSD() {
    InitMSD(&MSD_D);
    InitMSD(&MSD_L);
}

// ��ʼ�����
void InitAll_Order() {
    #define OPB(_) g_orders.PushBack(&g_orders, NewOrder##_().base)
    
    g_orders = NewVectorOrder();
    // MPB(ERR); // �������ERRָ��
    OPB(LMA); OPB(LMS); OPB(LMF); OPB(LMR); OPB(LM2);
    OPB(DMA); OPB(DMS); OPB(DMF); OPB(DMR); OPB(DM2);
    OPB(LCA); OPB(LCS); OPB(DCA); OPB(DCS); OPB(ACA); OPB(ACS);
    OPB(STA); OPB(END); OPB(RES);
}

// ���ָ����ݣ���ֹ�ڴ�й©
void ClearAll_Order() {
    g_orders.Clear(&g_orders);
}
