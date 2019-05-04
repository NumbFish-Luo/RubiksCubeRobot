#include "MyThetaT_NF.h"

int main() {
    // ·Âsin: 2 - 4 / pi = 0.726
    MyThetaT_NF nf{ 5000, 30000, 10e-6, 0.726 };
    nf.OutputData("arr.txt");

    cin.get();
    return 0;
}
