// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"
// #include <stdio.h>
// #include <GL/gl3w.h>
// #include <GLFW/glfw3.h>

#include "UI.h"

#include <cmath>
#include <opencv2/opencv.hpp>
#include <windows.h>

#include "colorDefine.hpp"
#include "Config.h"
#include "cubeTracker.h"
#include "Timer.hpp"
#include "Serial.h"
#include "Grid.hpp"
#include "RubiksCubeCapturer.hpp"
#include "k_KociMainFunc.h"

// #define DEBUGGING

struct SetVSlider {
    SetVSlider()
        : label(), value(NULL), min(0), max(0), tips() {}
    SetVSlider(const std::string _label, int* _value, const int _min, const int _max, const std::string _tips)
        : label(_label), value(_value), min(_min), max(_max), tips(_tips) {}
    SetVSlider(const SetVSlider& s)
        : label(s.label), value(s.value), min(s.min), max(s.max), tips(s.tips) {}
    SetVSlider& operator=(const SetVSlider&) = delete;
    const std::string label;
    int*              value;
    const int         min;
    const int         max;
    const std::string tips;
};

const std::string    TITLE_NAME{ u8" 魔方机器人" };
const std::string CUBE_EXE_NAME{ "opengl3_3DRubiksCube.exe" };
const std::string    LINKS_NAME{ "start www.zhbit.com" };
static const int COLS{ 512 }, ROWS{ 384 };

enum CubeSide {
    U, D, F, B, L, R
};
std::vector<std::string> g_cubeColor{}; // U D F B L R
std::vector<std::string> g_faceletStr{};

extern IOBuf g_IOBuf; // 外部变量，不要初始化
extern int   g_sendNext;

int          g_serialPortIdx{ 1 };
bool         g_showMainWindow{ true };
bool         g_serialIsOpened{ false };
bool         g_openSerial{ true };
RccSet       g_rccSet_U{ 2, Gtype::TYPE_B, g_hsvColor_U, COLS, ROWS };
RccSet       g_rccSet_F{ 1, Gtype::TYPE_A, g_hsvColor_F, COLS, ROWS };
RccSet       g_rccSet_B{ 3, Gtype::TYPE_A, g_hsvColor_B, COLS, ROWS };
Rcc          g_rcc_U{ g_rccSet_U };
Rcc          g_rcc_F{ g_rccSet_F };
Rcc          g_rcc_B{ g_rccSet_B };
std::string  g_serialTips{};
CSerialPort  g_mySerialPort{};
ImFont*      g_smallFont{};
ImFont*      g_bigFont{};
int          g_nowStep{ -1 };
DWORD        g_timerPre{};
bool         g_stopButton{ false };

// 函数声明
ImTextureID GetMatTextureID(const cv::Mat& mat, GLuint& tex);
static void GlfwErrorCallback(int error, const char* description);
int  CallBackInputWrap(ImGuiTextEditCallbackData* data);
bool Init(GLFWwindow*& window);
void CleanUp(GLFWwindow*& window);
void ColorConfiger(HSV& color);
void Render(GLFWwindow* window, const ImVec4& clear_color);
void SerialMain();
bool ConfigWindow(const std::string& label, const std::string& treeNodeName, Rcc& rcc, RccSet& rccSet, const int cols, const int rows);
void SaveConfig(std::ofstream& fout, const std::string& label, const RccSet& rccSetter);
void ReadConfig(const Config& config, const std::string& label, const RccSet& rccSetter);
bool ColorIdentify_1stTime(const std::string& colorStr_U, const std::string& colorStr_F, const std::string& colorStr_B, bool reset = false);
bool ColorIdentify_2ndTime(const std::string& colorStr_U, const std::string& colorStr_F, const std::string& colorStr_B, bool reset = false);
void SendMsgToSerial();
void Reset(int step);
void ShowMainWindow(const std::string& title, const cv::Mat& matL, const cv::Mat& matM, const cv::Mat& matR, const int cols, const int rows);
void InitCubeInputTxt();
void ChangeCubeColorsInput();
void NextCubeStep();
void Help();
int GetFileWriteTime(const LPCTSTR& lpszFilePath);

inline void Help() {
    std::string help[]{
        u8"-------------------------------------------------- 必读说明 --------------------------------------------------\n"
        u8"1. COM数字一直在跳是通讯异常导致的。此时请切断电源重启机器，并关闭操作界面重新测试。\n"
        u8"2. 按下Start后计时器正常计时，但机器不动，可能是摄像头配置出错了。请检查摄像头序号是否正确，若不正确请调整序号。\n"
        u8"3. 每次启动机器后时请调整好电机的角度，若没有调整好角度，很有可能会发生碰撞。\n"
        u8"4. 准备关闭机器时，建议先调整好电机角度至初始位置再关机。\n"
        u8"5. 机器不使用时，请用布盖住以防尘。\n\n"
        u8"-------- 指令集 --------\n",
        u8">> LM+\t-> L正转\n",
        u8">> LM-\t-> L反转\n",
        u8">> LM2\t-> L180度旋转\n",
        u8">> LC+\t-> L夹紧\n",
        u8">> LC-\t-> L松开\n\n",
        u8">> DM+\t-> D正转\n",
        u8">> DM-\t-> D反转\n",
        u8">> DM2\t-> D180度旋转\n",
        u8">> DC+\t-> D夹紧\n",
        u8">> DC-\t-> D松开\n" };
    for (const auto& h : help) {
        g_IOBuf.PushBack(h);
    }
}

inline int GetFileWriteTime(const LPCTSTR& lpszFilePath) {
    WIN32_FIND_DATA FindFileData = { 0 };

    HANDLE hFile = ::FindFirstFile(lpszFilePath, &FindFileData);

    if (INVALID_HANDLE_VALUE == hFile) {
        return -1;
    }
    SYSTEMTIME WriteTime = { 0 }; // 最近修改时间
    if (!::FileTimeToSystemTime(&FindFileData.ftLastWriteTime, &WriteTime)) {
        return -1;
    }

    int time = WriteTime.wMilliseconds + WriteTime.wSecond * 1000;
    return time;
}

inline void NextCubeStep() {
    std::ofstream next{ "NEXT.txt" };
    next.clear();
    next << g_sendNext;
    next.close();
}

inline void ChangeCubeColorsInput() {
    std::ofstream input("INPUT.txt");
    input << "U:" + g_cubeColor[U] + '\n'
        << "D:" + g_cubeColor[D] + '\n'
        << "F:" + g_cubeColor[F] + '\n'
        << "B:" + g_cubeColor[B] + '\n'
        << "L:" + g_cubeColor[L] + '\n'
        << "R:" + g_cubeColor[R];
    input.close();
}

