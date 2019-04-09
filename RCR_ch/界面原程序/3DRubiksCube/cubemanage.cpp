#define _USE_MATH_DEFINES

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include "cubemanage.h"

CubeManage::CubeManage()
{
    for (int i = 0; i < CUBE_LAYER; i++)
    {
        for (int j = 0; j < CUBE_LAYER; j++)
        {
            for (int k = 0; k < CUBE_LAYER; k++)
            {
                cubes[i][j][k] = new WcgCube(); // 创建总共 3x3x3 个方块
            }
        }
    }
}

CubeManage::~CubeManage()
{
    for (int i = 0; i < CUBE_LAYER; i++)
    {
        for (int j = 0; j < CUBE_LAYER; j++)
        {
            for (int k = 0; k < CUBE_LAYER; k++)
            {
                delete cubes[i][j][k];
            }
        }
    }
}

void CubeManage::turn(int rotateType)
{
    switch (rotateType)
    {
    case ZCW_2:
        turnByZCW(2);
        break;
    case XCW_2:
        turnByXCW(2);
        break;
    case ZCCW_0:
        turnByZCCW(0);
        break;
    case XCCW_0:
        turnByXCCW(0);
        break;
    case YCW_2:
        turnByYCW(2);
        break;
    case YCCW_0:
        turnByYCCW(0);
        break;
    case ZCCW_2:
        turnByZCCW(2);
        break;
    case XCCW_2:
        turnByXCCW(2);
        break;
    case ZCW_0:
        turnByZCW(0);
        break;
    case XCW_0:
        turnByXCW(0);
        break;
    case YCCW_2:
        turnByYCCW(2);
        break;
    case YCW_0:
        turnByYCW(0);
        break;
    default:
        break;
    }
}

