#pragma once
#include <opencv2/opencv.hpp>
#include "Grid.hpp"
#include "colorDefine.hpp"
#include "RccSetter.hpp"

inline bool MatIsEqual(const cv::Mat mat1, const cv::Mat mat2)
{
    if (mat1.empty() && mat2.empty())
    { return true; }
    if (mat1.cols != mat2.cols || mat1.rows != mat2.rows || mat1.dims != mat2.dims || mat1.channels() != mat2.channels())
    { return false; }
    if (mat1.size() != mat2.size() || mat1.channels() != mat2.channels() || mat1.type() != mat2.type())
    { return false; }
    size_t nrOfElements1 = mat1.total() * mat1.elemSize();
    if (nrOfElements1 != mat2.total() * mat2.elemSize())
    { return false; }
    return (memcmp(mat1.data, mat2.data, nrOfElements1) == 0);
}

template<typename T>
struct Data3 {
    T data[3];
    T operator[](int i) const{
        return data[i];
    }
    T& operator[](int i) {
        return data[i];
    }
};

class Rcc // RubiksCubeCapturer
{
public:
    Rcc(RccSet rccSetter) :
        m_camera(),
        m_frame(),
        m_blurMask(),
        m_hsvMask(),
        m_grid(),
        m_x(0), m_y(0),
        m_rccSet(rccSetter),
        m_colorStr("?") {
        m_camera.open(rccSetter.m_index);
        m_camera.set(CV_CAP_PROP_FRAME_WIDTH, 640 * 0.5); // default 640
        m_camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480 * 0.5); // default 480
        m_camera.set(CV_CAP_PROP_BRIGHTNESS, 0); // default 0
        m_camera.set(CV_CAP_PROP_CONTRAST, 50); // default 50
        m_camera.set(CV_CAP_PROP_EXPOSURE, -5); // default -4 // 曝光越低越流畅的样子？
        m_camera.set(CV_CAP_PROP_SATURATION, 64); // default 64
        m_camera.set(CV_CAP_PROP_HUE, 0); // default 0
        m_camera.set(CV_CAP_PROP_FPS, 30); // default 0
    }

    Rcc(const Rcc& r) = delete;
    Rcc& operator= (const Rcc& r) = delete;

    const cv::VideoCapture& GetVideoCapture() const { return m_camera; }
    const cv::Mat& GetFrame()    const { return m_frame;    }
    const cv::Mat& GetBlurMask() const { return m_blurMask; }
    const cv::Mat& GetHsvMask()  const { return m_hsvMask;  }
    const std::string& GetColorString() const { return m_colorStr; }

    bool Setting(RccSet rccSet)
    {
        if (rccSet.m_index != m_rccSet.m_index)
        {
            m_camera.open(rccSet.m_index);
        }
        m_rccSet = rccSet;
        return true;
    }

    const cv::Mat& ReadVideoFrame();
    bool DrawGrid();
    const std::string& ReadColorStr();
    void ReleaseMat();
    void ChangeBrightness();
    void ChangeContrast();
    Data3<float> m_whiteBlance[18];

private:
    bool ImgProc();
    void ROI_SafeRange();
    void FindAndDrawColor(std::string &saveCubeColor);

    cv::VideoCapture m_camera;
    cv::Mat          m_frame;
    cv::Mat          m_blurMask;
    cv::Mat          m_hsvMask;
    Grid             m_grid;
    RccSet           m_rccSet;
    std::string      m_colorStr;
    int              m_x, m_y;
};

inline void Rcc::ChangeBrightness()
{
    m_camera.set(CV_CAP_PROP_BRIGHTNESS, m_rccSet.m_brightness);
}

inline void Rcc::ChangeContrast()
{
    m_camera.set(CV_CAP_PROP_CONTRAST, m_rccSet.m_contrast);
}

inline void Rcc::ReleaseMat()
{
    m_frame.release();
    m_blurMask.release();
    m_hsvMask.release();
}

inline const cv::Mat& Rcc::ReadVideoFrame()
{
    cv::Mat framePre = m_frame;
    m_camera >> m_frame;
    if (MatIsEqual(framePre, m_frame))
    {
        m_camera.open(m_rccSet.m_index);
    }
    if (m_frame.empty()) { return m_frame; }
    cv::resize(m_frame, m_frame, cv::Size(m_rccSet.m_cols, m_rccSet.m_rows));
    this->ImgProc();
    return m_frame;
}

