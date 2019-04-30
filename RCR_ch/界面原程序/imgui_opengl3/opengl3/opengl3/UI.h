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

// 矩形区域类
struct Rect {
    float W, H;
    // 提供乘法操作
    friend Rect operator*(const Rect& l, const Rect& r) {
        return Rect{ l.W * r.W, l.H * r.H };
    }
    // 提供Imgui::ImVec2的隐式转换
    operator ImVec2() const {
        return ImVec2(W, H);
    }
};

// UI矩形设置
struct UIRect {
    // 主UI区域
    Rect mainUI;
    // 其他UI区域
    Rect cameraUI;
    Rect cameraConfigUI;
    Rect cube3DUI;
    Rect serialPortUI;
};

// UI接口类
class UI {
public:
    UI(Rect rect) : m_rect{ rect } {}
    virtual ~UI() = default;
    virtual void Show(GLFWwindow* window, bool& show) = 0;
    Rect GetRect() const {
        return m_rect;
    }
    static ImTextureID GetMatTextureID(const cv::Mat &mat, GLuint& tex) {
        glDeleteTextures(1, &tex);  // 删除前一张无用的图，防止内存泄漏
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
        // 会引发C4312警告，但是不会造成什么坏的影响，请无视它
        return reinterpret_cast<ImTextureID>(tex);
    }
protected:
    const Rect m_rect;
};
