#include "Cube3DUI.h"

void Help() {
    std::string help[]{
        u8"-------------------------------------------------- 必读说明 --------------------------------------------------\n"
        u8"1. COM数字一直在跳是通讯异常导致的。此时请切断电源重启机器，并关闭操作界面重新测试。\n"
        u8"2. 按下Start后计时器正常计时，但机器不动，可能是摄像头配置出错了。请检查摄像头序号是否正确，若不正确请调整序号。\n"
        u8"3. 每次启动机器后时请调整好电机的角度，若没有调整好角度，很有可能会发生碰撞。\n"
        u8"4. 准备关闭机器时，建议先调整好电机角度至初始位置再关机。\n"
        u8"5. 机器不使用时，请用布盖住以防尘。\n\n"
        u8"-------- 指令集 --------\n",
        u8">> LM+\t-> L正转\n",
        u8">> LM-\t-> L反转\n",
        u8">> LM2\t-> L180度旋转\n",
        u8">> LC+\t-> L夹紧\n",
        u8">> LC-\t-> L松开\n\n",
        u8">> DM+\t-> D正转\n",
        u8">> DM-\t-> D反转\n",
        u8">> DM2\t-> D180度旋转\n",
        u8">> DC+\t-> D夹紧\n",
        u8">> DC-\t-> D松开\n" };
    for (const auto& h : help) {
        g_IOBuf.PushBack(h);
    }
}

bool ColorIdentify_1stTime(const std::string& colorStr_U, const std::string& colorStr_F, const std::string& colorStr_B, bool reset) {
    /*
    L       D
    2 5 8   0 1 2
    1 4 7   3 4 5
    0 3 6   6 7 8
    */

    for (auto& color : g_cubeColor) {
        color = "?????????";
    }

    static bool mappingOK_L{ false };
    if (mappingOK_L == false) {
        bool foundOK{ true };
        for (int i = 0; i < 9; ++i) {
            if (colorStr_U[i] == '?') {
                foundOK = false;
                break;
            }
        }
        if (foundOK == true) {
            const size_t mapping[]{ 2, 5, 8, 1, 4, 7, 0, 3, 6 };
            for (int i = 0; i < 9; ++i) {
                g_cubeColor[L][mapping[i]] = colorStr_U[i];
            }
            mappingOK_L = true;
        }
    }

    /*
    L       D
    2 5 8   0 1 2
    1 4 7   3 4 5
    0 3 6   6 7 8
    */
    static bool mappingOK_D{ false };
    if (mappingOK_D == false) {
        bool foundOK = true;
        for (int i = 0; i < 9; ++i) {
            if (colorStr_U[i + 9] == '?') {
                foundOK = false;
                break;
            }
        }
        if (foundOK == true) {
            const size_t mapping[]{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };
            for (int i = 0; i < 9; ++i) {
                g_cubeColor[D][mapping[i]] = colorStr_U[i + 9];
            }
            mappingOK_D = true;
        }
    }

    /*
    F
    8 5 2
    7 4 1
    6 3 0
    */
    static bool mappingOK_F{ false };
    if (mappingOK_F == false) {
        bool foundOK = true;
        for (int i = 0; i < 9; ++i) {
            if (colorStr_F[i] == '?') {
                if (i == 1 || i == 5) {
                    continue;
                }
                foundOK = false;
                break;
            }
        }
        if (foundOK == true) {
            const size_t mapping[]{ 8, 5, 2, 7, 4, 1, 6, 3, 0 };
            for (int i = 0; i < 9; ++i) {
                g_cubeColor[F][mapping[i]] = colorStr_F[i];
            }
            mappingOK_F = true;
        }
    }

    /*
    B
    2 1 0
    5 4 3
    8 7 6
    */
    static bool mappingOK_B{ false };
    if (mappingOK_B == false) {
        bool foundOK = true;
        for (int i = 0; i < 9; ++i) {
            if (colorStr_B[i] == '?') {
                if (i == 1 || i == 5) {
                    continue;
                }
                foundOK = false;
                break;
            }
        }
        if (foundOK == true) {
            const size_t mapping[]{ 2, 1, 0, 5, 4, 3, 8, 7, 6 };
            for (int i = 0; i < 9; ++i) {
                g_cubeColor[B][mapping[i]] = colorStr_B[i];
            }
            mappingOK_B = true;
        }
    }

    if (reset == true) {
        mappingOK_B = mappingOK_D = mappingOK_F = mappingOK_L = false;
        return false;
    }

    if (mappingOK_B &&
        mappingOK_D &&
        mappingOK_F &&
        mappingOK_L) {
        return true;
    }

    return false;
}

