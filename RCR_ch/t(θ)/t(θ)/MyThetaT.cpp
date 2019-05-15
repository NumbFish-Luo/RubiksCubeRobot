#include "MyThetaT.h"

MyThetaT::MyThetaT(const int theta0, const int t0, const double minDelta) :
    m_theta0      { theta0 },
    m_t0          { t0 },
    m_minDelta    { minDelta },
    m_tArr        { vector<double>(theta0 + 1) },
    m_delayTimeArr{ vector<double>(theta0 + 1) }{
}

MyThetaT::~MyThetaT() {}

void MyThetaT::OutputData(const string& fileName, bool half) {
    ofstream fout{ fileName };
    if (!fout.is_open()) {
        cerr << "Cannot open arr.txt!" << endl;
        return;
    }
    m_delayTimeArr[0] = 0;
    m_tArr[0] = 0;

    vector<int> output;

    double timeSum = 0;
    int timeSum_int = 0;

    vector<double> tmp_t;
    vector<double> tmp_v;
    double tmp_a;

    constexpr int NEW_LINE = 10;
    constexpr int LENGHT = 6;

    for (int n = 1; n <= m_theta0; ++n) {
        m_tArr[n] = T(n);
        m_delayTimeArr[n] = m_tArr[n] - m_tArr[n - 1];
        timeSum += m_delayTimeArr[n];
        output.push_back((int)round(m_delayTimeArr[n]));
        timeSum_int += output[n - 1];

        if (n != 1) {
            fout << ", ";
        }
        if ((n - 1) % NEW_LINE == 0) {
            fout << endl;
        }

        if (half && n > (m_theta0 / 2)) {
            cout << "half" << endl;
            for (int i = n; i <= m_theta0; ++i) {
                if (i != n) {
                    fout << ", ";
                }
                if ((i - 1) % NEW_LINE == 0) {
                    fout << endl;
                }
                fout << setiosflags(ios::fixed) << fixed << setprecision(0) << setw(LENGHT) << output[m_theta0 - i];
            }
            break;
        } else {
            fout << setiosflags(ios::fixed) << fixed << setprecision(0) << setw(LENGHT) << output[n - 1];
        }

        //:T[¦È]
        //fout << output[n - 1] << endl;

        //:theta(t)
        //for (int i = 0; i < output[n - 1]; ++i)
        //    fout << n << endl;

        //if (n > 1) {
            //:v(t)
            //tmp_t.push_back((timeSum_int - output[n - 1]) / 2.0 - (output[n - 1] + output[n - 2]) / 4.0);
            //tmp_v.push_back(2.0 / (output[n - 2] + output[n - 1]));
            //fout << tmp_t[n - 2] << ',' << tmp_v[n - 2] << endl;

            //:a(t)
            //if (tmp_t.size() > 1) {
            //    tmp_a = (tmp_v[n - 2] - tmp_v[n - 3]) / (tmp_t[n - 2] - tmp_t[n - 3]);
            //    fout << tmp_t[n - 3] + (tmp_t[n - 2] - tmp_t[n - 3]) / 2.f << ',' << tmp_a << endl;
            //}
        //}
    }
    std::cout
        << "timeSum = " << timeSum << endl
        << "timeSum_int = " << timeSum_int << endl
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
