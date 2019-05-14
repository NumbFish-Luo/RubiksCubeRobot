#include "IO/IO.h"

void ON(IO* io) {
    io->GPIO->BSRRL = io->Pin;
}

void OFF(IO* io) {
    io->GPIO->BSRRH = io->Pin;
}

void Toggle(IO* io) {
    io->GPIO->ODR ^= io->Pin;
}

void Set(IO* io, Bool on) {
    on ? ON(io) : OFF(io);
}

Bool Read(IO* io) {
    return GPIO_ReadInputDataBit(io->GPIO, io->Pin);
}

IO NewIO(GPIO_Type gpio, Pin_Type pin, CLK_Type clk, GPIOSpeed_TypeDef speed) {
    IO io;
    GPIO_InitTypeDef g;
    
    io.GPIO = gpio;
    io.Pin = pin;
    io.GPIO_CLK = clk;
    io.ON = ON;
    io.OFF = OFF;
    io.Toggle = Toggle;
    io.Set = Set;
    io.Read = Read;
    
    RCC_AHB1PeriphClockCmd(io.GPIO_CLK, ENABLE);
    g.GPIO_Pin   = io.Pin;
    g.GPIO_Mode  = GPIO_Mode_OUT;
    g.GPIO_OType = GPIO_OType_PP;
    g.GPIO_PuPd  = GPIO_PuPd_UP; // 上拉, 默认高电平, LED亮
    g.GPIO_Speed = speed;
    GPIO_Init(io.GPIO, &g);
    
    return io;
}
