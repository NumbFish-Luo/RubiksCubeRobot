#include "k_Step.h"
#include <Windows.h>
#include "Serial.h"
#include <iostream>
#include <fstream>

OSteps::OSteps() {
}
OSteps::OSteps(const OSteps& oSteps) : m_datas{ oSteps.m_datas } {
}
void OSteps::Clear() {
    m_datas.clear();
}
void OSteps::PushBack(const std::string& step) {
    m_datas.push_back(step);
}
std::string OSteps::Get(size_t idx, const CoordSys& coord) const {
    std::string result = m_datas[idx];
    result[0] = coord[result[0]];
    return result;
}
size_t OSteps::Size() const {
    return m_datas.size();
}

ASteps::ASteps(const OSteps& oSteps) :
    m_coord{}, m_oSteps{}, m_aSteps{} {
    Translate(oSteps);
}
ASteps::ASteps(const ASteps& aSteps) :
    m_coord{ aSteps.m_coord },
    m_oSteps{ aSteps.m_oSteps },
    m_aSteps{ aSteps.m_aSteps } {
}
void ASteps::Translate(const OSteps& oSteps) {
    m_oSteps = oSteps;
    m_aSteps.clear();
    size_t size = oSteps.Size();
    size_t stepNumber = 0;
    for (size_t i = 0; i < size;) {
        stepNumber = 0;

        switch (GetOStep(i)[0]) {
        case 'U':
            // 直接扔给U函数去处理，并返回截取的原始步数的数量
            stepNumber = DealU(i, i + 1);
            break;
        case 'D':
            stepNumber = DealD(i, i + 1);
            break;
        case 'L':
            stepNumber = DealL(i, i + 1);
            break;
        case 'R':
            stepNumber = DealR(i, i + 1);
            break;
        case 'F':
            stepNumber = DealF(i, i + 1);
            break;
        case 'B':
            stepNumber = DealB(i, i + 1);
            break;
        }
        if (stepNumber == 0) {
            return;
        }
        i += stepNumber;
    }
}
void ASteps::Clear() {
    m_coord.Init();
    m_oSteps.Clear();
    m_aSteps.clear();
}
const std::string& ASteps::operator[](size_t idx) {
    return m_aSteps[idx];
}
size_t ASteps::AStepsSize() const {
    return m_aSteps.size();
}
size_t ASteps::OStepsSize() const {
    return m_oSteps.Size();
}

#define A_LAST           (m_aSteps.size() - 1)
#define A_LAST_IS(X)     (m_aSteps.size() > 0 && m_aSteps[A_LAST][0] == #X[0])
#define AND_A_LAST_IS(X) (m_aSteps[A_LAST][1] == #X[0])
#define IS_O_LAST(X)     (X + 1 >= m_oSteps.Size())
#define IS_A_LAST(X)     (X + 1 >= m_aSteps.AStepsSize())

