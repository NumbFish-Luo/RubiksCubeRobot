#pragma once
#include <opencv2/opencv.hpp>

const double PI = 3.141592653589793;

class DirectedSegment // 有向线段类
{
public:
    DirectedSegment();
    DirectedSegment(const cv::Point2d& pointO, const cv::Point2d& pointA); // 用两点OA坐标初始化有向线段，计算角度和长度
    DirectedSegment(const cv::Point2d& pointO, const double angle, const double length); // 用始点O坐标，角度，长度初始化有向线段，计算终点A坐标
    DirectedSegment(const DirectedSegment& ds);
    DirectedSegment& operator= (const DirectedSegment& ds) = delete;

    const double GetAngle() const; // 获取角度
    const double GetLength() const; // 获取长度
    const cv::Point2d& GetPO() const; // 获取始点O
    const cv::Point2d& GetPA() const; // 获取终点A

    void SetAngle(const double angle);   // 改变角度，始点O不变，长度不变，重新计算改变角度后的终点A的位置
    void SetLength(const double length); // 改变长度，始点O不变，角度不变，重新计算改变长度后的终点A的位置
    void SetPO(const cv::Point2d& pointO); // 改变始点O坐标，终点A不变，根据两点坐标重新计算角度和长度
    void SetPA(const cv::Point2d& pointA); // 改变终点A坐标，始点O不变，根据两点坐标重新计算角度和长度
    void SetAxKeepAy(const double Ax); // 改变终点A的x值，且保持Ay不变，始点O不变（但是长度、角度肯定会变）
    void SetAyKeepAx(const double Ay); // 改变终点A的y值，且保持Ax不变，始点O不变（但是长度、角度肯定会变）
    void SetAxPercentKeepAy(const double AxPercent); // 改变终点A的x值的百分比（0~100%），且保持Ay不变，始点O不变（但是长度、角度肯定会变）
    void SetAyPercentKeepAx(const double AyPercent); // 改变终点A的y值的百分比（0~100%），且保持Ax不变，始点O不变（但是长度、角度肯定会变）
    void SetAxPercentKeepLength(const double AxPercent); // 改变终点A的x值的百分比（0~100%），且保持始点O不变，长度不变（但是角度和Ay肯定会变）
    void SetAyPercentKeepLength(const double AyPercent); // 改变终点A的y值的百分比（0~100%），且保持始点O不变，长度不变（但是角度和Ax肯定会变）

    void MoveDirectedSegment(const cv::Point2d& pointO); // 平移有向线段，以始点O为移动基准点，角度不变，长度不变
    void AddAngle(const double addAngle); // 增加角度（+顺时针 -逆时针），始点O不变，长度不变，重新计算改变角度后的终点A的位置

private:
    void SetRandian(const double randian); // 改变弧度（也是改变角度），始点O不变，长度不变，重新计算改变角度后的终点A的位置
    void CalculateAngleAndLength(); // 根据两点坐标重新计算角度和长度
    void CalculatePointA(); // 重新计算改变角度或长度后的终点A的位置

    cv::Point2d m_pointO; // 始点O坐标
    cv::Point2d m_pointA; // 终点A坐标
    double m_randian; // 弧度
    double m_length;  // 长度
};

inline DirectedSegment::DirectedSegment()
    : DirectedSegment(cv::Point2d(), cv::Point2d()) {}

inline DirectedSegment::DirectedSegment(const cv::Point2d& pointO, const cv::Point2d& pointA)
    : m_pointO(pointO), m_pointA(pointA), m_randian(0), m_length(0) {
    CalculateAngleAndLength();
}

inline DirectedSegment::DirectedSegment(const cv::Point2d& pointO, const double angle, const double length)
    : m_pointO(pointO), m_pointA(), m_randian(angle * PI / 180.0), m_length(length) {
    CalculatePointA();
}

inline DirectedSegment::DirectedSegment(const DirectedSegment& ds)
    : m_pointO(ds.m_pointO), m_pointA(ds.m_pointA), m_randian(ds.m_randian), m_length(ds.m_length) {}

inline const double DirectedSegment::GetAngle() const {
    return m_randian * 180 / PI;
}

inline const double DirectedSegment::GetLength() const {
    return m_length;
}

inline const cv::Point2d& DirectedSegment::GetPO() const {
    return m_pointO;
}

inline const cv::Point2d& DirectedSegment::GetPA() const {
    return m_pointA;
}

inline void DirectedSegment::SetAngle(const double angle) {
    m_randian = angle * PI / 180.0;
    CalculatePointA();
}

inline void DirectedSegment::SetLength(const double length) {
    m_length = length;
    CalculatePointA();
}

inline void DirectedSegment::SetPO(const cv::Point2d& pointO) {
    m_pointO = pointO;
    CalculateAngleAndLength();
}

inline void DirectedSegment::SetPA(const cv::Point2d& pointA) {
    m_pointA = pointA;
    CalculateAngleAndLength();
}

inline void DirectedSegment::SetAxKeepAy(const double Ax) {
    SetPA(cv::Point2d(Ax, m_pointA.y));
}

inline void DirectedSegment::SetAyKeepAx(const double Ay) {
    SetPA(cv::Point2d(m_pointA.x, Ay));
}

inline void DirectedSegment::SetAxPercentKeepAy(const double AxPercent) {
    double deltaX = (m_pointA.x - m_pointO.x) * (AxPercent / 100);
    SetPA(cv::Point2d(m_pointO.x + deltaX, m_pointA.y));
}

inline void DirectedSegment::SetAyPercentKeepAx(const double AyPercent) {
    double deltaY = (m_pointA.y - m_pointO.y) * (AyPercent / 100);
    SetPA(cv::Point2d(m_pointA.x, m_pointO.y + deltaY));
}

inline void DirectedSegment::SetAxPercentKeepLength(const double AxPercent) {
    double deltaX = (m_pointA.x - m_pointO.x) * (AxPercent / 100);
    m_randian = acos(deltaX / m_length);
    if (m_pointA.y - m_pointO.y < 0) {
        m_randian = 2 * PI - m_randian;
    }
    CalculatePointA();
}

inline void DirectedSegment::SetAyPercentKeepLength(const double AyPercent) {
    double deltaY = (m_pointA.y - m_pointO.y) * (AyPercent / 100);
    m_randian = asin(deltaY / m_length);
    if (m_pointA.x - m_pointO.x < 0) {
        m_randian = PI - m_randian;
    }
    CalculatePointA();
}

inline void DirectedSegment::MoveDirectedSegment(const cv::Point2d& pointO) {
    m_pointA.x = pointO.x - m_pointO.x;
    m_pointA.y = pointO.y - m_pointO.y;
    m_pointO = pointO;
}

inline void DirectedSegment::AddAngle(const double addAngle) {
    m_randian += addAngle * PI / 180.0;
    CalculatePointA();
}

inline void DirectedSegment::SetRandian(const double randian) {
    m_randian = randian;
}

inline void DirectedSegment::CalculateAngleAndLength() {
    m_randian = atan2((m_pointA.y - m_pointO.y), (m_pointA.x - m_pointO.x));
    double delta_x = m_pointA.x - m_pointO.x;
    double delta_y = m_pointA.y - m_pointO.y;
    m_length = sqrt(pow(delta_x, 2) + pow(delta_y, 2));
}

inline void DirectedSegment::CalculatePointA() {
    m_pointA.x = m_pointO.x + m_length * cos(m_randian);
    m_pointA.y = m_pointO.y + m_length * sin(m_randian);
}

