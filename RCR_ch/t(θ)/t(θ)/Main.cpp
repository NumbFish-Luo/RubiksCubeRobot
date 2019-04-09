#include "MyThetaT_NF.h"
#include "MyThetaT_CK.h"

int main() {
    MyThetaT_NF nf{ 10000, 40000, 10e-6, 0.5 };
    nf.OutputData("arr.txt");

    cin.get();
    return 0;
}
