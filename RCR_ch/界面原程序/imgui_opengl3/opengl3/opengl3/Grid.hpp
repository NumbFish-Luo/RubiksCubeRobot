#pragma once

#include <opencv2/opencv.hpp>
#include "DirectedSegment.hpp"

class Grid // 网格类
{
public:
    enum GridType
    {
        TYPE_A = 1,
        TYPE_B = 2
    };

    Grid();
    Grid(const cv::Point2d& center, const int radius, const GridType& gridType = TYPE_A,
        const int rotateAngle = 0, const int perspective = 100, const int aspectRatio = 100,
        const cv::Scalar& color = cv::Scalar(255, 255, 255));
    Grid(const Grid& g) = delete;
    Grid& operator= (const Grid& g) = delete;

    const int  GetRadius()        const;
    const char GetGridType()      const;
    const int  GetRotateAngle()   const;
    const int  GetAspectRatio()   const;
    const cv::Point2d& GetCenter() const;
    const cv::Scalar&  GetColor()  const;
    const std::vector<DirectedSegment>& GetNetPoint() const;
    const std::vector<DirectedSegment>& GetBPs() const;

    bool Setting(const cv::Point2d& center, const int radius, const GridType& gridType,
        const int rotateAngle, const int perspective, const int aspectRatio,
        const cv::Scalar& color = cv::Scalar(255, 255, 255));

    bool DrawGrid(cv::Mat& background);

private:
    enum NetPointNum
    {
        NET_TYPE_A = 12,
        NET_TYPE_B = 20,
    };
    enum BlockPointNum
    {
        BLOCK_TYPE_A = 9,
        BLOCK_TYPE_B = 18,
    };
    enum NetLayoutNum
    {
        LAYOUT_NET_TYPE_A = 4,
        LAYOUT_NET_TYPE_B = 7,
    };
    enum BlockLayoutNum
    {
        LAYOUT_BLOCK_TYPE_A = 3,
        LAYOUT_BLOCK_TYPE_B = 6,
    };

    cv::Point2d CrossPoint(const cv::Point2d& line1_point1, const cv::Point2d& line1_point2,
        const cv::Point2d& line2_point1, const cv::Point2d& line2_point2);
    bool MoveAllPoint();
    bool CalculatePoints_radius();
    bool CalculateNetPointTypeB_aspectRatio_perspective();
    bool CompleteNetPointTypeB();
    bool CalculatePoints_rotate();
    bool CalculateBlockPointTypeB();

    std::vector<DirectedSegment> m_netPoint_TypeA;   // 网格线段端点TypeA
    std::vector<DirectedSegment> m_netPoint_TypeB;   // TypeB
    std::vector<DirectedSegment> m_blockPoint_TypeA; // 格子中心点TypeA
    std::vector<DirectedSegment> m_blockPoint_TypeB; // TypeB
    /*
        netPoint          netPoint
        (type A)          (type B)
        *  *  *  *      * * * * * * *
        *        *      *     *     *
        *        *      *     *     *
        *  *  *  *      * * * * * * *

        blockPoint        blockPoint
        (type A)          (type B)
        *   *   *        * * * * * *
        *   *   *        * * * * * *
        *   *   *        * * * * * *
    */
    const int m_layoutNetNum_TypeA[LAYOUT_NET_TYPE_A]{ 4, 2, 2, 4 };          // 每一竖排的网格端点数目TypeA
    const int m_layoutNetNum_TypeB[LAYOUT_NET_TYPE_B]{ 4, 2, 2, 4, 2, 2, 4 }; // TypeB

    const int m_layoutBlockNum_TypeA[LAYOUT_BLOCK_TYPE_A]{ 3, 3, 3 };          // 每一竖排的格子中心点数目TypeA
    const int m_layoutBlockNum_TypeB[LAYOUT_BLOCK_TYPE_B]{ 3, 3, 3, 3, 3, 3 }; // TypeB

    cv::Point2d     m_center;        // 中心点
    int             m_radius;        // 网格大小
    GridType        m_gridType;      // 网格类型，A或者B
    int             m_rotateAngle;   // 旋转度数(-180~180)
    int             m_perspective;   // 透视度数(0~100)
    int             m_aspectRatio;   // 长宽比
    cv::Scalar      m_color;         // 线条颜色
    DirectedSegment m_zeroDirection; // 极坐标0度方向
};

typedef Grid::GridType Gtype;

