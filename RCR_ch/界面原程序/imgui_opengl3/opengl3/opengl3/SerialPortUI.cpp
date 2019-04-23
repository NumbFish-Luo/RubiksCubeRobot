#include "SerialPortUI.h"

int CallBackInputWrap(ImGuiTextEditCallbackData* data) {
    float controlWidth = *(float*)data->UserData;
    float textWidth = ImGui::CalcTextSize(data->Buf).x;

    if (controlWidth <= textWidth) {
        std::string textStr_tmp = data->Buf;
        float  textWidth_tmp = ImGui::CalcTextSize(textStr_tmp.data()).x;
        size_t wrapPos = textStr_tmp.size();

        while (wrapPos > 0) {
            if (controlWidth >= textWidth_tmp) {
                data->InsertChars(static_cast<int>(wrapPos), "\n"); // "hard" word wrap...
                data->BufDirty = true;
                break;
            } else {
                textStr_tmp.pop_back();
                textWidth_tmp = ImGui::CalcTextSize(textStr_tmp.data()).x;
                --wrapPos;
            }
        }
    }
    return data->BufTextLen;
}

void SerialPortUI::Show(GLFWwindow* window, bool& show) {
    ImGui::BeginChild("SerialPort", m_rect, true);
    {
        ImGui::Text(u8"\t\t\t\t\t\t\t\t\t\t\t\t\t  <对话框>");

        ImVec2 outputTextAreaSize = ImVec2(m_rect.W, ImGui::GetTextLineHeight() * 14.5f); // set the output text window size 
        ImGui::InputTextMultiline("##Output Text", g_IOBuf.output,
            IM_ARRAYSIZE(g_IOBuf.output), outputTextAreaSize,
            ImGuiInputTextFlags_ReadOnly);

        if (g_IOBuf.outputChange == true) {
            ImGui::BeginChild(ImGui::GetID("##Output Text"));
            ImGui::SetScrollHere(1.0f);
            ImGui::EndChild();
            g_IOBuf.outputChange = false;
        }

        // 左侧两个选项
        {
            ImGui::BeginChild("2Checkbox", ImVec2(990, ImGui::GetTextLineHeight() * 2), false);

            ImGui::PushItemWidth(50.0f);
            ImGui::DragInt("##SerialPortIdx_test", &g_serialPortIdx, 0.01f, 1, 16, "COM%.0f"); ImGui::SameLine();
            ImGui::PopItemWidth();

            ImGui::RadioButton("##Serial", g_serialIsOpened);
            if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
                ImGui::SetTooltip(g_serialTips.c_str());
            }
            ImGui::SameLine();
            if (g_serialIsOpened == true) {
                ImGui::Text("ON ");
            } else {
                ImGui::Text("OFF");
            }

            ImGui::SameLine(); ImGui::Text("  "); ImGui::SameLine();
            ImGui::Checkbox(u8"打开串口", &g_openSerial);

            ImGui::SameLine(); ImGui::Text("  "); ImGui::SameLine();
            static bool testMode;
            ImGui::Checkbox(u8"测试模式", &testMode);
            g_rccSet_F.m_testMode = testMode;
            g_rccSet_U.m_testMode = testMode;
            g_rccSet_B.m_testMode = testMode;

            ImGui::SameLine(); ImGui::Text("  "); ImGui::SameLine();
            static bool whiteBlanceMode;
            ImGui::Checkbox(u8"白平衡模式", &whiteBlanceMode);
            g_rccSet_F.m_whiteBlanceMode = whiteBlanceMode;
            g_rccSet_U.m_whiteBlanceMode = whiteBlanceMode;
            g_rccSet_B.m_whiteBlanceMode = whiteBlanceMode;

            ImGui::SameLine(); ImGui::Text("  "); ImGui::SameLine();
            static bool rgbMode;
            ImGui::Checkbox(u8"RGB", &rgbMode);
            g_rccSet_F.m_rgbMode = rgbMode;
            g_rccSet_U.m_rgbMode = rgbMode;
            g_rccSet_B.m_rgbMode = rgbMode;

            ImGui::EndChild();
        }
        ImGui::SameLine();
        if (ImGui::Button(u8"清除!")) {
            memset(g_IOBuf.output, '\0', g_IOBuf.outputIndex * sizeof(char));
            g_IOBuf.outputIndex = 0;
        }

        // set the input text window size 
        ImVec2 inputTextAreaSize = ImVec2(m_rect.W, ImGui::GetTextLineHeight() * 5.5f);
        float inputTextAreaWidth = ImGui::CalcItemWidth() * 1.5f;

        // show the input text window
        static bool focusOnInputTextItem = false;
        if (focusOnInputTextItem == true) {
            ImGui::SetKeyboardFocusHere();
            focusOnInputTextItem = false;
        }
        if (ImGui::InputTextMultiline("##input Text", g_IOBuf.input, IM_ARRAYSIZE(g_IOBuf.input), inputTextAreaSize,
            ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CtrlEnterForNewLine | ImGuiInputTextFlags_CallbackAlways,
            CallBackInputWrap, &(inputTextAreaWidth)) == true) {
            g_IOBuf.PushBack(">> ");
            g_IOBuf.Send(g_IOBuf.input);
            focusOnInputTextItem = true;
        }

        // show the send button
        {
            ImGui::BeginChild("ControlButton", ImVec2(990, 0), false);
            static bool abFlag = true;
            if (ImGui::ArrowButton("AB", ImGuiDir())) {
                abFlag = !abFlag;
            }
            if (abFlag == true) {
                ImGui::SameLine();
                ImGui::BeginChild("ABWindow");
                ImGui::BeginChild("Blank", ImVec2(0, 1));
                ImGui::EndChild();
                if (ImGui::SmallButton(u8"L顺")) {
                    g_IOBuf.PushBack(">> "); g_IOBuf.Send("LMF");
                } ImGui::SameLine();
                if (ImGui::SmallButton(u8"L逆")) {
                    g_IOBuf.PushBack(">> "); g_IOBuf.Send("LMR");
                } ImGui::SameLine();
                ImGui::Text("/"); ImGui::SameLine();
                if (ImGui::SmallButton(u8"D顺")) {
                    g_IOBuf.PushBack(">> "); g_IOBuf.Send("DMF");
                } ImGui::SameLine();
                if (ImGui::SmallButton(u8"D逆")) {
                    g_IOBuf.PushBack(">> "); g_IOBuf.Send("DMR");
                } ImGui::SameLine();
                ImGui::Text("/"); ImGui::SameLine();
                if (ImGui::SmallButton(u8"松开")) {
                    g_IOBuf.PushBack(">> "); g_IOBuf.Send("AC-");
                } ImGui::SameLine();
                if (ImGui::SmallButton(u8"夹紧")) {
                    g_IOBuf.PushBack(">> "); g_IOBuf.Send("AC+");
                } ImGui::SameLine();
                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();
        if (ImGui::Button(u8"发送!")) {
            g_IOBuf.PushBack(">> ");
            g_IOBuf.Send(g_IOBuf.input);
            focusOnInputTextItem = true;
        }
    }
    ImGui::EndChild();
}
