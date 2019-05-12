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
    size_t  idx           = 0; // ָ���ַ����±��ʼ��Ϊ0
    Bool    okCmd         = FALSE;
    Invoker ivk           = NewInvoker();
    
    // ����Ƿ���յ�ָ��
    if (MSD_D.status.cmd == TRUE || MSD_L.status.cmd == TRUE) {
        // ÿ��ָ�ռ3���ַ�, ָ����ָ��֮���Կո���, ���һ��ָ����'\0'����
        if (len >= 3) {
            printf(" \n");
            while (idx + 2 <= len) { // ���±�idx + 2δ�����ַ�������len, ��һֱѭ�����ָ��
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
                idx += 3; // �±�������һ��ָ��
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

        // ��մ��ڽ��ջ�������
        MSD_D.status.cmd = FALSE;
        MSD_L.status.cmd = FALSE;
        Usart_FlushRxBuffer(&usart1);
    } // end if(cmd)
    
    ivk.Clear(&ivk);
}
