#include "key/key.h"

// NVIC�����ж����ȼ�
__HIDE_METHOD__ void KEY_NVIC_Config(const C_Key* key) {
    NVIC_InitTypeDef n;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);                 // ����NVICΪ���ȼ���1
    n.NVIC_IRQChannel = key->EXTI_IRQn;                             // �����ж�Դ
    n.NVIC_IRQChannelPreemptionPriority = key->preemptionPriority;  // ������ռ���ȼ���1
    n.NVIC_IRQChannelSubPriority = key->subPriority;                // ���������ȼ���2
    n.NVIC_IRQChannelCmd = ENABLE;                                  // ʹ���ж�ͨ��
    NVIC_Init(&n);
}

__HIDE_METHOD__ void KEY_GPIO_Config(const C_Key* key) {
    GPIO_InitTypeDef g;
    RCC_AHB1PeriphClockCmd(key->GPIO_CLK, ENABLE);
    g.GPIO_Pin = key->Pin;
    g.GPIO_Mode = GPIO_Mode_IN;
    g.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(key->GPIO, &g);
}

__HIDE_METHOD__ void KEY_EXTI_Config(const C_Key* key) {
    EXTI_InitTypeDef e;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);           // ʹ��SYSCFGʱ�ӣ�ʹ��GPIO�ⲿ�ж�ʱ����ʹ��SYSCFGʱ��
    KEY_NVIC_Config(key);                                            // ����NVIC, �������ȼ�
    SYSCFG_EXTILineConfig(key->EXTI_PinSource, key->EXTI_PinSource); // ����EXTI�ж�Դ��KEY1����
    e.EXTI_Line = key->EXTI_Line;                                    // ѡ��EXTI�ж�Դ
    e.EXTI_Mode = EXTI_Mode_Interrupt;                               // �ж�ģʽ
    e.EXTI_Trigger = key->trigger;                                   // xx�ش���
    e.EXTI_LineCmd = ENABLE;                                         // ʹ���ж�/�¼���
    EXTI_Init(&e);
}

void InitKEY(const C_Key* key) {
    KEY_GPIO_Config(key);
    KEY_EXTI_Config(key);
}
