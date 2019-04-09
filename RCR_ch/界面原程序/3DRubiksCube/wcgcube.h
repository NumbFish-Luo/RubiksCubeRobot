#pragma once

#include <gl/gl.h>
#include <gl/glu.h>

class WcgCube // World Cyber Games(笑) 应该是单独一块方块的设置
{
public:
    WcgCube();
    ~WcgCube();

    void turnByXCW  (int x);
    void turnByXCCW (int x);
    void turnByYCW  (int y);
    void turnByYCCW (int y);
    void turnByZCW  (int z);
    void turnByZCCW (int z);
    void output     (int face);
    void input      (std::string WcgCubeColor);
    void draw       (GLfloat x0, GLfloat y0, GLfloat z0);

private:
    int direct[12];				// 小方块的六个面 + 六个黑面
    GLfloat sideColor[12][3];	// 小方块六个面的颜色 + 六个黑面

    void turnByX(int x, int sign);
    void turnByY(int y, int sign);
    void turnByZ(int z, int sign);
    void renewBlackFace();
};
