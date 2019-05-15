#ifndef IO_H
#define IO_H

#include "typedefine.h"

// 简单IO控制
typedef struct IO IO;
struct IO {
    GPIO_Type     GPIO;     // GPIOx
    Pin_Type      Pin;      // Pin_x
    void(*ON)     (IO* io);
    void(*OFF)    (IO* io);
    void(*Toggle) (IO* io); // 反转
    void(*Set)    (IO* io, Bool on); // 设置
    Bool(*Read)   (IO* io);
};
IO NewIO(GPIO_Type gpio, Pin_Type pin, CLK_Type clk, GPIOSpeed_TypeDef speed);

#endif // IO_H
