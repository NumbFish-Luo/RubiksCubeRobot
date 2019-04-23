#pragma once
#include "UI.h"
#include "CameraConfigUI.h"

// Ïà»úUI
class CameraUI : public UI {
public:
    CameraUI(Rect rect) : UI(rect) {}
    virtual void Show(GLFWwindow* window, bool& show);
    void GetData();
    std::string GetColorF() {
        return m_colorF;
    }
    std::string GetColorU() {
        return m_colorU;
    }
    std::string GetColorB() {
        return m_colorB;
    }
private:
    void ShowCameraF(GLFWwindow* window);
    void ShowCameraU(GLFWwindow* window);
    void ShowCameraB(GLFWwindow* window);
    cv::Mat m_matF;
    cv::Mat m_matU;
    cv::Mat m_matB;
    std::string m_colorF;
    std::string m_colorU;
    std::string m_colorB;
    static const int COLS{ 512 }, ROWS{ 384 };
};
