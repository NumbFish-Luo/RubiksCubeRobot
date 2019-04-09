#include "MyThetaT_CK.h"
MyThetaT_CK::MyThetaT_CK(const int theta0, const int t0, const double minDelta, const double tp[7])
    :MyThetaT(theta0, t0, minDelta){
    for (int i = 0; i < 7; ++i) {
        m_Ts[i] = tp[i] * theta0;
        if (i == 0) {
            m_ts[i] = m_Ts[i];
        } else {
            m_ts[i] = m_ts[i - 1] + m_Ts[i - 1];
        }
    }

    auto& T1 = m_Ts[0];
    auto& T2 = m_Ts[1];
    auto& T3 = m_Ts[2];
    auto& T4 = m_Ts[3];
    auto& T5 = m_Ts[4];
    auto& T6 = m_Ts[5];
    auto& T7 = m_Ts[6];

    const double ft = 
        + 1. / 2 * T1 * T1 * T2
        + 1. / 2 * T1 * T2 * T2
        + 1. / 6 * T1 * T1 * T1
        - 1. / 6 * T3 * T3 * T3
        + 1. / 2 * T1 * T3 * T3
        + 1. / 2 * T1 * T1 * T4
        + 1. * T1 * T2 * T4
        + 1. * T1 * T3 * T4
        - 1. / 2 * T3 * T3 * T3 * T4
        + 1. / 2 * T1 * T1 * T5
        + 1. * T1 * T2 * T5
        + 1. * T1 * T3 * T5
        - 1. / 2 * T3 * T3 * T3 * T5
        - 1. / 6 * T5 * T5 * T5
        + 1. / 2 * T1 * T1 * T6
        + 1. * T1 * T2 * T6
        + 1. * T1 * T3 * T6
        - 1. / 2 * T3 * T3 * T3 * T6
        - 1. / 2 * T5 * T5 * T6
        - 1. / 2 * T5 * T6 * T6
        + 1. / 2 * T1 * T1 * t0
        + 1. * T1 * T2 * t0
        + 1. * T1 * T3 * t0
        - 1. / 2 * T3 * T3 * t0
        - 1. / 2 * T5 * T5 * t0
        - 1. * T5 * T6 * t0
        + 1. / 6 * t0 * t0 * t0
        - 1. / 2 * T5 * t0 * t0;
    m_j = theta0 / ft;
}
MyThetaT_CK::~MyThetaT_CK() {}
double MyThetaT_CK::Theta(double t) {
    auto& theta0 = m_theta0;
    auto& t0 = m_t0;
    auto& j = m_j;

    auto& T1 = m_Ts[0];
    auto& T2 = m_Ts[1];
    auto& T3 = m_Ts[2];
    auto& T4 = m_Ts[3];
    auto& T5 = m_Ts[4];
    auto& T6 = m_Ts[5];
    auto& T7 = m_Ts[6];

    auto& t1 = m_ts[0];
    auto& t2 = m_ts[1];
    auto& t3 = m_ts[2];
    auto& t4 = m_ts[3];
    auto& t5 = m_ts[4];
    auto& t6 = m_ts[5];
    auto& t7 = m_ts[6];

    if (0 <= t && t < t1) {         // 1
        return j / 6 * t * t * t;
    } else if (t1 <= t && t < t2) { // 2
        t -= t1;
        return j / 2 * T1 * T1 * t
            + j / 2 * T1 * t * t
            + j / 6 * T1 * T1 * T1;
    } else if (t2 <= t && t < t3) { // 3
        t -= t2;
        return j / 2 * T1 * T1 * T2
            + j / 2 * T1 * T2 * T2
            + j / 6 * T1 * T1 * T1
            - j / 6 * t * t * t
            + j / 2 * T1 * t  * t;
    } else if (t3 <= t && t < t4) { // 4
        t -= t3;
        return j / 2 * T1 * T1 * T2
            + j / 2 * T1 * T2 * T2
            + j / 6 * T1 * T1 * T1
            - j / 6 * T3 * T3 * T3
            + j / 2 * T1 * T3 * T3
            + j / 2 * T1 * T1 * t
            + j * T1 * T2 * t
            + j * T1 * T3 * t
            - j / 2 * T3 * T3 * T3 * t;
    } else if (t4 <= t && t < t5) { // 5
        t -= t4;
        return j / 2 * T1 * T1 * T2
            + j / 2 * T1 * T2 * T2
            + j / 6 * T1 * T1 * T1
            - j / 6 * T3 * T3 * T3
            + j / 2 * T1 * T3 * T3
            + j / 2 * T1 * T1 * T4
            + j * T1 * T2 * T4
            + j * T1 * T3 * T4
            - j / 2 * T3 * T3 * T3 * T4
            + j / 2 * T1 * T1 * t
            + j * T1 * T2 * t
            + j * T1 * T3 * t
            - j / 2 * T3 * T3 * T3 * t
            - j / 6 * t * t * t;
    } else if (t5 <= t && t < t6) { // 6
        t -= t5;
        return j / 2 * T1 * T1 * T2
            + j / 2 * T1 * T2 * T2
            + j / 6 * T1 * T1 * T1
            - j / 6 * T3 * T3 * T3
            + j / 2 * T1 * T3 * T3
            + j / 2 * T1 * T1 * T4
            + j * T1 * T2 * T4
            + j * T1 * T3 * T4
            - j / 2 * T3 * T3 * T3 * T4
            + j / 2 * T1 * T1 * T5
            + j * T1 * T2 * T5
            + j * T1 * T3 * T5
            - j / 2 * T3 * T3 * T3 * T5
            - j / 6 * T5 * T5 * T5
            + j / 2 * T1 * T1 * t
            + j * T1 * T2 * t
            + j * T1 * T3 * t
            - j / 2 * T3 * T3 * T3 * t
            - j / 2 * T5 * T5 * t
            - j / 2 * T5 * t * t;
    } else if (t6 <= t && t <= t7) { // 7
        t -= t6;
        return j / 2 * T1 * T1 * T2
            + j / 2 * T1 * T2 * T2
            + j / 6 * T1 * T1 * T1
            - j / 6 * T3 * T3 * T3
            + j / 2 * T1 * T3 * T3
            + j / 2 * T1 * T1 * T4
            + j * T1 * T2 * T4
            + j * T1 * T3 * T4
            - j / 2 * T3 * T3 * T3 * T4
            + j / 2 * T1 * T1 * T5
            + j * T1 * T2 * T5
            + j * T1 * T3 * T5
            - j / 2 * T3 * T3 * T3 * T5
            - j / 6 * T5 * T5 * T5
            + j / 2 * T1 * T1 * T6
            + j * T1 * T2 * T6
            + j * T1 * T3 * T6
            - j / 2 * T3 * T3 * T3 * T6
            - j / 2 * T5 * T5 * T6
            - j / 2 * T5 * T6 * T6
            + j / 2 * T1 * T1 * t
            + j * T1 * T2 * t
            + j * T1 * T3 * t
            - j / 2 * T3 * T3 * t
            - j / 2 * T5 * T5 * t
            - j * T5 * T6 * t
            + j / 6 * t * t * t
            - j / 2 * T5 * t * t;
    } else {
        return 0;
    }
}
