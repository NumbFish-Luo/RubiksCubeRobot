#pragma once
#include "UI.h"
#include "CameraConfigUI.h"
#include "CameraUI.h"
#include "Cube3DUI.h"
#include "SerialPortUI.h"

constexpr UIRect UI_RECT{
    // 主UI区域
    { 1920.f, 1080.f },
    // 其他UI区域，以下为mainUI的百分比
    { 0.8250f, 0.4250f },
    { 0.1570f, 0.9600f },
    { 0.2750f, 0.5310f },
    { 0.5545f, 0.5310f }
};

extern IOBuf g_IOBuf; // 外部变量，不要初始化
extern CSerialPort  g_mySerialPort;

class MainUI : public UI {
public:
    MainUI(Rect rect);
    virtual void Show(GLFWwindow* window, bool& show);
private:
    void DealData();
    std::shared_ptr<CameraUI> m_cameraUI;
    std::shared_ptr<CameraConfigUI> m_cameraConfigUI;
    std::shared_ptr<Cube3DUI> m_cube3DUI;
    std::shared_ptr<SerialPortUI> m_serialPortUI;
};

bool ColorIdentify_1stTime(const std::string& colorStr_U, const std::string& colorStr_F, const std::string& colorStr_B, bool reset = false);
bool ColorIdentify_2ndTime(const std::string& colorStr_U, const std::string& colorStr_F, const std::string& colorStr_B, bool reset = false);
void InitCubeInputTxt();
