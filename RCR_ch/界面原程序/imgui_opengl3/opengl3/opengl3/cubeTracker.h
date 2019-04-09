#pragma once

#include <opencv2/opencv.hpp>
#include <windows.h>
#include <fstream>
#include "colorDefine.hpp"

extern std::string g_faceletStrings[6]; // 颜色识别结果保存

void init();
