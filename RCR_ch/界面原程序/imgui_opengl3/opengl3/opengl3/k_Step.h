#pragma once
#include <string>
#include <vector>
#include "k_CoordSys.h"

// ԭʼ����
class OSteps {
public:
    OSteps();                                                 // ���캯��
    OSteps(const OSteps& oSteps);                             // ���ƹ��캯��
    void Clear();                                             // �������
    void PushBack(const std::string& step);                   // ��������
    std::string Get(size_t idx, const CoordSys& coord) const; // ��ȡ����
    size_t Size() const;                                      // ��ȡ��С
private:
    std::vector<std::string> m_datas;                         // ����
};

// ��A���м�����
class ASteps {
public:
    ASteps(const OSteps& oSteps);                               // ���캯��
    ASteps(const ASteps& aStesp);                               // ���ƹ��캯��
    void Translate(const OSteps& oSteps);                       // oת��a
    void Clear();                                               // �������
    const std::string& operator[](size_t idx);                  // ��ȡ����
    size_t AStepsSize() const;                                  // ��ȡ��С
    size_t OStepsSize() const;                                  // ��ȡOSteps��С
    std::string GetOStep(size_t idx) const;                     // ��ݵػ�ȡOStep
private:
    size_t DealU(size_t currentIdx, size_t nextIdx);            // ����U
    size_t DealD(size_t currentIdx, size_t nextIdx);            // ����D
    size_t DealL(size_t currentIdx, size_t nextIdx);            // ����L
    size_t DealR(size_t currentIdx, size_t nextIdx);            // ����R
    size_t DealF(size_t currentIdx, size_t nextIdx);            // ����F
    size_t DealB(size_t currentIdx, size_t nextIdx);            // ����B
    void PushBack(const std::string& aStep, int oStepIdx = -1); // �������ݣ���oSteps�л�ȡ�Ƕ�ֵ
    CoordSys m_coord;                                           // ����ϵ
    OSteps m_oSteps;                                            // ԭʼ����
    std::vector<std::string> m_aSteps;                          // ת��������
};

// ��M����������
class MSteps {
public:
    MSteps(const ASteps& aSteps);                   // ���캯��
    MSteps(const MSteps& mSteps);                   // ���ƹ��캯��
    void Translate(const ASteps& aSteps);           // aת��m
    void Clear();                                   // �������
    const std::string& operator[](size_t idx);      // ��ȡ����
    size_t MStepsSize() const;                      // ��ȡ��С
    size_t AStepsSize() const;                      // ��ȡaSteps��С
    void Output(const std::string& fileName) const; // ����ⷨ
private:
    void DealNormalCase(size_t idx);                // �����������
    bool DealSpecialCase(size_t idx);               // �����������
    bool DealSp1(size_t idx);                       // �����������1
    bool DealSp2(size_t idx);                       // �����������2
    bool DealSp3(size_t idx);                       // �����������3
    bool DealSp4(size_t idx);                       // �����������4
    void PushBack(size_t idx, const std::string& step, int aStepIdx = -1, bool reverse = false); // ��������ǰ���ȴ��������
    ASteps m_aSteps;                                // ԭʼ����
    std::vector<std::string> m_mSteps;              // ת���������
};
