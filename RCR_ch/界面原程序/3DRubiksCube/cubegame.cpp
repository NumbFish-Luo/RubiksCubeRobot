// 通过键盘上的按键 WASD(上下左右顺时针) QE(前后顺时针) IJKL(上下左右逆时针) UO(前后逆时针) 来旋转改变魔方的各种组合
// 多字节字符集，附加依赖项 opengl32.lib glu32.lib

#include "cubegame.hpp"
#include <opencv2/opencv.hpp>

const int WIDTH = 280, HEIGHT = 280;

int GetFileWriteTime(const LPCTSTR &lpszFilePath);

inline int GetFileWriteTime(const LPCTSTR& lpszFilePath) {
    WIN32_FIND_DATA FindFileData = { 0 };

    HANDLE hFile = ::FindFirstFile(lpszFilePath, &FindFileData);

    if (INVALID_HANDLE_VALUE == hFile) {
        cout << "INVALID HANDLE VALUE!" << endl;
        return -1;
    }
    SYSTEMTIME WriteTime = { 0 }; // 最近修改时间
    if (!::FileTimeToSystemTime(&FindFileData.ftLastWriteTime, &WriteTime)) {
        cout << "Write time to system time error!" << endl;
        return -1;
    }

    int time = WriteTime.wMilliseconds + WriteTime.wSecond * 1000; // 0 ~ 59999
    return time;
}

//int main(int argc, char **argv)
//{
//    HWND hwnd = FindWindow("ConsoleWindowClass", argv[0]);
//    if (hwnd) { ShowWindow(hwnd, SW_MAXIMIZE /*SW_HIDE*/); }
//    HINSTANCE   hInstance{};

