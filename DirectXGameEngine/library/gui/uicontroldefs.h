/************************************************************************
*    FILE NAME:       uicontroldefs.h
*
*    DESCRIPTION:     game ui control defines
************************************************************************/

#ifndef __ui_control_defs_h__
#define __ui_control_defs_h__


namespace NUIControl
{
    const bool TOGGLE_STATE_ON(true);
    const bool TOGGLE_STATE_OFF(false);

    // Value for no active control
    const int NO_ACTIVE_CONTROL = -1;

    // Increment/Decement constants
    enum
    { 
        BTN_DECREMENT,
        BTN_INCREMENT
    };

    enum EControlType
    {
        ECT_NULL=0,
        ECT_LABEL,
        ECT_BUTTON,
        ECT_BUTTON_LIST,
        ECT_CHECK_BOX,
        ECT_SLIDER,
        ECT_SCROLL_BOX,
        ECT_SUB_CONTROL,
        ECT_TAB_CONTROL,
        ECT_PROGRESS_BAR,
        ECT_AMOUNT_BUTTON,
        ECT_MAX_CONTROL_TYPES
    };

    enum EControlState
    {
        ECS_NULL=0,
        ECS_DISABLED,
        ECS_INACTIVE,
        ECS_ACTIVE,
        ECS_SELECTED,
        ECS_MAX_CONTROL_STATES
    };

    enum EControlActionType
    {
        ECAT_ACTION=0,
        ECAT_TO_MENU,
        ECAT_RETURN,
        ECAT_CLOSE,
        ECAT_DEACTIVATE,
        ECAT_CHANGE_FOCUS,
        ECAT_BACK_TO_MAIN_MENU,
        ECAT_MAX_CONTROL_ACTION_TYPES
    };

    enum ETransitionState
    {
        ETS_NULL=0,
        ETS_TRANSITION_IN,
        ETS_TRANSITION_OUT,
        ETS_MAX_TRANSITION_STATES
    };
}

#endif  // __ui_control_defs_h__