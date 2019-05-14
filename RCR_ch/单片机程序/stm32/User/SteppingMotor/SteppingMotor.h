#ifndef STEPPING_MOTOR
#define STEPPING_MOTOR

#include "IO/IO.h"

// 步进电机旋转角度与步数的对应关系
typedef enum TurnStep {
    TURN_180 = 400,
    TURN_90  = 200,
    TURN_1   =   1
} TurnStep;

// 步进电机，包含三个io口，分别为plu脉冲，ena使能，dir方向。还包含两个方法，旋转和使能。
typedef struct SteppingMotor SteppingMotor;
struct SteppingMotor {
    IO plu;
    IO ena;
    IO dir;
    void(*Turn)(SteppingMotor* motor, int step); // 旋转一定步数，如果step为负，则逆时针旋转
    void(*Enable)(SteppingMotor* motor, Bool enable); // 控制使能
};
SteppingMotor NewSteppingMotor(IO plu, IO ena, IO dir);

#endif // STEPPING_MOTOR
