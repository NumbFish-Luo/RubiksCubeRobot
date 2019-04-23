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
        SYSTEMTIME WriteTime = { 0 }; // ����޸�ʱ��
        if (!::FileTimeToSystemTime(&FindFileData.ftLastWriteTime, &WriteTime)) {
            return -1;
        }

        int time = WriteTime.wMilliseconds + WriteTime.wSecond * 1000;
        return time;
    };

    static int ldcm_outputTxtWriteTime = GetFileWriteTime("LDCM_output.txt");
    static size_t bufLenPre{ strlen(g_IOBuf.output) };

    switch (g_nowStep) {
    case -1: // step == -1 ���¿�ʼ��ť����step = 0
        break;
    case 0: // step == 0  ��һ����ɫʶ�� ��
        if (ColorIdentify_1stTime(m_cameraUI->GetColorU(), m_cameraUI->GetColorF(), m_cameraUI->GetColorB())) // ��һ����ɫʶ��
        {
            g_IOBuf.PushBack("\n>> [Flip] ");
            g_IOBuf.Send("DC- LM2 DC+ LC- DM2 LC+ "); // ���ͷ�תָ��
            g_nowStep = 1;
        }
        break;
    case 1: // step == 1  ��תħ�� ��
        if (strlen(g_IOBuf.output) > 3 && g_IOBuf.output[strlen(g_IOBuf.output) - 3] == '!') // ����[Finish!]
        {
            g_nowStep = 2;
        }
        break;
    case 2: // step == 2  �ڶ�����ɫʶ�� �� // ��ԭ�ڶ���ħ����ʱ������Ῠס
#ifndef DEBUGGING
        if (ColorIdentify_2ndTime(m_cameraUI->GetColorU(), m_cameraUI->GetColorF(), m_cameraUI->GetColorB())) // �ڶ�����ɫʶ�� 
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
            // д��INPUT.txt
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
                g_IOBuf.PushBack(u8"\n>> [����] ��ɫ���������������ͷ�Ƿ�������ȷ\n");
                g_nowStep = -1;
                break;
            }
            if (TranslateToLD() == READ_FAIL) {
                g_IOBuf.PushBack(u8"\n>> [����] ��ɫ���������������ͷ�Ƿ�������ȷ\n");
                g_nowStep = -1;
                break;
            }
            g_nowStep = 3;
        }
        break;
    case 3: // step == 3  ����ⷨ ��
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
    case 4: // step == 4  ��ɼ��㣬��ʼִ�� ����
        if (strlen(g_IOBuf.output) > 3 && g_IOBuf.output[strlen(g_IOBuf.output) - 3] == '!') // ����[Finish!]������ͣ��ʱ
        {
            g_IOBuf.Send("AC-");
            g_stopButton = true; // ��ͣ��ʱ
            g_nowStep = 5;
        } else {
            std::ofstream next{ "NEXT.txt" };
            next.clear();
            next << g_sendNext;
            next.close();
        }
        break;
    case 5: // step == 5  ���
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
                g_serialTips = u8"���ڳ�ʼ��ʧ��";
                g_serialIsOpened = false;
                if (++g_serialPortIdx > 16) {
                    g_serialPortIdx = 1;
                }
                Sleep(100);
                continue;
            } else {
                g_serialTips = u8"���ڳ�ʼ���ɹ�";
            }

            // open listen threaad
            if (!g_mySerialPort.OpenListenThread()) {
                g_serialTips = u8"�����̴߳�ʧ��";
                g_serialIsOpened = false;
                Sleep(1000);
                continue;
            } else {
                g_serialTips = u8"�����̴߳򿪳ɹ�";
            }

            // read input data send to output
            g_serialTips = u8"�ȴ��û�����";
            g_serialIsOpened = true;
            while (run && g_openSerial) {
                SendMsgToSerial();
            }
            g_mySerialPort.CloseListenTread();
            g_serialIsOpened = false;
            //Sleep(1000); // ��֮bug��û��Sleepһ��ʱ��Ļ����˳����ڵ�ʱ��ͻ���ѭ��������
        }
        //Sleep(1000); // ��֮bug��û��Sleepһ��ʱ��Ļ����˳����ڵ�ʱ��ͻ���ѭ��������
    }
    return;
}

// ��ʾ��UI
void MainUI::Show(GLFWwindow* window, bool& show) {
    while (!glfwWindowShouldClose(window) && show) {
        // �ȴ�����֡
        NewFrame();
        // Ȼ���ȡ����ͷ����
        m_cameraUI->GetData();
        // ��������
        DealData();
        // ��ʼ���������
        ImGui::Begin(u8"ħ����", &show, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        {
            // �������
            ImGui::BeginChild("Left", { m_cameraUI->GetRect().W + 16, 0 }, false);
            {
                // ��������ͷ����
                m_cameraUI->Show(window, show);
                // ����3Dħ������
                m_cube3DUI->Show(window, show);
                ImGui::SameLine();
                // ���ƶԻ�������
                m_serialPortUI->Show(window, show);
            }
            ImGui::EndChild(); // End Left
            ImGui::SameLine();
            // �����Ҳ�
            m_cameraConfigUI->Show(window, show);
        }
        ImGui::End(); // End ħ����
        // ��ʼ���ڳ���
        static Timer serial(SerialMain, show, Timer::Interval(1000), true); // ����SerialMain
        serial.start(true);
        // �����Ⱦ
        Render(window, { 0.45f, 0.55f, 0.60f, 1.00f });
    }
    // �������
    CleanUp(window);
    //g_showMainWindow = false;
}
