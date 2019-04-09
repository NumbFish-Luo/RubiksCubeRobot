#include "k_CoordSys.h"
#include "Serial.h"

CoordSys::CoordSys() {
    Init();
}
CoordSys::CoordSys(const CoordSys& coord) {
    for (size_t i = 0; i < 6; ++i) {
        m_datas[i] = coord.m_datas[i];
    }
}
const CoordSys& CoordSys::Origin() {
    static CoordSys instance = CoordSys();
    return instance;
}
void CoordSys::Init() {
    m_datas[U] = 'U';
    m_datas[D] = 'D';
    m_datas[L] = 'L';
    m_datas[R] = 'R';
    m_datas[F] = 'F';
    m_datas[B] = 'B';
}
void CoordSys::Rotate(const std::string& rot) {
    if (rot[1] == 'L') {
        if (rot[2] == '+') {
            AL1();
        } else if (rot[2] == '2') {
            AL2();
        } else if (rot[2] == '-') {
            AL3();
        }
    } else if (rot[1] == 'D') {
        if (rot[2] == '+') {
            AD1();
        } else if (rot[2] == '2') {
            AD2();
        } else if (rot[2] == '-') {
            AD3();
        }
    }
}
char CoordSys::operator[](char faceName) const {
    static const char face[6] = { 'U', 'D', 'L', 'R', 'F', 'B' };
    for (int i = 0; i < 6; ++i) {
        if (faceName == m_datas[i]) {
            return face[i];
        }
    }
    return 'E'; // error
}
void CoordSys::AL1() {
    char   tmp = m_datas[U];
    m_datas[U] = m_datas[B];
    m_datas[B] = m_datas[D];
    m_datas[D] = m_datas[F];
    m_datas[F] = tmp;
}
void CoordSys::AL2() {
    std::swap(m_datas[U], m_datas[D]);
    std::swap(m_datas[F], m_datas[B]);
}
void CoordSys::AL3() {
    char   tmp = m_datas[U];
    m_datas[U] = m_datas[F];
    m_datas[F] = m_datas[D];
    m_datas[D] = m_datas[B];
    m_datas[B] = tmp;
}
void CoordSys::AD1() {
    char   tmp = m_datas[F];
    m_datas[F] = m_datas[L];
    m_datas[L] = m_datas[B];
    m_datas[B] = m_datas[R];
    m_datas[R] = tmp;
}
void CoordSys::AD2() {
    std::swap(m_datas[L], m_datas[R]);
    std::swap(m_datas[F], m_datas[B]);
}
void CoordSys::AD3() {
    char   tmp = m_datas[F];
    m_datas[F] = m_datas[R];
    m_datas[R] = m_datas[B];
    m_datas[B] = m_datas[L];
    m_datas[L] = tmp;
}
