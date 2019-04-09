#include "DealSerialData/DealSerialData.h"
#include "initAll/initAll.h"

void InitAll() {
    InitAll_LED();      // 初始化LED
    InitAll_KEY();      // 初始化按键
    InitAll_USART();    // 初始化串口
    InitAll_Cylinder(); // 气缸初始化
    InitAll_MSD();      // 步进电机初始化
    InitAll_Order();    // 初始化指令集
}

void Clear() {
    ClearAll_Order(); // 清除指令集数据，防止内存泄漏
}

int main() {
    InitAll();
    while(1) { DealSerialData(); } // 处理串口数据
}