inline Grid::Grid()
    : m_center(cv::Point2d()), m_radius(0), m_gridType(TYPE_A),
    m_rotateAngle(0), m_perspective(100),
    m_aspectRatio(100), m_color(cv::Scalar(255, 255, 255)),
    m_zeroDirection(cv::Point2d(), 0, 0),
    m_netPoint_TypeA(NET_TYPE_A, DirectedSegment()),
    m_netPoint_TypeB(NET_TYPE_B, DirectedSegment()),
    m_blockPoint_TypeA(BLOCK_TYPE_A, DirectedSegment()),
    m_blockPoint_TypeB(BLOCK_TYPE_B, DirectedSegment())
{
}

inline Grid::Grid(const cv::Point2d& center, const int radius, const GridType& gridType,
    const int rotateAngle, const int perspective, const int aspectRatio,
    const cv::Scalar& color)
    : m_center(center), m_radius(radius), m_gridType(gridType),
    m_rotateAngle(rotateAngle), m_perspective(perspective),
    m_aspectRatio(aspectRatio), m_color(color),
    m_zeroDirection(center, 0, radius),
    m_netPoint_TypeA(NET_TYPE_A, DirectedSegment()),
    m_netPoint_TypeB(NET_TYPE_B, DirectedSegment()),
    m_blockPoint_TypeA(BLOCK_TYPE_A, DirectedSegment()),
    m_blockPoint_TypeB(BLOCK_TYPE_B, DirectedSegment())
{
    Setting(m_center, m_radius, m_gridType, m_rotateAngle, m_perspective, m_aspectRatio, m_color);
}

inline const int Grid::GetRadius() const
{
    return m_radius;
}

inline const char Grid::GetGridType() const
{
    if (m_gridType == TYPE_A)
    {
        return 'A';
    }
    else if (m_gridType == TYPE_B)
    {
        return 'B';
    }
    else
    {
        return '?';
    }
}

inline const int Grid::GetRotateAngle() const
{
    return m_rotateAngle;
}

inline const int Grid::GetAspectRatio() const
{
    return m_aspectRatio;
}

inline const cv::Point2d& Grid::GetCenter() const
{
    return m_center;
}

inline const cv::Scalar& Grid::GetColor() const
{
    return m_color;
}

inline const std::vector<DirectedSegment>& Grid::GetNetPoint() const
{
    if (m_gridType == TYPE_A)
    {
        return m_netPoint_TypeA;
    }
    else if (m_gridType == TYPE_B)
    {
        return m_netPoint_TypeB;
    }
    else
    {
        return std::vector<DirectedSegment>{};
    }
}

inline const std::vector<DirectedSegment>& Grid::GetBPs() const
{
    if (m_gridType == TYPE_A)
    {
        return m_blockPoint_TypeA;
    }
    else
    {
        return m_blockPoint_TypeB;
    }
}

inline bool Grid::Setting(const cv::Point2d& center, const int radius,
    const GridType& gridType, const int rotateAngle, const int perspective,
    const int aspectRatio, const cv::Scalar& color)
{
    m_center = center;
    m_radius = radius;
    m_gridType = gridType;
    m_rotateAngle = rotateAngle;
    m_perspective = perspective;
    m_aspectRatio = aspectRatio;
    m_color = color;

    // 0. 移动
    MoveAllPoint();
    // 1. 根据半径，计算点位（无形变）
    CalculatePoints_radius();
    // 2. 变长宽比，变透视度（TYPE_A不变长宽比）
    CalculateNetPointTypeB_aspectRatio_perspective();
    // 3. 补充TypeB剩余点
    CompleteNetPointTypeB();
    // 4. 旋转
    CalculatePoints_rotate();
    // 5. 算BlockTypeB
    CalculateBlockPointTypeB();
    return true;
}