size_t ASteps::DealU(size_t x, size_t y) {
    // Ux _ [1]
    // AL2 Dx
    if (IS_O_LAST(x)) {
        PushBack("AL2");
        PushBack("D_", x);
        return 1;
    }
    switch (GetOStep(y)[0]) {
    case 'U':
        // Ux Uy [0]
        // error
        return 0;
    case 'D':
        // Ux Dy [1 + ?]
        // Dy ->(Ux ?z)
        PushBack(GetOStep(y));
        return 1 + DealU(x, y + 1);
    case 'L':
        // Ux Ly [2]
        // AL2 Dx Ly
        PushBack("AL2");
        PushBack("D_", x);
        PushBack(GetOStep(y));
        return 2;
    case 'R':
        // Ux Ry [2]
        // AL2 AD2 Dx Ly
        // AD2 AL2 Dx Ly
        if (A_LAST_IS(L)) {
            PushBack("AL2");
            PushBack("AD2");
        } else {
            PushBack("AD2");
            PushBack("AL2");
        }
        PushBack("D_", x);
        PushBack("L_", y);
        return 2;
    case 'F':
        // Ux Fy [2]
        // AL2 AD+ Dx Ly
        // AL+ AD- Lx Dy
        // AD- AL2 Dx Ly
        if (A_LAST_IS(L)) {
            if (AND_A_LAST_IS(2)) {
                PushBack("AL2");
                PushBack("AD+");
                PushBack("D_", x);
                PushBack("L_", y);
            } else {
                PushBack("AL+");
                PushBack("AD-");
                PushBack("L_", x);
                PushBack("D_", y);
            }
        } else {
            PushBack("AD-");
            PushBack("AL2");
            PushBack("D_", x);
            PushBack("L_", y);
        }
        return 2;
    case 'B':
        // Ux By [2]
        // AL2 AD - Dx Ly
        // AL- AD+ Lx Dy
        // AD+ AL2 Dx Ly
        if (A_LAST_IS(L)) {
            if (AND_A_LAST_IS(2)) {
                PushBack("AL2");
                PushBack("AD-");
                PushBack("D_", x);
                PushBack("L_", y);
            } else {
                PushBack("AL-");
                PushBack("AD+");
                PushBack("L_", x);
                PushBack("D_", y);
            }
        } else {
            PushBack("AD+");
            PushBack("AL2");
            PushBack("D_", x);
            PushBack("L_", y);
        }
        return 2;
    default:
        return 0;
    }
}
size_t ASteps::DealD(size_t x, size_t y) {
    // Dx _ [1]
    // Dx
    PushBack(GetOStep(x));
    if (IS_O_LAST(x)) {
        return 1;
    }
    switch (GetOStep(y)[0]) {
    case 'U':
        // Dx Uy [1 + ?]
        // Dx ->(Uy ?z)
        return 1 + DealU(y, y + 1);
    case 'D':
        // Dx Dy [0]
        // error
        return 0;
    case 'L':
        // Dx Ly [2]
        // Dx Ly
        PushBack(GetOStep(y));
        return 2;
    case 'R':
        // Dx Ry [1 + ?]
        // Dx ->(Ry ?z)
        return 1 + DealR(y, y + 1);
    case 'F':
        // Dx Fy [1 + ?]
        // Dx ->(Fy ?z)
        return 1 + DealF(y, y + 1);
    case 'B':
        // Dx By [1 + ?]
        // Dx ->(By ?z)
        return 1 + DealB(y, y + 1);
    default:
        return 0;
    }
}
size_t ASteps::DealL(size_t x, size_t y) {
    // Lx _ [1]
    // Lx
    PushBack(GetOStep(x));
    if (IS_O_LAST(x)) {
        return 1;
    }
    switch (GetOStep(y)[0]) {
    case 'U':
        // Lx Uy [1 + ?]
        // Lx ->(Uy ?z)
        return 1 + DealU(y, y + 1);
    case 'D':
        // Lx Dy [1 + ?]
        // Lx ->(Dy ?z)
        PushBack(GetOStep(y));
        return 2;
    case 'L':
        // Lx Ly [0]
        // error
        return 0;
    case 'R':
        // Lx Ry [1 + ?]
        // Lx ->(Ry ?z)
        return 1 + DealR(y, y + 1);
    case 'F':
        // Lx Fy [1 + ?]
        // Lx ->(Fy ?z)
        return 1 + DealF(y, y + 1);
    case 'B':
        // Lx By [1 + ?]
        // Lx ->(By ?z)
        return 1 + DealB(y, y + 1);
    default:
        return 0;
    }
}
size_t ASteps::DealR(size_t x, size_t y) {
    // Rx _ [1]
    // AD2 Lx
    if (IS_O_LAST(x)) {
        PushBack("AD2");
        PushBack("L_", x);
        return 1;
    }

    switch (GetOStep(y)[0]) {
    case 'U':
        // Rx Uy [2]
        // AL2 AD2 Lx Dy
        // AD2 AL2 Lx Dy
        if (A_LAST_IS(L)) {
            PushBack("AL2");
            PushBack("AD2");
        } else {
            PushBack("AD2");
            PushBack("AL2");
        }
        PushBack("L_", x);
        PushBack("D_", y);
        return 2;
    case 'D':
        // Rx Dy [2]
        // AD2 Lx Dy
        PushBack("AD2");
        PushBack("L_", x);
        PushBack("D_", y);
        return 2;
    case 'L':
        // Rx Ly [1 + ?]
        // Ly ->(Rx ?z)
        PushBack(GetOStep(y));
        return 1 + DealR(x, y + 1);
    case 'R':
        // Rx Ry [0]
        // error
        return 0;
    case 'F':
        // Rx Fy [2]
        // AD2 AL- Lx Dy
        // AD- AL+ Dx Ly
        // AL+ AD2 Lx Dy
        if (A_LAST_IS(D)) {
            if (AND_A_LAST_IS(2)) {
                PushBack("AD2");
                PushBack("AL-");
                PushBack("L_", x);
                PushBack("D_", y);
            } else {
                PushBack("AD-");
                PushBack("AL+");
                PushBack("D_", x);
                PushBack("L_", y);
            }
        } else {
            PushBack("AL+");
            PushBack("AD2");
            PushBack("L_", x);
            PushBack("D_", y);
        }
        return 2;
    case 'B':
        // Rx By [2]
        // AD2 AL+ Lx Dy
        // AD+ AL- Dx Ly
        // AL- AD2 Lx Dy
        if (A_LAST_IS(D)) {
            if (AND_A_LAST_IS(2)) {
                PushBack("AD2");
                PushBack("AL+");
                PushBack("L_", x);
                PushBack("D_", y);
            } else {
                PushBack("AD+");
                PushBack("AL-");
                PushBack("D_", x);
                PushBack("L_", y);
            }
        } else {
            PushBack("AL-");
            PushBack("AD2");
            PushBack("L_", x);
            PushBack("D_", y);
        }
        return 2;
    default:
        return 0;
    }
}
size_t ASteps::DealF(size_t x, size_t y) {
    // Fx _ [1]
    // AL+ Dx
    // AD- Lx
    if (IS_O_LAST(x)) {
        if (A_LAST_IS(L)) {
            PushBack("AL+");
            PushBack("D_", x);
        } else {
            PushBack("AD-");
            PushBack("L_", x);
        }
        return 1;
    }
    switch (GetOStep(y)[0]) {
    case 'U':
        // Fx Uy [2]
        // AL2 AD+ Lx Dy
        // AL+ AD- Dx Ly
        // AD- AL2 Lx Dy
        if (A_LAST_IS(L)) {
            if (AND_A_LAST_IS(2)) {
                PushBack("AL2");
                PushBack("AD+");
                PushBack("L_", x);
                PushBack("D_", y);
            } else {
                PushBack("AL+");
                PushBack("AD-");
                PushBack("D_", x);
                PushBack("L_", y);
            }
        } else {
            PushBack("AD-");
            PushBack("AL2");
            PushBack("L_", x);
            PushBack("D_", y);
        }
        return 2;
    case 'D':
        // Fx Dy [2]
        // AD- Lx Dy
        PushBack("AD-");
        PushBack("L_", x);
        PushBack("D_", y);
        return 2;
    case 'L':
        // Fx Ly [2]
        // AL+ Dx Ly
        PushBack("AL+");
        PushBack("D_", x);
        PushBack("L_", y);
        return 2;
    case 'R':
        // Fx Ry [2]
        // AD2 AL- Dx Ly
        // AD- AL+ Lx Dy
        // AL+ AD2 Dx Ly
        if (A_LAST_IS(D)) {
            if (AND_A_LAST_IS(2)) {
                PushBack("AD2");
                PushBack("AL-");
                PushBack("D_", x);
                PushBack("L_", y);
            } else {
                PushBack("AD-");
                PushBack("AL+");
                PushBack("L_", x);
                PushBack("D_", y);
            }
        } else {
            PushBack("AL+");
            PushBack("AD2");
            PushBack("D_", x);
            PushBack("L_", y);
        }
        return 2;
    case 'F':
        // Fx Fy [0]
        // error
        return 0;
    case 'B':
        // Fx By [2]
        // AL+ Dx AL2 Dy
        // AD- Lx AD2 Ly
        if (A_LAST_IS(L)) {
            PushBack("AL+");
            PushBack("D_", x);
            PushBack("AL2");
            PushBack("D_", y);
        } else {
            PushBack("AD-");
            PushBack("L_", x);
            PushBack("AD2");
            PushBack("L_", y);
        }
        return 2;
    default:
        return 0;
    }
}
size_t ASteps::DealB(size_t x, size_t y) {
    // Bx _ [1]
    // AL- Dx
    // AD+ Lx
    if (IS_O_LAST(x)) {
        if (A_LAST_IS(L)) {
            PushBack("AL-");
            PushBack("D_", x);
        } else {
            PushBack("AD+");
            PushBack("L_", x);
        }
        return 1;
    }
    switch (GetOStep(y)[0]) {
    case 'U':
        // Bx Uy [2]
        // AL2 AD- Lx Dy
        // AL- AD+ Dx Ly
        // AD+ AL2 Lx Dy
        if (A_LAST_IS(L)) {
            if (AND_A_LAST_IS(2)) {
                PushBack("AL2");
                PushBack("AD-");
                PushBack("L_", x);
                PushBack("D_", y);
            } else {
                PushBack("AL-");
                PushBack("AD+");
                PushBack("D_", x);
                PushBack("L_", y);
            }
        } else {
            PushBack("AD+");
            PushBack("AL2");
            PushBack("L_", x);
            PushBack("D_", y);
        }
        return 2;
    case 'D':
        // Bx Dy [2]
        // AD+ Lx Dy
        PushBack("AD+");
        PushBack("L_", x);
        PushBack("D_", y);
        return 2;
    case 'L':
        // Bx Ly [2]
        // AL- Dx Ly
        PushBack("AL-");
        PushBack("D_", x);
        PushBack("L_", y);
        return 2;
    case 'R':
        // Bx Ry [2]
        // AD2 AL+ Dx Ly
        // AD+ AL- Lx Dy
        // AL- AD2 Dx Ly
        if (A_LAST_IS(D)) {
            if (AND_A_LAST_IS(2)) {
                PushBack("AD2");
                PushBack("AL+");
                PushBack("D_", x);
                PushBack("L_", y);
            } else {
                PushBack("AD+");
                PushBack("AL-");
                PushBack("L_", x);
                PushBack("D_", y);
            }
        } else {
            PushBack("AL-");
            PushBack("AD2");
            PushBack("D_", x);
            PushBack("L_", y);
        }
        return 2;
    case 'F':
        // Bx Fy [2]
        // AL- Dx AL2 Dy
        // AD+ Lx AD2 Ly
        if (A_LAST_IS(L)) {
            PushBack("AL-");
            PushBack("D_", x);
            PushBack("AL2");
            PushBack("D_", y);
        } else {
            PushBack("AD+");
            PushBack("L_", x);
            PushBack("AD2");
            PushBack("L_", y);
        }
        return 2;
    case 'B':
        // Bx By [0]
        // error
        return 0;
    default:
        return 0;
    }
}
std::string ASteps::GetOStep(size_t idx) const {
    return m_oSteps.Get(idx, m_coord);
}
void ASteps::PushBack(const std::string& aStep, int oStepIdx) {
    m_aSteps.push_back(aStep);
    if (aStep[0] == 'A') { // 坐标系旋转
        m_coord.Rotate(aStep);
    } else if (oStepIdx != -1) { // 从oSteps中获取旋转角度
        m_aSteps[A_LAST][1] = GetOStep(oStepIdx)[1];
    }
}

