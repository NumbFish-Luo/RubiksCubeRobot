#pragma once
#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#define OUT(n) cout << #n << " = " << n << endl
using namespace std;

class MyThetaT {
public:
    MyThetaT(const int theta0, const int t0, const double minDelta);
    virtual ~MyThetaT() = 0;

    void OutputData(const string& fileName, bool half = false);
    virtual double Theta(double t) = 0;
protected:
    
    double T(double theta);

    const int      m_theta0;
    const int      m_t0;
    const double   m_minDelta;
    vector<double> m_tArr;
    vector<double> m_delayTimeArr;
};
