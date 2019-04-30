#include "MainUI.h"

// #define DEBUGGING

std::vector<std::string> g_cubeColor{}; // U D F B L R
std::vector<std::string> g_faceletStr{};

int          g_serialPortIdx{ 1 };
bool         g_serialIsOpened{ false };
bool         g_openSerial{ true };
RccSet       g_rccSet_U{ 2, Gtype::TYPE_B, g_hsvColor_U, COLS, ROWS };
RccSet       g_rccSet_F{ 1, Gtype::TYPE_A, g_hsvColor_F, COLS, ROWS };
RccSet       g_rccSet_B{ 3, Gtype::TYPE_A, g_hsvColor_B, COLS, ROWS };
Rcc          g_rcc_U{ g_rccSet_U };
Rcc          g_rcc_F{ g_rccSet_F };
Rcc          g_rcc_B{ g_rccSet_B };
std::string  g_serialTips{};
CSerialPort  g_mySerialPort{};
ImFont*      g_smallFont{};
ImFont*      g_bigFont{};
int          g_nowStep{ -1 };
DWORD        g_timerPre{};
bool         g_stopButton{ false };

void ReadConfig(const Config& config, const std::string& label, RccSet& rccSetter) {
    rccSetter.m_radius = config.Read(label + "radius", rccSetter.m_radius);
    rccSetter.m_horizontal = config.Read(label + "horizontal", rccSetter.m_horizontal);
    rccSetter.m_vertical = config.Read(label + "vertical", rccSetter.m_vertical);
    rccSetter.m_blurSize = config.Read(label + "blurSize", rccSetter.m_blurSize);
    rccSetter.m_rotateAngle = config.Read(label + "rotateAngle", rccSetter.m_rotateAngle);
    rccSetter.m_perspective = config.Read(label + "perspective", rccSetter.m_perspective);
    rccSetter.m_aspectRatio = config.Read(label + "aspectRatio", rccSetter.m_aspectRatio);

    for (auto& color : rccSetter.m_colors) {
        if (color.name == "ALL") {
            continue;
        }
        color.ReadColorFromFile("Config.ini", label);
    }
}

bool Init(GLFWwindow*& window) {
    InitCubeInputTxt();
    WinExec(CUBE_EXE_NAME.c_str(), SW_HIDE); // 隐藏opengl程序

    for (int i = 0; i < 6; ++i) {
        g_cubeColor.push_back(std::string(9, '?'));
        g_faceletStr.push_back(std::string());
    }

    // read data from the Config.ini
    Config config("Config.ini");
    ReadConfig(config, "F", g_rccSet_F);
    ReadConfig(config, "U", g_rccSet_U);
    ReadConfig(config, "B", g_rccSet_B);
    g_rcc_F.Set(g_rccSet_F);
    g_rcc_U.Set(g_rccSet_U);
    g_rcc_B.Set(g_rccSet_B);

    // Setup window
    glfwSetErrorCallback([](int error, const char* description) {
        fprintf(stderr, "Error %d: %s\n", error, description);
    });

    if (!glfwInit()) {
        return false;
    }
    const char* glsl_version = "#version 130";
    GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
    const int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    const int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    window = glfwCreateWindow(cx, cy, "ImGui_GLFW_OpenGL3", pMonitor/*NULL*/, NULL);

    if (window == NULL) {
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // Enable vsync 垂直同步
    gl3wInit();

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    io.Fonts->AddFontFromFileTTF("../../misc/fonts/YaHei.Consolas.1.12.ttf", 22.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    g_bigFont = io.Fonts->AddFontFromFileTTF("../../misc/fonts/YaHei.Consolas.1.12.ttf", 35.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
    g_smallFont = io.Fonts->AddFontFromFileTTF("../../misc/fonts/YaHei.Consolas.1.12.ttf", 5.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());

    static ImGuiStyle& style = ImGui::GetStyle();
    style.GrabRounding = 8;
    style.FrameRounding = 8;

    return true;
}

//int main(int, char **argv)
//{
//    // hide console window
//    HWND hwnd = FindWindow("ConsoleWindowClass", argv[0]);
//    if (hwnd) { ShowWindow(hwnd, /*SW_HIDE*/ SW_MAXIMIZE); }

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    GLFWwindow* window{};
    if (Init(window) == false) {
        return -1;
    }
    MainUI mainUI{ { 1920, 1080 } };
    bool show = true;
    mainUI.Show(window, show);
    return 0;
}