// 渲染旋转时每一帧的动画
void CubeManage::draw(int rotateType, GLfloat rotate) // rotateType旋转类型 rotate旋转角度
{
    GLfloat cubeRadium = CUBE_SIZE;
    GLfloat cubeSpace = CUBE_SPACE;
    GLfloat x, y, z;
    int i, j, k;

    x = X_ORIENT - (CUBE_LAYER / 2) * (cubeRadium * 2 + cubeSpace);
    y = Y_ORIENT - (CUBE_LAYER / 2) * (cubeRadium * 2 + cubeSpace);
    z = Z_ORIENT - (CUBE_LAYER / 2) * (cubeRadium * 2 + cubeSpace);

    if (rotateType == 0) // 若不旋转
    {
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                for (k = 0; k < CUBE_LAYER; k++)
                {
                    cubes[i][j][k]->draw(
                        x + (cubeRadium * 2 + cubeSpace) * i,
                        y + (cubeRadium * 2 + cubeSpace) * j,
                        z + (cubeRadium * 2 + cubeSpace) * k);
                }
            }
        }
    }
    else if (rotateType == ZCW_2)
    {
        glPushMatrix();
        glRotatef(360 - 180 * rotate / PI, 0.0f, 0.0f, 1.0f);
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                cubes[i][j][CUBE_LAYER - 1]->draw(
                    x + (cubeRadium * 2 + cubeSpace) * i,
                    y + (cubeRadium * 2 + cubeSpace) * j,
                    z + (cubeRadium * 2 + cubeSpace) * (CUBE_LAYER - 1));
            }
        }
        glPopMatrix();
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                for (k = 0; k < CUBE_LAYER - 1; k++)
                {
                    cubes[i][j][k]->draw(
                        x + (cubeRadium * 2 + cubeSpace) * i,
                        y + (cubeRadium * 2 + cubeSpace) * j,
                        z + (cubeRadium * 2 + cubeSpace) * k);
                }
            }
        }
    }
    else if (rotateType == XCW_2)
    {
        glPushMatrix();
        glRotatef(360 - 180 * rotate / PI, 1.0f, 0.0f, 0.0f);
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                cubes[CUBE_LAYER - 1][i][j]->draw(
                    x + (cubeRadium * 2 + cubeSpace) * (CUBE_LAYER - 1),
                    y + (cubeRadium * 2 + cubeSpace) * i,
                    z + (cubeRadium * 2 + cubeSpace) * j);
            }
        }
        glPopMatrix();
        for (i = 0; i < CUBE_LAYER - 1; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                for (k = 0; k < CUBE_LAYER; k++)
                {
                    cubes[i][j][k]->draw(
                        x + (cubeRadium * 2 + cubeSpace) * i,
                        y + (cubeRadium * 2 + cubeSpace) * j,
                        z + (cubeRadium * 2 + cubeSpace) * k);
                }
            }
        }
    }
    else if (rotateType == ZCCW_0)
    {
        glPushMatrix();
        glRotatef(360 - 180 * rotate / PI, 0.0f, 0.0f, -1.0f);
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                cubes[i][j][0]->draw(
                    x + (cubeRadium * 2 + cubeSpace) * i,
                    y + (cubeRadium * 2 + cubeSpace) * j,
                    z);
            }
        }
        glPopMatrix();
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                for (k = 1; k < CUBE_LAYER; k++)
                {
                    cubes[i][j][k]->draw(
                        x + (cubeRadium * 2 + cubeSpace) * i,
                        y + (cubeRadium * 2 + cubeSpace) * j,
                        z + (cubeRadium * 2 + cubeSpace) * k);
                }
            }
        }
    }
    else if (rotateType == XCCW_0)
    {
        glPushMatrix();
        glRotatef(360 - 180 * rotate / PI, -1.0f, 0.0f, 0.0f);
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                cubes[0][i][j]->draw(
                    x,
                    y + (cubeRadium * 2 + cubeSpace) * i,
                    z + (cubeRadium * 2 + cubeSpace) * j);
            }
        }
        glPopMatrix();
        for (i = 1; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                for (k = 0; k < CUBE_LAYER; k++)
                {
                    cubes[i][j][k]->draw(
                        x + (cubeRadium * 2 + cubeSpace) * i,
                        y + (cubeRadium * 2 + cubeSpace) * j,
                        z + (cubeRadium * 2 + cubeSpace) * k);
                }
            }
        }
    }
    else if (rotateType == YCW_2)
    {
        glPushMatrix();
        glRotatef(360 - 180 * rotate / PI, 0.0f, 1.0f, 0.0f);
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                cubes[i][CUBE_LAYER - 1][j]->draw(
                    x + (cubeRadium * 2 + cubeSpace) * i,
                    y + (cubeRadium * 2 + cubeSpace) * (CUBE_LAYER - 1),
                    z + (cubeRadium * 2 + cubeSpace) * j);
            }
        }
        glPopMatrix();
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER - 1; j++)
            {
                for (k = 0; k < CUBE_LAYER; k++)
                {
                    cubes[i][j][k]->draw(
                        x + (cubeRadium * 2 + cubeSpace) * i,
                        y + (cubeRadium * 2 + cubeSpace) * j,
                        z + (cubeRadium * 2 + cubeSpace) * k);
                }
            }
        }
    }
    else if (rotateType == YCCW_0)
    {
        glPushMatrix();
        glRotatef(360 - 180 * rotate / PI, 0.0f, -1.0f, 0.0f);
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                (cubes[i][0][j])->draw(
                    x + (cubeRadium * 2 + cubeSpace) * i,
                    y,
                    z + (cubeRadium * 2 + cubeSpace) * j);
            }
        }
        glPopMatrix();
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 1; j < CUBE_LAYER; j++)
            {
                for (k = 0; k < CUBE_LAYER; k++)
                {
                    cubes[i][j][k]->draw(
                        x + (cubeRadium * 2 + cubeSpace) * i,
                        y + (cubeRadium * 2 + cubeSpace) * j,
                        z + (cubeRadium * 2 + cubeSpace) * k);
                }
            }
        }
    }
    else if (rotateType == ZCCW_2)
    {
        glPushMatrix();
        glRotatef(180 * rotate / PI, 0.0f, 0.0f, 1.0f);
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                cubes[i][j][CUBE_LAYER - 1]->draw(
                    x + (cubeRadium * 2 + cubeSpace) * i,
                    y + (cubeRadium * 2 + cubeSpace) * j,
                    z + (cubeRadium * 2 + cubeSpace) * (CUBE_LAYER - 1));
            }
        }
        glPopMatrix();
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                for (k = 0; k < CUBE_LAYER - 1; k++)
                {
                    cubes[i][j][k]->draw(
                        x + (cubeRadium * 2 + cubeSpace) * i,
                        y + (cubeRadium * 2 + cubeSpace) * j,
                        z + (cubeRadium * 2 + cubeSpace) * k);
                }
            }
        }
    }
    else if (rotateType == XCCW_2)
    {
        glPushMatrix();
        glRotatef(180 * rotate / PI, 1.0f, 0.0f, 0.0f);
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                cubes[CUBE_LAYER - 1][i][j]->draw(
                    x + (cubeRadium * 2 + cubeSpace) * (CUBE_LAYER - 1),
                    y + (cubeRadium * 2 + cubeSpace) * i,
                    z + (cubeRadium * 2 + cubeSpace) * j);
            }
        }
        glPopMatrix();
        for (i = 0; i < CUBE_LAYER - 1; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                for (k = 0; k < CUBE_LAYER; k++)
                {
                    cubes[i][j][k]->draw(
                        x + (cubeRadium * 2 + cubeSpace) * i,
                        y + (cubeRadium * 2 + cubeSpace) * j,
                        z + (cubeRadium * 2 + cubeSpace) * k);
                }
            }
        }
    }
    else if (rotateType == ZCW_0)
    {
        glPushMatrix();
        glRotatef(180 * rotate / PI, 0.0f, 0.0f, -1.0f);
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                (cubes[i][j][0])->draw(
                    x + (cubeRadium * 2 + cubeSpace) * i,
                    y + (cubeRadium * 2 + cubeSpace) * j,
                    z);
            }
        }
        glPopMatrix();
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                for (k = 1; k < CUBE_LAYER; k++)
                {
                    cubes[i][j][k]->draw(
                        x + (cubeRadium * 2 + cubeSpace) * i,
                        y + (cubeRadium * 2 + cubeSpace) * j,
                        z + (cubeRadium * 2 + cubeSpace) * k);
                }
            }
        }
    }
    else if (rotateType == XCW_0)
    {
        glPushMatrix();
        glRotatef(180 * rotate / PI, -1.0f, 0.0f, 0.0f);
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                cubes[0][i][j]->draw(
                    x,
                    y + (cubeRadium * 2 + cubeSpace) * i,
                    z + (cubeRadium * 2 + cubeSpace) * j);
            }
        }
        glPopMatrix();
        for (i = 1; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                for (k = 0; k < CUBE_LAYER; k++)
                {
                    cubes[i][j][k]->draw(
                        x + (cubeRadium * 2 + cubeSpace) * i,
                        y + (cubeRadium * 2 + cubeSpace) * j,
                        z + (cubeRadium * 2 + cubeSpace) * k);
                }
            }
        }
    }
    else if (rotateType == YCCW_2)
    {
        glPushMatrix();
        glRotatef(180 * rotate / PI, 0.0f, 1.0f, 0.0f);
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                cubes[i][CUBE_LAYER - 1][j]->draw(
                    x + (cubeRadium * 2 + cubeSpace) * i,
                    y + (cubeRadium * 2 + cubeSpace) * (CUBE_LAYER - 1),
                    z + (cubeRadium * 2 + cubeSpace) * j);
            }
        }
        glPopMatrix();
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER - 1; j++)
            {
                for (k = 0; k < CUBE_LAYER; k++)
                {
                    cubes[i][j][k]->draw(
                        x + (cubeRadium * 2 + cubeSpace) * i,
                        y + (cubeRadium * 2 + cubeSpace) * j,
                        z + (cubeRadium * 2 + cubeSpace) * k);
                }
            }
        }
    }
    else if (rotateType == YCW_0)
    {
        glPushMatrix();
        glRotatef(180 * rotate / PI, 0.0f, -1.0f, 0.0f);
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 0; j < CUBE_LAYER; j++)
            {
                cubes[i][0][j]->draw(
                    x + (cubeRadium * 2 + cubeSpace) * i,
                    y,
                    z + (cubeRadium * 2 + cubeSpace) * j);
            }
        }
        glPopMatrix();
        for (i = 0; i < CUBE_LAYER; i++)
        {
            for (j = 1; j < CUBE_LAYER; j++)
            {
                for (k = 0; k < CUBE_LAYER; k++)
                {
                    cubes[i][j][k]->draw(
                        x + (cubeRadium * 2 + cubeSpace) * i,
                        y + (cubeRadium * 2 + cubeSpace) * j,
                        z + (cubeRadium * 2 + cubeSpace) * k);
                }
            }
        }
    }
}

