#pragma once
#include <string>
#include <vector>
#include "k_CoordSys.h"

// 原始数据
class OSteps {
public:
    OSteps();                                                 // 构造函数
    OSteps(const OSteps& oSteps);                             // 复制构造函数
    void Clear();                                             // 清除数据
    void PushBack(const std::string& step);                   // 插入数据
    std::string Get(size_t idx, const CoordSys& coord) const; // 获取数据
    size_t Size() const;                                      // 获取大小
private:
    std::vector<std::string> m_datas;                         // 数据
};

// 有A的中间数据
class ASteps {
public:
    ASteps(const OSteps& oSteps);                               // 构造函数
    ASteps(const ASteps& aStesp);                               // 复制构造函数
    void Translate(const OSteps& oSteps);                       // o转换a
    void Clear();                                               // 清除数据
    const std::string& operator[](size_t idx);                  // 获取数据
    size_t AStepsSize() const;                                  // 获取大小
    size_t OStepsSize() const;                                  // 获取OSteps大小
    std::string GetOStep(size_t idx) const;                     // 便捷地获取OStep
private:
    size_t DealU(size_t currentIdx, size_t nextIdx);            // 处理U
    size_t DealD(size_t currentIdx, size_t nextIdx);            // 处理D
    size_t DealL(size_t currentIdx, size_t nextIdx);            // 处理L
    size_t DealR(size_t currentIdx, size_t nextIdx);            // 处理R
    size_t DealF(size_t currentIdx, size_t nextIdx);            // 处理F
    size_t DealB(size_t currentIdx, size_t nextIdx);            // 处理B
    void PushBack(const std::string& aStep, int oStepIdx = -1); // 插入数据，从oSteps中获取角度值
    CoordSys m_coord;                                           // 坐标系
    OSteps m_oSteps;                                            // 原始数据
    std::vector<std::string> m_aSteps;                          // 转换后数据
};

// 有M的最终数据
class MSteps {
public:
    MSteps(const ASteps& aSteps);                   // 构造函数
    MSteps(const MSteps& mSteps);                   // 复制构造函数
    void Translate(const ASteps& aSteps);           // a转换m
    void Clear();                                   // 清除数据
    const std::string& operator[](size_t idx);      // 获取数据
    size_t MStepsSize() const;                      // 获取大小
    size_t AStepsSize() const;                      // 获取aSteps大小
    void Output(const std::string& fileName) const; // 输出解法
private:
    void DealNormalCase(size_t idx);                // 处理正常情况
    bool DealSpecialCase(size_t idx);               // 处理特殊情况
    bool DealSp1(size_t idx);                       // 处理特殊情况1
    bool DealSp2(size_t idx);                       // 处理特殊情况2
    bool DealSp3(size_t idx);                       // 处理特殊情况3
    bool DealSp4(size_t idx);                       // 处理特殊情况4
    void PushBack(size_t idx, const std::string& step, int aStepIdx = -1, bool reverse = false); // 插入数据前，先处理好数据
    ASteps m_aSteps;                                // 原始数据
    std::vector<std::string> m_mSteps;              // 转换后的数据
};
