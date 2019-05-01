#include "initAll/initAll.h"
#include "order/order.h"
#include "order/Orders.h"

void InitAll_LED() {
//    InitLED(&g_LED_R);
//    InitLED(&g_LED_G);
//    InitLED(&g_LED_B);

//    LED_RGB_OFF();
//    LED_ON(&g_LED_Flag); // 指示灯默认开启
}

void InitAll_Clock() {
    // 暂时不初始化数码管看看 -- 190501
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

// 初始化气缸(爪子)
void InitAll_Cylinder() {
    InitCylinder(&cylinder_D);
    InitCylinder(&cylinder_L);
}

// 初始化步进电机
void InitAll_MSD() {
    InitMSD(&MSD_D);
    InitMSD(&MSD_L);
}

// 初始化命令集
void InitAll_Order() {
    #define OPB(_) g_orders.PushBack(&g_orders, NewOrder##_().base)
    
    g_orders = NewVectorOrder();
    // MPB(ERR); // 无需加载ERR指令
    OPB(LMA); OPB(LMS); OPB(LMF); OPB(LMR); OPB(LM2);
    OPB(DMA); OPB(DMS); OPB(DMF); OPB(DMR); OPB(DM2);
    OPB(LCA); OPB(LCS); OPB(DCA); OPB(DCS); OPB(ACA); OPB(ACS);
    OPB(STA); OPB(END); OPB(RES);
}

// 清除指令集数据，防止内存泄漏
void ClearAll_Order() {
    g_orders.Clear(&g_orders);
}
