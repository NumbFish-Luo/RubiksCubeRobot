#pragma once
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

class UI;
class MainUI;
class CameraUI;
class CameraConfigUI;
class Cube3DUI;
class SerialPortUI;

struct Rect {
    float W, H;
    friend Rect operator*(const Rect& l, const Rect& r);
    operator ImVec2() const;
};

struct UIConfig {
    Rect mainUI;

    Rect cameraUI;
    Rect cameraConfigUI;
    Rect cube3DUI;
    Rect serialPortUI;
};

constexpr UIConfig UI_CONFIG{
    { 1920.f, 1080.f },
    // 以下为mainUI的百分比
    { 0.84f, 0.48f },
    { 0.12f, 0.96f },
    { 0.28f, 0.48f },
    { 0.56f, 0.48f }
};

class UI { // UI接口类
public:
    UI(Rect rect);
    virtual ~UI() = default;
    virtual void Show(GLFWwindow* window) = 0;
    Rect GetRect() const;
protected:
    const Rect m_rect;
};

class MainUI : public UI {
public:
    MainUI(Rect rect);
    virtual void Show(GLFWwindow* window);
private:
    std::shared_ptr<CameraUI> m_cameraUI;
    std::shared_ptr<CameraConfigUI> m_cameraConfigUI;
    std::shared_ptr<Cube3DUI> m_cube3DUI;
    std::shared_ptr<SerialPortUI> m_serialPortUI;
};

class CameraUI : public UI {
public:
    CameraUI(Rect rect);
    virtual void Show(GLFWwindow* window);
private:
    void ShowCamera(GLFWwindow* window, const std::string& name);
};

class CameraConfigUI : public UI {
public:
    CameraConfigUI(Rect rect);
    virtual void Show(GLFWwindow* window);
};

class Cube3DUI : public UI {
public:
    Cube3DUI(Rect rect);
    virtual void Show(GLFWwindow* window);
};

class SerialPortUI : public UI {
public:
    SerialPortUI(Rect rect);
    virtual void Show(GLFWwindow* window);
};