//void AutoWhiteBalance(cv::Mat& img, float PR, float PG, float PB) {
//    std::vector<cv::Mat> whiteBalance;
//    cv::split(img, whiteBalance);
//    double B = mean(whiteBalance[0])[0];
//    double G = mean(whiteBalance[1])[0];
//    double R = mean(whiteBalance[2])[0];
//    double KB = (R + G + B) / (3 * B);
//    double KG = (R + G + B) / (3 * G);
//    double KR = (R + G + B) / (3 * R);
//    whiteBalance[0] = whiteBalance[0] * KB * PB;
//    whiteBalance[1] = whiteBalance[1] * KG * PG;
//    whiteBalance[2] = whiteBalance[2] * KR * PR;
//    merge(whiteBalance, img);
//}

void WhiteBalance(cv::Mat& img, const Data3<float>& rgb) {
    std::vector<cv::Mat> whiteBalance;
    cv::split(img, whiteBalance);
    whiteBalance[0] = whiteBalance[0] * rgb[2];
    whiteBalance[1] = whiteBalance[1] * rgb[1];
    whiteBalance[2] = whiteBalance[2] * rgb[0];
    merge(whiteBalance, img);
}

inline bool Rcc::ImgProc()
{
    if (m_frame.empty())
    {
        return false;
    }

    cv::GaussianBlur(
        m_frame,
        m_blurMask,
        cv::Size(m_rccSet.m_blurSize * 2 + 1, m_rccSet.m_blurSize * 2 + 1),
        0);

    // AutoWhiteBalance(m_blurMask, m_rccSet.m_pRGB[0], m_rccSet.m_pRGB[1], m_rccSet.m_pRGB[2]); // 自动白平衡(灰度世界法)
    cv::cvtColor(m_blurMask, m_hsvMask, cv::COLOR_BGR2HSV); // BGR转HSV

    return true;
}

inline bool Rcc::DrawGrid()
{
    ROI_SafeRange();
    const cv::Mat ROIcircular{ cv::Mat::zeros(m_frame.size(), CV_8UC1) };
    cv::circle(
        ROIcircular,
        cv::Point(m_x + m_rccSet.m_radius, m_y + m_rccSet.m_radius),
        m_rccSet.m_radius,
        cv::Scalar(255, 255, 255),
        -1
    );
    cv::subtract(m_blurMask, m_blurMask, m_frame, ROIcircular);
    m_blurMask.copyTo(m_frame, ROIcircular);
    m_grid.Setting(
        cv::Point2d(m_x + m_rccSet.m_radius, m_y + m_rccSet.m_radius),
        m_rccSet.m_radius,
        m_rccSet.m_type,
        m_rccSet.m_rotateAngle,
        m_rccSet.m_perspective,
        m_rccSet.m_aspectRatio
    );
    m_grid.DrawGrid(m_frame);

    return true;
}

inline const std::string& Rcc::ReadColorStr()
{
    if (m_frame.empty()) 
    {
        return m_colorStr = std::string(18, ' ');
    }

    std::string saveColor;
    if (m_rccSet.m_type == Grid::GridType::TYPE_A)
    {
        saveColor = std::string(9, '?') + std::string(9, ' ');
    }
    else if (m_rccSet.m_type == Grid::GridType::TYPE_B)
    {
        saveColor = std::string(18, '?');
    }

    FindAndDrawColor(saveColor);
    m_colorStr = saveColor;
    return m_colorStr;
}

