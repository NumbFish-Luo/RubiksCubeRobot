#include "MyThetaT.h"

MyThetaT::MyThetaT(const int theta0, const int t0, const double minDelta) :
    m_theta0      { theta0 },
    m_t0          { t0 },
    m_minDelta    { minDelta },
    m_tArr        { vector<double>(theta0 + 1) },
    m_delayTimeArr{ vector<double>(theta0 + 1) }{
}
MyThetaT::~MyThetaT() {}
void MyThetaT::OutputData(const string& fileName) {
    ofstream fout{ fileName };
    if (!fout.is_open()) {
        cerr << "Cannot open arr.txt!" << endl;
        return;
    }
    m_delayTimeArr[0] = 0;
    m_tArr[0] = 0;

    double timeSum = 0;
    for (int n = 1; n <= m_theta0; ++n) {
        m_tArr[n] = T(n);
        m_delayTimeArr[n] = m_tArr[n] - m_tArr[n - 1];
        timeSum += abs(m_delayTimeArr[n]);
        if (n != 1) {
            fout << ", ";
        }
        if ((n - 1) % 13 == 0) {
            fout << endl;
        }
        fout << setiosflags(ios::fixed) << fixed << setprecision(0) << setw(4) << m_delayTimeArr[n];
    }
    cout << "timeSum = " << timeSum << endl
         << "OK!" << endl;
    fout.close();
}
double MyThetaT::Theta(double t) {
    return 0.0;
}
double MyThetaT::T(double theta) {
    double l = 0;
    double r = m_t0;
    double m = (l + r) / 2;
    double delta;
    while (1) {
        delta = Theta(m) - theta;
        if (abs(delta) < m_minDelta) {
            return m;
        } else {
            delta < 0 ? l = m : r = m;
            m = (l + r) / 2;
        }
    }
    return 0.0;
}
