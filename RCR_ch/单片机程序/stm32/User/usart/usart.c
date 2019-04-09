#include "usart/usart.h"

__HIDE_METHOD__ void Usart_NVIC_Config(const C_USART* usart) {
    NVIC_InitTypeDef n;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);          // Group2
    n.NVIC_IRQChannel                   = usart->USART_IRQn; // ����USARTΪ�ж�Դ
    n.NVIC_IRQChannelPreemptionPriority = 1;                 // �������ȼ�Ϊ1
    n.NVIC_IRQChannelSubPriority        = 3;                 // �����ȼ�Ϊ3
    n.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&n);
}

// DEBUG_USART GPIO����, ����ģʽ����. 115200 8-N-1, �жϽ���ģʽ
__HIDE_METHOD__ void Usart_Config(const C_USART* usart) {
    GPIO_InitTypeDef  g;
    USART_InitTypeDef u;

    RCC_AHB1PeriphClockCmd(usart->RX_GPIO_CLK | usart->TX_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(usart->USART_CLK, ENABLE);

    g.GPIO_OType = GPIO_OType_PP;
    g.GPIO_PuPd  = GPIO_PuPd_UP;
    g.GPIO_Speed = GPIO_Speed_50MHz;

    // ����Tx����Ϊ���ù���
    /* ���ù��ܾ������ó�оƬԤ���ģ�鹦�ܣ�����ĳ���ܽ�оƬ���óɴ��ڣ�
       ���뽫����ܽ���Ϊ�����ã������óɸ��ù��ܣ�
       ���ֻ����ΪGPIO�ã������ó�OUTģʽ */
    g.GPIO_Mode = GPIO_Mode_AF;
    g.GPIO_Pin  = usart->TX_Pin;
    GPIO_Init(usart->TX_GPIO, &g);

    // ����Rx����Ϊ���ù���
    g.GPIO_Mode = GPIO_Mode_AF;
    g.GPIO_Pin  = usart->RX_Pin;
    GPIO_Init(usart->RX_GPIO, &g);

    GPIO_PinAFConfig(usart->RX_GPIO, usart->RX_PinSource, usart->GPIO_AF_USART);
    GPIO_PinAFConfig(usart->TX_GPIO, usart->TX_PinSource, usart->GPIO_AF_USART);

    // ���ô�DEBUG_USARTģʽ
    u.USART_BaudRate            = usart->Baudrate;                // ���������ã�DEBUG_USART_BAUDRATE
    u.USART_WordLength          = USART_WordLength_8b;            // �ֳ�(����λ+У��λ)��8
    u.USART_StopBits            = USART_StopBits_1;               // ֹͣλ��1��ֹͣλ
    u.USART_Parity              = USART_Parity_No;                // У��λѡ�񣺲�ʹ��У��
    u.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // Ӳ�������ƣ���ʹ��Ӳ����
    u.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  // USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ���
    USART_Init(usart->USART, &u);

    Usart_NVIC_Config(usart);

    USART_ITConfig(usart->USART, USART_IT_RXNE, ENABLE);          // ʹ�ܴ��ڽ����ж�
    USART_Cmd(usart->USART, ENABLE);                              // ʹ�ܴ���
}

// �ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
// https://blog.csdn.net/dragon12345666/article/details/24940395
__NOTICE__
__HIDE_METHOD__ int fputc(int ch, FILE* f) {
    C_USART* usart = &usart1;

    USART_SendData(usart->USART, (uint8_t)ch);                          // ����һ���ֽ����ݵ�����
    while (USART_GetFlagStatus(usart->USART, USART_FLAG_TXE) == RESET); // �ȴ��������
    return ch;
}

// �ض���c�⺯��scanf�����ڣ���д����ʹ��scanf, getchar�Ⱥ���
__NOTICE__
__HIDE_METHOD__ int fgetc(FILE* f) {
    C_USART* usart = &usart1;

    while (USART_GetFlagStatus(usart->USART, USART_FLAG_RXNE) == RESET); // �ȴ�������������
    return (int)USART_ReceiveData(usart->USART);
}

// ����һ���ַ�
void Usart_SendByte(USART_Type pUSART, uint8_t ch) {
    USART_SendData(pUSART, ch);                                   // ����һ���ֽ����ݵ�USART
    while (USART_GetFlagStatus(pUSART, USART_FLAG_TXE) == RESET); // �ȴ��������ݼĴ���Ϊ��
}

// �����ַ���
void Usart_SendString(USART_Type pUSART, char* str) {
    unsigned int k = 0;
    do {
        Usart_SendByte(pUSART, *(str + k));
        k++;
    }
    while (*(str + k) != '\0'); // '\0'��ʾһ���ַ����Ľ���

    while (USART_GetFlagStatus(pUSART, USART_FLAG_TC) == RESET);
}

// ����һ��16λ��
void Usart_SendHalfWord(USART_Type pUSART, uint16_t ch) {
    uint8_t temp_H, temp_L;

    temp_H = (ch & 0XFF00) >> 8; // ȡ���߰�λ
    temp_L = ch & 0XFF;          // ȡ���Ͱ�λ

    USART_SendData(pUSART, temp_H); // ���͸߰�λ
    while (USART_GetFlagStatus(pUSART, USART_FLAG_TXE) == RESET);

    USART_SendData(pUSART, temp_L); // ���͵Ͱ�λ
    while (USART_GetFlagStatus(pUSART, USART_FLAG_TXE) == RESET);
}

// ��շ��ͻ���
void Usart_FlushRxBuffer(C_USART* usart) {
    usart->RxPtr = 0;
    usart->RxBuffer[usart->RxPtr] = 0;
}

void InitUsart(const C_USART* usart) {
    Usart_Config(usart);
}
