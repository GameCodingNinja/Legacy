
/************************************************************************
*    FILE NAME:   defs.h
*
*    DESCRIPTION: misc defines. Some expected to be used with windows.h
************************************************************************/

#ifndef __defs_h__
#define __defs_h__

#define _USE_MATH_DEFINES
#include <math.h>

// define an unsigned int
typedef unsigned int uint;

// A Windows handle is just a void pointer. To make the code more readable, let's
// define a void type to use with the below safe pointer
typedef void WIN_HANDLE;

#define DEG_TO_RAD 0.0174532925199432957
#define RAD_TO_DEG 57.29577951308232
#define EPSILON 8.854187817e-12

// User defined windows messages
enum
{
    WM_LOADING = 0x0400,
    WM_LOAD_COMPLETE
};

enum EHorzAlignment
{
    EHA_HORZ_LEFT,
    EHA_HORZ_CENTER,
    EHA_HORZ_RIGHT
};

enum EVertAlignment
{
    EVA_VERT_TOP,
    EVA_VERT_CENTER,
    EVA_VERT_BOTTOM
};


enum EPoint2D
{
    EP2D_X,
    EP2D_Y,
    EP2D_MAX,
};

enum EPoint3D
{
    EP3D_X,
    EP3D_Y,
    EP3D_Z,
    EP3D_MAX,
};

enum EVert
{
    EVERT_1,
    EVERT_2,
    EVERT_3,
    EVERT_MAX,
};


// Windows message crackers
#define HANDLE_WM_LOADING(hwnd, wParam, lParam, fn) \
    ((fn)(), 0L)
#define FORWARD_WM_LOADING(hwnd, fn) \
    (void)(fn)((), WM_DESTROY, 0L, 0L)

#define HANDLE_WM_LOAD_COMPLETE(hwnd, wParam, lParam, fn) \
    ((fn)(), 0L)
#define FORWARD_WM_LOAD_COMPLETE(hwnd, fn) \
    (void)(fn)((), WM_DESTROY, 0L, 0L)

#endif // __defs_h__
