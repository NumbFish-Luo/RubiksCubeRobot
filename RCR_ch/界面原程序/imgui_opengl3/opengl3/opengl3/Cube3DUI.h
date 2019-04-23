#pragma once
#include "UI.h"

extern std::vector<std::string> g_cubeColor; // U D F B L R
extern std::vector<std::string> g_faceletStr;
const std::string    TITLE_NAME{ u8" Ä§·½»úÆ÷ÈË" };
const std::string CUBE_EXE_NAME{ "opengl3_3DRubiksCube.exe" };
const std::string    LINKS_NAME{ "start www.zhbit.com" };
extern int          g_nowStep;
extern ImFont*      g_smallFont;
extern ImFont*      g_bigFont;
extern bool         g_stopButton;
extern DWORD        g_timerPre;
extern RccSet       g_rccSet_U;
extern RccSet       g_rccSet_F;
extern RccSet       g_rccSet_B;
extern int   g_sendNext;

enum CubeSide {
    U, D, F, B, L, R
};

class Cube3DUI : public UI {
public:
    Cube3DUI(Rect rect) : UI(rect) {}
    virtual void Show(GLFWwindow* window, bool& show);
private:
    static const int COLS{ 512 }, ROWS{ 384 };
};
