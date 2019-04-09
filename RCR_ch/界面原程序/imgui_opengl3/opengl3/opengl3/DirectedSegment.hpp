#pragma once
#include <opencv2/opencv.hpp>

const double PI = 3.141592653589793;

class DirectedSegment // �����߶���
{
public:
    DirectedSegment();
    DirectedSegment(const cv::Point2d& pointO, const cv::Point2d& pointA); // ������OA�����ʼ�������߶Σ�����ǶȺͳ���
    DirectedSegment(const cv::Point2d& pointO, const double angle, const double length); // ��ʼ��O���꣬�Ƕȣ����ȳ�ʼ�������߶Σ������յ�A����
    DirectedSegment(const DirectedSegment& ds);
    DirectedSegment& operator= (const DirectedSegment& ds) = delete;

    const double GetAngle() const; // ��ȡ�Ƕ�
    const double GetLength() const; // ��ȡ����
    const cv::Point2d& GetPO() const; // ��ȡʼ��O
    const cv::Point2d& GetPA() const; // ��ȡ�յ�A

    void SetAngle(const double angle);   // �ı�Ƕȣ�ʼ��O���䣬���Ȳ��䣬���¼���ı�ǶȺ���յ�A��λ��
    void SetLength(const double length); // �ı䳤�ȣ�ʼ��O���䣬�ǶȲ��䣬���¼���ı䳤�Ⱥ���յ�A��λ��
    void SetPO(const cv::Point2d& pointO); // �ı�ʼ��O���꣬�յ�A���䣬���������������¼���ǶȺͳ���
    void SetPA(const cv::Point2d& pointA); // �ı��յ�A���꣬ʼ��O���䣬���������������¼���ǶȺͳ���
    void SetAxKeepAy(const double Ax); // �ı��յ�A��xֵ���ұ���Ay���䣬ʼ��O���䣨���ǳ��ȡ��Ƕȿ϶���䣩
    void SetAyKeepAx(const double Ay); // �ı��յ�A��yֵ���ұ���Ax���䣬ʼ��O���䣨���ǳ��ȡ��Ƕȿ϶���䣩
    void SetAxPercentKeepAy(const double AxPercent); // �ı��յ�A��xֵ�İٷֱȣ�0~100%�����ұ���Ay���䣬ʼ��O���䣨���ǳ��ȡ��Ƕȿ϶���䣩
    void SetAyPercentKeepAx(const double AyPercent); // �ı��յ�A��yֵ�İٷֱȣ�0~100%�����ұ���Ax���䣬ʼ��O���䣨���ǳ��ȡ��Ƕȿ϶���䣩
    void SetAxPercentKeepLength(const double AxPercent); // �ı��յ�A��xֵ�İٷֱȣ�0~100%�����ұ���ʼ��O���䣬���Ȳ��䣨���ǽǶȺ�Ay�϶���䣩
    void SetAyPercentKeepLength(const double AyPercent); // �ı��յ�A��yֵ�İٷֱȣ�0~100%�����ұ���ʼ��O���䣬���Ȳ��䣨���ǽǶȺ�Ax�϶���䣩

    void MoveDirectedSegment(const cv::Point2d& pointO); // ƽ�������߶Σ���ʼ��OΪ�ƶ���׼�㣬�ǶȲ��䣬���Ȳ���
    void AddAngle(const double addAngle); // ���ӽǶȣ�+˳ʱ�� -��ʱ�룩��ʼ��O���䣬���Ȳ��䣬���¼���ı�ǶȺ���յ�A��λ��

private:
    void SetRandian(const double randian); // �ı仡�ȣ�Ҳ�Ǹı�Ƕȣ���ʼ��O���䣬���Ȳ��䣬���¼���ı�ǶȺ���յ�A��λ��
    void CalculateAngleAndLength(); // ���������������¼���ǶȺͳ���
    void CalculatePointA(); // ���¼���ı�ǶȻ򳤶Ⱥ���յ�A��λ��

    cv::Point2d m_pointO; // ʼ��O����
    cv::Point2d m_pointA; // �յ�A����
    double m_randian; // ����
    double m_length;  // ����
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

