#pragma once
#include "MyThetaT.h"

class MyThetaT_NF : public MyThetaT {
public:
    MyThetaT_NF(const int theta0, const int t0, const double minDelta, const double p);
    virtual ~MyThetaT_NF();
    virtual double Theta(double t);
private:
    
    double m_p;
};
