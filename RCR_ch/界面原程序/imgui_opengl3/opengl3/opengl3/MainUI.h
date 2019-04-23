#pragma once
#include "UI.h"
#include "CameraConfigUI.h"
#include "CameraUI.h"
#include "Cube3DUI.h"
#include "SerialPortUI.h"

constexpr UIRect UI_RECT{
    // ��UI����
    { 1920.f, 1080.f },
    // ����UI��������ΪmainUI�İٷֱ�
    { 0.8250f, 0.4250f },
    { 0.1570f, 0.9600f },
    { 0.2750f, 0.5310f },
    { 0.5545f, 0.5310f }
};

extern IOBuf g_IOBuf; // �ⲿ��������Ҫ��ʼ��


extern CSerialPort  g_mySerialPort;

class MainUI;
class CameraUI;
class CameraConfigUI;
class Cube3DUI;
class SerialPortUI;

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
void Reset(int step);
void InitCubeInputTxt();
void Help();
int CallBackInputWrap(ImGuiTextEditCallbackData* data);
void ColorConfiger(HSV &color);
void SaveConfig(std::ofstream& fout, const std::string& label, const RccSet& rccSetter);
bool ConfigWindow(const std::string& label, const std::string& treeNodeName, Rcc& rcc, RccSet& rccSet, const int cols, const int rows);
