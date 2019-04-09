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

// 坐标系
class CoordSys {
public:
    CoordSys();                           // 初始化
    CoordSys(const CoordSys& coord);      // 复制构造函数
    const static CoordSys& Origin();      // 原始坐标系
    void Init();                          // 初始化
    void Rotate(const std::string& rot);  // 旋转{ "AL+", "AL2", "AL-", "AD+", "AD2", "AD-" }
    char operator[](char faceName) const; // 获取数据
private:
    void AL1();      // AL+
    void AL2();      // AL2
    void AL3();      // AL-
    void AD1();      // AD+
    void AD2();      // AD2
    void AD3();      // AD-
    char m_datas[6]; // 数据
};
