#include <string.h>
#include "Init.h"
#include "order.h"
#include "Orders.h"
#include "SerialPort.h"

void Init() {
    Init_SerialPort();         // 初始化串口
    Init_Claw();          // 气缸初始化
    Init_StepMotor(); // 初始化步进电机
    Init_Orders();         // 初始化指令集
}

void Clear() {
    Clear_Orders(); // 清除指令集数据，防止内存泄漏
}

void DealSerialData() {
    char        orderName[4]  = { 0 };
    char const* buffer        = (char const*)serialPort.buffer;
    int         len           = strlen(buffer);
    size_t      idx           = 0; // 指令字符串下标初始化为0
    Bool        okCmd         = 0;
    Invoker     ivk           = NewInvoker();
    
    if (serialPort.cmdOK == 1) {
        // 每个指令都占3个字符, 指令与指令之间以空格间隔, 最后一个指令以'\0'结束
        if (len >= 3) {
            printf(" \n");
            while (idx + 2 <= len) { // 若下标idx + 2未超过字符串长度len, 则一直循环检测指令
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
                idx += 3; // 下标移至下一个指令
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
        DealSerialData(); // 处理串口数据
    }
}

int main() {
    Init();
    Loop();
    Clear();
}