inline bool Grid::DrawGrid(cv::Mat& background)
{
    cv::circle(background, m_center, 2, m_color, -1);
    cv::circle(background, m_center, m_radius, m_color);
    cv::circle(background, m_zeroDirection.GetPA(), 2, m_color, -1);
    cv::line(background, m_center, m_zeroDirection.GetPA(), m_color);
    if (m_gridType == TYPE_A)
    {
        for (int i = 0; i < 4; ++i)
        {
            cv::line(background, m_netPoint_TypeA[i].GetPA(), m_netPoint_TypeA[i + 8].GetPA(), m_color, 1);
        }
        cv::line(background, m_netPoint_TypeA[0].GetPA(), m_netPoint_TypeA[3].GetPA(), m_color, 1);
        cv::line(background, m_netPoint_TypeA[4].GetPA(), m_netPoint_TypeA[5].GetPA(), m_color, 1);
        cv::line(background, m_netPoint_TypeA[6].GetPA(), m_netPoint_TypeA[7].GetPA(), m_color, 1);
        cv::line(background, m_netPoint_TypeA[8].GetPA(), m_netPoint_TypeA[11].GetPA(), m_color, 1);

        for (auto& BPA : m_blockPoint_TypeA)
        {
            cv::circle(background, BPA.GetPA(), 1, m_color);
        }

        return true;
    }
    else if (m_gridType == TYPE_B)
    {
        for (int i = 0; i < 4; ++i)
        {
            cv::line(background, m_netPoint_TypeB[i].GetPA(), m_netPoint_TypeB[i + 8].GetPA(), m_color, 1);
            cv::line(background, m_netPoint_TypeB[i + 8].GetPA(), m_netPoint_TypeB[i + 16].GetPA(), m_color, 1);
        }
        for (int i = 0; i < 3; ++i)
        {
            cv::line(background, m_netPoint_TypeB[i * 8].GetPA(), m_netPoint_TypeB[i * 8 + 3].GetPA(), m_color, 1);
        }
        cv::line(background, m_netPoint_TypeB[4].GetPA(), m_netPoint_TypeB[5].GetPA(), m_color, 1);
        cv::line(background, m_netPoint_TypeB[6].GetPA(), m_netPoint_TypeB[7].GetPA(), m_color, 1);
        cv::line(background, m_netPoint_TypeB[12].GetPA(), m_netPoint_TypeB[13].GetPA(), m_color, 1);
        cv::line(background, m_netPoint_TypeB[14].GetPA(), m_netPoint_TypeB[15].GetPA(), m_color, 1);

        for (auto& BPB : m_blockPoint_TypeB)
        {
            cv::circle(background, BPB.GetPA(), 1, m_color);
        }

        return true;
    }
    else
    {
        return false;
    }
}

inline cv::Point2d Grid::CrossPoint(
    const cv::Point2d& line1_point1, const cv::Point2d& line1_point2,
    const cv::Point2d& line2_point1, const cv::Point2d& line2_point2)
{
    // 计算line1_point1，2形成直线与line2_point1，4形成直线交点
    // 如果平行或有无穷个交点就取line1_point2和3的中间点
    double x, y;
    double X1 = line1_point1.x - line1_point2.x;
    double Y1 = line1_point1.y - line1_point2.y;
    double X2 = line2_point1.x - line2_point2.x;
    double Y2 = line2_point1.y - line2_point2.y;
    if (X1 * Y2 == X2 * Y1)
    {
        return cv::Point2d((line1_point2.x + line2_point1.x) / 2, (line1_point2.y + line2_point1.y) / 2);
    }
    double A = X1 * line1_point1.y - Y1 * line1_point1.x;
    double B = X2 * line2_point1.y - Y2 * line2_point1.x;
    y = (A * Y2 - B * Y1) / (X1 * Y2 - X2 * Y1);
    x = (B * X1 - A * X2) / (Y1 * X2 - Y2 * X1);
    return cv::Point2d(x, y);
}

inline bool Grid::MoveAllPoint()
{
    for (auto& NPA : m_netPoint_TypeA)
    {
        NPA.MoveDirectedSegment(m_center);
    }
    for (auto& BPA : m_blockPoint_TypeA)
    {
        BPA.MoveDirectedSegment(m_center);
    }
    for (auto& NPB : m_netPoint_TypeB)
    {
        NPB.MoveDirectedSegment(m_center);
    }
    for (auto& BPB : m_blockPoint_TypeB)
    {
        BPB.MoveDirectedSegment(m_center);
    }
    m_zeroDirection.MoveDirectedSegment(m_center);
    m_zeroDirection.SetLength(m_radius);
    return true;
}

