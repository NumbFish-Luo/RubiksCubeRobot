#include "config/config.h"

// #################################### led ####################################
C_LED g_LED_R    = { GPIOH, GPIO_Pin_10, RCC_AHB1Periph_GPIOH }; // PH10
C_LED g_LED_G    = { GPIOH, GPIO_Pin_11, RCC_AHB1Periph_GPIOH }; // PH11
C_LED g_LED_B    = { GPIOH, GPIO_Pin_12, RCC_AHB1Periph_GPIOH }; // PH12
C_LED g_LED_Flag = { GPIOD, GPIO_Pin_10, RCC_AHB1Periph_GPIOD }; // PD10

// ################################### clock ###################################
C_Clock clock = {
    .nuxieTube =
    {   // a b c d e f g dp
        {
            { GPIOH, GPIO_Pin_2,  RCC_AHB1Periph_GPIOH }, // a  PH2
            { GPIOH, GPIO_Pin_8,  RCC_AHB1Periph_GPIOH }, // b  PH8
            { GPIOA, GPIO_Pin_11, RCC_AHB1Periph_GPIOA }, // c  PA11
            { GPIOB, GPIO_Pin_1,  RCC_AHB1Periph_GPIOB }, // d  PB1
            { GPIOE, GPIO_Pin_5,  RCC_AHB1Periph_GPIOE }, // e  PE5
            { GPIOH, GPIO_Pin_13, RCC_AHB1Periph_GPIOH }, // f  PH13
            { GPIOH, GPIO_Pin_15, RCC_AHB1Periph_GPIOH }, // g  PH15
            { GPIOC, GPIO_Pin_7,  RCC_AHB1Periph_GPIOC }, // dp PC7
        },
        // A B C
        {
            { GPIOH, GPIO_Pin_3,  RCC_AHB1Periph_GPIOH }, // A  PH3
            { GPIOB, GPIO_Pin_0,  RCC_AHB1Periph_GPIOB }, // B  PB0
            { GPIOA, GPIO_Pin_12, RCC_AHB1Periph_GPIOA }, // C  PA12
        }
    },
    .TIM = TIM1,
    .TIM_CLK = RCC_APB2Periph_TIM1,
    .TIM_IRQn = TIM1_UP_TIM10_IRQn,
    .IRQHandler = TIM1_UP_TIM10_IRQHandler,
    .Pulse_APB_CLK_FUN = RCC_APB2PeriphClockCmd,
    .count = 0,
    .Start = 0
};

// #################################### key ####################################
const C_Key key_MSD_Enable =   // PA0, EXIT0
{
    .GPIO     = GPIOA,
    .Pin      = GPIO_Pin_0,
    .GPIO_CLK = RCC_AHB1Periph_GPIOA,

    .EXTI_GPIOSource = EXTI_PortSourceGPIOA,
    .EXTI_PinSource  = EXTI_PinSource0,
    .EXTI_Line       = EXTI_Line0,

    .EXTI_IRQn       = EXTI0_IRQn,
    .EXTI_IRQHandler = EXTI0_IRQHandler,
    .trigger         = EXTI_Trigger_Rising,
    
    .preemptionPriority = 1,
    .subPriority = 2
};

const C_Key key_MSD_Disable =   // PC13, EXIT15_10
{
    .GPIO     = GPIOC,
    .Pin      = GPIO_Pin_13,
    .GPIO_CLK = RCC_AHB1Periph_GPIOC,

    .EXTI_GPIOSource = EXTI_PortSourceGPIOC,
    .EXTI_PinSource  = EXTI_PinSource13,
    .EXTI_Line       = EXTI_Line13,

    .EXTI_IRQn       = EXTI15_10_IRQn,
    .EXTI_IRQHandler = EXTI15_10_IRQHandler,
    .trigger         = EXTI_Trigger_Rising,
    
    .preemptionPriority = 2,
    .subPriority = 2
};

// const C_Key key_LMF = { // PI2
//     .GPIO     = GPIOI,
//     .Pin      = GPIO_Pin_2,
//     .GPIO_CLK = RCC_AHB1Periph_GPIOI,
   