int APIENTRY    WinMain(
    HINSTANCE   hInstance,
    HINSTANCE   hPrevInstance,
    LPSTR       lpCmdLine,
    int         nCmdShow) {
    myCubeManage.input("INPUT.txt");
    solutionSteps = steps("OUTPUT.txt");

    const LPCTSTR lpszAppName{ "WcgCube" };
    MSG           message;
    WNDCLASS      wndClass;
    HWND          hWnd;
    HWND          hDesktopWnd;
    HDC           hDesktopDC;

    wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wndClass.lpfnWndProc = (WNDPROC)WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = NULL;
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = NULL; // No need for background brush for OpenGL window
    wndClass.lpszClassName = lpszAppName;
    wndClass.lpszMenuName = NULL;

    if (RegisterClass(&wndClass) == 0) {
        return FALSE;
    }

    // 获取整个桌面的句柄，以此获取桌面大小
    hDesktopWnd = GetDesktopWindow();
    hDesktopDC = GetDC(hDesktopWnd);

    int ScreenW = WIDTH;
    int ScreenH = HEIGHT;
    int ScreenX = (GetDeviceCaps(hDesktopDC, HORZRES) - ScreenW) / 2;
    int ScreenY = (GetDeviceCaps(hDesktopDC, VERTRES) - ScreenH) / 2;

    ReleaseDC(hDesktopWnd, hDesktopDC);

    hWnd = CreateWindow(
        lpszAppName,
        lpszAppName,
        // OpenGL requires WS_CLIPCHILDREN and WS_CLIPSIBLINGS
        WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
        // Window position and size
        ScreenX, ScreenY,
        ScreenW, ScreenH,
        NULL,
        NULL,
        hInstance,
        NULL);

    // If window was not created, quit
    if (hWnd == NULL) {
        return FALSE;
    }

    //CTransparent::SetTransparentWnd(hWnd, 0xffffff, 0, 2); // 窗口透明化

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    int inputFileWriteTime{ GetFileWriteTime("INPUT.txt") };
    int outputFileWriteTime{ GetFileWriteTime("OUTPUT.txt") };

    // Process application messages until the application closes
    while (GetMessage(&message, NULL, 0, 0)) {
        if (GetFileWriteTime("INPUT.txt") != inputFileWriteTime) {
            myCubeManage.input("INPUT.txt");
            step = 0;
            lostStepMessages = 0;
            inputFileWriteTime = GetFileWriteTime("INPUT.txt");
        }
        if (GetFileWriteTime("OUTPUT.txt") != outputFileWriteTime) {
            solutionSteps = steps("OUTPUT.txt");
            step = 0;
            lostStepMessages = 0;
            outputFileWriteTime = GetFileWriteTime("OUTPUT.txt");
        }

        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return static_cast<int>(message.wParam);
}

LRESULT
CALLBACK    WndProc(
    HWND    hWnd,
    UINT    message,
    WPARAM  wParam,
    LPARAM  lParam) {
    static HGLRC hRenderContext; // Permenant Rendering context
    static HDC   hDeviceContext; // Private GDI Device context

    switch (message) {
        // Window creation, setup for OpenGL
    case WM_CREATE:
        // Store the device context
        hDeviceContext = GetDC(hWnd);

        // Select the pixel format
        SetDCPixelFormat(hDeviceContext);

        // Create the rendering context and make it current
        hRenderContext = wglCreateContext(hDeviceContext); // https://blog.csdn.net/xuqiang918/article/details/8916293
        wglMakeCurrent(hDeviceContext, hRenderContext); // https://msdn.microsoft.com/en-us/library/windows/desktop/dd374387(v=vs.85).aspx

        // Create the palette
        hPalette = GetOpenGLPalette(hDeviceContext);

        // Black background
        glClearColor(BLACK_R, BLACK_B, BLACK_G, 1.0f); // RBGA
        glEnable(GL_DEPTH_TEST);

        // glEnable(GL_DITHER);  
        glShadeModel(GL_SMOOTH);

        // Create a timer that fires 30 times a second
        SetTimer(hWnd, 33, 15, NULL);
        break;

        // Window is being destroyed, cleanup
    case WM_DESTROY:
        ExitGame(hWnd, hDeviceContext, hRenderContext);
        break;

    case WM_KEYDOWN:
        DealKey(hWnd, hDeviceContext, hRenderContext, static_cast<int>(wParam));
        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_CHAR:
        DealKey(hWnd, hDeviceContext, hRenderContext, static_cast<int>(wParam));
        InvalidateRect(hWnd, NULL, FALSE);
        break;

    case WM_SIZE:
        ChangeSize(LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_TIMER:
    {
        if (SPIN_FLAG == true) {
            xRotate += 0.5f;
            yRotate -= 0.5f;
            zRotate -= 0.5f;
        }
        if (xRotate > 360.0f) {
            xRotate -= 360.0f;
        }
        else if (xRotate < 0.0f) {
            xRotate += 360.0f;
        }
        if (yRotate > 360.0f) {
            yRotate -= 360.0f;
        }
        else if (yRotate < 0.0f) {
            yRotate += 360.0f;
        }
        if (zRotate > 360.0f) {
            zRotate -= 360.0f;
        }
        else if (zRotate < 0.0f) {
            zRotate += 360.0f;
        }

        static int nextFileWriteTime{ GetFileWriteTime("NEXT.txt") };
        if (GetFileWriteTime("NEXT.txt") != nextFileWriteTime) {
            Sleep(10);
            int nextStep{};
            std::ifstream nextTxt{ "NEXT.txt" };
            nextTxt >> nextStep;
            nextTxt.close();
            if (nextStep == 0) {
                step = 0;
                lostStepMessages = 0;
            }
            else if (nextStep + 2 > step + lostStepMessages) {
                DealKey(hWnd, hDeviceContext, hRenderContext, NEXT_STEP);
            }
            nextFileWriteTime = GetFileWriteTime("NEXT.txt");
        }

        RotateAngleFunction();

        // 截图给opencv
        unsigned char* buffer = new unsigned char[WIDTH * HEIGHT * 3];
        glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);
        cv::Mat image(WIDTH, HEIGHT, CV_8UC3, buffer);
        cv::cvtColor(image, image, CV_BGR2RGB);
        cv::flip(image, image, 0);

        static const std::string imgName = "cube3d.bmp";
        cv::imwrite(imgName, image);

        delete[] buffer;

        InvalidateRect(hWnd, NULL, FALSE);
    }
    break;

    case WM_PAINT:
    {
        // Call OpenGL drawing code
        RenderScene();

        // Call function to swap the buffers
        SwapBuffers(hDeviceContext);

        // Validate the newly painted client area
        ValidateRect(hWnd, NULL);
    }
    break;

    // Windows is telling the application that it may modify
    // the system palette.  This message in essance asks the
    // application for a new palette.  
    case WM_QUERYNEWPALETTE:
        // If the palette was created.  
        if (hPalette) {
            int nRet;

            // Selects the palette into the current device context
            SelectPalette(hDeviceContext, hPalette, FALSE);

            // Map entries from the currently selected palette to
            // the system palette.  The return value is the number
            // of palette entries modified.
            nRet = RealizePalette(hDeviceContext);

            // Repaint, forces remap of palette in current window
            InvalidateRect(hWnd, NULL, FALSE);

            return nRet;
        }
        break;

        // This window may set the palette, even though it is not the
        // currently active window.  
    case WM_PALETTECHANGED:
        // Don't do anything if the palette does not exist, or if
        // this is the window that changed the palette.
        if ((hPalette != NULL) && ((HWND)wParam != hWnd)) {
            // Select the palette into the device context
            SelectPalette(hDeviceContext, hPalette, FALSE);

            // Map entries to system palette
            RealizePalette(hDeviceContext);

            // Remap the current colors to the newly realized palette
            UpdateColors(hDeviceContext);
            return 0;
        }
        break;

    default:   // Passes it on if unproccessed
        return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (0L);
}
