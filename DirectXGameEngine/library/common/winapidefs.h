
/************************************************************************
*    FILE NAME:   winapidefs.h
*
*    DESCRIPTION: misc defines. Some expected to be used with windows.h
************************************************************************/

#ifndef __win_api_defs_h__
#define __win_api_defs_h__

// define a function pointer to pass the windows callback function
typedef LRESULT (CALLBACK * winCallbackFuncPtrType)(HWND, UINT, WPARAM, LPARAM);

// define a function pointer to pass the windows callback function
typedef void (CALLBACK * timerCallbackFuncPtrType)(HWND, UINT, UINT, DWORD);

#endif // __win_api_defs_h__
