#include "cubeTracker.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::fstream;

using cv::Mat;
using cv::Point2f;

//int g_writeTime{ GetFileWriteTime("GET.txt") };
std::string g_faceletStrings[6]{};

inline void SendToTxt(string filePath, string str)
{
    fstream file{ filePath };
    if (file.good())
    {
        file << str << endl;
        file.close();
    }
    else
    {
        return;
    }
}

inline bool GetV3(const string &value, cv::Scalar &color)
{
    if (value.empty()) { return false; }
    // _(255,_255,_255)
    // 0123456789abcdef
    color = cv::Scalar(
        ((value[2] - '0') * 100 + (value[3] - '0') * 10 + (value[4] - '0')),
        ((value[7] - '0') * 100 + (value[8] - '0') * 10 + (value[9] - '0')),
        ((value[12] - '0') * 100 + (value[13] - '0') * 10 + (value[14] - '0'))
    );
    return true;
}

inline bool GetV1(const string &value, int &Trackbar)
{
    if (value.empty()) { return false; }
    Trackbar = ((value[1] - '0') * 100 + (value[2] - '0') * 10 + (value[3] - '0'));
    return true;
}

inline void init() // 初始化
{
    for (int i = 0; i < 6; i++) { g_faceletStrings[i] = "?"; } // 初始化faceletStrings字符
}


