#pragma once
#include "UI.h"

extern int          g_serialPortIdx;
extern bool         g_serialIsOpened;
extern std::string  g_serialTips;
extern bool         g_openSerial;
extern RccSet       g_rccSet_U;
extern RccSet       g_rccSet_F;
extern RccSet       g_rccSet_B;

class SerialPortUI : public UI {
public:
    SerialPortUI(Rect rect) : UI(rect) {}
    virtual void Show(GLFWwindow* window, bool& show);
};
