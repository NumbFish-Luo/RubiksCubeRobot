#include "MainUI.h"

void NewFrame() {
    glfwPollEvents();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Render(GLFWwindow* window, const ImVec4 &clear_color) {
    ImGui::Render();
    int display_w, display_h;
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
}

MainUI::MainUI(Rect rect) :
    UI(rect),
    m_cameraUI{ std::make_shared<CameraUI>(rect * UI_RECT.cameraUI) },
    m_cameraConfigUI{ std::make_shared<CameraConfigUI>(rect * UI_RECT.cameraConfigUI) },
    m_cube3DUI{ std::make_shared<Cube3DUI>(rect * UI_RECT.cube3DUI) },
    m_serialPortUI{ std::make_shared<SerialPortUI>(rect * UI_RECT.serialPortUI) } {}

void CleanUp(GLFWwindow *&window) {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    std::string temp = std::string("TASKKILL /F /IM ") + CUBE_EXE_NAME;
    system(temp.c_str());
}

void MainUI::DealData() {
    auto GetFileWriteTime = [](const LPCTSTR& lpszFilePath) {
        WIN32_FIND_DATA FindFileData = { 0 };

        HANDLE hFile = ::FindFirstFile(lpszFilePath, &FindFileData);

        if (INVALID_HANDLE_VALUE == hFile) {
            return -1;
        }
        SYSTEMTIME WriteTime = { 0 }; // 最近修改时间
        if (!::FileTimeToSystemTime(&FindFileData.ftLastWriteTime, &WriteTime)) {
            return -1;
        }

        int time = WriteTime.wMilliseconds + WriteTime.wSecond * 1000;
        return time;
    };

    static int ldcm_outputTxtWriteTime = GetFileWriteTime("LDCM_output.txt");
    static size_t bufLenPre{ strlen(g_IOBuf.output) };

    switch (g_nowStep) {
    case -1: // step == -1 按下开始按钮，令step = 0
        break;
    case 0: // step == 0  第一次颜色识别 ↓
        if (ColorIdentify_1stTime(m_cameraUI->GetColorU(), m_cameraUI->GetColorF(), m_cameraUI->GetColorB())) // 第一次颜色识别
        {
            g_IOBuf.PushBack("\n>> [Flip] ");
            g_IOBuf.Send("DC- LM2 DC+ LC- DM2 LC+ "); // 发送翻转指令
            g_nowStep = 1;
        }
        break;
    case 1: // step == 1  翻转魔方 ↑
        if (strlen(g_IOBuf.output) > 3 && g_IOBuf.output[strlen(g_IOBuf.output) - 3] == '!') // 遇到[Finish!]
        {
            g_nowStep = 2;
        }
        break;
    case 2: // step == 2  第二次颜色识别 ↓ // 还原第二个魔方的时候这里会卡住
#ifndef DEBUGGING
        if (ColorIdentify_2ndTime(m_cameraUI->GetColorU(), m_cameraUI->GetColorF(), m_cameraUI->GetColorB())) // 第二次颜色识别 
#endif
        {
#ifdef DEBUGGING
            //test
            g_cubeColor[U] = "RRGBYGRWG";
            g_cubeColor[D] = "YYOOWWYRY";
            g_cubeColor[F] = "WBYRRYBBB";
            g_cubeColor[B] = "OGWROGOWO";
            g_cubeColor[L] = "GOBWBBGYR";
            g_cubeColor[R] = "ROWGGYWOB";
#endif
            // 写入INPUT.txt
            std::ofstream input("INPUT.txt");
            input << "U:" + g_cubeColor[U] + '\n'
                << "D:" + g_cubeColor[D] + '\n'
                << "F:" + g_cubeColor[F] + '\n'
                << "B:" + g_cubeColor[B] + '\n'
                << "L:" + g_cubeColor[L] + '\n'
                << "R:" + g_cubeColor[R];
            input.close();

            g_faceletStr[U] = "U:" + g_cubeColor[U];
            g_faceletStr[D] = "D:" + g_cubeColor[D];
            g_faceletStr[F] = "F:" + g_cubeColor[F];
            g_faceletStr[B] = "B:" + g_cubeColor[B];
            g_faceletStr[L] = "L:" + g_cubeColor[L];
            g_faceletStr[R] = "R:" + g_cubeColor[R];

            if (SolveCube(g_faceletStr) == READ_FAIL) {
                g_IOBuf.PushBack(u8"\n>> [错误] 颜色输入错误，请检查摄像头是否连接正确\n");
                g_nowStep = -1;
                break;
            }
            if (TranslateToLD() == READ_FAIL) {
                g_IOBuf.PushBack(u8"\n>> [错误] 颜色输入错误，请检查摄像头是否连接正确\n");
                g_nowStep = -1;
                break;
            }
            g_nowStep = 3;
        }
        break;
    case 3: // step == 3  计算解法 ↑
        if (ldcm_outputTxtWriteTime != GetFileWriteTime("LDCM_output.txt")) {
            Sleep(100);
            std::ifstream send{ "LDCM_output.txt" };
            send.seekg(0, std::ios::beg);
            std::string sendOrder{};
            getline(send, sendOrder);
            send.close();
            g_IOBuf.PushBack(">> [LDCM] ");
            g_IOBuf.Send(sendOrder);
            bufLenPre = strlen(g_IOBuf.output);
            g_nowStep = 4;
            ldcm_outputTxtWriteTime = GetFileWriteTime("LDCM_output.txt");
        }
        break;
    case 4: // step == 4  完成计算，开始执行 ↑↓
        if (strlen(g_IOBuf.output) > 3 && g_IOBuf.output[strlen(g_IOBuf.output) - 3] == '!') // 遇到[Finish!]，则暂停计时
        {
            g_IOBuf.Send("AC-");
            g_stopButton = true; // 暂停计时
            g_nowStep = 5;
        } else {
            std::ofstream next{ "NEXT.txt" };
            next.clear();
            next << g_sendNext;
            next.close();
        }
        break;
    case 5: // step == 5  完毕
        break;
    default:
        break;
    }
}

void SendMsgToSerial() {
    if (g_IOBuf.serialBufferChange == true) {
        std::string msg{};
        for (int i = 0; i < strlen(g_IOBuf.serialBuffer); ++i) {
            if (g_IOBuf.serialBuffer[i] == '\n' ||
                g_IOBuf.serialBuffer[i] == '\r' ||
                g_IOBuf.serialBuffer[i] == ' ') {
                continue;
            }
            msg.push_back(g_IOBuf.serialBuffer[i]);
        }
        msg += '\r';
        g_mySerialPort.WriteData((unsigned char*)(msg).c_str(), static_cast<unsigned int>((msg).size()));
        g_IOBuf.serialBufferChange = false;
    }
}

void SerialMain(bool& run) {
    while (run) {
        g_serialTips = "OFF";
        while (run && g_openSerial) {
            g_serialTips = "ON";
            // Init port
            if (!g_mySerialPort.InitPort(g_serialPortIdx)) {
                g_serialTips = u8"串口初始化失败";
                g_serialIsOpened = false;
                if (++g_serialPortIdx > 16) {
                    g_serialPortIdx = 1;
                }
                Sleep(100);
                continue;
            } else {
                g_serialTips = u8"串口初始化成功";
            }

            // open listen threaad
            if (!g_mySerialPort.OpenListenThread()) {
                g_serialTips = u8"监听线程打开失败";
                g_serialIsOpened = false;
                Sleep(1000);
                continue;
            } else {
                g_serialTips = u8"监听线程打开成功";
            }

            // read input data send to output
            g_serialTips = u8"等待用户输入";
            g_serialIsOpened = true;
            while (run && g_openSerial) {
                SendMsgToSerial();
            }
            g_mySerialPort.CloseListenTread();
            g_serialIsOpened = false;
            //Sleep(1000); // 谜之bug，没有Sleep一点时间的话，退出窗口的时候就会死循环在这里
        }
        //Sleep(1000); // 谜之bug，没有Sleep一点时间的话，退出窗口的时候就会死循环在这里
    }
    return;
}

// 显示主UI
void MainUI::Show(GLFWwindow* window, bool& show) {
    while (!glfwWindowShouldClose(window) && show) {
        // 先创建新帧
        NewFrame();
        // 然后获取摄像头数据
        m_cameraUI->GetData();
        // 处理数据
        DealData();
        // 开始主界面绘制
        ImGui::Begin(u8"魔杰作", &show, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        {
            // 绘制左侧
            ImGui::BeginChild("Left", { m_cameraUI->GetRect().W + 16, 0 }, false);
            {
                // 绘制摄像头区域
                m_cameraUI->Show(window, show);
                // 绘制3D魔方区域
                m_cube3DUI->Show(window, show);
                ImGui::SameLine();
                // 绘制对话框区域
                m_serialPortUI->Show(window, show);
            }
            ImGui::EndChild(); // End Left
            ImGui::SameLine();
            // 绘制右侧
            m_cameraConfigUI->Show(window, show);
        }
        ImGui::End(); // End 魔杰作
        // 开始串口程序
        static Timer serial(SerialMain, show, Timer::Interval(1000), true); // 启动SerialMain
        serial.start(true);
        // 最后渲染
        Render(window, { 0.45f, 0.55f, 0.60f, 1.00f });
    }
    // 清除数据
    CleanUp(window);
    //g_showMainWindow = false;
}
