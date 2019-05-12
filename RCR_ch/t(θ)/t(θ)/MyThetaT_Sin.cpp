#include "MyThetaT_Sin.h"

MyThetaT_Sin::MyThetaT_Sin(const int theta0, const int t0, const double minDelta)
    : MyThetaT(theta0, t0, minDelta) {}

MyThetaT_Sin::~MyThetaT_Sin() {}

double MyThetaT_Sin::Theta(double t) {
    auto& theta0 = m_theta0;
    auto& t0 = m_t0;

    static bool flag = false;
    if (flag == false) {
        flag = true;
        OUT(theta0);
        OUT(t0);
    }

    const double A = 2 * M_PI * theta0 / t0 / t0;
    const double w = 2 * M_PI / t0;
    return -A / w / w * sin(w * t) + A / w * t;
}