bool ColorIdentify_2ndTime(const std::string& colorStr_U, const std::string& colorStr_F, const std::string& colorStr_B, bool reset) {
    /*
    R       U
    6 3 0   8 7 6
    7 4 1   5 4 3
    8 5 2   2 1 0
    */
    static bool mappingOK_R{ false };
    if (mappingOK_R == false) {
        bool foundOK{ true };
        for (int i = 0; i < 9; ++i) {
            if (colorStr_U[i] == '?') {
                foundOK = false;
                break;
            }
        }
        if (foundOK == true) {
            const size_t mapping[]{ 6, 3, 0, 7, 4, 1, 8, 5, 2 };
            for (int i = 0; i < 9; ++i) {
                g_cubeColor[R][mapping[i]] = colorStr_U[i];
            }
            mappingOK_R = true;
        }
    }

    static bool mappingOK_U{ false };
    if (mappingOK_U == false) {
        bool foundOK = true;
        for (int i = 0; i < 9; ++i) {
            if (colorStr_U[i + 9] == '?') {
                foundOK = false;
                break;
            }
        }
        if (foundOK == true) {
            const size_t mapping[]{ 8, 7, 6, 5, 4, 3, 2, 1, 0 };
            for (int i = 0; i < 9; ++i) {
                g_cubeColor[U][mapping[i]] = colorStr_U[i + 9];
            }
            mappingOK_U = true;
        }
    }

    /*
    F
    0 3 6
    1 4 7
    2 5 8
    */
    static bool mappingOK_F{ false };
    if (mappingOK_F == false) {
        if (colorStr_F[3] != '?' && colorStr_F[7] != '?') {
            g_cubeColor[F][1] = colorStr_F[3];
            g_cubeColor[F][5] = colorStr_F[7];
            mappingOK_F = true;
        }
    }

    /*
    B
    6 7 8
    3 4 5
    0 1 2
    */
    static bool mappingOK_B{ false };
    if (mappingOK_B == false) {
        if (colorStr_B[3] != '?' && colorStr_B[7] != '?') {
            g_cubeColor[B][3] = colorStr_B[3];
            g_cubeColor[B][1] = colorStr_B[7];
            mappingOK_B = true;
        }
    }

    if (reset == true) {
        mappingOK_B = mappingOK_F = mappingOK_R = mappingOK_U = false;
        return false;
    }

    if (mappingOK_B &&
        mappingOK_F &&
        mappingOK_R &&
        mappingOK_U) {
        return true;
    }

    return false;
}

void InitCubeInputTxt() {
    for (auto& c : g_cubeColor) {
        c = std::string(9, '?');
    }
    for (auto& f : g_faceletStr) {
        f = "";
    }
    std::ofstream input("INPUT.txt");
    input << "U:YYYYYYYYY\n"
        << "D:WWWWWWWWW\n"
        << "F:RRRRRRRRR\n"
        << "B:OOOOOOOOO\n"
        << "L:BBBBBBBBB\n"
        << "R:GGGGGGGGG";
    input.close();
    std::ofstream next("NEXT.txt");
    next << (g_sendNext = 0);
    next.close();
}

void Reset(int step) {
    std::string tmp(9, '?');
    ColorIdentify_1stTime(tmp + tmp, tmp, tmp, true);
    ColorIdentify_2ndTime(tmp + tmp, tmp, tmp, true);
    for (auto& cc : g_cubeColor) {
        cc = tmp;
    }
    g_nowStep = step;
    g_sendNext = 0;
    InitCubeInputTxt();
    g_timerPre = GetTickCount();
    g_stopButton = false;
}