void CubeManage::output()
{
    std::cout << "U:";
    for (int i = 0; i < CUBE_LAYER; ++i)
    {
        for (int j = 0; j < CUBE_LAYER; ++j)
        {
            cubes[i][j][2]->output(Z_AXIS);
        }
    }
    std::cout << std::endl;

    std::cout << "D:";
    for (int i = CUBE_LAYER - 1; i >= 0; --i)
    {
        for (int j = 0; j < CUBE_LAYER; ++j)
        {
            cubes[i][j][0]->output(-Z_AXIS);
        }
    }
    std::cout << std::endl;

    std::cout << "L:";
    for (int k = CUBE_LAYER - 1; k >= 0; --k)
    {
        for (int i = 0; i < CUBE_LAYER; ++i)
        {
            cubes[i][0][k]->output(-Y_AXIS);
        }
    }
    std::cout << std::endl;

    std::cout << "R:";
    for (int k = CUBE_LAYER - 1; k >= 0; --k)
    {
        for (int i = CUBE_LAYER - 1; i >= 0; --i)
        {
            cubes[i][2][k]->output(Y_AXIS);
        }
    }
    std::cout << std::endl;

    std::cout << "F:";
    for (int k = CUBE_LAYER - 1; k >= 0; --k)
    {
        for (int j = 0; j < CUBE_LAYER; ++j)
        {
            cubes[2][j][k]->output(X_AXIS);
        }
    }
    std::cout << std::endl;

    std::cout << "B:";
    for (int k = CUBE_LAYER - 1; k >= 0; --k)
    {
        for (int j = CUBE_LAYER - 1; j >= 0; --j)
        {
            cubes[0][j][k]->output(-X_AXIS);
        }
    }
    std::cout << std::endl;
}

