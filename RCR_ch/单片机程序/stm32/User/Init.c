#include "Init.h"
#include "Orders.h"
#include "stm32f4xx_it.h"

// 步进电机
SteppingMotor motor_L;
SteppingMotor motor_D;
Claw          claw_L;
Claw          claw_D;
SerialPort    serialPort;

// 初始化步进电机
void Init_StepMotor() {
    IO motor_L_Plu = NewIO(GPIOB, GPIO_Pin_13, RCC_AHB1Periph_GPIOB, GPIO_Speed_50MHz);
    IO motor_L_Dir = NewIO(GPIOC, GPIO_Pin_6,  RCC_AHB1Periph_GPIOC, GPIO_Speed_50MHz);
    IO motor_L_Ena = NewIO(GPIOB, GPIO_Pin_15, RCC_AHB1Periph_GPIOB, GPIO_Speed_50MHz);
    IO motor_D_Plu = NewIO(GPIOA, GPIO_Pin_3,  RCC_AHB1Periph_GPIOA, GPIO_Speed_50MHz);
    IO motor_D_Dir = NewIO(GPIOC, GPIO_Pin_0,  RCC_AHB1Periph_GPIOC, GPIO_Speed_50MHz);
    IO motor_D_Ena = NewIO(GPIOB, GPIO_Pin_14, RCC_AHB1Periph_GPIOB, GPIO_Speed_50MHz);
    motor_L = NewSteppingMotor(motor_L_Plu, motor_L_Ena, motor_L_Dir);
    motor_D = NewSteppingMotor(motor_D_Plu, motor_D_Ena, motor_D_Dir);
}

void Init_SerialPort() {
    serialPort = NewSerialPort(
        USART1, GPIO_AF_USART1, RCC_APB2Periph_USART1, 115200,
        USART1_IRQn, USART1_IRQHandler,
        GPIOA, GPIO_Pin_10, RCC_AHB1Periph_GPIOA, GPIO_PinSource10,
        GPIOA, GPIO_Pin_9,  RCC_AHB1Periph_GPIOA, GPIO_PinSource9);
}

// 初始化爪子
void Init_Claw() {
    claw_L = NewClaw(GPIOA, GPIO_Pin_4, RCC_AHB1Periph_GPIOD, GPIO_Speed_2MHz);
    claw_D = NewClaw(GPIOD, GPIO_Pin_6, RCC_AHB1Periph_GPIOD, GPIO_Speed_2MHz);
}

// 初始化命令集
void Init_Orders() {
    #define OPB(_) g_orders.PushBack(&g_orders, NewOrder##_().base)
    
    g_orders = NewVectorOrder();
    // OPB(ERR); // 无需加载ERR指令
    OPB(LMA); OPB(LMS); OPB(LMF); OPB(LMR); OPB(LM2); OPB(LMT);
    OPB(DMA); OPB(DMS); OPB(DMF); OPB(DMR); OPB(DM2); OPB(DMT);
    OPB(LCA); OPB(LCS); OPB(DCA); OPB(DCS); OPB(ACA); OPB(ACS);
    
    #undef OPB
}

// 清除指令集数据，防止内存泄漏
void Clear_Orders() {
    g_orders.Clear(&g_orders);
}
