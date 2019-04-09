#include "colorDefine.hpp"

const cv::Scalar g_bgrBlack (  0,   0,   0);
const cv::Scalar g_bgrWhite (255, 255, 255);
const cv::Scalar g_bgrRed   (  0,   0, 255);
const cv::Scalar g_bgrOrange(  0, 128, 255);
const cv::Scalar g_bgrYellow(  0, 255, 255);
const cv::Scalar g_bgrGreen (  0, 255,   0);
const cv::Scalar g_bgrBlue  (255,   0,   0);

// hsv: 色调(Hue) 饱和度(Saturation) 亮度(Value)
const HSV g_hsvAll(
    cv::Scalar(  0,   0,   0),
    cv::Scalar(180, 255, 255),
    g_bgrBlack,
    "ALL"
);

HSV g_hsvWhite(
    cv::Scalar(  0,  0,  65),
    cv::Scalar(180, 50, 255),
    g_bgrWhite,
    "White"
);

HSV g_hsvRed(
    cv::Scalar(170, 160,  55),
    cv::Scalar( 15, 255, 140),
    g_bgrRed,
    "Red"
);

HSV g_hsvOrange(
    cv::Scalar(170, 175, 180),
    cv::Scalar( 10, 255, 255),
    g_bgrOrange,
    "Orange"
);

HSV g_hsvYellow(
    cv::Scalar(25, 145, 150),
    cv::Scalar(45, 255, 255),
    g_bgrYellow,
    "Yellow"
);

HSV g_hsvGreen(
    cv::Scalar(45, 150,  95),
    cv::Scalar(70, 255, 255),
    g_bgrGreen,
    "Green"
);

HSV g_hsvBlue(
    cv::Scalar( 90, 110,  80),
    cv::Scalar(110, 255, 255),
    g_bgrBlue,
    "Blue"
);

std::vector<HSV> g_hsvColor_U{
    g_hsvAll, g_hsvWhite, g_hsvRed, g_hsvOrange, g_hsvYellow, g_hsvGreen, g_hsvBlue
};

std::vector<HSV> g_hsvColor_F{
    g_hsvAll, g_hsvWhite, g_hsvRed, g_hsvOrange, g_hsvYellow, g_hsvGreen, g_hsvBlue
};

std::vector<HSV> g_hsvColor_B{
    g_hsvAll, g_hsvWhite, g_hsvRed, g_hsvOrange, g_hsvYellow, g_hsvGreen, g_hsvBlue
};
