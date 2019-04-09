#pragma once

#define PI                      3.1415926f
#define ROTATE_ANGEL_PER_FRAME  (10 * PI / 180)
#define SPIN_FLAG               true

#define CUBE_LAYER  3
#define CUBE_SIZE   90.0f               // 魔方大小
#define CUBE_SPACE  CUBE_SIZE / 2.0f    // 魔方间隙

const float BLACK_R = 21 / 255.0f;
const float BLACK_B = 22 / 255.0f;
const float BLACK_G = 23 / 255.0f;

enum KEY :char {
    RESET_COLOR = 'c',
    // CW
    TURN_U      = 'w',
    TURN_D      = 's',
    TURN_L      = 'a',
    TURN_R      = 'd',
    TURN_F      = 'q',
    TURN_B      = 'e',
    // CCW(reverse)
    TURN_U_R    = 'i',
    TURN_D_R    = 'k',
    TURN_L_R    = 'j',
    TURN_R_R    = 'l',
    TURN_F_R    = 'u',
    TURN_B_R    = 'o',

    PRINT       = 'p',
    NEXT_STEP   = 'n'
};

enum AXIS
{
    X_AXIS = 1,
    Y_AXIS = 2,
    Z_AXIS = 3
};

#define X_ORIENT    0
#define Y_ORIENT    0
#define Z_ORIENT    0

#define X_ROTATE    295.0f
#define Y_ROTATE    0.0f
#define Z_ROTATE    225.0f

enum ROTATE_TYPE {
    ZCW_2       = 1,
    XCW_2       = 2,
    ZCCW_0      = 3,
    XCCW_0      = 4,
    YCW_2       = 5,
    YCCW_0      = 6,
    ZCCW_2      = 7,
    XCCW_2      = 8,
    ZCW_0       = 9,
    XCW_0       = 10,
    YCCW_2      = 11,
    YCW_0       = 12
};

enum CUEB_FACE {
    FACE_U      = 0,
    FACE_F      = 1,
    FACE_D      = 2,
    FACE_B      = 3,
    FACE_R      = 4,
    FACE_L      = 5,
    FACE_VOID   = 6
};

#define COLOR_WHITE     FACE_U
#define COLOR_RED       FACE_F
#define COLOR_YELLOW    FACE_D
#define COLOR_ORANGE    FACE_B
#define COLOR_BLUE      FACE_R
#define COLOR_GREEN     FACE_L
#define COLOR_BLACK     FACE_VOID
