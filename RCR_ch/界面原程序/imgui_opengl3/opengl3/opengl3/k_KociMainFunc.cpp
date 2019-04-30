#include "k_KociMainFunc.h"
#include "Serial.h"
#include <random>
#include "k_Step.h"

const std::string fName{ "INPUT.txt" };
unsigned int status;

int GetFileWriteTime_TimeBombVersion(const LPCTSTR& lpszFilePath) {
    WIN32_FIND_DATA FindFileData = { 0 };

    HANDLE hFile = ::FindFirstFile(lpszFilePath, &FindFileData);

    if (INVALID_HANDLE_VALUE == hFile) {
        std::ofstream bug{ lpszFilePath };
        bug << "0";
        bug.close();
        hFile = ::FindFirstFile(lpszFilePath, &FindFileData);
        //return -1;
    }
    SYSTEMTIME WriteTime = { 0 }; // ����޸�ʱ��
    if (!::FileTimeToSystemTime(&FindFileData.ftLastWriteTime, &WriteTime)) {
        return -1;
    }
    int time = WriteTime.wDay + WriteTime.wMonth * 31 + (WriteTime.wYear - 2000) * 12 * 31;
    return time;
}

bool ColorInputCorrecter(std::vector<std::string>& faceletStrings) {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 9; ++j) {
            switch (faceletStrings[i][2 + j]) // ԭ�������������Ҫ�������е�W��Y�Ŷ�
            {
            case 'Y':
                faceletStrings[i][2 + j] = 'W';
                break;
            case 'W':
                faceletStrings[i][2 + j] = 'Y';
                break;
            default:
                break;
            }
        }
    }
    return READ_SUCCESS;
}

bool stringToCube(std::vector<std::string>& faceletStrings, FaceletCube &faceletCube, RubiksCube &cube) {
    CubeParser cubeParser; // ��ʱ����
    if ((status = cubeParser.parseFacelets(faceletStrings, faceletCube))
        != CubeParser::VALID) {
        g_IOBuf.PushBack(cubeParser.ErrorText(status));
        return READ_FAIL;
    }
    // Validate the FaceletCube
    if ((status = faceletCube.Validate(cube)) != FaceletCube::VALID) {
        g_IOBuf.PushBack(faceletCube.ErrorText(status));
        return READ_FAIL;
    }
    // Cube is in a valid configuration at this point
    return READ_SUCCESS;
}

bool SolveCube(std::vector<std::string>& faceletStrings) {
    ColorInputCorrecter(faceletStrings); // ԭ������ɫ˳�����ͨ���˺�������
    // Parse the input and initialize FaceletCube
    FaceletCube faceletCube;
    RubiksCube cube;
    if (stringToCube(faceletStrings, faceletCube, cube) == READ_FAIL) {
        return READ_FAIL;
    }
    // Initialize tables and solve
    Solver solver; // �ڶ���ʶ���ʱ����ܻ����ĵط�...��һ�¸ĳ�static?
    solver.InitializeTables(); // �ڶ���ʶ���ʱ����ܻ����ĵط�...
    solver.Solve(cube, 500); // ��ʱ�˳�
    return READ_SUCCESS;
}

bool TranslateToLD()
{
    g_IOBuf.PushBack("\nOSteps:\n");
    OSteps oSteps{};
    std::fstream outputTxt{ "OUTPUT.txt" };
    if (!outputTxt.is_open()) {
        g_IOBuf.PushBack("File \"OUTPUT.txt\" open failed!\n");
        return false;
    }
    while (!outputTxt.eof()) {
        std::string tmpStep{};
        outputTxt >> tmpStep;
        if (tmpStep[0] == '(') {
            break;
        }
        if (tmpStep[0] != '.') {
            if (tmpStep.size() == 1) {
                tmpStep += "+";
            } else if (tmpStep.size() == 2 && tmpStep[1] == '\'') {
                tmpStep[1] = '-';
            }
            oSteps.PushBack(tmpStep);
            g_IOBuf.PushBack(tmpStep + " ");
        }
    }
    g_IOBuf.PushBack("[");
    g_IOBuf.PushBack(oSteps.Size());
    g_IOBuf.PushBack("]\n");

    g_IOBuf.PushBack("\nASteps:\n");
    ASteps aSteps{ oSteps };
    for (size_t i = 0; i < aSteps.AStepsSize(); ++i) {
        g_IOBuf.PushBack(aSteps[i]);
        g_IOBuf.PushBack(" ");
    }
    g_IOBuf.PushBack("[");
    g_IOBuf.PushBack(aSteps.AStepsSize());
    g_IOBuf.PushBack("]\n");

    g_IOBuf.PushBack("\nMSteps:\n");
    MSteps mSteps{ aSteps };
    for (size_t i = 0; i < mSteps.MStepsSize(); ++i) {
        g_IOBuf.PushBack(mSteps[i]);
        g_IOBuf.PushBack(" ");
    }
    g_IOBuf.PushBack("[");
    g_IOBuf.PushBack(mSteps.MStepsSize());
    g_IOBuf.PushBack("]\n");

    mSteps.Output("LDCM_output.txt");

    return READ_SUCCESS;
}
