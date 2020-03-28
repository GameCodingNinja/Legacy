/************************************************************************
*    FILE NAME:       uimenudefs.h
*
*    DESCRIPTION:     game menu defines
************************************************************************/

#ifndef __ui_menu_defs_h__
#define __ui_menu_defs_h__

namespace NUIMenu
{
    enum EMenuAlienment
    {
        EMA_LEFT=0,
        EMA_CENTER,
        EMA_RIGHT
    };

    enum EMenuState
    {
        EMS_HIDDEN=0,
        EMS_ACTIVE,
        EMS_INACTIVE,
        EMS_TRANSITION_IN,
        EMS_TRANSITION_OUT,
        EMS_TRANSITION,
        EMS_MAX_MENU_STATES
    };

    enum EMenuNavigationState
    {
        EMNS_NULL=0,
        EMNS_MOUSE,
        EMNS_GAMEPAD_KEYBAORD,
        EMNS_MAX_MENU_NAV_STATES
    };

    enum EMenuTreeState
    {
        EMTS_NULL=0,
        EMTS_DEACTIVATE,
        EMTS_HIDDEN,
        EMTS_WAIT_INPUT,
        EMTS_WAIT_SELECTED,
        EMTS_WAIT_TRANSITION_OUT,
        EMTS_WAIT_TRANSITION_OUT_TO_MENU,
        EMTS_WAIT_TRANSITION_OUT_RETURN,
        EMTS_WAIT_TRANSITION_OUT_TOGGLE,
        EMTS_WAIT_TRANSITION_IN,
        EMTS_MAX_MENU_TREE_STATES
    };
}

#endif  // __ui_menu_defs_h__