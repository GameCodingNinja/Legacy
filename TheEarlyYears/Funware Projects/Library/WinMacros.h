
/************************************************************************
*
*    PRODUCT:         Windowsx Macro
*
*    FILE NAME:       WinMacros.h
*
*    DESCRIPTION:     Windowsx macros that were never developed 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __MACROLIST_H__
#define __MACROLIST_H__

#include <windows.h>
#include <windowsx.h>        // Macros that makes windows programming easer and more readable. Doesn't add to code size


#define TBBtn_SetState( hToolBar, BTN_ID, state )  ((UINT)(DWORD)SNDMSG((hToolBar), TB_SETSTATE, (WPARAM)(int)(BTN_ID), \
                                                    MAKELPARAM( (state ? TBSTATE_ENABLED : TBSTATE_INDETERMINATE), 0 )))

#define Spinner_SetText( hwndCtl, lpsz )           SetWindowText((hwndCtl), (lpsz))
#define Spinner_GetPos( hwndCtl )                   ((int)(DWORD)SNDMSG((hwndCtl), UDM_GETPOS, 0L, 0L))

/* void Cls_OnMouseWheel(HWND hwnd, int xPos, int yPos, int zDelta, UINT fwKeys) */
#define HANDLE_WM_MOUSEWHEEL(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (int)(short)HIWORD(wParam), (UINT)(short)LOWORD(wParam)))
#define FORWARD_WM_MOUSEWHEEL(hwnd, xPos, yPos, zDelta, fwKeys, fn) \
    (void)(fn)((hwnd), WM_MOUSEWHEEL, MAKEWPARAM((fwKeys),(zDelta)), MAKELPARAM((x),(y)))

#endif  /* __MACROLIST__ */