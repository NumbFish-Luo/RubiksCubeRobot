#pragma once

#include <gl/gl.h>
#include <gl/glu.h>

#include "wcgcube.h"
#include "cube3Dconfig.h"

class CubeManage // 方块管理者
{
public:
    CubeManage();
    ~CubeManage();

    void turn       (int rotateType);
    void turnByXCW  (int x);
    void turnByXCCW (int x);
    void turnByYCW  (int y);
    void turnByYCCW (int y);
    void turnByZCW  (int z);
    void turnByZCCW (int z);
    void output     ();
    void input      (const std::string filePath);
    void draw       (int rotateType, GLfloat rotate);

private:
    WcgCube* cubes[CUBE_LAYER][CUBE_LAYER][CUBE_LAYER];
    void     goStep(int *leftLeg, int *rightLeg, int *goDirection, int step, int leftEdge, int rightEdge);
};