//     .EXTI_GPIOSource = EXTI_PortSourceGPIOI,
//     .EXTI_PinSource  = EXTI_PinSource2,
//     .EXTI_Line       = EXTI_Line2,
   
//     .EXTI_IRQn       = EXTI2_IRQn,
//     
//     .EXTI_IRQHandler = EXTI2_IRQHandler,
//     .trigger         = EXTI_Trigger_Falling,
//     
//     .preemptionPriority = 3,
//     .subPriority = 2
// };
   
// const C_Key key_LMR = { // PA3
//     .GPIO     = GPIOA,
//     .Pin      = GPIO_Pin_3,
//     .GPIO_CLK = RCC_AHB1Periph_GPIOA,
   
//     .EXTI_GPIOSource = EXTI_PortSourceGPIOA,
//     .EXTI_PinSource  = EXTI_PinSource3,
//     .EXTI_Line       = EXTI_Line3,
   
//     .EXTI_IRQn       = EXTI3_IRQn,
//     
//     .EXTI_IRQHandler = EXTI3_IRQHandler,
//     .trigger         = EXTI_Trigger_Falling,
//     
//     .preemptionPriority = 4,
//     .subPriority = 2
// };
   
// const C_Key key_DMF = { // PI4
//     .GPIO     = GPIOA,
//     .Pin      = GPIO_Pin_4,
//     .GPIO_CLK = RCC_AHB1Periph_GPIOA,
   
//     .EXTI_GPIOSource = EXTI_PortSourceGPIOA,
//     .EXTI_PinSource  = EXTI_PinSource4,
//     .EXTI_Line       = EXTI_Line4,
   
//     .EXTI_IRQn       = EXTI4_IRQn,
//     
//     .EXTI_IRQHandler = EXTI4_IRQHandler,
//     .trigger         = EXTI_Trigger_Falling,
//     
//     .preemptionPriority = 5,
//     .subPriority = 2
// };
   
// const C_Key key_DMR = { // PB9
//     .GPIO     = GPIOB,
//     .Pin      = GPIO_Pin_9,
//     .GPIO_CLK = RCC_AHB1Periph_GPIOB,
   
//     .EXTI_GPIOSource = EXTI_PortSourceGPIOB,
//     .EXTI_PinSource  = EXTI_PinSource9,
//     .EXTI_Line       = EXTI_Line9,
   
//     .EXTI_IRQn       = EXTI9_5_IRQn,
//     
//     .EXTI_IRQHandler = EXTI9_5_IRQHandler,
//     .trigger         = EXTI_Trigger_Falling,
//     
//     .preemptionPriority = 6,
//     .subPriority = 2
// };

// ################################### usart ###################################
C_USART usart1 =   // USART1, 115200, Rx: PA10, Tx: PA9
{
    .USART         = USART1,
    .GPIO_AF_USART = GPIO_AF_USART1,
    .USART_CLK     = RCC_APB2Periph_USART1,
    .Baudrate      = 115200,

    .RX_GPIO      = GPIOA,
    .RX_Pin       = GPIO_Pin_10,
    .RX_GPIO_CLK  = RCC_AHB1Periph_GPIOA,
    .RX_PinSource = GPIO_PinSource10,

    .TX_GPIO      = GPIOA,
    .TX_Pin       = GPIO_Pin_9,
    .TX_GPIO_CLK  = RCC_AHB1Periph_GPIOA,
    .TX_PinSource = GPIO_PinSource9,

    .USART_IRQn       = USART1_IRQn,
    .USART_IRQHandler = USART1_IRQHandler,

    .RxPtr = 0
};

// #################################### cylinder ###############################
C_Cylinder cylinder_D = { GPIOA, GPIO_Pin_4, RCC_AHB1Periph_GPIOD };
C_Cylinder cylinder_L = { GPIOD, GPIO_Pin_6, RCC_AHB1Periph_GPIOD };

