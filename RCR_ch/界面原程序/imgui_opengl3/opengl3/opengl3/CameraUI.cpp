#include "CameraUI.h"

// 获取摄像头数据
void CameraUI::GetData() {
    auto RccProc = [&](Rcc& rcc, const RccSet& rccSet, std::string& colorStr, cv::Mat& frame) {
        // 传入set参数
        rcc.Set(rccSet);
        // 读取摄像头画面
        rcc.ReadFrame();
        // 防止读到空帧
        if (rcc.GetFrame().empty()) {
            return;
        }
        // 绘制魔方网格
        rcc.DrawGrid();
        // 读取颜色
        colorStr = rcc.ReadColor();
        // 获取帧
        frame = rcc.GetFrame();
    };

    // 轮流读取画面，防止cpu太高
    static int takeTurns = 0;
    switch (takeTurns) {
    case 0:
        RccProc(g_rcc_F, g_rccSet_F, m_colorF, m_matF);
        ++takeTurns;
        break;
    case 1:
        RccProc(g_rcc_U, g_rccSet_U, m_colorU, m_matU);
        ++takeTurns;
        break;
    case 2:
        RccProc(g_rcc_B, g_rccSet_B, m_colorB, m_matB);
        takeTurns = 0;
        break;
    default:
        break;
    }
}

void CameraUI::ShowCameraF(GLFWwindow* window) {
    ImGui::Text(u8"\t\t\t\t\t\t<前摄像头>");

    static GLuint tex{};
    const ImTextureID   textureID{ GetMatTextureID(m_matF, tex) };
    static const ImVec2 textureSize{ COLS * 1.f, ROWS * 1.f };
    ImGui::Image(textureID, textureSize);
    std::string getColors{ std::string(23, ' ') + g_rcc_F.GetColorString() };
    ImGui::PushItemWidth(480);
    ImGui::InputText("##Video_F", const_cast<char*>(getColors.c_str()), getColors.size(), ImGuiInputTextFlags_ReadOnly);
    ImGui::PopItemWidth();
    static int cameraIdx{ g_rccSet_F.m_index };
    ImGui::SameLine();
    ImGui::PushItemWidth(17);
    ImGui::DragInt("##CameraIdx", &cameraIdx, 0.1f, 1, 3);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (cameraIdx != g_rccSet_F.m_index) {
        g_rccSet_F.m_index = cameraIdx;
    }
}

void CameraUI::Show(GLFWwindow* window, bool& show) {
    const ImVec2 size{ m_rect.W / 3, m_rect.H };

    ImGui::BeginChild("Camera-F", size, true);
    ShowCameraF(window);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Camera-U", size, true);
    ShowCameraU(window);
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("Camera-B", size, true);
    ShowCameraB(window);
    ImGui::EndChild();
}

void CameraUI::ShowCameraU(GLFWwindow* window) {
    static GLuint tex{};
    const ImTextureID   textureID{ GetMatTextureID(m_matU, tex) };
    static const ImVec2 textureSize{ COLS * 1.f, ROWS * 1.f };
    ImGui::Text(u8"\t\t\t\t\t\t<上摄像头>");
    ImGui::Image(textureID, textureSize);
    std::string getColors{ std::string(20, ' ') + g_rcc_U.GetColorString() };
    ImGui::PushItemWidth(480);
    ImGui::InputText("##Video_U", const_cast<char*>(getColors.c_str()), getColors.size(), ImGuiInputTextFlags_ReadOnly);
    ImGui::PopItemWidth();
    static int cameraIdx{ g_rccSet_U.m_index };
    ImGui::SameLine();
    ImGui::PushItemWidth(17);
    ImGui::DragInt("##CameraIdx", &cameraIdx, 0.1f, 1, 3);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (cameraIdx != g_rccSet_U.m_index) {
        g_rccSet_U.m_index = cameraIdx;
    }
}

void CameraUI::ShowCameraB(GLFWwindow* window) {
    static GLuint tex{};
    const ImTextureID   textureID{ GetMatTextureID(m_matB, tex) };
    static const ImVec2 textureSize{ COLS * 1.f, ROWS * 1.f };
    ImGui::Text(u8"\t\t\t\t\t\t<后摄像头>");
    ImGui::Image(textureID, textureSize);
    std::string getColors{ std::string(23, ' ') + g_rcc_B.GetColorString() };
    ImGui::PushItemWidth(480);
    ImGui::InputText("##Video_B", const_cast<char*>(getColors.c_str()), getColors.size(), ImGuiInputTextFlags_ReadOnly);
    ImGui::PopItemWidth();
    static int cameraIdx{ g_rccSet_B.m_index };
    ImGui::SameLine();
    ImGui::PushItemWidth(17);
    ImGui::DragInt("##CameraIdx", &cameraIdx, 0.1f, 1, 3);
    ImGui::PopItemWidth();
    ImGui::SameLine();
    if (cameraIdx != g_rccSet_B.m_index) {
        g_rccSet_B.m_index = cameraIdx;
    }
}