MSteps::MSteps(const ASteps& aSteps) : m_aSteps{ aSteps } {
    Translate(aSteps);
}
MSteps::MSteps(const MSteps& mSteps) :
    m_aSteps{ mSteps.m_aSteps },
    m_mSteps{ mSteps.m_mSteps } {
}
void MSteps::Translate(const ASteps& aSteps) {
    m_mSteps.clear();
    m_aSteps = aSteps;
    for (size_t i = 0; i < m_aSteps.AStepsSize(); ++i) {
        if (DealSpecialCase(i) == false) {
            DealNormalCase(i);
        } else {
            ++i;
        }
        // m_mSteps.push_back("...");
    }
    // 清除"XC+ XC-"
    auto& beg = m_mSteps.begin();
    for (size_t i = 1; i < m_mSteps.size(); ++i) {
        if (m_mSteps[i][0] == m_mSteps[i - 1][0] &&
            m_mSteps[i][1] == 'C' && m_mSteps[i - 1][1] == 'C' &&
            m_mSteps[i][2] == '-' && m_mSteps[i - 1][2] == '+') {
            m_mSteps.erase(beg + i);
            m_mSteps.erase(beg + i - 1);
        }
    }
}
void MSteps::Clear() {
    m_aSteps.Clear();
    m_mSteps.clear();
}
const std::string& MSteps::operator[](size_t idx) {
    return m_mSteps[idx];
}
size_t MSteps::MStepsSize() const {
    return m_mSteps.size();
}
size_t MSteps::AStepsSize() const {
    return m_aSteps.AStepsSize();
}
void MSteps::DealNormalCase(size_t i) {
    std::string lastMStep;
    size_t mStepsSize = m_mSteps.size();
    std::string XCA = "XC+";

    XCA[0] = m_aSteps[i][0];
    if (mStepsSize > 0) {
        lastMStep = m_mSteps[mStepsSize - 1];
    }

    if (m_aSteps[i][0] != 'A') {
        switch (m_aSteps[i][1]) {
        case '2':
            // X2
            // XM2
            PushBack(i, "XM2");
            break;
        case '+':
            // X+
            // XM+ XC- XM- XC+
            // XC- XM- XC+ XM+
            if (lastMStep.size() > 0 &&
                lastMStep == XCA) {
                PushBack(i, "XC-");
                PushBack(i, "XM-");
                PushBack(i, "XC+");
                PushBack(i, "XM+");
            } else {
                PushBack(i, "XM+");
                PushBack(i, "XC-");
                PushBack(i, "XM-");
                PushBack(i, "XC+");
            }
            break;
        case '-':
            // X-
            // XM- XC- XM+ XC+
            // XC- XM+ XC+ XM-
            if (lastMStep.size() > 0 &&
                lastMStep == XCA) {
                PushBack(i, "XC-");
                PushBack(i, "XM+");
                PushBack(i, "XC+");
                PushBack(i, "XM-");
            } else {
                PushBack(i, "XM-");
                PushBack(i, "XC-");
                PushBack(i, "XM+");
                PushBack(i, "XC+");
            }
            break;
        }
    } else {
        switch (m_aSteps[i][2]) {
        case '2':
            // AX2
            // YC- XM2 YC+
            PushBack(i, "YC-");
            PushBack(i, "XM2");
            PushBack(i, "YC+");
            break;
        case '+':
            // AX+
            // YC- XM+ YC+ XC- XM- XC+
            PushBack(i, "YC-");
            PushBack(i, "XM+");
            PushBack(i, "YC+");
            PushBack(i, "XC-");
            PushBack(i, "XM-");
            PushBack(i, "XC+");
            break;
        case '-':
            // AX-
            // YC- XM- YC+ XC- XM+ XC+
            PushBack(i, "YC-");
            PushBack(i, "XM-");
            PushBack(i, "YC+");
            PushBack(i, "XC-");
            PushBack(i, "XM+");
            PushBack(i, "XC+");
            break;
        }
    }
}
bool MSteps::DealSpecialCase(size_t i) {
    if (IS_A_LAST(i)) {
        return false;
    }

    char X = m_aSteps[i][0];
    if (X == 'A') {
        X = m_aSteps[i][1];
    }
    char Y = (X == 'L' ? 'D' : 'L');

    auto IsX1 = [&](size_t idx) -> bool {
        return
            idx < m_aSteps.AStepsSize() &&
            m_aSteps[idx][0] == X &&
            m_aSteps[idx][1] != '2';
    };
    auto IsY1 = [&](size_t idx) -> bool {
        return
            idx < m_aSteps.AStepsSize() &&
            m_aSteps[idx][0] == Y &&
            m_aSteps[idx][1] != '2';
    };
    auto IsAX1 = [&](size_t idx) -> bool {
        return
            idx < m_aSteps.AStepsSize() &&
            m_aSteps[idx][0] == 'A' &&
            m_aSteps[idx][1] == X &&
            m_aSteps[idx][2] != '2';
    };
    auto IsAX2 = [&](size_t idx) -> bool {
        return
            idx < m_aSteps.AStepsSize() &&
            m_aSteps[idx][0] == 'A' &&
            m_aSteps[idx][1] == X &&
            m_aSteps[idx][2] == '2';
    };
    auto IsAY1 = [&](size_t idx) -> bool {
        return
            idx < m_aSteps.AStepsSize() &&
            m_aSteps[idx][0] == 'A' &&
            m_aSteps[idx][1] == Y &&
            m_aSteps[idx][2] != '2';
    };

    if (IsAX1(i) && IsX1(i + 1)) {
        // ... AX1 X1 ...
        return DealSp1(i);
    } else if (IsX1(i) && IsAX1(i + 1)) {
        // ... X1 AX1 ...
        return DealSp2(i);
    } else if (IsAX2(i) && IsY1(i + 1) && !IsAY1(i + 2)) {
        // ... AX2 Y1 !AY1 ...
        return DealSp3(i);
    } else if (!IsX1(i - 1) && IsAX1(i) && IsY1(i + 1) && !IsAY1(i + 2)) {
        // ... !X1 AX1 Y1 !AY1 ...
        return DealSp4(i);
    }
    return false;
}
bool MSteps::DealSp1(size_t i) {
    // ... AX1 X1 ...
    // AXi Xj
    // XMj YC- XMi YC+
    size_t j = i + 1;
    PushBack(i, "XMj", j);
    PushBack(i, "YC-");
    PushBack(i, "XMi", i);
    PushBack(i, "YC+");
    return true;
}
bool MSteps::DealSp2(size_t i) {
    // ... X1 AX1 ...
    // Xi Axj
    // XMi YC- XMj YC+
    size_t j = i + 1;
    PushBack(i, "XMi", i);
    PushBack(i, "YC-");
    PushBack(i, "XMj", j);
    PushBack(i, "YC+");
    return true;
}
bool MSteps::DealSp3(size_t i) {
    // ... AX2 Y1 !AY1 ...
    // AX2 Yi
    // YC- XM2 YMI YC+ YMi
    PushBack(i, "YC-");
    PushBack(i, "XM2");
    PushBack(i, "YMI", i + 1, true);
    PushBack(i, "YC+");
    PushBack(i, "YMi", i + 1);
    return true;
}
bool MSteps::DealSp4(size_t i) {
    // ... !X1 AX1 Y1 !AY1 ...
    // AXi Yj
    // XC- XMI XC+ YC- XMi YMJ YC+ YMj
    size_t j = i + 1;
    PushBack(i, "XC-");
    PushBack(i, "XMI", i, true);
    PushBack(i, "XC+");
    PushBack(i, "YC-");
    PushBack(i, "XMi", i);
    PushBack(i, "YMJ", j, true);
    PushBack(i, "YC+");
    PushBack(i, "YMj", j);
    return true;
}
void MSteps::PushBack(size_t i, const std::string& step, int aStepsIdx, bool reverse) {
    std::string tmp = step;

    char X = m_aSteps[i][0];
    if (X == 'A') {
        X = m_aSteps[i][1];
    }
    char Y = (X == 'D' ? 'L' : 'D');
    tmp[0] = (step[0] == 'X' ? X : Y);

    if (aStepsIdx != -1) {
        size_t size = m_aSteps[aStepsIdx].size();
        char angle = m_aSteps[aStepsIdx][size - 1];
        if (reverse == true) {
            angle = (angle == '+' ? '-' : '+');
        }
        tmp[2] = angle;
    }

    m_mSteps.push_back(tmp);
}
void MSteps::Output(const std::string& fileName) const {
    std::ofstream fout{ fileName };
    if (!fout) {
        g_IOBuf.PushBack("\n");
        g_IOBuf.PushBack(fileName);
        g_IOBuf.PushBack(u8"打开失败!\n");
        return;
    }
    g_IOBuf.PushBack("\n");
    for (auto& s : m_mSteps) {
        fout << s << " ";
    }
    fout.close();
}