inline void InitCubeInputTxt() {
    for (auto& c : g_cubeColor) {
        c = std::string(9, '?');
    }
    for (auto& f : g_faceletStr) {
        f = "";
    }
    std::ofstream input("INPUT.txt");
    input << "U:YYYYYYYYY\n"
          << "D:WWWWWWWWW\n"
          << "F:RRRRRRRRR\n"
          << "B:OOOOOOOOO\n"
          << "L:BBBBBBBBB\n"
          << "R:GGGGGGGGG";
    input.close();
    std::ofstream next("NEXT.txt");
    next << (g_sendNext = 0);
    next.close();
}

inline void SendMsgToSerial() {
    if (g_IOBuf.serialBufferChange == true) {
        std::string msg{};
        for (int i = 0; i < strlen(g_IOBuf.serialBuffer); ++i) {
            if (g_IOBuf.serialBuffer[i] == '\n' ||
                g_IOBuf.serialBuffer[i] == '\r' ||
                g_IOBuf.serialBuffer[i] == ' ') {
                continue;
            }
            msg.push_back(g_IOBuf.serialBuffer[i]);
        }
        msg += '\r';
        g_mySerialPort.WriteData((unsigned char*)(msg).c_str(), static_cast<unsigned int>((msg).size()));
        g_IOBuf.serialBufferChange = false;
    }
}

inline void Reset(int step) {
    std::string tmp(9, '?');
    ColorIdentify_1stTime(tmp + tmp, tmp, tmp, true);
    ColorIdentify_2ndTime(tmp + tmp, tmp, tmp, true);
    for (auto& cc : g_cubeColor) {
        cc = tmp;
    }
    g_nowStep = step;
    g_sendNext = 0;
    InitCubeInputTxt();
    g_timerPre = GetTickCount();
    g_stopButton = false;
}

inline bool ColorIdentify_1stTime(const std::string& colorStr_U, const std::string& colorStr_F, const std::string& colorStr_B, bool reset) {
    /*
    L       D
    2 5 8   0 1 2
    1 4 7   3 4 5
    0 3 6   6 7 8
    */
    static bool mappingOK_L{ false };
    if (mappingOK_L == false) {
        bool foundOK{ true };
        for (int i = 0; i < 9; ++i) {
            if (colorStr_U[i] == '?') {
                foundOK = false;
                break;
            }
        }
        if (foundOK == true) {
            const size_t mapping[]{ 2, 5, 8, 1, 4, 7, 0, 3, 6 };
            for (int i = 0; i < 9; ++i) {
                g_cubeColor[L][mapping[i]] = colorStr_U[i];
            }
            mappingOK_L = true;
        }
    }

    /*
    L       D
    2 5 8   0 1 2
    1 4 7   3 4 5
    0 3 6   6 7 8
    */
    static bool mappingOK_D{ false };
    if (mappingOK_D == false) {
        bool foundOK = true;
        for (int i = 0; i < 9; ++i) {
            if (colorStr_U[i + 9] == '?') {
                foundOK = false;
                break;
            }
        }
        if (foundOK == true) {
            const size_t mapping[]{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };
            for (int i = 0; i < 9; ++i) {
                g_cubeColor[D][mapping[i]] = colorStr_U[i + 9];
            }
            mappingOK_D = true;
        }
    }

    /*
    F
    8 5 2
    7 4 1
    6 3 0
    */
    static bool mappingOK_F{ false };
    if (mappingOK_F == false) {
        bool foundOK = true;
        for (int i = 0; i < 9; ++i) {
            if (colorStr_F[i] == '?') {
                if (i == 1 || i == 5) {
                    continue;
                }
                foundOK = false;
                break;
            }
        }
        if (foundOK == true) {
            const size_t mapping[]{ 8, 5, 2, 7, 4, 1, 6, 3, 0 };
            for (int i = 0; i < 9; ++i) {
                g_cubeColor[F][mapping[i]] = colorStr_F[i];
            }
            mappingOK_F = true;
        }
    }

    /*
    B
    2 1 0
    5 4 3
    8 7 6
    */
    static bool mappingOK_B{ false };
    if (mappingOK_B == false) {
        bool foundOK = true;
        for (int i = 0; i < 9; ++i) {
            if (colorStr_B[i] == '?') {
                if (i == 1 || i == 5) {
                    continue;
                }
                foundOK = false;
                break;
            }
        }
        if (foundOK == true) {
            const size_t mapping[]{ 2, 1, 0, 5, 4, 3, 8, 7, 6 };
            for (int i = 0; i < 9; ++i) {
                g_cubeColor[B][mapping[i]] = colorStr_B[i];
            }
            mappingOK_B = true;
        }
    }

    if (reset == true) {
        mappingOK_B = mappingOK_D = mappingOK_F = mappingOK_L = false;
        return false;
    }

    if (mappingOK_B &&
        mappingOK_D &&
        mappingOK_F &&
        mappingOK_L) {
        return true;
    }

    return false;
}

inline bool ColorIdentify_2ndTime(const std::string& colorStr_U, const std::string& colorStr_F, const std::string& colorStr_B, bool reset) {
    /*
    R       U
    6 3 0   8 7 6
    7 4 1   5 4 3
    8 5 2   2 1 0
    */
    static bool mappingOK_R{ false };
    if (mappingOK_R == false) {
        bool foundOK{ true };
        for (int i = 0; i < 9; ++i) {
            if (colorStr_U[i] == '?') {
                foundOK = false;
                break;
            }
        }
        if (foundOK == true) {
            const size_t mapping[]{ 6, 3, 0, 7, 4, 1, 8, 5, 2 };
            for (int i = 0; i < 9; ++i) {
                g_cubeColor[R][mapping[i]] = colorStr_U[i];
            }
            mappingOK_R = true;
        }
    }

    static bool mappingOK_U{ false };
    if (mappingOK_U == false) {
        bool foundOK = true;
        for (int i = 0; i < 9; ++i) {
            if (colorStr_U[i + 9] == '?') {
                foundOK = false;
                break;
            }
        }
        if (foundOK == true) {
            const size_t mapping[]{ 8, 7, 6, 5, 4, 3, 2, 1, 0 };
            for (int i = 0; i < 9; ++i) {
                g_cubeColor[U][mapping[i]] = colorStr_U[i + 9];
            }
            mappingOK_U = true;
        }
    }

    /*
    F
    0 3 6
    1 4 7
    2 5 8
    */
    static bool mappingOK_F{ false };
    if (mappingOK_F == false) {
        if (colorStr_F[3] != '?' && colorStr_F[7] != '?') {
            g_cubeColor[F][1] = colorStr_F[3];
            g_cubeColor[F][5] = colorStr_F[7];
            mappingOK_F = true;
        }
    }

    /*
    B
    6 7 8
    3 4 5
    0 1 2
    */
    static bool mappingOK_B{ false };
    if (mappingOK_B == false) {
        if (colorStr_B[3] != '?' && colorStr_B[7] != '?') {
            g_cubeColor[B][3] = colorStr_B[3];
            g_cubeColor[B][1] = colorStr_B[7];
            mappingOK_B = true;
        }
    }

    if (reset == true) {
        mappingOK_B = mappingOK_F = mappingOK_R = mappingOK_U = false;
        return false;
    }

    if (mappingOK_B &&
        mappingOK_F &&
        mappingOK_R &&
        mappingOK_U) {
        return true;
    }

    return false;
}

