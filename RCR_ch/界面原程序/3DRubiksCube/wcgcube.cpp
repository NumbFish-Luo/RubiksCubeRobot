#include <windows.h>
#include <math.h>
#include <iostream>
#include "wcgcube.h"  
#include "cube3Dconfig.h"

void WcgCube::renewBlackFace()
{
    const int face[6]{ Z_AXIS, X_AXIS, -Z_AXIS, -X_AXIS, Y_AXIS, -Y_AXIS };
    int index = 0;
    for (int i = 0; i < 6; ++i)
    {
        bool flag = false;
        for (int j = 0; j < 6; ++j)
        {
            if (direct[j] == face[i])
            {
                flag = true;
                break;
            }
        }

        if (flag == false)
        {
            direct[COLOR_BLACK + index] = face[i];
            ++index;
        }
    }
}

WcgCube::WcgCube()
{
    direct[FACE_U] =  Z_AXIS;
    direct[FACE_F] =  X_AXIS;
    direct[FACE_D] = -Z_AXIS;
    direct[FACE_B] = -X_AXIS;
    direct[FACE_R] =  Y_AXIS;
    direct[FACE_L] = -Y_AXIS;

    for (int i = 0; i < 6; ++i)
    {
        direct[FACE_VOID + i] = 0;
    }

    // RGB
    // White
    sideColor[FACE_U][0] = 255 / 255.0f;
    sideColor[FACE_U][1] = 255 / 255.0f;
    sideColor[FACE_U][2] = 255 / 255.0f;

    // Red
    sideColor[FACE_F][0] = 255 / 255.0f;
    sideColor[FACE_F][1] =   0 / 255.0f;
    sideColor[FACE_F][2] =   0 / 255.0f;

    // Yellow
    sideColor[FACE_D][0] = 255 / 255.0f;
    sideColor[FACE_D][1] = 255 / 255.0f;
    sideColor[FACE_D][2] =   0 / 255.0f;

    // Orange
    sideColor[FACE_B][0] = 255 / 255.0f;
    sideColor[FACE_B][1] = 125 / 255.0f;
    sideColor[FACE_B][2] =  38 / 255.0f;

    // Blue
    sideColor[FACE_R][0] =   0 / 255.0f;
    sideColor[FACE_R][1] =   0 / 255.0f;
    sideColor[FACE_R][2] = 255 / 255.0f;

    // Green
    sideColor[FACE_L][0] =   0 / 255.0f;
    sideColor[FACE_L][1] = 153 / 255.0f;
    sideColor[FACE_L][2] =   0 / 255.0f;

    // Black
    for (int i = 0; i < 6; ++i)
    {
        sideColor[FACE_VOID + i][0] = BLACK_R;
        sideColor[FACE_VOID + i][1] = BLACK_B;
        sideColor[FACE_VOID + i][2] = BLACK_G;
    }
}

WcgCube::~WcgCube()
{

}

void WcgCube::draw(GLfloat orientX, GLfloat orientY, GLfloat orientZ) // orient 坐标，xyz的坐标
{
    GLfloat cubeRadium = CUBE_SIZE; // typedef float GLfloat 方块大小
    GLfloat cubeSpace = CUBE_SPACE; // 方块间隙

    for (int i = 0; i < 12; i++)    // 对于六个面 + 六个黑面
    {
        glColor3f(sideColor[i][0], sideColor[i][1], sideColor[i][2]);
        if (direct[i] == Z_AXIS)
        {
            glBegin(GL_POLYGON); // POLYGON：多边形 开始绘制多边形
            glVertex3f(orientX + cubeRadium, orientY + cubeRadium, orientZ + cubeRadium); // 这里是根据XYZ位置，绘制矩形
            glVertex3f(orientX + cubeRadium, orientY - cubeRadium, orientZ + cubeRadium);
            glVertex3f(orientX - cubeRadium, orientY - cubeRadium, orientZ + cubeRadium);
            glVertex3f(orientX - cubeRadium, orientY + cubeRadium, orientZ + cubeRadium);
            glEnd(); // 结束多边形绘制
        }
        else if (direct[i] == -Z_AXIS)
        {
            glBegin(GL_POLYGON);
            glVertex3f(orientX + cubeRadium, orientY + cubeRadium, orientZ - cubeRadium);
            glVertex3f(orientX + cubeRadium, orientY - cubeRadium, orientZ - cubeRadium);
            glVertex3f(orientX - cubeRadium, orientY - cubeRadium, orientZ - cubeRadium);
            glVertex3f(orientX - cubeRadium, orientY + cubeRadium, orientZ - cubeRadium);
            glEnd();
        }
        else if (direct[i] == Y_AXIS)
        {
            glBegin(GL_POLYGON);
            glVertex3f(orientX + cubeRadium, orientY + cubeRadium, orientZ - cubeRadium);
            glVertex3f(orientX + cubeRadium, orientY + cubeRadium, orientZ + cubeRadium);
            glVertex3f(orientX - cubeRadium, orientY + cubeRadium, orientZ + cubeRadium);
            glVertex3f(orientX - cubeRadium, orientY + cubeRadium, orientZ - cubeRadium);
            glEnd();
        }
        else if (direct[i] == -Y_AXIS)
        {
            glBegin(GL_POLYGON);
            glVertex3f(orientX + cubeRadium, orientY - cubeRadium, orientZ - cubeRadium);
            glVertex3f(orientX + cubeRadium, orientY - cubeRadium, orientZ + cubeRadium);
            glVertex3f(orientX - cubeRadium, orientY - cubeRadium, orientZ + cubeRadium);
            glVertex3f(orientX - cubeRadium, orientY - cubeRadium, orientZ - cubeRadium);
            glEnd();
        }
        else if (direct[i] == X_AXIS)
        {
            glBegin(GL_POLYGON);
            glVertex3f(orientX + cubeRadium, orientY + cubeRadium, orientZ + cubeRadium);
            glVertex3f(orientX + cubeRadium, orientY + cubeRadium, orientZ - cubeRadium);
            glVertex3f(orientX + cubeRadium, orientY - cubeRadium, orientZ - cubeRadium);
            glVertex3f(orientX + cubeRadium, orientY - cubeRadium, orientZ + cubeRadium);
            glEnd();
        }
        else if (direct[i] == -X_AXIS)
        {
            glBegin(GL_POLYGON);
            glVertex3f(orientX - cubeRadium, orientY + cubeRadium, orientZ + cubeRadium);
            glVertex3f(orientX - cubeRadium, orientY + cubeRadium, orientZ - cubeRadium);
            glVertex3f(orientX - cubeRadium, orientY - cubeRadium, orientZ - cubeRadium);
            glVertex3f(orientX - cubeRadium, orientY - cubeRadium, orientZ + cubeRadium);
            glEnd();
        }
    }
}

