#include "MyThetaT_NF.h"
#include "MyThetaT_Sin.h"

int main() {
    MyThetaT_Sin mySin{ 400, 4500000, 10e-6 };
    mySin.OutputData("sin.txt", true);

    cin.get();
    return 0;
}
