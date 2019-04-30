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

constexpr int COLS{ 512 }, ROWS{ 384 };

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
    static ImTextureID GetMatTextureID(const cv::Mat &mat, GLuint& tex) {
        glDeleteTextures(1, &tex);  // ɾ��ǰһ�����õ�ͼ����ֹ�ڴ�й©
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
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
        // ������C4312���棬���ǲ������ʲô����Ӱ�죬��������
        return reinterpret_cast<ImTextureID>(tex);
    }
protected:
    const Rect m_rect;
};