inline void ReadConfig(const Config& config, const std::string& label, RccSet& rccSetter) {
    rccSetter.m_radius = config.Read(label + "radius", rccSetter.m_radius);
    rccSetter.m_horizontal = config.Read(label + "horizontal", rccSetter.m_horizontal);
    rccSetter.m_vertical = config.Read(label + "vertical", rccSetter.m_vertical);
    rccSetter.m_blurSize = config.Read(label + "blurSize", rccSetter.m_blurSize);
    rccSetter.m_rotateAngle = config.Read(label + "rotateAngle", rccSetter.m_rotateAngle);
    rccSetter.m_perspective = config.Read(label + "perspective", rccSetter.m_perspective);
    rccSetter.m_aspectRatio = config.Read(label + "aspectRatio", rccSetter.m_aspectRatio);

    for (auto& color : rccSetter.m_colors) {
        if (color.name == "ALL") {
            continue;
        }
        color.ReadColorFromFile("Config.ini", label);
    }
}

inline void SaveConfig(std::ofstream& fout, const std::string& label, const RccSet& rccSetter) {
    for (auto color : rccSetter.m_colors) {
        if (color.name == "ALL") {
            continue;
        }
        color.WriteColorToFile(fout, label);
    }
    fout << (label + "radius=") << rccSetter.m_radius << std::endl
        << (label + "horizontal=") << rccSetter.m_horizontal << std::endl
        << (label + "vertical=") << rccSetter.m_vertical << std::endl
        << (label + "blurSize=") << rccSetter.m_blurSize << std::endl
        << (label + "rotateAngle=") << rccSetter.m_rotateAngle << std::endl
        << (label + "perspective=") << rccSetter.m_perspective << std::endl
        << (label + "aspectRatio=") << rccSetter.m_aspectRatio << std::endl;
}

inline bool ConfigWindow(const std::string& label, const std::string& treeNodeName, Rcc& rcc, RccSet& rccSet, const int cols, const int rows) {
    bool treeNodeFlag = ImGui::TreeNode(treeNodeName.c_str());
    if (treeNodeFlag) {
        {
            ImGui::BeginChild((label + "Blank").c_str(), ImVec2(1, 550));
            ImGui::EndChild();
        }

        ImGui::SameLine();
        {
            ImGui::BeginChild((label + "ColorConfig").c_str(), ImVec2(0, 650), false);
            ImGui::PushItemWidth(180);
            // show the color configer
            for (auto &color : rccSet.m_colors) {
                if (&color == &rccSet.m_colors[0]) {
                    continue;
                }
                ColorConfiger(color);
            }
            ImGui::Text("White balance(RGB)\n");
            for (int i = 0; i < 18; ++i) {
                if (((label == "F" || label == "B") && !(i == 1 || i == 5 || i >= 9)) || (label == "U")) {
                    ImGui::DragFloat3((std::to_string(i) + "##" + label).c_str(), rcc.m_whiteBlance[i].data);
                }
            }

            ImGui::PopItemWidth();
            ImGui::EndChild();
        }

        {
            ImGui::BeginChild((label + "VSlider").c_str(), ImVec2(0, 170), false);
            const std::vector<SetVSlider> svs{
                SetVSlider("##" + label + "blurSize", &rccSet.m_blurSize, 0, 100, u8"模糊\n%d"),
                SetVSlider("##" + label + "radius", &rccSet.m_radius, 10, rows / 2, u8"半径\n%d"),
                SetVSlider("##" + label + "horizontal", &rccSet.m_horizontal, -cols / 2, cols / 2, u8"水平移动\n%d"),
                SetVSlider("##" + label + "vertical", &rccSet.m_vertical, -rows / 2, rows / 2, u8"垂直移动\n%d"),
                SetVSlider("##" + label + "rotateAngle", &rccSet.m_rotateAngle, -90, 90, u8"旋转角度\n%d"),
                SetVSlider("##" + label + "perspective", &rccSet.m_perspective, 30, 100, u8"透视\n%d"),
                SetVSlider("##" + label + "aspectRatio", &rccSet.m_aspectRatio, 1, 100, u8"长宽比\n%d"),
                SetVSlider("##" + label + "brightness", &rccSet.m_brightness, -64, 64, u8"亮度\n%d"),
                SetVSlider("##" + label + "contrast", &rccSet.m_contrast, 0, 100, u8"对比度\n%d") };

            const ImVec2 vsSize = ImVec2(14, 170);
            static int showTime{ 0 };
            static bool showTimeFlag{ false };
            if (showTimeFlag == false && ++showTime > 100) {
                showTimeFlag = true;
            }
            if (showTimeFlag == true && --showTime < 0) {
                showTimeFlag = false;
            }
            for (int i = 0; i < svs.size(); ++i) {
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::HSV((i % 9) / 9.0f, 0.0f + showTime * 0.5f / 100, 0.2f + showTime * 0.15f / 100));
                ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor::HSV((i % 9) / 9.0f, 0.7f, 0.35f));
                ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor::HSV((i % 9) / 9.0f, 0.7f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor::HSV((i % 9) / 9.0f, 1.0f, 0.9f));
                ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, (ImVec4)ImColor::HSV((i % 9) / 9.0f, 1.0f, 0.9f));
                if (ImGui::VSliderInt(svs[i].label.c_str(), vsSize, svs[i].value, svs[i].min, svs[i].max, "") == true) {
                    if (svs[i].value == &rccSet.m_brightness) {
                        rcc.ChangeBrightness();
                    }
                    else if (svs[i].value == &rccSet.m_contrast) {
                        rcc.ChangeContrast();
                    }
                }
                if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
                    ImGui::SetTooltip(svs[i].tips.c_str(), *(svs[i].value));
                }
                ImGui::PopStyleColor(5);
            }
            ImGui::EndChild();
        }

        ImGui::TreePop();
    }
    return treeNodeFlag;
}

