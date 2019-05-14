#ifndef STEPPING_MOTOR
#define STEPPING_MOTOR

#include "IO/IO.h"

// ���������ת�Ƕ��벽���Ķ�Ӧ��ϵ
typedef enum TurnStep {
    TURN_180 = 400,
    TURN_90  = 200,
    TURN_1   =   1
} TurnStep;

// �����������������io�ڣ��ֱ�Ϊplu���壬enaʹ�ܣ�dir���򡣻�����������������ת��ʹ�ܡ�
typedef struct SteppingMotor SteppingMotor;
struct SteppingMotor {
    IO plu;
    IO ena;
    IO dir;
    void(*Turn)(SteppingMotor* motor, int step); // ��תһ�����������stepΪ��������ʱ����ת
    void(*Enable)(SteppingMotor* motor, Bool enable); // ����ʹ��
};
SteppingMotor NewSteppingMotor(IO plu, IO ena, IO dir);

#endif // STEPPING_MOTOR
