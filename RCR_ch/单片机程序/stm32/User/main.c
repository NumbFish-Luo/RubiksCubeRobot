#include "DealSerialData/DealSerialData.h"
#include "initAll/initAll.h"

void InitAll() {
    InitAll_LED();      // ��ʼ��LED
    InitAll_KEY();      // ��ʼ������
    InitAll_USART();    // ��ʼ������
    InitAll_Cylinder(); // ���׳�ʼ��
    InitAll_MSD();      // ���������ʼ��
    InitAll_Order();    // ��ʼ��ָ�
}

void Clear() {
    ClearAll_Order(); // ���ָ����ݣ���ֹ�ڴ�й©
}

int main() {
    InitAll();
    while(1) { DealSerialData(); } // ����������
}