void Cube3DUI::Show(GLFWwindow* window, bool& show) {
    ImGui::BeginChild("Cube3D", m_rect, true);
    {
        ImGui::Text(u8"\t\t\t\t\t\t<3D-魔方>\n");
        {
            ImGui::BeginChild("CubeImg", ImVec2(0, 280), false);
            ImGui::Text("\t\t\t");
            ImGui::SameLine();

            static const int FPS_COUNT{ 1000 / 45 };
            static DWORD timePre{ GetTickCount() - FPS_COUNT };
            static ImTextureID texID{};
            if (GetTickCount() - timePre >= FPS_COUNT) {
                cv::Mat cubeMat{};
                do {
                    cubeMat = cv::imread("cube3d.bmp");
                } while (cubeMat.empty());
                static GLuint tex{};
                texID = GetMatTextureID(cubeMat, tex);
                timePre = GetTickCount();
            }
            static const ImVec2 cubeSize{ 280, 280 };
            ImGui::Image(texID, cubeSize);

            ImGui::EndChild();
        }

        {
            ImGui::BeginChild("StepTips", ImVec2(0, 30));
            static const std::string tips[]{
                u8"请按下开始按键",         // step -1
                u8"正在进行第一次颜色识别", // 0
                u8"正在翻转魔方",           // 1
                u8"正在进行第二次颜色识别", // 2
                u8"正在计算魔方解法",       // 3
                u8"结束计算，开始运行",     // 4
                u8"完成，请关闭程序"        // 5
            };
            static std::string scrollTips[]{
                "> > > [" + tips[0] + "] > > >" + std::string(57 - tips[0].size(), ' '),
                "> > > [" + tips[1] + "] > > >" + std::string(57 - tips[1].size(), ' '),
                "> > > [" + tips[2] + "] > > >" + std::string(57 - tips[2].size(), ' '),
                "> > > [" + tips[3] + "] > > >" + std::string(57 - tips[3].size(), ' '),
                "> > > [" + tips[4] + "] > > >" + std::string(57 - tips[4].size(), ' '),
                "> > > [" + tips[5] + "] > > >" + std::string(57 - tips[5].size(), ' '),
                "> > > [" + tips[6] + "] > > >" + std::string(57 - tips[6].size(), ' ')
            };
            static const size_t tipSize{ scrollTips[0].size() };
            static int i = 0;
            static DWORD timePre = GetTickCount();
            if (GetTickCount() - timePre > 200) {
                char tmp = scrollTips[g_nowStep + 1][i];
                if (tmp != u8' ' && tmp != u8'>' && tmp != u8'[' && tmp != u8']' && tmp != u8'\0') {
                    i += 3;
                } else {
                    i += 2;
                }
                if (i > tipSize - 1) {
                    i = 0;
                }
                timePre = GetTickCount();
            }
            for (auto& st : scrollTips[g_nowStep + 1]) {
                if (st == '>' || st == 'v' || st == '<' || st == '^') {
                    switch (i / 2 % 4) {
                    case 0:
                        st = '>';
                        break;
                    case 1:
                        st = 'v';
                        break;
                    case 2:
                        st = '<';
                        break;
                    case 3:
                        st = '^';
                    default:
                        break;
                    }
                }
            }
            ImGui::Text((scrollTips[g_nowStep + 1].substr(i, tipSize - i) + scrollTips[g_nowStep + 1].substr(0, i)).c_str());
            ImGui::EndChild();
        }

        {
            ImGui::BeginChild("Blank", ImVec2(25, 100));
            ImGui::EndChild();
        }
        ImGui::SameLine();
        {
            ImGui::BeginChild("TimerButtonL", ImVec2(145, 100));
            ImGui::Text(" "); ImGui::SameLine();
            if (ImGui::Button("  L i n k s  ")) {
                g_IOBuf.PushBack("<Links>\n");
                system(LINKS_NAME.c_str());
            }
            if (ImGui::Button("   H e l p   ")) {
                g_IOBuf.PushBack("<Help>\n");
                Help();
            }
            ImGui::Text(" "); ImGui::SameLine();
            if (ImGui::Button("  S t a f f  ")) {
                g_IOBuf.PushBack("<Staff>\n");
                g_IOBuf.PushBack(u8"--------[魔杰座：魔方机器人]--------\n");
                g_IOBuf.PushBack(u8"\t\t北京理工大学珠海学院\n  工业自动化学院  2015级机械电子工程\n");
                g_IOBuf.PushBack(u8"   程开 温开旺 罗泽奇 梁锦华 杨继康\n");
                g_IOBuf.PushBack("----------------------------------\n");
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();
        {
            ImGui::BeginChild("Timer", ImVec2(140, 105));
            ImGui::PushFont(g_bigFont);
            ImGui::Text(u8" <计时器>");
            static DWORD timer{};
            if (g_nowStep == -1) {
                timer = 0;
            } else if (g_stopButton == true) {

            } else {
                timer = GetTickCount() - g_timerPre;
            }
            ImGui::Text("  %02d:%02d", timer / 1000, (timer % 1000) / 10);
            ImGui::PopFont();
            ImGui::SameLine();
            ImGui::Text("%02d", (timer % 10) * 10);
            if (g_stopButton == true) {
                ImGui::Text("<----STOP---->");
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();
        {
            ImGui::BeginChild("TimerButtonR", ImVec2(0, 100));
            if (ImGui::Button("  S t a r t  ")) {
                g_IOBuf.PushBack("<Start>\n");
                g_IOBuf.Send("STA");
                Reset(0);
            }
            ImGui::Text(" "); ImGui::SameLine();
            if (ImGui::Button("   S t o p   ")) {
                g_IOBuf.PushBack("<Stop>\n");
                g_IOBuf.Send("END");
                g_stopButton = !g_stopButton;
            }
            if (ImGui::Button("  R e s e t  ")) {
                g_IOBuf.PushBack("<Reset>\n");
                g_IOBuf.Send("RES");
                Reset(-1);
            }
            ImGui::EndChild();
        }
        {
            ImGui::BeginChild("Color", ImVec2(0, 50));

            ImGui::Text(("\tU: " + g_cubeColor[U]).c_str()); ImGui::SameLine();
            ImGui::Text(("\t L: " + g_cubeColor[L]).c_str()); ImGui::SameLine();
            ImGui::Text(("\t F: " + g_cubeColor[F]).c_str());

            ImGui::Text(("\tD: " + g_cubeColor[D]).c_str()); ImGui::SameLine();
            ImGui::Text(("\t R: " + g_cubeColor[R]).c_str()); ImGui::SameLine();
            ImGui::Text(("\t B: " + g_cubeColor[B]).c_str());

            ImGui::EndChild();
        }

        ImGuiIO& io = ImGui::GetIO();
        ImGui::Text("\t\t\t\t\tFPS:%2.1f  step%2d  n%2d", io.Framerate, g_nowStep, g_sendNext);
        if (ImGui::IsMousePosValid()) {
            const int posX{ static_cast<int>(io.MousePos.x) };
            const int posY{ static_cast<int>(io.MousePos.y) };
            const float wheel{ io.MouseWheel };

            // 摄像头区域鼠标定位
            static const int Y{ 68 };
            static const int FX{ 13 };
            static const int UX{ 548 };
            static const int BX{ 1084 };
            if (wheel != 0) {
                if (posY >= Y && posY <= Y + ROWS) {
                    if (posX >= FX && posX <= FX + COLS) {
                        g_rccSet_F.m_radius += static_cast<int>(5 * wheel);
                    } else if (posX >= UX && posX <= UX + COLS) {
                        g_rccSet_U.m_radius += static_cast<int>(5 * wheel);
                    } else if (posX >= BX && posX <= BX + COLS) {
                        g_rccSet_B.m_radius += static_cast<int>(5 * wheel);
                    }
                }
            }
            if (ImGui::IsMouseClicked(0)) { // 点击
                if (posY >= Y && posY <= Y + ROWS) {
                    if (posX >= FX && posX <= FX + COLS) {
                        g_rccSet_F.m_horizontal = (int)COLS / 2 + FX - posX;
                        g_rccSet_F.m_vertical = (int)ROWS / 2 + Y - posY;
                    } else if (posX >= UX && posX <= UX + COLS) {
                        g_rccSet_U.m_horizontal = (int)COLS / 2 + UX - posX;
                        g_rccSet_U.m_vertical = (int)ROWS / 2 + Y - posY;
                    } else if (posX >= BX && posX <= BX + COLS) {
                        g_rccSet_B.m_horizontal = (int)COLS / 2 + BX - posX;
                        g_rccSet_B.m_vertical = (int)ROWS / 2 + Y - posY;
                    }
                }
            }
        } else {
            ImGui::Text("<INVALID>");
        }
    }
    ImGui::EndChild();
}