inline bool Grid::CompleteNetPointTypeB()
{
    double x0 = m_netPoint_TypeB[0].GetPA().x;
    double x16 = m_netPoint_TypeB[16].GetPA().x;
    double y0 = m_netPoint_TypeB[0].GetPA().y;
    double deltaY = y0 - m_center.y;
    m_netPoint_TypeB[1].SetPA(cv::Point2d(x0, y0 - deltaY / 3 * 2));
    m_netPoint_TypeB[2].SetPA(cv::Point2d(x0, y0 - deltaY / 3 * 4));
    m_netPoint_TypeB[17].SetPA(cv::Point2d(x16, y0 - deltaY / 3 * 2));
    m_netPoint_TypeB[18].SetPA(cv::Point2d(x16, y0 - deltaY / 3 * 4));

    cv::Point2d cp;
    for (int i = 0; i < 2; ++i)
    {
        if (i == 1) { i = 8; }
        cp = CrossPoint(m_netPoint_TypeB[0 + i].GetPA(), m_netPoint_TypeB[11 + i].GetPA(),
            m_netPoint_TypeB[1 + i].GetPA(), m_netPoint_TypeB[9 + i].GetPA());
        cp = CrossPoint(m_netPoint_TypeB[0 + i].GetPA(), m_netPoint_TypeB[8 + i].GetPA(),
            cp, cv::Point2d(cp.x, cp.y + 1));
        m_netPoint_TypeB[4 + i].SetPA(cp);

        cp = CrossPoint(m_netPoint_TypeB[3 + i].GetPA(), m_netPoint_TypeB[11 + i].GetPA(),
            cp, cv::Point2d(cp.x, cp.y + 1));
        m_netPoint_TypeB[5 + i].SetPA(cp);

        cp = CrossPoint(m_netPoint_TypeB[3 + i].GetPA(), m_netPoint_TypeB[8 + i].GetPA(),
            m_netPoint_TypeB[1 + i].GetPA(), m_netPoint_TypeB[9 + i].GetPA());
        cp = CrossPoint(m_netPoint_TypeB[0 + i].GetPA(), m_netPoint_TypeB[8 + i].GetPA(),
            cp, cv::Point2d(cp.x, cp.y + 1));
        m_netPoint_TypeB[6 + i].SetPA(cp);

        cp = CrossPoint(m_netPoint_TypeB[3 + i].GetPA(), m_netPoint_TypeB[11 + i].GetPA(),
            cp, cv::Point2d(cp.x, cp.y + 1));
        m_netPoint_TypeB[7 + i].SetPA(cp);
    }
    return true;
}

inline bool Grid::CalculatePoints_radius()
{
    cv::Point2d pointA{};
    int k = 0;
    for (int i = 0; i < LAYOUT_NET_TYPE_A; ++i)
    {
        for (int j = 0; j < m_layoutNetNum_TypeA[i]; ++j)
        {
            if (m_layoutNetNum_TypeA[i] == 2 && j == 1) { j += 2; }
            pointA = cv::Point2d(m_center.x + sqrt(0.5) * m_radius * (2.0 / 3 * i - 1),
                m_center.y + sqrt(0.5) * m_radius * (2.0 / 3 * j - 1));
            m_netPoint_TypeA[k++].SetPA(pointA);
        }
    }
    k = 0;
    for (int i = 0; i < LAYOUT_BLOCK_TYPE_A; ++i)
    {
        for (int j = 0; j < m_layoutBlockNum_TypeA[i]; ++j)
        {
            pointA = cv::Point2d(m_center.x + (2.0 / 3) * sqrt(0.5) * m_radius * (i - 1),
                m_center.y + (2.0 / 3) * sqrt(0.5) * m_radius * (j - 1));
            m_blockPoint_TypeA[k++].SetPA(pointA);
        }
    }
    k = 0;
    for (int i = 0; i < LAYOUT_NET_TYPE_B; ++i)
    {
        for (int j = 0; j < m_layoutNetNum_TypeB[i]; ++j)
        {
            if (m_layoutNetNum_TypeB[i] == 2 && j == 1) { j += 2; }
            pointA = cv::Point2d(m_center.x + m_radius * (sqrt(2.0 / 27) * i - sqrt(2.0 / 3)),
                m_center.y + m_radius * (sqrt(4.0 / 27) * j - sqrt(1.0 / 3)));
            m_netPoint_TypeB[k++].SetPA(pointA);
        }
    }
    return true;
}