void WcgCube::output(int face)
{
    for (int i = 0; i < 6; ++i)
    {
        if (direct[i] == face)
        {
            switch (i)
            {
            case COLOR_WHITE:
                std::cout << "W";
                break;
            case COLOR_RED:
                std::cout << "R";
                break;
            case COLOR_YELLOW:
                std::cout << "Y";
                break;
            case COLOR_ORANGE:
                std::cout << "O";
                break;
            case COLOR_BLUE:
                std::cout << "B";
                break;
            case COLOR_GREEN:
                std::cout << "G";
                break;
            default:
                break;
            }
            break;
        }
    }
}

// 这边传入的应该是单个方块的上下左右前后颜色设置，顺序是UFDBRL，举例：GWAAOA，这里A是黑色
void WcgCube::input(std::string WcgCubeColor)
{
    const int face[6]{ Z_AXIS, X_AXIS, -Z_AXIS, -X_AXIS, Y_AXIS, -Y_AXIS };

    for (int i = 0; i < 12; ++i) // 先清空 direct
    {
        direct[i] = 0;
    }

    for (int i = 0; i < 6; ++i)
    {
        switch (WcgCubeColor[i])
        {
        case 'W':
            direct[COLOR_WHITE] = face[i];
            break;
        case 'R':
            direct[COLOR_RED] = face[i];
            break;
        case 'Y':
            direct[COLOR_YELLOW] = face[i];
            break;
        case 'O':
            direct[COLOR_ORANGE] = face[i];
            break;
        case 'B':
            direct[COLOR_BLUE] = face[i];
            break;
        case 'G':
            direct[COLOR_GREEN] = face[i];
            break;
        default:
            break;
        }
    }

    renewBlackFace();
}

void WcgCube::turnByXCW(int x)
{
    turnByX(x, -1);
}

void WcgCube::turnByXCCW(int x)
{
    turnByX(x, 1);
}

void WcgCube::turnByX(int x, int sign) // 以X轴为旋转轴旋转
{
    for (int i = 0; i < 6; i++)
    {
        switch (direct[i])
        {
        case Z_AXIS:
            direct[i] = (-1) * sign * Y_AXIS;
            break;
        case -Z_AXIS:
            direct[i] = sign * Y_AXIS;
            break;
        case Y_AXIS:
            direct[i] = sign * Z_AXIS;
            break;
        case -Y_AXIS:
            direct[i] = (-1) * sign * Z_AXIS;
            break;
        }
    }
    renewBlackFace();
}

void WcgCube::turnByYCW(int y)
{
    turnByY(y, -1);
}

void WcgCube::turnByYCCW(int y)
{
    turnByY(y, 1);
}

void WcgCube::turnByY(int y, int sign) // 以Y轴为旋转轴旋转
{
    for (int i = 0; i < 6; i++)
    {
        switch (direct[i])
        {
        case Z_AXIS:
            direct[i] = sign * X_AXIS;
            break;
        case -Z_AXIS:
            direct[i] = (-1) * sign * X_AXIS;
            break;
        case X_AXIS:
            direct[i] = (-1) * sign * Z_AXIS;
            break;
        case -X_AXIS:
            direct[i] = sign * Z_AXIS;
            break;
        }
    }
    renewBlackFace();
}

void WcgCube::turnByZCW(int z)
{
    turnByZ(z, -1);
}

void WcgCube::turnByZCCW(int z)
{
    turnByZ(z, 1);
}

void WcgCube::turnByZ(int z, int sign) // 以Z轴为旋转轴旋转
{
    for (int i = 0; i < 6; i++)
    {
        switch (direct[i])
        {
        case Y_AXIS:
            direct[i] = (-1) * sign * X_AXIS;
            break;
        case -Y_AXIS:
            direct[i] = sign * X_AXIS;
            break;
        case X_AXIS:
            direct[i] = sign * Y_AXIS;
            break;
        case -X_AXIS:
            direct[i] = (-1) * sign * Y_AXIS;
            break;
        }
    }
    renewBlackFace();
}
