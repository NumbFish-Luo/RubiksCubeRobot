#include <string.h>
#include "Init.h"
#include "order.h"
#include "Orders.h"
#include "SerialPort.h"

void Init() {
    Init_SerialPort();         // ��ʼ������
    Init_Claw();          // ���׳�ʼ��
    Init_StepMotor(); // ��ʼ���������
    Init_Orders();         // ��ʼ��ָ�
}

void Clear() {
    Clear_Orders(); // ���ָ����ݣ���ֹ�ڴ�й©
}

void DealSerialData() {
    char        orderName[4]  = { 0 };
    char const* buffer        = (char const*)serialPort.buffer;
    int         len           = strlen(buffer);
    size_t      idx           = 0; // ָ���ַ����±��ʼ��Ϊ0
    Bool        okCmd         = 0;
    Invoker     ivk           = NewInvoker();
    
    if (serialPort.cmdOK == 1) {
        // ÿ��ָ�ռ3���ַ�, ָ����ָ��֮���Կո���, ���һ��ָ����'\0'����
        if (len >= 3) {
            printf(" \n");
            while (idx + 2 <= len) { // ���±�idx + 2δ�����ַ�������len, ��һֱѭ�����ָ��
                orderName[0] = buffer[idx];
                orderName[1] = buffer[idx + 1];
                orderName[2] = buffer[idx + 2];
                orderName[3] = '\0';
    
                ivk.SetOrder(&ivk, g_orders.Find(&g_orders, orderName));
                ivk.DoOrder(&ivk);
                if (strcmp(ivk.order.name, "ERR") == 0) {
                    okCmd = 0;
                    break;
                }
                okCmd = 1;
                idx += 3; // �±�������һ��ָ��
                printf("\n");
            }
            if (okCmd == 0) {
                printf("\n[Error!]\n");
            } else {
                printf("[OK!]\n");
            }
            serialPort.cmdOK = 0;
            serialPort.Flush(&serialPort);
        }
    }
    ivk.Clear(&ivk);
}

void Loop() {
    while(1) {
        DealSerialData(); // ����������
    }
}

int main() {
    Init();
    Loop();
    Clear();
}