void CubeManage::input(const std::string filePath)
{
    std::fstream fileStream{ filePath };
    std::string colorStr[6]{};
    for (int i = 0; i < 6 && fileStream.eof() == false; ++i)
    {
        char line[12];
        fileStream.getline(line, 12);
        colorStr[i] = line;
    }
    fileStream.close();

    char cubesStr[3][3][3][6]{};
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                for (int s = 0; s < 6; ++s)
                {
                    cubesStr[i][j][k][s] = '?';
                }
            }
        }
    }

    int index = 2;
    for (int i = 0; i < CUBE_LAYER; ++i)
    {
        for (int j = 0; j < CUBE_LAYER; ++j)
        {
            cubesStr[i][j][2][FACE_U] = colorStr[0][index++];
        }
    }
    index = 2;
    for (int i = CUBE_LAYER - 1; i >= 0; --i)
    {
        for (int j = 0; j < CUBE_LAYER; ++j)
        {
            cubesStr[i][j][0][FACE_D] = colorStr[1][index++];
        }
    }
    index = 2;
    for (int k = CUBE_LAYER - 1; k >= 0; --k)
    {
        for (int j = 0; j < CUBE_LAYER; ++j)
        {
            cubesStr[2][j][k][FACE_F] = colorStr[2][index++];
        }
    }
    index = 2;
    for (int k = CUBE_LAYER - 1; k >= 0; --k)
    {
        for (int j = CUBE_LAYER - 1; j >= 0; --j)
        {
            cubesStr[0][j][k][FACE_B] = colorStr[3][index++];
        }
    }
    index = 2;
    for (int k = CUBE_LAYER - 1; k >= 0; --k)
    {
        for (int i = 0; i < CUBE_LAYER; ++i)
        {
            cubesStr[i][0][k][FACE_L] = colorStr[4][index++];
        }
    }
    index = 2;
    for (int k = CUBE_LAYER - 1; k >= 0; --k)
    {
        for (int i = CUBE_LAYER - 1; i >= 0; --i)
        {
            cubesStr[i][2][k][FACE_R] = colorStr[5][index++];
        }
    }

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                std::string test{};
                for (int s = 0; s < 6; ++s)
                {
                    test.push_back(cubesStr[i][j][k][s]);
                }
                cubes[i][j][k]->input(test);
            }
        }
    }
}