inline void SerialMain() {
    while (g_showMainWindow) {
        g_serialTips = "OFF";
        while (g_showMainWindow && g_openSerial) {
            g_serialTips = "ON";
            // Init port
            if (!g_mySerialPort.InitPort(g_serialPortIdx)) {
                g_serialTips = u8"串口初始化失败";
                g_serialIsOpened = false;
                if (++g_serialPortIdx > 16) {
                    g_serialPortIdx = 1;
                }
                Sleep(100);
                continue;
            }
            else {
                g_serialTips = u8"串口初始化成功";
            }

            // open listen threaad
            if (!g_mySerialPort.OpenListenThread()) {
                g_serialTips = u8"监听线程打开失败";
                g_serialIsOpened = false;
                Sleep(1000);
                continue;
            }
            else {
                g_serialTips = u8"监听线程打开成功";
            }

            // read input data send to output
            g_serialTips = u8"等待用户输入";
            g_serialIsOpened = true;
            while (g_showMainWindow && g_openSerial) {
                SendMsgToSerial();
            }
            g_mySerialPort.CloseListenTread();
            g_serialIsOpened = false;
            Sleep(1000); // 谜之bug，没有Sleep一点时间的话，退出窗口的时候就会死循环在这里
        }
        Sleep(1000); // 谜之bug，没有Sleep一点时间的话，退出窗口的时候就会死循环在这里
    }
    return;
}

inline int CallBackInputWrap(ImGuiTextEditCallbackData* data) {
    float controlWidth = *(float*)data->UserData;
    float textWidth = ImGui::CalcTextSize(data->Buf).x;

    if (controlWidth <= textWidth) {
        std::string textStr_tmp = data->Buf;
        float  textWidth_tmp = ImGui::CalcTextSize(textStr_tmp.data()).x;
        size_t wrapPos = textStr_tmp.size();

        while (wrapPos > 0) {
            if (controlWidth >= textWidth_tmp) {
                data->InsertChars(static_cast<int>(wrapPos), "\n"); // "hard" word wrap...
                data->BufDirty = true;
                break;
            }
            else {
                textStr_tmp.pop_back();
                textWidth_tmp = ImGui::CalcTextSize(textStr_tmp.data()).x;
                --wrapPos;
            }
        }
    }
    return data->BufTextLen;
}

inline void RccProc(Rcc& rcc, RccSet& rccSet, std::string& colorStr, cv::Mat& frame) {
    rcc.Setting(rccSet);
    rcc.ReadVideoFrame();
    if (rcc.GetFrame().empty()) {
        return;
    }
    rcc.DrawGrid();
    colorStr = rcc.ReadColorStr();
    frame = rcc.GetFrame();
}

