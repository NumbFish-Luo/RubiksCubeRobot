#include "UI.h"
using namespace std;
Rect operator*(const Rect& l, const Rect& r) {
    return Rect{ l.W * r.W, l.H * r.H };
};

Rect::operator ImVec2() const {
    return ImVec2(W, H);
}

UI::UI(Rect rect) : m_rect{ rect } {}

Rect UI::GetRect() const {
    return m_rect;
}

MainUI::MainUI(Rect rect) :
    UI(rect),
    m_cameraUI{ std::make_shared<CameraUI>(rect * UI_CONFIG.cameraUI) },
    m_cameraConfigUI{ std::make_shared<CameraConfigUI>(rect * UI_CONFIG.cameraConfigUI) },
    m_cube3DUI{ std::make_shared<Cube3DUI>(rect * UI_CONFIG.cube3DUI) },
    m_serialPortUI{ std::make_shared<SerialPortUI>(rect * UI_CONFIG.serialPortUI) } {}

void MainUI::Show(GLFWwindow* window) {
    static bool mainWindow = true;
    ImGui::Begin("MainWindow", &mainWindow);
    {
        ImGui::BeginChild("Left", { m_cameraUI->GetRect().W, 0 }, false);
        {
            m_cameraUI->Show(window);
            m_cube3DUI->Show(window);
            ImGui::SameLine();
            m_serialPortUI->Show(window);
            ImGui::EndChild(); // Left
        }
        ImGui::SameLine();
        m_cameraConfigUI->Show(window);
        ImGui::End(); // MainWindow
    }
}

CameraUI::CameraUI(Rect rect) : UI(rect) {}

void CameraUI::Show(GLFWwindow* window) {
    ShowCamera(window, "F");
    ImGui::SameLine();
    ShowCamera(window, "U");
    ImGui::SameLine();
    ShowCamera(window, "B");
}

void CameraUI::ShowCamera(GLFWwindow* window, const string& name) {
    ImGui::BeginChild(name.c_str(), { m_rect.W / 3, m_rect.H }, true);
    //: TODO
    ImGui::EndChild();
}

CameraConfigUI::CameraConfigUI(Rect rect) : UI(rect) {}

void CameraConfigUI::Show(GLFWwindow* window) {
    ImGui::BeginChild("CameraConfig", m_rect, true);
    //: TODO
    ImGui::EndChild();
}

Cube3DUI::Cube3DUI(Rect rect) : UI(rect) {}

void Cube3DUI::Show(GLFWwindow* window) {
    ImGui::BeginChild("Cube3D", m_rect, true);
    //: TODO
    ImGui::EndChild();
}

SerialPortUI::SerialPortUI(Rect rect) : UI(rect) {}

void SerialPortUI::Show(GLFWwindow* window) {
    ImGui::BeginChild("SerialPort", m_rect, true);
    //: TODO
    ImGui::EndChild();
}
