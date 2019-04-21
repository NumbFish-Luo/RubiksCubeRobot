#include "DealSerialData/DealSerialData.h"
#include "initAll/initAll.h"

void InitAll() {
    InitAll_LED();       // ��ʼ��LED
    InitAll_Clock();     // ��ʼ����ʱ��
    InitAll_KEY();       // ��ʼ������
    InitAll_USART();     // ��ʼ������
    InitAll_Cylinder();  // ���׳�ʼ��
    InitAll_MSD();       // ���������ʼ��
    InitAll_Order();     // ��ʼ��ָ�
}

void Clear() {
    ClearAll_Order(); // ���ָ����ݣ���ֹ�ڴ�й©
}

int main() {
//    int show = 0;
    InitAll();
    while(1) {
//        if (++show > 800) { // ��ֹ�������Ϊλѡ�ٶ�̫�쵼��ģ��
//            show = 0;
//            ShowNuxieTube(&clock.nuxieTube); // ��ʾ�����
//        }
        DealSerialData(); // ����������
    }
}
