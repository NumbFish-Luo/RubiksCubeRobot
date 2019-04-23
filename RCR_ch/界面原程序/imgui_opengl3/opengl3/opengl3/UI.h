#pragma once
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <windows.h>

#include "RubiksCubeCapturer.hpp"
#include "colorDefine.hpp"
#include "Config.h"
#include "cubeTracker.h"
#include "Timer.hpp"
#include "Serial.h"
#include "Grid.hpp"
#include "k_KociMainFunc.h"

ImTextureID GetMatTextureID(const cv::Mat &mat, GLuint& tex);

static const int COLS{ 512 }, ROWS{ 384 };

// ����������
struct Rect {
    float W, H;
    // �ṩ�˷�����
    friend Rect operator*(const Rect& l, const Rect& r) {
        return Rect{ l.W * r.W, l.H * r.H };
    }
    // �ṩImgui::ImVec2����ʽת��
    operator ImVec2() const {
        return ImVec2(W, H);
    }
};

// UI��������
struct UIRect {
    // ��UI����
    Rect mainUI;
    // ����UI����
    Rect cameraUI;
    Rect cameraConfigUI;
    Rect cube3DUI;
    Rect serialPortUI;
};

// UI�ӿ���
class UI {
public:
    UI(Rect rect) : m_rect{ rect } {}
    virtual ~UI() = default;
    virtual void Show(GLFWwindow* window, bool& show) = 0;
    Rect GetRect() const {
        return m_rect;
    }
protected:
    const Rect m_rect;
};