// #################################### MSD ####################################
C_MSD MSD_D =   // Pulse: PA3 TIM2 OC4, Dir: PC0; Ena: PB14
{
    // Pulse_TIM: TIM2
    .Pulse_TIM         = TIM2,
    .Pulse_APB_CLK_FUN = RCC_APB1PeriphClockCmd,
    .Pulse_TIM_CLK     = RCC_APB1Periph_TIM2,
    // Pulse_OC: OC4
    .Pulse_OC_Init           = TIM_OC4Init,
    .Pulse_OC_Preload_Config = TIM_OC4PreloadConfig,
    .TIM_SetCompare          = TIM_SetCompare4,
    // Pulse_TIM: TIM2
    .Pulse_TIM_IRQn       = TIM2_IRQn,
    .Pulse_TIM_IRQHandler = TIM2_IRQHandler,
    // Pulse: PA3, TIM2
    .Pulse_AHB_CLK_FUN = RCC_AHB1PeriphClockCmd,
    .Pulse_GPIO_CLK    = RCC_AHB1Periph_GPIOA,
    .Pulse_GPIO        = GPIOA,      // GPIOB
    .Pulse_Pin         = GPIO_Pin_3, // GPIO_Pin_11
    .Pulse_AF_TIM      = GPIO_AF_TIM2,
    .Pulse_Pin_Source  = GPIO_PinSource3,
    // Dir: PC0
    .Dir_AHB_CLK_FUN = RCC_AHB1PeriphClockCmd,
    .Dir_GPIO_CLK    = RCC_AHB1Periph_GPIOC,
    .Dir_GPIO        = GPIOC,
    .Dir_Pin         = GPIO_Pin_0,
    // Ena: PB14
    .Ena_AHB_CLK_FUN = RCC_AHB1PeriphClockCmd,
    .Ena_GPIO_CLK    = RCC_AHB1Periph_GPIOB,
    .Ena_GPIO        = GPIOB,
    .Ena_Pin         = GPIO_Pin_14,

    .status       = { 0, 1, 1 },
    .steps        = 5000,
    .angle        = 0
};

C_MSD MSD_L =
{
    // Pulse_TIM: TIM3
    .Pulse_TIM         = TIM3,
    .Pulse_APB_CLK_FUN = RCC_APB1PeriphClockCmd,
    .Pulse_TIM_CLK     = RCC_APB1Periph_TIM3,
    // Pulse_OC: OC4
    .Pulse_OC_Init           = TIM_OC4Init,
    .Pulse_OC_Preload_Config = TIM_OC4PreloadConfig,
    .TIM_SetCompare          = TIM_SetCompare4,
    // Pulse_TIM: TIM3
    .Pulse_TIM_IRQn       = TIM3_IRQn,
    .Pulse_TIM_IRQHandler = TIM3_IRQHandler,
    // Pulse: PC9, TIM3
    .Pulse_AHB_CLK_FUN = RCC_AHB1PeriphClockCmd,
    .Pulse_GPIO_CLK    = RCC_AHB1Periph_GPIOC,
    .Pulse_GPIO        = GPIOC,
    .Pulse_Pin         = GPIO_Pin_9,
    .Pulse_AF_TIM      = GPIO_AF_TIM3,
    .Pulse_Pin_Source  = GPIO_PinSource9,
    // Dir: PC6
    .Dir_AHB_CLK_FUN = RCC_AHB1PeriphClockCmd,
    .Dir_GPIO_CLK    = RCC_AHB1Periph_GPIOC,
    .Dir_GPIO        = GPIOC,
    .Dir_Pin         = GPIO_Pin_6,
    // Ena: PB15
    .Ena_AHB_CLK_FUN = RCC_AHB1PeriphClockCmd,
    .Ena_GPIO_CLK    = RCC_AHB1Periph_GPIOB,
    .Ena_GPIO        = GPIOB,
    .Ena_Pin         = GPIO_Pin_15,

    .status       = { 0, 1, 1 },
    .steps        = 5000,
    .angle        = 0
};
