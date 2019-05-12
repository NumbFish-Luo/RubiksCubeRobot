#pragma once
#include "MyThetaT.h"

class MyThetaT_Sin : public MyThetaT {
public:
    MyThetaT_Sin(const int theta0, const int t0, const double minDelta);
    virtual ~MyThetaT_Sin();
    virtual double Theta(double t);
private:
    
};
