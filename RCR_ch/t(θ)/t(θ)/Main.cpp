#include "MyThetaT_NF.h"

int main() {
    MyThetaT_NF nf{ 10000, 35000, 10e-6, 0.5 };
    nf.OutputData("arr.txt");

    cin.get();
    return 0;
}