inline bool Grid::CalculateBlockPointTypeB()
{
    cv::Point2d pointA{};
    int k = 0;
    for (int i = 0; i < 2; ++i)
    {
        if (i == 1) { i = 8; }
        pointA = CrossPoint(m_netPoint_TypeB[0 + i].GetPA(), m_netPoint_TypeB[11 + i].GetPA(),
            m_netPoint_TypeB[1 + i].GetPA(), m_netPoint_TypeB[4 + i].GetPA());
        m_blockPoint_TypeB[k++].SetPA(pointA);

        pointA = CrossPoint(m_netPoint_TypeB[4 + i].GetPA(), m_netPoint_TypeB[10 + i].GetPA(),
            m_netPoint_TypeB[2 + i].GetPA(), m_netPoint_TypeB[6 + i].GetPA());
        m_blockPoint_TypeB[k++].SetPA(pointA);

        pointA = CrossPoint(m_netPoint_TypeB[6 + i].GetPA(), m_netPoint_TypeB[9 + i].GetPA(),
            m_netPoint_TypeB[3 + i].GetPA(), m_netPoint_TypeB[8 + i].GetPA());
        m_blockPoint_TypeB[k++].SetPA(pointA);

        pointA = CrossPoint(m_netPoint_TypeB[2 + i].GetPA(), m_netPoint_TypeB[6 + i].GetPA(),
            m_netPoint_TypeB[1 + i].GetPA(), m_netPoint_TypeB[7 + i].GetPA());
        m_blockPoint_TypeB[k++].SetPA(pointA);

        pointA = CrossPoint(m_netPoint_TypeB[0 + i].GetPA(), m_netPoint_TypeB[11 + i].GetPA(),
            m_netPoint_TypeB[3 + i].GetPA(), m_netPoint_TypeB[8 + i].GetPA());
        m_blockPoint_TypeB[k++].SetPA(pointA);

        pointA = CrossPoint(m_netPoint_TypeB[4 + i].GetPA(), m_netPoint_TypeB[10 + i].GetPA(),
            m_netPoint_TypeB[5 + i].GetPA(), m_netPoint_TypeB[9 + i].GetPA());
        m_blockPoint_TypeB[k++].SetPA(pointA);

        pointA = CrossPoint(m_netPoint_TypeB[2 + i].GetPA(), m_netPoint_TypeB[5 + i].GetPA(),
            m_netPoint_TypeB[3 + i].GetPA(), m_netPoint_TypeB[8 + i].GetPA());
        m_blockPoint_TypeB[k++].SetPA(pointA);

        pointA = CrossPoint(m_netPoint_TypeB[5 + i].GetPA(), m_netPoint_TypeB[9 + i].GetPA(),
            m_netPoint_TypeB[1 + i].GetPA(), m_netPoint_TypeB[7 + i].GetPA());
        m_blockPoint_TypeB[k++].SetPA(pointA);

        pointA = CrossPoint(m_netPoint_TypeB[0 + i].GetPA(), m_netPoint_TypeB[11 + i].GetPA(),
            m_netPoint_TypeB[7 + i].GetPA(), m_netPoint_TypeB[10 + i].GetPA());
        m_blockPoint_TypeB[k++].SetPA(pointA);
    }
    return true;
}

inline bool Grid::CalculateNetPointTypeB_aspectRatio_perspective()
{
    m_netPoint_TypeB[0].SetAxPercentKeepLength(m_aspectRatio);
    m_netPoint_TypeB[3].SetAxPercentKeepLength(m_aspectRatio);
    m_netPoint_TypeB[16].SetAxPercentKeepLength(m_aspectRatio);
    m_netPoint_TypeB[19].SetAxPercentKeepLength(m_aspectRatio);
    double y0 = m_netPoint_TypeB[0].GetPA().y;
    double deltaY = y0 - m_center.y;
    for (int i = 8; i < 12; ++i)
    {
        m_netPoint_TypeB[i].SetAyKeepAx(y0 - deltaY / 3 * 2 * (i - 8));
    }

    m_netPoint_TypeB[0].SetAyPercentKeepLength(m_perspective);
    m_netPoint_TypeB[3].SetAyPercentKeepLength(m_perspective);
    m_netPoint_TypeB[16].SetAyPercentKeepLength(m_perspective);
    m_netPoint_TypeB[19].SetAyPercentKeepLength(m_perspective);
    return true;
}

inline bool Grid::CalculatePoints_rotate()
{
    for (auto& NPA : m_netPoint_TypeA)
    {
        NPA.AddAngle(m_rotateAngle);
    }
    for (auto& BPA : m_blockPoint_TypeA)
    {
        BPA.AddAngle(m_rotateAngle);
    }
    for (auto& NPB : m_netPoint_TypeB)
    {
        NPB.AddAngle(m_rotateAngle);
    }
    m_zeroDirection.SetAngle(m_rotateAngle);
    return true;
}
