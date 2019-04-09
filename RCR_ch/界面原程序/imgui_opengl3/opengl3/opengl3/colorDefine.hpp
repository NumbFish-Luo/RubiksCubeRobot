#pragma once

#include <opencv2/opencv.hpp>

class HSV
{
public:
    HSV(const cv::Scalar& t_Lo, const cv::Scalar& t_Hi, const cv::Scalar& t_bgr, const std::string& t_name);
    HSV(const HSV& hsv);
    HSV& operator= (const HSV& hsv);

    void RefreshData();
    void ReadColorFromFile(const std::string& path, const std::string& label);
    void WriteColorToFile(std::ofstream& fout, const std::string& label);

    cv::Scalar Lo;
    cv::Scalar Hi;

    cv::Scalar mid_Lo;
    cv::Scalar mid_Hi;

    cv::Scalar bgr;

    std::string name;

    int m_dataSave[2][3];
};

inline HSV::HSV(const cv::Scalar& t_Lo, const cv::Scalar& t_Hi, const cv::Scalar& t_bgr, const std::string& name)
    :Lo(t_Lo), Hi(t_Hi), bgr(t_bgr), name(name)
{
    for (int i = 0; i < 3; ++i)
    {
        m_dataSave[0][i] = (int)Lo[i];
        m_dataSave[1][i] = (int)Hi[i];
    }

    if (Lo[0] > Hi[0])
    {
        /*
        例如：
        Lo = (170, 210,  55),
        Hi = ( 15, 255, 120)
        则：
        mid_Lo = (  0, 210,  55),
        mid_Hi = (180, 255, 120)
        取区域为：
        (Lo, mid_Hi) U (mid_Lo, Hi)
        即：
        (170, 210,  55) ~ (180, 255, 120) 并
        (  0, 210,  55) ~ ( 15, 255, 120)
        */
        mid_Lo = cv::Scalar(0, Lo[1], Lo[2]);
        mid_Hi = cv::Scalar(180, Hi[1], Hi[2]);
    }
    else
    {
        /*
        例如：
        Lo = ( 45, 210,  95),
        Hi = ( 70, 255, 165)
        则：
        mid_Lo = ( 45, 210,  95),
        mid_Hi = ( 70, 255, 165)
        取区域为：
        (Lo, mid_Hi) U (mid_Lo, Hi)
        即：
        ( 45, 210,  95) ~ ( 70, 255, 165) 并
        ( 45, 210,  95) ~ ( 70, 255, 165)
        */
        mid_Lo = Lo;
        mid_Hi = Hi;
    }
}

inline HSV::HSV(const HSV& hsv)
    : Lo(hsv.Lo), Hi(hsv.Hi), mid_Lo(hsv.mid_Lo), mid_Hi(hsv.mid_Hi), bgr(hsv.bgr), name(hsv.name), m_dataSave()
{
    for (size_t i = 0; i < 2; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            m_dataSave[i][j] = hsv.m_dataSave[i][j];
        }
    }
}

inline HSV& HSV::operator= (const HSV& hsv)
{
    Lo = hsv.Lo;
    Hi = hsv.Hi;
    mid_Lo = hsv.mid_Lo;
    mid_Hi = hsv.mid_Hi;
    bgr = hsv.bgr;
    name = hsv.name;
    for (size_t i = 0; i < 2; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            m_dataSave[i][j] = hsv.m_dataSave[i][j];
        }
    }
    return *this;
}

inline void HSV::RefreshData()
{
    Lo = cv::Scalar(m_dataSave[0][0], m_dataSave[0][1], m_dataSave[0][2]);
    Hi = cv::Scalar(m_dataSave[1][0], m_dataSave[1][1], m_dataSave[1][2]);
    if (Lo[0] > Hi[0])
    {
        mid_Lo = cv::Scalar(0, Lo[1], Lo[2]);
        mid_Hi = cv::Scalar(180, Hi[1], Hi[2]);
    }
    else
    {
        mid_Lo = Lo;
        mid_Hi = Hi;
    }
}

inline void HSV::ReadColorFromFile(const std::string &path, const std::string& label)
{
    std::ifstream fin{ path };
    if (!fin.good())
    {
        std::cout << "找不到" << path << std::endl;
        fin.close();
        return;
    }
    char lineChar[256];
    while (!fin.eof())
    {
        fin.getline(lineChar, 256);
        std::string lineString = lineChar;
        size_t index = lineString.find('=');
        if (index != (size_t)-1)
        {
            if (label[0] == lineString[0] && name == lineString.substr(1, index - 1))
            {
                std::string valueString = lineString.substr(index + 1, lineString.size() - index);
                int valueInt[8]{};
                int valueIntIndex = 7;
                int t = 1;

                for (size_t i = valueString.size() - 2; i != 0; --i)
                {
                    if (valueString[i] == ' ' || valueString[i] == ',' || valueString[i] == '[' || valueString[i] == ']')
                    {
                        --valueIntIndex;
                        --i;
                        t = 1;
                    }
                    else
                    {
                        valueInt[valueIntIndex] += (valueString[i] - '0') * t;
                        t *= 10;
                    }

                    if (valueIntIndex == -1)
                    {
                        std::cout << "INI_ERROR!" << std::endl;
                        fin.close();
                        return;
                    }
                }

                for (int i = 0; i < 3; ++i)
                {
                    m_dataSave[0][i] = valueInt[i];
                    m_dataSave[1][i] = valueInt[i + 4];
                }
                RefreshData();
                fin.close();
                return;
            }
        }
    }
    fin.close();
    return;
}

inline void HSV::WriteColorToFile(std::ofstream& fout, const std::string& label)
{
    fout << label << name << "=" << Lo << Hi << std::endl;
}

extern const cv::Scalar g_bgrBlack;
extern const cv::Scalar g_bgrWhite;
extern const cv::Scalar g_bgrRed;
extern const cv::Scalar g_bgrOrange;
extern const cv::Scalar g_bgrYellow;
extern const cv::Scalar g_bgrGreen;
extern const cv::Scalar g_bgrBlue;

// hsv: 色调(Hue) 饱和度(Saturation) 亮度(Value)
extern const HSV g_hsvAll; // 测试用

extern HSV g_hsvWhite;
extern HSV g_hsvRed;
extern HSV g_hsvOrange;
extern HSV g_hsvYellow;
extern HSV g_hsvGreen;
extern HSV g_hsvBlue;

extern std::vector<HSV> g_hsvColor_U;
extern std::vector<HSV> g_hsvColor_F;
extern std::vector<HSV> g_hsvColor_B;
