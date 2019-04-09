#pragma once
#include "MyThetaT.h"

class MyThetaT_CK : public MyThetaT {
public:
    MyThetaT_CK(const int theta0, const int t0, const double minDelta, const double tp[7]);
    virtual ~MyThetaT_CK();
private:
    virtual double Theta(double t);
    double m_j;
    double m_ts[7];
    double m_Ts[7];
};
