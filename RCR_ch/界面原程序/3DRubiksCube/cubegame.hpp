#pragma once

#include <windows.h>
#include <gl/gl.h>
//#include <gl/glu.h>
#include <iostream>
#include <fstream>

#include "CTransparent.hpp"
#include "cubemanage.h"
#include "turn_steps.h"

using       std::cout;
using       std::endl;
using       std::string;

steps       solutionSteps       {};
CubeManage  myCubeManage        {};
HPALETTE    hPalette            = NULL; // handel of palette 调色板句柄

GLfloat     xRotate             = X_ROTATE;
GLfloat     yRotate             = Y_ROTATE;
GLfloat     zRotate             = Z_ROTATE;
GLfloat     rotateAngle         = 0.0f;
int         rotateType          = 0;
bool        rotateOK            = false;

size_t      step                = 0;
int         lostStepMessages    = 0;

void        ChangeSize          (GLsizei width, GLsizei height);
void        SetDCPixelFormat    (HDC hDC);
void        RotateAngleFunction ();
void        RenderScene         ();
void        DealKey             (HWND hWnd, HDC hDC, HGLRC hRC, int wParam);
void        ExitGame            (HWND hWnd, HDC hDC, HGLRC hRC);
LRESULT
CALLBACK    WndProc             (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void ChangeSize(GLsizei width, GLsizei height)
{
    const GLfloat nRange = 550.0f;

    // Prevent a divide by zero  
    if (height == 0)
    {
        height = 1;
    }

    // Set Viewport to window dimensions  
    glViewport(0, 0, width, height);


    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Establish clipping volume (left, right, bottom, top, near, far)  
    if (width <= height)
    {
        glOrtho(-nRange,
                 nRange,
                -nRange * height / width,
                 nRange * height / width,
                -nRange,
                 nRange);
    }
    else
    {
        glOrtho(-nRange * width / height,
                 nRange * width / height,
                -nRange,
                 nRange,
                -nRange,
                 nRange);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Called by timer routine to effect movement of the rectangle.  
void RotateAngleFunction()
{
    if (rotateAngle >= PI / 2)
    {
        myCubeManage.turn(rotateType);
        rotateType = 0;
        rotateOK = false;
        rotateAngle = 0.0f;
        return;
    }
    rotateAngle += ROTATE_ANGEL_PER_FRAME;
}

// Called by AUX library to draw scene
void RenderScene() // scene 场景
{
    // Clear the window with current clearing color  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glRotatef(xRotate, 1.0f, 0.0f, 0.0f); // 旋转角度
    glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
    glRotatef(zRotate, 0.0f, 0.0f, 1.0f);

    myCubeManage.draw(rotateType, rotateAngle);

    glPopMatrix();
}

// If necessary, creates a 3-3-2 palette for the device context listed.
HPALETTE GetOpenGLPalette(HDC hDC)
{
    HPALETTE                hRetPal = NULL; // Handle to palette to be created
    PIXELFORMATDESCRIPTOR   pfd;            // Pixel Format Descriptor 像素格式结构
    LOGPALETTE             *pPal;           // Pointer to memory for logical palette
    int                     nPixelFormat;   // Pixel format index
    int                     nColors;        // Number of entries in palette
    BYTE RedRange, GreenRange, BlueRange;
    // Range for each color entry (7,7,and 3)

    // Get the pixel format index and retrieve the pixel format description
    nPixelFormat = GetPixelFormat(hDC);
    DescribePixelFormat(hDC, nPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    // Does this pixel format require a palette?  If not, do not create a
    // palette and just return NULL
    if (!(pfd.dwFlags & PFD_NEED_PALETTE))
    {
        return NULL;
    }

    // Number of entries in palette.  8 bits yeilds 256 entries
    nColors = 1 << pfd.cColorBits;

    // Allocate space for a logical palette structure plus all the palette entries
    pPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) + nColors * sizeof(PALETTEENTRY));

    // Fill in palette header
    pPal->palVersion    = 0x300;   // Windows 3.0
    pPal->palNumEntries = nColors; // table size

    // Build mask of all 1's.  This creates a number represented by having
    // the low order x bits set, where x = pfd.cRedBits, pfd.cGreenBits, and
    // pfd.cBlueBits.
    RedRange    = (1 << pfd.cRedBits) - 1;
    GreenRange  = (1 << pfd.cGreenBits) - 1;
    BlueRange   = (1 << pfd.cBlueBits) - 1;

    // Loop through all the palette entries
    for (int i = 0; i < nColors; i++)
    {
        // Fill in the 8-bit equivalents for each component
        pPal->palPalEntry[i].peRed = (i >> pfd.cRedShift) & RedRange;
        pPal->palPalEntry[i].peRed = (unsigned char)(
            (double)pPal->palPalEntry[i].peRed * 255.0 / RedRange);

        pPal->palPalEntry[i].peGreen = (i >> pfd.cGreenShift) & GreenRange;
        pPal->palPalEntry[i].peGreen = (unsigned char)(
            (double)pPal->palPalEntry[i].peGreen * 255.0 / GreenRange);

        pPal->palPalEntry[i].peBlue = (i >> pfd.cBlueShift) & BlueRange;
        pPal->palPalEntry[i].peBlue = (unsigned char)(
            (double)pPal->palPalEntry[i].peBlue * 255.0 / BlueRange);

        pPal->palPalEntry[i].peFlags = (unsigned char)NULL;
    }

    // Create the palette
    hRetPal = CreatePalette(pPal);

    // Go ahead and select and realize the palette for this device context
    SelectPalette(hDC, hRetPal, FALSE);
    RealizePalette(hDC);

    // Free the memory used for the logical palette structure
    free(pPal);

    // Return the handle to the new palette
    return hRetPal;
}

// Select the pixel format for a given device context
void SetDCPixelFormat(HDC hDC)
{
    int nPixelFormat;

    static PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
        1,                              // Version of this structure
        PFD_DRAW_TO_WINDOW |            // Draw to Window (not to bitmap)
        PFD_SUPPORT_OPENGL |            // Support OpenGL calls in window
        PFD_DOUBLEBUFFER,               // Double buffered mode
        PFD_TYPE_RGBA,                  // RGBA Color mode
        32,                             // Want 32 bit color
        0, 0, 0, 0, 0, 0,               // Not used to select mode
        0, 0,                           // Not used to select mode
        0, 0, 0, 0, 0,                  // Not used to select mode
        16,                             // Size of depth buffer
        0,                              // Not used to select mode
        0,                              // Not used to select mode
        0,                              // Not used to select mode
        0,                              // Not used to select mode
        0, 0, 0 };                      // Not used to select mode

    // Choose a pixel format that best matches that described in pfd
    nPixelFormat = ChoosePixelFormat(hDC, &pfd);

    // Set the pixel format for the device context
    SetPixelFormat(hDC, nPixelFormat, &pfd);
}

void DealKey(HWND hWnd, HDC hDC, HGLRC hRC, int wParam)
{
    switch (wParam)
    {
    case TURN_U:
        if (rotateOK == true)
        {
            return;
        }
        rotateType  = ZCW_2;
        rotateOK    = true;
        rotateAngle = 0.0f;
        break;
    case TURN_F:
        if (rotateOK == true)
        {
            return;
        }
        rotateType  = XCW_2;
        rotateOK    = true;
        rotateAngle = 0.0f;
        break;
    case TURN_D:
        if (rotateOK == true)
        {
            return;
        }
        rotateType  = ZCCW_0;
        rotateOK    = true;
        rotateAngle = 0.0f;
        break;
    case TURN_B:
        if (rotateOK == true)
        {
            return;
        }
        rotateType  = XCCW_0;
        rotateOK    = true;
        rotateAngle = 0.0f;
        break;
    case TURN_R:
        if (rotateOK == true)
        {
            return;
        }
        rotateType  = YCW_2;
        rotateOK    = true;
        rotateAngle = 0.0f;
        break;
    case TURN_L:
        if (rotateOK == true)
        {
            return;
        }
        rotateType  = YCCW_0;
        rotateOK    = true;
        rotateAngle = 0.0f;
        break;
    case TURN_U_R:
        if (rotateOK == true)
        {
            return;
        }
        rotateType  = ZCCW_2;
        rotateOK    = true;
        rotateAngle = 0.0f;
        break;
    case TURN_F_R:
        if (rotateOK == true)
        {
            return;
        }
        rotateType  = XCCW_2;
        rotateOK    = true;
        rotateAngle = 0.0f;
        break;
    case TURN_D_R:
        if (rotateOK == true)
        {
            return;
        }
        rotateType  = ZCW_0;
        rotateOK    = true;
        rotateAngle = 0.0f;
        break;
    case TURN_B_R:
        if (rotateOK == true)
        {
            return;
        }
        rotateType  = XCW_0;
        rotateOK    = true;
        rotateAngle = 0.0f;
        break;
    case TURN_R_R:
        if (rotateOK == true)
        {
            return;
        }
        rotateType  = YCCW_2;
        rotateOK    = true;
        rotateAngle = 0.0f;
        break;
    case TURN_L_R:
        if (rotateOK == true)
        {
            return;
        }
        rotateType  = YCW_0;
        rotateOK    = true;
        rotateAngle = 0.0f;
        break;
    case NEXT_STEP:
        if (step < solutionSteps.size() && rotateOK == false)
        {
            SendMessage(hWnd, WM_KEYDOWN, solutionSteps.getStep(step++), NULL);
        }
        else if (rotateOK == true)
        {
            ++lostStepMessages; // I have lost some messages now!
        }
        break;
    default:
        if (lostStepMessages > 0 && step < solutionSteps.size() && rotateOK == false)
        {
            SendMessage(hWnd, WM_KEYDOWN, solutionSteps.getStep(step++), NULL);
            --lostStepMessages;
        }
        break;
    }
}

void ExitGame(HWND hWnd, HDC hDC, HGLRC hRC)
{
    // Kill the timer that we created  
    KillTimer(hWnd, 101);
    
    // Deselect the current rendering context and delete it
    wglMakeCurrent(hDC, NULL);
    wglDeleteContext(hRC);

    // Delete the palette  
    if (hPalette != NULL)
    {
        DeleteObject(hPalette);
    }

    // Tell the application to terminate after the window
    // is gone.  
    PostQuitMessage(0);
}