inline void ShowMainWindow(const std::string &title, const cv::Mat& matU,
    const cv::Mat& matF, const cv::Mat& matB, const int cols, const int rows) {
    // creat window
    ImGui::Begin(title.c_str(), &g_showMainWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    {
        ImGui::BeginChild("Blank", ImVec2(8, 0));
        ImGui::EndChild();
    }

    ImGui::SameLine();
    // 主视区
    {
        ImGui::BeginChild("MainWindow", ImVec2(1602, 0), false);

        // 前方摄像头画面
        {
            ImGui::BeginChild("CameraF", ImVec2(528, 485), true);
            ImGui::Text(u8"\t\t\t\t\t\t<前摄像头>");

            static GLuint tex{};
            const ImTextureID   textureID{ GetMatTextureID(matF, tex) };
            static const ImVec2 textureSize{ static_cast<float>(cols), static_cast<float>(rows) };
            ImGui::Image(textureID, textureSize);
            std::string getColors{ std::string(23, ' ') + g_rcc_F.GetColorString() };
            ImGui::PushItemWidth(480);
            ImGui::InputText("##Video_F", const_cast<char*>(getColors.c_str()), getColors.size(), ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();
            static int cameraIdx{ g_rccSet_F.m_index };
            ImGui::SameLine();
            ImGui::PushItemWidth(15);
            ImGui::DragInt("##CameraIdx", &cameraIdx, 0.1f, 1, 3);
            ImGui::PopItemWidth();
            ImGui::SameLine();
            if (cameraIdx != g_rccSet_F.m_index) {
                g_rccSet_F.m_index = cameraIdx;
            }
            ImGui::EndChild();
        }

        ImGui::SameLine();
        // 上方摄像头画面
        {
            ImGui::BeginChild("Camera U", ImVec2(529, 485), true);

            static GLuint tex{};
            const ImTextureID   textureID{ GetMatTextureID(matU, tex) };
            static const ImVec2 textureSize{ static_cast<float>(cols), static_cast<float>(rows) };
            ImGui::Text(u8"\t\t\t\t\t\t<上摄像头>");
            ImGui::Image(textureID, textureSize);
            std::string getColors{ std::string(20, ' ') + g_rcc_U.GetColorString() };
            ImGui::PushItemWidth(480);
            ImGui::InputText("##Video_U", const_cast<char*>(getColors.c_str()), getColors.size(), ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();
            static int cameraIdx{ g_rccSet_U.m_index };
            ImGui::SameLine();
            ImGui::PushItemWidth(15);
            ImGui::DragInt("##CameraIdx", &cameraIdx, 0.1f, 1, 3);
            ImGui::PopItemWidth();
            ImGui::SameLine();
            if (cameraIdx != g_rccSet_U.m_index) {
                g_rccSet_U.m_index = cameraIdx;
            }
            ImGui::EndChild();
        }

        ImGui::SameLine();
        // 后方摄像头画面
        {
            ImGui::BeginChild("CameraB", ImVec2(529, 485), true);

            static GLuint tex{};
            const ImTextureID   textureID{ GetMatTextureID(matB, tex) };
            static const ImVec2 textureSize{ static_cast<float>(cols), static_cast<float>(rows) };
            ImGui::Text(u8"\t\t\t\t\t\t<后摄像头>");
            ImGui::Image(textureID, textureSize);
            std::string getColors{ std::string(23, ' ') + g_rcc_B.GetColorString() };
            ImGui::PushItemWidth(480);
            ImGui::InputText("##Video_B", const_cast<char*>(getColors.c_str()), getColors.size(), ImGuiInputTextFlags_ReadOnly);
            ImGui::PopItemWidth();
            static int cameraIdx{ g_rccSet_B.m_index };
            ImGui::SameLine();
            ImGui::PushItemWidth(15);
            ImGui::DragInt("##CameraIdx", &cameraIdx, 0.1f, 1, 3);
            ImGui::PopItemWidth();
            ImGui::SameLine();
            if (cameraIdx != g_rccSet_B.m_index) {
                g_rccSet_B.m_index = cameraIdx;
            }
            ImGui::EndChild();
        }

        // 3D魔方
        {
            ImGui::BeginChild("3DCube", ImVec2(528, 545), true);
            ImGui::Text(u8"\t\t\t\t\t\t<3D-魔方>\n");
            {
                ImGui::BeginChild("CubeImg", ImVec2(0, 280), false);
                ImGui::Text("\t\t\t");
                ImGui::SameLine();

                static const int FPS_COUNT{ 1000 / 45 };
                static DWORD timePre{ GetTickCount() - FPS_COUNT };
                static ImTextureID texID{};
                if (GetTickCount() - timePre >= FPS_COUNT) {
                    cv::Mat cubeMat{};
                    do {
                        cubeMat = cv::imread("cube3d.bmp");
                    }
                    while (cubeMat.empty());
                    static GLuint tex{};
                    texID = GetMatTextureID(cubeMat, tex);
                    timePre = GetTickCount();
                }
                static const ImVec2 cubeSize{ 280, 280 };
                ImGui::Image(texID, cubeSize);

                ImGui::EndChild();
            }

            {
                ImGui::BeginChild("StepTips", ImVec2(0, 30));
                static const std::string tips[]{
                    u8"请按下开始按键",         // step -1
                    u8"正在进行第一次颜色识别", // 0
                    u8"正在翻转魔方",           // 1
                    u8"正在进行第二次颜色识别", // 2
                    u8"正在计算魔方解法",       // 3
                    u8"结束计算，开始运行",     // 4
                    u8"完成，请关闭程序"        // 5
                };
                static std::string scrollTips[]{
                    "> > > [" + tips[0] + "] > > >" + std::string(57 - tips[0].size(), ' '),
                    "> > > [" + tips[1] + "] > > >" + std::string(57 - tips[1].size(), ' '),
                    "> > > [" + tips[2] + "] > > >" + std::string(57 - tips[2].size(), ' '),
                    "> > > [" + tips[3] + "] > > >" + std::string(57 - tips[3].size(), ' '),
                    "> > > [" + tips[4] + "] > > >" + std::string(57 - tips[4].size(), ' '),
                    "> > > [" + tips[5] + "] > > >" + std::string(57 - tips[5].size(), ' '),
                    "> > > [" + tips[6] + "] > > >" + std::string(57 - tips[6].size(), ' ')
                };
                static const size_t tipSize{ scrollTips[0].size() };
                static int i = 0;
                static DWORD timePre = GetTickCount();
                if (GetTickCount() - timePre > 200) {
                    char tmp = scrollTips[g_nowStep + 1][i];
                    if (tmp != u8' ' && tmp != u8'>' && tmp != u8'[' && tmp != u8']' && tmp != u8'\0') {
                        i += 3;
                    }
                    else {
                        i += 2;
                    }
                    if (i > tipSize - 1) {
                        i = 0;
                    }
                    timePre = GetTickCount();
                }
                for (auto& st : scrollTips[g_nowStep + 1]) {
                    if (st == '>' || st == 'v' || st == '<' || st == '^') {
                        switch (i / 2 % 4) {
                        case 0:
                            st = '>';
                            break;
                        case 1:
                            st = 'v';
                            break;
                        case 2:
                            st = '<';
                            break;
                        case 3:
                            st = '^';
                        default:
                            break;
                        }
                    }
                }
                ImGui::Text((scrollTips[g_nowStep + 1].substr(i, tipSize - i) + scrollTips[g_nowStep + 1].substr(0, i)).c_str());
                ImGui::EndChild();
            }

            {
                ImGui::BeginChild("Blank", ImVec2(25, 100));
                ImGui::EndChild();
            }
            ImGui::SameLine();
            {
                ImGui::BeginChild("TimerButtonL", ImVec2(145, 100));
                ImGui::Text(" "); ImGui::SameLine();
                if (ImGui::Button("  L i n k s  ")) {
                    g_IOBuf.PushBack("<Links>\n");
                    system(LINKS_NAME.c_str());
                }
                if (ImGui::Button("   H e l p   ")) {
                    g_IOBuf.PushBack("<Help>\n");
                    Help();
                }
                ImGui::Text(" "); ImGui::SameLine();
                if (ImGui::Button("  S t a f f  ")) {
                    g_IOBuf.PushBack("<Staff>\n");
                    g_IOBuf.PushBack(u8"--------[魔杰座：魔方机器人]--------\n");
                    g_IOBuf.PushBack(u8"\t\t北京理工大学珠海学院\n  工业自动化学院  2015级机械电子工程\n");
                    g_IOBuf.PushBack(u8"   程开 温开旺 罗泽奇 梁锦华 杨继康\n");
                    g_IOBuf.PushBack("----------------------------------\n");
                }
                ImGui::EndChild();
            }
            ImGui::SameLine();
            {
                ImGui::BeginChild("Timer", ImVec2(140, 105));
                ImGui::PushFont(g_bigFont);
                ImGui::Text(u8" <计时器>");
                static DWORD timer{};
                if (g_nowStep == -1) {
                    timer = 0;
                }
                else if (g_stopButton == true) {

                }
                else {
                    timer = GetTickCount() - g_timerPre;
                }
                ImGui::Text("  %02d:%02d", timer / 1000, (timer % 1000) / 10);
                ImGui::PopFont();
                ImGui::SameLine();
                ImGui::Text("%02d", (timer % 10) * 10);
                if (g_stopButton == true) {
                    ImGui::Text("<----STOP---->");
                }
                ImGui::EndChild();
            }
            ImGui::SameLine();
            {
                ImGui::BeginChild("TimerButtonR", ImVec2(0, 100));
                if (ImGui::Button("  S t a r t  ")) {
                    g_IOBuf.PushBack("<Start>\n");
                    Reset(0);
                }
                ImGui::Text(" "); ImGui::SameLine();
                if (ImGui::Button("   S t o p   ")) {
                    g_IOBuf.PushBack("<Stop>\n");
                    g_stopButton = !g_stopButton;
                }
                if (ImGui::Button("  R e s e t  ")) {
                    g_IOBuf.PushBack("<Reset>\n");
                    Reset(-1);
                }
                ImGui::EndChild();
            }
            {
                ImGui::BeginChild("Color", ImVec2(0, 50));

                ImGui::Text(("\tU: " + g_cubeColor[U]).c_str()); ImGui::SameLine();
                ImGui::Text(("\t L: " + g_cubeColor[L]).c_str()); ImGui::SameLine();
                ImGui::Text(("\t F: " + g_cubeColor[F]).c_str());

                ImGui::Text(("\tD: " + g_cubeColor[D]).c_str()); ImGui::SameLine();
                ImGui::Text(("\t R: " + g_cubeColor[R]).c_str()); ImGui::SameLine();
                ImGui::Text(("\t B: " + g_cubeColor[B]).c_str());

                ImGui::EndChild();
            }

            static DWORD timePre = GetTickCount();
            DWORD timeNow = GetTickCount();
            static int show{};
            static int fps{};
            if (GetTickCount() != timePre && show == 0) {
                fps = 1000 / (GetTickCount() - timePre);
            }
            if (++show > 15) {
                show = 0;
            }
            ImGui::Text("\t\t\t\t\tFPS:%3d  step%2d  n%2d", fps, g_nowStep, g_sendNext);
            ImGuiIO& io = ImGui::GetIO();
            if (ImGui::IsMousePosValid()) {
                const int posX{ static_cast<int>(io.MousePos.x) };
                const int posY{ static_cast<int>(io.MousePos.y) };
                const float wheel{ io.MouseWheel };

                static const int Y{ 68 };
                static const int FX{ 30 };
                static const int UX{ 566 };
                static const int BX{ 1102 };
                if (wheel != 0) {
                    if (posY >= Y && posY <= Y + rows) {
                        if (posX >= FX && posX <= FX + cols) {
                            g_rccSet_F.m_radius += static_cast<int>(5 * wheel);
                        }
                        else if (posX >= UX && posX <= UX + cols) {
                            g_rccSet_U.m_radius += static_cast<int>(5 * wheel);
                        }
                        else if (posX >= BX && posX <= BX + cols) {
                            g_rccSet_B.m_radius += static_cast<int>(5 * wheel);
                        }
                    }
                }
                if (ImGui::IsMouseClicked(0)) {
                    if (posY >= Y && posY <= Y + rows) {
                        if (posX >= FX && posX <= FX + cols) {
                            g_rccSet_F.m_horizontal = (int)cols / 2 + FX - posX;
                            g_rccSet_F.m_vertical = (int)rows / 2 + Y - posY;
                        }
                        else if (posX >= UX && posX <= UX + cols) {
                            g_rccSet_U.m_horizontal = (int)cols / 2 + UX - posX;
                            g_rccSet_U.m_vertical = (int)rows / 2 + Y - posY;
                        }
                        else if (posX >= BX && posX <= BX + cols) {
                            g_rccSet_B.m_horizontal = (int)cols / 2 + BX - posX;
                            g_rccSet_B.m_vertical = (int)rows / 2 + Y - posY;
                        }
                    }
                }
            }
            else {
                ImGui::Text("<INVALID>");
            }
            timePre = timeNow;

            ImGui::EndChild();
        }

        ImGui::SameLine();
        // 对话框
        {
            ImGui::BeginChild("Dialog", ImVec2(1066, 545), true);
            ImGui::Text(u8"\t\t\t\t\t\t\t\t\t\t\t\t\t  <对话框>");

            ImVec2 outputTextAreaSize = ImVec2(1050, ImGui::GetTextLineHeight() * 13.5f); // set the output text window size 
            ImGui::InputTextMultiline("##Output Text", g_IOBuf.output,
                IM_ARRAYSIZE(g_IOBuf.output), outputTextAreaSize,
                ImGuiInputTextFlags_ReadOnly);

            if (g_IOBuf.outputChange == true) {
                ImGui::BeginChild(ImGui::GetID("##Output Text"));
                ImGui::SetScrollHere(1.0f);
                ImGui::EndChild();
                g_IOBuf.outputChange = false;
            }

            // 左侧两个选项
            {
                ImGui::BeginChild("2Checkbox", ImVec2(990, ImGui::GetTextLineHeight() * 2), false);

                ImGui::PushItemWidth(50.0f);
                ImGui::DragInt("##SerialPortIdx_test", &g_serialPortIdx, 0.01f, 1, 16, "COM%.0f"); ImGui::SameLine();
                ImGui::PopItemWidth();

                ImGui::RadioButton("##Serial", g_serialIsOpened);
                if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
                    ImGui::SetTooltip(g_serialTips.c_str());
                }
                ImGui::SameLine();
                if (g_serialIsOpened == true) {
                    ImGui::Text("ON ");
                }
                else {
                    ImGui::Text("OFF");
                }

                ImGui::SameLine(); ImGui::Text("  "); ImGui::SameLine();
                ImGui::Checkbox(u8"打开串口", &g_openSerial);

                ImGui::SameLine(); ImGui::Text("  "); ImGui::SameLine();
                static bool testMode;
                ImGui::Checkbox(u8"测试模式", &testMode);
                g_rccSet_F.m_testMode = testMode;
                g_rccSet_U.m_testMode = testMode;
                g_rccSet_B.m_testMode = testMode;

                ImGui::SameLine(); ImGui::Text("  "); ImGui::SameLine();
                static bool whiteBlanceMode;
                ImGui::Checkbox(u8"白平衡模式", &whiteBlanceMode);
                g_rccSet_F.m_whiteBlanceMode = whiteBlanceMode;
                g_rccSet_U.m_whiteBlanceMode = whiteBlanceMode;
                g_rccSet_B.m_whiteBlanceMode = whiteBlanceMode;

                ImGui::SameLine(); ImGui::Text("  "); ImGui::SameLine();
                static bool rgbMode;
                ImGui::Checkbox(u8"RGB", &rgbMode);
                g_rccSet_F.m_rgbMode = rgbMode;
                g_rccSet_U.m_rgbMode = rgbMode;
                g_rccSet_B.m_rgbMode = rgbMode;

                ImGui::EndChild();
            }
            ImGui::SameLine();
            if (ImGui::Button(u8"清除!")) {
                memset(g_IOBuf.output, '\0', g_IOBuf.outputIndex * sizeof(char));
                g_IOBuf.outputIndex = 0;
            }

            // set the input text window size 
            ImVec2 inputTextAreaSize = ImVec2(1050, ImGui::GetTextLineHeight() * 5.5f);
            float inputTextAreaWidth = ImGui::CalcItemWidth() * 0.975f;

            // show the input text window
            static bool focusOnInputTextItem = false;
            if (focusOnInputTextItem == true) {
                ImGui::SetKeyboardFocusHere();
                focusOnInputTextItem = false;
            }
            if (ImGui::InputTextMultiline("##input Text", g_IOBuf.input, IM_ARRAYSIZE(g_IOBuf.input), inputTextAreaSize,
                ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CtrlEnterForNewLine | ImGuiInputTextFlags_CallbackAlways,
                CallBackInputWrap, &(inputTextAreaWidth)) == true) {
                g_IOBuf.PushBack(">> ");
                g_IOBuf.Send(g_IOBuf.input);
                focusOnInputTextItem = true;
            }

            // show the send button
            {
                ImGui::BeginChild("ControlButton", ImVec2(990, 0), false);
                static bool abFlag = true;
                if (ImGui::ArrowButton("AB", ImGuiDir())) {
                    abFlag = !abFlag;
                }
                if (abFlag == true) {
                    ImGui::SameLine();
                    ImGui::BeginChild("ABWindow");
                    ImGui::BeginChild("Blank", ImVec2(0, 1));
                    ImGui::EndChild();
                    if (ImGui::SmallButton(u8"L顺")) {
                        g_IOBuf.PushBack(">> "); g_IOBuf.Send("LMF");
                    } ImGui::SameLine();
                    if (ImGui::SmallButton(u8"L逆")) {
                        g_IOBuf.PushBack(">> "); g_IOBuf.Send("LMR");
                    } ImGui::SameLine();
                    ImGui::Text("/"); ImGui::SameLine();
                    if (ImGui::SmallButton(u8"D顺")) {
                        g_IOBuf.PushBack(">> "); g_IOBuf.Send("DMF");
                    } ImGui::SameLine();
                    if (ImGui::SmallButton(u8"D逆")) {
                        g_IOBuf.PushBack(">> "); g_IOBuf.Send("DMR");
                    } ImGui::SameLine();
                    ImGui::Text("/"); ImGui::SameLine();
                    if (ImGui::SmallButton(u8"松开")) {
                        g_IOBuf.PushBack(">> "); g_IOBuf.Send("AC-");
                    } ImGui::SameLine();
                    if (ImGui::SmallButton(u8"夹紧")) {
                        g_IOBuf.PushBack(">> "); g_IOBuf.Send("AC+");
                    } ImGui::SameLine();
                    //ImGui::Text("///"); ImGui::SameLine();
                    //if (ImGui::SmallButton(u8"TEST!!!")) {
                    //    g_IOBuf.PushBack(">> "); g_IOBuf.Send("LM2 DC- LM2 DM+ DC+ DM- LC- LM+ LC+ DC- LM- DM+ DC+ DM- LM- LC- LM+ LC+ DM- DC- DM+ LM- DC+ LC- LM+ LC+ DM2 LC- DM2 LC+ LM2 DM- LC- DM+ LC+ LM- LC- LM+ LC+ DM2 LC- LM+ LC+ DC- LM- DM- DC+ DM+ DC- LM- DC+ LC- LM+ LC+ DM2 LM2 LC- DM2 LC+ LM2 DC- LM2 DM- DC+ DM+ LM2 DC- LM2 DC+ DM2 DC- LM2 DM+ DC+ DM- DC- LM- DC+ LC- LM+ LC+ DM2 ");
                    //} ImGui::SameLine();
                    ImGui::EndChild();
                }
                ImGui::EndChild();
            }
            ImGui::SameLine();
            if (ImGui::Button(u8"发送!")) {
                g_IOBuf.PushBack(">> ");
                g_IOBuf.Send(g_IOBuf.input);
                focusOnInputTextItem = true;
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();
    }
    // ----------------------------------------------------------------------------- //
    ImGui::SameLine();
    {
        ImGui::BeginChild("ConfigWindow", ImVec2(270, 1034), true);

        ImGui::Text(u8"\t\t   <设置>");

        {
            ImGui::BeginChild("Configs", ImVec2(0, 910));

            if (ImGui::SmallButton("D##F")) {
                g_rccSet_F.DefaultColors();
            } ImGui::SameLine();
            ConfigWindow("F", u8"前摄像头设置", g_rcc_F, g_rccSet_F, cols, rows);
            if (ImGui::SmallButton("D##U")) {
                g_rccSet_U.DefaultColors();
            } ImGui::SameLine();
            ConfigWindow("U", u8"上摄像头设置", g_rcc_U, g_rccSet_U, cols, rows);
            if (ImGui::SmallButton("D##B")) {
                g_rccSet_B.DefaultColors();
            } ImGui::SameLine();
            ConfigWindow("B", u8"后摄像头设置", g_rcc_B, g_rccSet_B, cols, rows);

            ImGui::EndChild();
        }
        // show the save button
        ImGui::NewLine();
        ImGui::Text("\t\t"); ImGui::SameLine();
        if (ImGui::Button(u8"保存所有设置")) {
            std::ofstream fout{ "Config.ini" };
            SaveConfig(fout, "F", g_rccSet_F);
            SaveConfig(fout, "U", g_rccSet_U);
            SaveConfig(fout, "B", g_rccSet_B);
            fout.close();
        }

        ImGui::EndChild();
    }
    // ----------------------------------------------------------------------------- //
    // end window
    ImGui::End();
}

inline void ColorConfiger(HSV &color) {
    ImGui::Text(color.name.c_str());
    ImGui::DragInt3(("L##" + color.name).c_str(),
        color.m_dataSave[0],
        0.1f,
        0,
        255);
    ImGui::DragInt3(("H##" + color.name).c_str(),
        color.m_dataSave[1],
        0.1f,
        0,
        255);
    color.RefreshData();
}

inline void Render(GLFWwindow* window, const ImVec4 &clear_color) {
    ImGui::Render();
    int display_w, display_h;
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
}

inline ImTextureID GetMatTextureID(const cv::Mat &mat, GLuint& tex) {
    glDeleteTextures(1, &tex); // 删除前一张无用的图，防止内存泄漏
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    /*
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Set texture clamping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_READ_COLOR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_READ_COLOR);
    */
    glTexImage2D(GL_TEXTURE_2D, // Type of texture
        0,                      // Pyramid level (for mip-mapping) - 0 is the top level
        GL_RGB,                 // Internal colour format to convert to
        mat.cols,               // Image width  i.e. 640 for Kinect in standard mode
        mat.rows,               // Image height i.e. 480 for Kinect in standard mode
        0,                      // Border width in pixels (can either be 1 or 0)
        GL_BGR,                 // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
        GL_UNSIGNED_BYTE,       // Image data type
        mat.ptr());             // The actual image data itself
    glGenerateMipmap(GL_TEXTURE_2D);

#pragma warning(disable:4312)
    // 会引发C4312警告，但是不会造成什么坏的影响，请无视它
    return reinterpret_cast<ImTextureID>(tex);
}

inline static void GlfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "Error %d: %s\n", error, description);
}

inline bool Init(GLFWwindow*& window) {
    InitCubeInputTxt();
    WinExec(CUBE_EXE_NAME.c_str(), SW_HIDE); // 隐藏opengl程序

    for (int i = 0; i < 6; ++i) {
        g_cubeColor.push_back(std::string(9, '?'));
        g_faceletStr.push_back(std::string());
    }

    // read data from the Config.ini
    Config config("Config.ini");
    ReadConfig(config, "F", g_rccSet_F);
    ReadConfig(config, "U", g_rccSet_U);
    ReadConfig(config, "B", g_rccSet_B);
    g_rcc_F.Setting(g_rccSet_F);
    g_rcc_U.Setting(g_rccSet_U);
    g_rcc_B.Setting(g_rccSet_B);

    // Setup window
    glfwSetErrorCallback(GlfwErrorCallback);
    if (!glfwInit()) {
        return false;
    }
    const char* glsl_version = "#version 130";
    GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
    const int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    const int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    window = glfwCreateWindow(cx, cy, "ImGui_GLFW_OpenGL3", pMonitor/*NULL*/, NULL);

    if (window == NULL) {
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // Enable vsync 垂直同步
    gl3wInit();

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    io.Fonts->AddFontFromFileTTF("../../misc/fonts/YaHei.Consolas.1.12.ttf", 22.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    g_bigFont = io.Fonts->AddFontFromFileTTF("../../misc/fonts/YaHei.Consolas.1.12.ttf", 35.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    g_smallFont = io.Fonts->AddFontFromFileTTF("../../misc/fonts/YaHei.Consolas.1.12.ttf", 5.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    static ImGuiStyle& style = ImGui::GetStyle();
    style.GrabRounding = 8;
    style.FrameRounding = 8;

    return true;
}

inline void CleanUp(GLFWwindow *&window) {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    std::string temp = std::string("TASKKILL /F /IM ") + CUBE_EXE_NAME;
    system(temp.c_str());
}

int Old() {
    GLFWwindow* window{};
    if (Init(window) == false) {
        return -1;
    }

#ifdef  DEBUGGING
    g_nowStep = 2;
#endif //  DEBUGGING

    while (!glfwWindowShouldClose(window) && g_showMainWindow) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static std::string colorStr_U(18, '?');
        static std::string colorStr_F(9, '?');
        static std::string colorStr_B(9, '?');
        static cv::Mat frame_U{}, frame_F{}, frame_B{};

        static int takeTurns = 0; // 轮流读取画面，防止cpu太高
        switch (takeTurns) {
        case 0:
            RccProc(g_rcc_F, g_rccSet_F, colorStr_F, frame_F);
            ++takeTurns;
            break;
        case 1:
            RccProc(g_rcc_U, g_rccSet_U, colorStr_U, frame_U);
            ++takeTurns;
            break;
        case 2:
            RccProc(g_rcc_B, g_rccSet_B, colorStr_B, frame_B);
            takeTurns = 0;
            break;
        default:
            break;
        }

        static int ldcm_outputTxtWriteTime = GetFileWriteTime("LDCM_output.txt");
        static size_t bufLenPre{ strlen(g_IOBuf.output) };

        switch (g_nowStep) {
        case -1: // step == -1 按下开始按钮，令step = 0
            break;
        case 0: // step == 0  第一次颜色识别 ↓
            if (ColorIdentify_1stTime(colorStr_U, colorStr_F, colorStr_B)) // 第一次颜色识别
            {
                g_IOBuf.PushBack("\n>> [Flip] ");
                g_IOBuf.Send("DC- LM2 DC+ LC- DM2 LC+ "); // 发送翻转指令
                g_nowStep = 1;
            }
            break;
        case 1: // step == 1  翻转魔方 ↑
            if (strlen(g_IOBuf.output) > 3 && g_IOBuf.output[strlen(g_IOBuf.output) - 3] == '!') // 遇到[Finish!]
            {
                g_nowStep = 2;
            }
            break;
        case 2: // step == 2  第二次颜色识别 ↓ // 还原第二个魔方的时候这里会卡住
#ifndef DEBUGGING
            if (ColorIdentify_2ndTime(colorStr_U, colorStr_F, colorStr_B)) // 第二次颜色识别 
#endif
            {
#ifdef DEBUGGING
                //test
                g_cubeColor[U] = "RRGBYGRWG";
                g_cubeColor[D] = "YYOOWWYRY";
                g_cubeColor[F] = "WBYRRYBBB";
                g_cubeColor[B] = "OGWROGOWO";
                g_cubeColor[L] = "GOBWBBGYR";
                g_cubeColor[R] = "ROWGGYWOB";
#endif
                ChangeCubeColorsInput(); // 写入INPUT.txt

                g_faceletStr[U] = "U:" + g_cubeColor[U];
                g_faceletStr[D] = "D:" + g_cubeColor[D];
                g_faceletStr[F] = "F:" + g_cubeColor[F];
                g_faceletStr[B] = "B:" + g_cubeColor[B];
                g_faceletStr[L] = "L:" + g_cubeColor[L];
                g_faceletStr[R] = "R:" + g_cubeColor[R];

                if (SolveCube(g_faceletStr) == READ_FAIL) {
                    g_IOBuf.PushBack(u8"\n>> [错误] 颜色输入错误，请检查摄像头是否连接正确\n");
                    g_nowStep = -1;
                    break;
                }
                if (TranslateToLD() == READ_FAIL) {
                    g_IOBuf.PushBack(u8"\n>> [错误] 颜色输入错误，请检查摄像头是否连接正确\n");
                    g_nowStep = -1;
                    break;
                }
                g_nowStep = 3;
            }
            break;
        case 3: // step == 3  计算解法 ↑
            if (ldcm_outputTxtWriteTime != GetFileWriteTime("LDCM_output.txt")) {
                Sleep(100);
                std::ifstream send{ "LDCM_output.txt" };
                send.seekg(0, std::ios::beg);
                std::string sendOrder{};
                getline(send, sendOrder);
                send.close();
                g_IOBuf.PushBack(">> [LDCM] ");
                g_IOBuf.Send(sendOrder);
                bufLenPre = strlen(g_IOBuf.output);
                g_nowStep = 4;
                ldcm_outputTxtWriteTime = GetFileWriteTime("LDCM_output.txt");
            }
            break;
        case 4: // step == 4  完成计算，开始执行 ↑↓
            if (strlen(g_IOBuf.output) > 3 && g_IOBuf.output[strlen(g_IOBuf.output) - 3] == '!') // 遇到[Finish!]，则暂停计时
            {
                g_IOBuf.Send("AC-");
                g_stopButton = true; // 暂停计时
                g_nowStep = 5;
            } else {
                NextCubeStep();
            }
            break;
        case 5: // step == 5  完毕
            break;
        default:
            break;
        }

        ShowMainWindow(TITLE_NAME, frame_U, frame_F, frame_B, COLS, ROWS);

        static Timer serial(SerialMain, Timer::Interval(1000), true); // 启动SerialMain
        serial.start(true);

        static const ImVec4 clearColor{ 0.45f, 0.55f, 0.60f, 1.00f };
        Render(window, clearColor);
    }

    CleanUp(window);
    return 0;
}

void NewFrame() {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

int New() {
    GLFWwindow* window{};
    if (Init(window) == false) {
        return -1;
    }
    constexpr int W = 1920, H = 1080;
    MainUI mainUI{ { W, H } };
    while (!glfwWindowShouldClose(window) && g_showMainWindow) {
        NewFrame();
        mainUI.Show(window);
        Render(window, { 0.45f, 0.55f, 0.60f, 1.00f });
    }
    CleanUp(window);
    return 0;
}

//int main(int, char **argv)
//{
//    // hide console window
//    HWND hwnd = FindWindow("ConsoleWindowClass", argv[0]);
//    if (hwnd) { ShowWindow(hwnd, /*SW_HIDE*/ SW_MAXIMIZE); }

int APIENTRY  WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    int result = 0;
    // result = New();
    result = Old();
    return result;
}