void CubeManage::goStep(int *leftLeg, int *rightLeg, int *goDirection, int step, int leftEdge, int rightEdge)
{
    for (int i = 0; i < step; i++)
    {
        switch (*goDirection)
        {
        case 0:
            *leftLeg = *leftLeg - 1;
            if (*leftLeg < leftEdge)
            {
                *leftLeg = *leftLeg + 1;
                *goDirection = 3;
                *rightLeg = *rightLeg + 1;
            }
            break;
        case 1:
            *rightLeg = *rightLeg - 1;
            if (*rightLeg < leftEdge)
            {
                *rightLeg = *rightLeg + 1;
                *goDirection = 0;
                *leftLeg = *leftLeg - 1;
            }
            break;
        case 2:
            *leftLeg = *leftLeg + 1;
            if (*leftLeg >= rightEdge)
            {
                *leftLeg = *leftLeg - 1;
                *goDirection = 1;
                *rightLeg = *rightLeg - 1;
            }
            break;
        case 3:
            *rightLeg = *rightLeg + 1;
            if (*rightLeg >= rightEdge)
            {
                *rightLeg = *rightLeg - 1;
                *goDirection = 2;
                *leftLeg = *leftLeg + 1;
            }
            break;
        }
    }
}

void CubeManage::turnByXCW(int x)
{
    int step = CUBE_LAYER - 1;
    int leftEdge = 0;
    int rightEdge = CUBE_LAYER;
    int goDirection0 = 3;
    int goDirection1 = 3;
    int y0 = 0;
    int z0 = 0;
    int y1 = 0;
    int z1 = 0;
    WcgCube *tempcubes[CUBE_LAYER][CUBE_LAYER];

    tempcubes[CUBE_LAYER / 2][CUBE_LAYER / 2] = cubes[x][CUBE_LAYER / 2][CUBE_LAYER / 2];
    cubes[x][CUBE_LAYER / 2][CUBE_LAYER / 2]->turnByXCW(x);
    for (int i = 0; i < CUBE_LAYER / 2; i++)
    {
        step = CUBE_LAYER - i * 2 - 1;
        goDirection0 = 3;
        goDirection1 = 3;
        leftEdge = i;
        rightEdge = CUBE_LAYER - i;
        y0 = leftEdge;
        z0 = leftEdge;
        y1 = leftEdge;
        z1 = leftEdge;
        goStep(&y1, &z1, &goDirection1, step, leftEdge, rightEdge);
        for (int j = 0; j < step * 4; j++)
        {
            tempcubes[y1][z1] = cubes[x][y0][z0];
            cubes[x][y0][z0]->turnByXCW(x);
            goStep(&y0, &z0, &goDirection0, 1, leftEdge, rightEdge);
            goStep(&y1, &z1, &goDirection1, 1, leftEdge, rightEdge);
        }
        for (int m = 0; m < CUBE_LAYER; m++)
        {
            for (int n = 0; n < CUBE_LAYER; n++)
            {
                cubes[x][m][n] = tempcubes[m][n];
            }
        }
    }
}