inline void Rcc::ROI_SafeRange()
{
    if (m_rccSet.m_radius < 1) { m_rccSet.m_radius = 1; }
    if (m_rccSet.m_radius > m_frame.rows / 2) { m_rccSet.m_radius = m_frame.rows / 2; }

    m_x = m_frame.cols / 2 - m_rccSet.m_radius - m_rccSet.m_horizontal;
    if (m_x < 0) { m_x = 0; }
    if (m_x > m_frame.cols) { m_x = m_frame.cols; }

    m_y = m_frame.rows / 2 - m_rccSet.m_radius - m_rccSet.m_vertical;
    if (m_y < 0) { m_y = 0; }
    if (m_y > m_frame.rows) { m_y = m_frame.rows; }

    int w{ 2 * m_rccSet.m_radius };
    int h{ 2 * m_rccSet.m_radius };
    if (m_x + w > m_frame.cols) { m_x = m_frame.cols - w; }
    if (m_y + h > m_frame.rows) { m_y = m_frame.rows - h; }

    m_rccSet.m_horizontal = m_frame.cols / 2 - m_rccSet.m_radius - m_x;
    m_rccSet.m_vertical = m_frame.rows / 2 - m_rccSet.m_radius - m_y;
}

inline void Rcc::FindAndDrawColor(std::string &saveCubeColor)
{
    std::vector<cv::Point2d> block{};
    for (const auto& bp : m_grid.GetBPs())
    {
        block.push_back(bp.GetPA());
    }

    size_t colorIdxMin{}, colorIdxMax{};
    if (m_rccSet.m_testMode == true)
    {
        colorIdxMin = 0;
        colorIdxMax = 1;
    }
    else
    {
        colorIdxMin = 1;
        colorIdxMax = m_rccSet.m_colors.size();
    }

    cv::Mat rangeMaskA{}, rangeMaskB{};
    for (size_t colorIdx = colorIdxMin; colorIdx < colorIdxMax; ++colorIdx) // 颜色循环
    {
        if (m_rccSet.m_whiteBlanceMode == true) {
            WhiteBalance(m_blurMask, m_whiteBlance[colorIdx]);
            cv::cvtColor(m_blurMask, m_hsvMask, cv::COLOR_BGR2HSV);
        }

        cv::inRange(m_hsvMask, m_rccSet.m_colors[colorIdx].Lo,     m_rccSet.m_colors[colorIdx].mid_Hi, rangeMaskA); // 找到指定范围内的颜色
        cv::inRange(m_hsvMask, m_rccSet.m_colors[colorIdx].mid_Lo, m_rccSet.m_colors[colorIdx].Hi,     rangeMaskB);

        for (size_t blockIdx = 0; blockIdx < block.size(); ++blockIdx) // 方块循环
        {
            if (rangeMaskA.at<uchar>(block[blockIdx]) > 0 || rangeMaskB.at<uchar>(block[blockIdx]) > 0)
            {
                if (m_rccSet.m_testMode == false)
                {
                    saveCubeColor[blockIdx] = m_rccSet.m_colors[colorIdx].name[0];           // save color
                    circle(m_frame, block[blockIdx], 5, m_rccSet.m_colors[colorIdx].bgr, 2); // print cicle
                }
            }
            // 测试模式
            if (m_rccSet.m_testMode == true)
            {
                std::string colorData[3];
                Data3<uchar> rgbData{ 0, 0, 0 };
                // HSV模式
                if (m_rccSet.m_rgbMode == false) for (int i = 0; i < 3; ++i)
                    colorData[i] = std::to_string(m_hsvMask.at<cv::Vec3b>(block[blockIdx])[i]);
                // RGB模式
                else for (int i = 0; i < 3; ++i) {
                    rgbData[3 - i - 1] = m_blurMask.at<cv::Vec3b>(block[blockIdx])[i];
                    colorData[3 - i - 1] = std::to_string(rgbData[3 - i - 1]);
                    if (m_rccSet.m_whiteBlanceMode == false) {
                        // 获取每个色块的白平衡数据 -- 190417
                        m_whiteBlance[blockIdx][3 - i - 1] = 255.f / (rgbData[3 - i - 1] + 0.1f);
                    }
                }
                for (int numPos = 0; numPos < 3; ++numPos) {
                    cv::Point2d posAdd(-10.0, numPos * 10.0 - 10);
                    putText(m_frame,
                        colorData[numPos],
                        block[blockIdx] + posAdd,
                        cv::HersheyFonts::FONT_HERSHEY_DUPLEX,
                        0.4,
                        cv::Scalar(0, 200, 255),
                        1,
                        cv::LineTypes::LINE_8);
                }
            }
        }
    }
}
