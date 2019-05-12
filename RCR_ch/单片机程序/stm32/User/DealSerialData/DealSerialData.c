#include <stdlib.h>
#include <string.h>
#include "config/config.h"
#include "DealSerialData/DealSerialData.h"
#include "order/order.h"
#include "order/Orders.h"

void DealSerialData() {
    static  Bool initFlag = FALSE;
    char    orderName[4]  = { 0 };
    int     len           = strlen((char const*)usart1.RxBuffer);
    size_t  idx           = 0; // 指令字符串下标初始化为0
    Bool    okCmd         = FALSE;
    Invoker ivk           = NewInvoker();
    
    // 检查是否接收到指令
    if (MSD_D.status.cmd == TRUE || MSD_L.status.cmd == TRUE) {
        // 每个指令都占3个字符, 指令与指令之间以空格间隔, 最后一个指令以'\0'结束
        if (len >= 3) {
            printf(" \n");
            while (idx + 2 <= len) { // 若下标idx + 2未超过字符串长度len, 则一直循环检测指令
                orderName[0] = usart1.RxBuffer[idx];
                orderName[1] = usart1.RxBuffer[idx + 1];
                orderName[2] = usart1.RxBuffer[idx + 2];
                orderName[3] = '\0';
                
                ivk.SetOrder(&ivk, g_orders.Find(&g_orders, orderName));
                ivk.DoOrder(&ivk);
                if (strcmp(ivk.order.name, "ERR") == 0) {
                    okCmd = FALSE;
                    break;
                }
                okCmd = TRUE;
                idx += 3; // 下标移至下一个指令
                printf("\n");
            }
        }
        if (initFlag == TRUE && okCmd == FALSE) {
            printf("\n[Error!]\n");
        }
        else {
            initFlag = TRUE;
            printf("[OK!]\n");
        }

        // 清空串口接收缓冲数组
        MSD_D.status.cmd = FALSE;
        MSD_L.status.cmd = FALSE;
        Usart_FlushRxBuffer(&usart1);
    } // end if(cmd)
    
    ivk.Clear(&ivk);
}