void CubeManage::turnByXCCW(int x)
{
    turnByXCW(x);
    turnByXCW(x);
    turnByXCW(x);
}

void CubeManage::turnByYCW(int y)
{
    int step = CUBE_LAYER - 1;
    int leftEdge = 0;
    int rightEdge = CUBE_LAYER;
    int goDirection0 = 3;
    int goDirection1 = 3;
    int x0 = 0;
    int z0 = 0;
    int x1 = 0;
    int z1 = 0;
    WcgCube *tempcubes[CUBE_LAYER][CUBE_LAYER];

    tempcubes[CUBE_LAYER / 2][CUBE_LAYER / 2] = cubes[CUBE_LAYER / 2][y][CUBE_LAYER / 2];
    cubes[CUBE_LAYER / 2][y][CUBE_LAYER / 2]->turnByYCW(y);
    for (int i = 0; i < CUBE_LAYER / 2; i++)
    {
        step = CUBE_LAYER - i * 2 - 1;
        goDirection0 = 3;
        goDirection1 = 3;
        leftEdge = i;
        rightEdge = CUBE_LAYER - i;
        x0 = leftEdge;
        z0 = leftEdge;
        x1 = leftEdge;
        z1 = leftEdge;
        goStep(&z1, &x1, &goDirection1, step, leftEdge, rightEdge);
        for (int j = 0; j < step * 4; j++)
        {
            tempcubes[x1][z1] = cubes[x0][y][z0];
            cubes[x0][y][z0]->turnByYCW(y);
            goStep(&z0, &x0, &goDirection0, 1, leftEdge, rightEdge);
            goStep(&z1, &x1, &goDirection1, 1, leftEdge, rightEdge);
        }
        for (int m = 0; m < CUBE_LAYER; m++)
        {
            for (int n = 0; n < CUBE_LAYER; n++)
            {
                cubes[m][y][n] = tempcubes[m][n];
            }
        }
    }
}

void CubeManage::turnByYCCW(int y)
{
    turnByYCW(y);
    turnByYCW(y);
    turnByYCW(y);
}

void CubeManage::turnByZCW(int z)
{
    int step = CUBE_LAYER - 1;
    int leftEdge = 0;
    int rightEdge = CUBE_LAYER;
    int goDirection0 = 3;
    int goDirection1 = 3;
    int x0 = 0;
    int y0 = 0;
    int x1 = 0;
    int y1 = 0;
    WcgCube *tempcubes[CUBE_LAYER][CUBE_LAYER];

    tempcubes[CUBE_LAYER / 2][CUBE_LAYER / 2] = cubes[CUBE_LAYER / 2][CUBE_LAYER / 2][z];
    cubes[CUBE_LAYER / 2][CUBE_LAYER / 2][z]->turnByZCW(z);
    for (int i = 0; i < CUBE_LAYER / 2; i++)
    {
        step = CUBE_LAYER - i * 2 - 1;
        goDirection0 = 3;
        goDirection1 = 3;
        leftEdge = i;
        rightEdge = CUBE_LAYER - i;
        x0 = leftEdge;
        y0 = leftEdge;
        x1 = leftEdge;
        y1 = leftEdge;
        goStep(&x1, &y1, &goDirection1, step, leftEdge, rightEdge);
        for (int j = 0; j < step * 4; j++)
        {
            tempcubes[x1][y1] = cubes[x0][y0][z];
            cubes[x0][y0][z]->turnByZCW(z);
            goStep(&x0, &y0, &goDirection0, 1, leftEdge, rightEdge);
            goStep(&x1, &y1, &goDirection1, 1, leftEdge, rightEdge);
        }
        for (int m = 0; m < CUBE_LAYER; m++)
        {
            for (int n = 0; n < CUBE_LAYER; n++)
            {
                cubes[m][n][z] = tempcubes[m][n];
            }
        }
    }
}

void CubeManage::turnByZCCW(int z)
{
    turnByZCW(z);
    turnByZCW(z);
    turnByZCW(z);
}
