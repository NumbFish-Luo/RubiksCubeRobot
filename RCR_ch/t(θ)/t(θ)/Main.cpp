#include "MyThetaT_NF.h"
#include "MyThetaT_Sin.h"

int main() {
    MyThetaT_Sin mySin{ 200, 25000, 10e-6 };
    mySin.OutputData("sin.txt", true);

    cin.get();
    return 0;
}
