#pragma once
#include "UI.h"

extern RccSet       g_rccSet_U;
extern RccSet       g_rccSet_F;
extern RccSet       g_rccSet_B;
extern Rcc          g_rcc_U;
extern Rcc          g_rcc_F;
extern Rcc          g_rcc_B;

struct SetVSlider {
    SetVSlider()
        : label(), value(NULL), min(0), max(0), tips() {}
    SetVSlider(const std::string _label, int* _value, const int _min, const int _max, const std::string _tips)
        : label(_label), value(_value), min(_min), max(_max), tips(_tips) {}
    SetVSlider(const SetVSlider& s)
        : label(s.label), value(s.value), min(s.min), max(s.max), tips(s.tips) {}
    SetVSlider& operator=(const SetVSlider&) = delete;
    const std::string label;
    int*              value;
    const int         min;
    const int         max;
    const std::string tips;
};

// œ‡ª˙…Ë÷√UI
class CameraConfigUI : public UI {
public:
    CameraConfigUI(Rect rect) : UI(rect) {}
    virtual void Show(GLFWwindow* window, bool& show);
private:
    static const int COLS{ 512 }, ROWS{ 384 };
};
