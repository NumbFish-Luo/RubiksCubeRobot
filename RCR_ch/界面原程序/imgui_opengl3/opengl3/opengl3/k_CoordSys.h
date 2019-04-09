#pragma once
#include <string>

enum Face : size_t {
    U = 0,
    D = 1,
    L = 2,
    R = 3,
    F = 4,
    B = 5
};

// ����ϵ
class CoordSys {
public:
    CoordSys();                           // ��ʼ��
    CoordSys(const CoordSys& coord);      // ���ƹ��캯��
    const static CoordSys& Origin();      // ԭʼ����ϵ
    void Init();                          // ��ʼ��
    void Rotate(const std::string& rot);  // ��ת{ "AL+", "AL2", "AL-", "AD+", "AD2", "AD-" }
    char operator[](char faceName) const; // ��ȡ����
private:
    void AL1();      // AL+
    void AL2();      // AL2
    void AL3();      // AL-
    void AD1();      // AD+
    void AD2();      // AD2
    void AD3();      // AD-
    char m_datas[6]; // ����
};
