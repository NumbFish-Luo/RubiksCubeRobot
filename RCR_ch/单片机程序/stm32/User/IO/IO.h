#ifndef IO_H
#define IO_H

#include "typedefine.h"

typedef struct IO IO;
struct IO {
    GPIO_Type     GPIO;     // GPIOx
    Pin_Type      Pin;      // Pin_x
    CLK_Type      GPIO_CLK; // RCC_AHB1Periph_GPIOx
    void(*ON)     (IO* io);
    void(*OFF)    (IO* io);
    void(*Toggle) (IO* io);
    void(*Set)    (IO* io, Bool on);
    Bool(*Read)   (IO* io);
};
IO NewIO(GPIO_Type gpio, Pin_Type pin, CLK_Type clk, GPIOSpeed_TypeDef speed);

#endif // IO_H
