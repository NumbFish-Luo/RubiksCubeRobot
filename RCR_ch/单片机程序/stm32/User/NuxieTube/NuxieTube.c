#include "NuxieTube/NuxieTube.h"

const unsigned char SHOW[25] = {
    // 0,    1,    2,    3,    4,    5,    6,    7,
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    // 8,    9,    A,    B,    C,    D,    E,    F,
    0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E,
    // H,    L,    n,    u,    P,    o,    -, Ï¨Ãð,
    0x89, 0xC7, 0xC8, 0xC1, 0x8C, 0xA3, 0xBF, 0xFF,
    // ×Ô¶¨Òå
    0xFF
};

void InitBit(const C_Bit* bit) {
    InitLED(bit);
}

void Bit_OFF(const C_Bit* bit) {
    LED_ON(bit);
}

void Bit_ON(const C_Bit* bit) {
    LED_OFF(bit);
}

void SetBit(const C_Bit* bit, Bool value) {
    value ? LED_ON(bit) : LED_OFF(bit);
}

void InitNuxieTube(const C_NuxieTube* nt) {
    int i = 0;
    for (i = 0; i < 8; ++i) {
        InitLED(&(nt->led[i]));
        Bit_ON(&(nt->led[i]));
    }
    for (i = 0; i < 3; ++i) {
        InitBit(&(nt->bit[i]));
        Bit_OFF(&(nt->bit[i]));
    }
}

void SetNuxieTube(C_NuxieTube* nt, int number[8], int dpPos) {
    int i = 0;
    for (i = 0; i < 8; ++i) {
        nt->data[i] = number[i];
    }
    nt->dpPos = dpPos;
}

void ShowNuxieTube(const C_NuxieTube* nt) {
    static unsigned char now = 0;
    int i = 0;
    for (i = 0; i < 3; ++i) {
        SetBit(&(nt->bit[i]), (now & (0x01 << i)));
    }
    for (i = 0; i < 7; ++i) {
        SetBit(&(nt->led[i]), (SHOW[nt->data[now]] & (0x01 << i)));
    }
    SetBit(&(nt->led[7]), !(nt->dpPos == now));
    if (++now >= 8) { now = 0; }
}
