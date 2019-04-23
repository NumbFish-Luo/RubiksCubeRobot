#include "CameraConfigUI.h"


void ColorConfiger(HSV &color) {
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

bool ConfigWindow(const std::string& label, const std::string& treeNodeName, Rcc& rcc, RccSet& rccSet, const int cols, const int rows) {
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
                    } else if (svs[i].value == &rccSet.m_contrast) {
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

void SaveConfig(std::ofstream& fout, const std::string& label, const RccSet& rccSetter) {
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

void CameraConfigUI::Show(GLFWwindow* window, bool& show) {
    ImGui::BeginChild("CameraConfig", m_rect, true);

    ImGui::Text(u8"\t\t   <设置>");

    {
        ImGui::BeginChild("Configs", ImVec2(0, 910));

        if (ImGui::SmallButton("D##F")) {
            g_rccSet_F.DefaultColors();
        } ImGui::SameLine();
        ConfigWindow("F", u8"前摄像头设置", g_rcc_F, g_rccSet_F, COLS, ROWS);
        if (ImGui::SmallButton("D##U")) {
            g_rccSet_U.DefaultColors();
        } ImGui::SameLine();
        ConfigWindow("U", u8"上摄像头设置", g_rcc_U, g_rccSet_U, COLS, ROWS);
        if (ImGui::SmallButton("D##B")) {
            g_rccSet_B.DefaultColors();
        } ImGui::SameLine();
        ConfigWindow("B", u8"后摄像头设置", g_rcc_B, g_rccSet_B, COLS, ROWS);

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

