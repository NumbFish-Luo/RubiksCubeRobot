#include "MyThetaT_NF.h"

MyThetaT_NF::MyThetaT_NF(const int theta0, const int t0, const double minDelta, const double p)
    :MyThetaT(theta0, t0, minDelta), m_p{ p }{
}
MyThetaT_NF::~MyThetaT_NF() {}
double MyThetaT_NF::Theta(double t) {
    auto& theta0 = m_theta0;
    auto& t0 = m_t0;
    auto& p = m_p;

    static bool flag = false;
    if (flag == false) {
        flag = true;
        OUT(theta0);
        OUT(t0);
        OUT(p);
    }

    const double a = 32. * theta0 / t0 / t0 / t0 / (2 - p) / p;
    const double t1 = p * t0 / 4;
    const double t2 = (2 - p) * t0 / 4;
    const double t3 = (2 + p) * t0 / 4;
    const double t4 = (4 - p) * t0 / 4;

    if (0 <= t && t < t1) {
        return a / 6 * t * t * t;
    } else if (t1 <= t && t < t2) {
        return a * p / 8 * t0 * t * t
            - a * p * p / 32 * t0 * t0 * t
            + a * p * p * p / 384 * t0 * t0 * t0;
    } else if (t2 <= t && t < t3) {
        return -a / 6 * t * t * t
            + a / 4 * t0 * t * t
            + (2 * a * p - a * p * p - 2 * a) / 16 * t0 * t0 * t
            + (4 * a - 6 * a * p + 3 * a * p * p) / 192 * t0 * t0 * t0;
    } else if (t3 <= t && t < t4) {
        return -a * p / 8 * t0 * t * t
            + (8 * a * p - a * p * p) / 32 * t0 * t0 * t
            + (-24 * a * p - a * p * p * p) / 384 * t0 * t0 * t0;
    } else if (t4 <= t && t <= t0) {
        return a / 6 * t * t * t
            - a / 2 * t0 * t * t
            + a / 2 * t0 * t0 * t
            + (-3 * a * p * p + 6 * a * p - 16 * a) / 96 * t0 * t0 * t0;
    } else {
        return 0;
    }
}
