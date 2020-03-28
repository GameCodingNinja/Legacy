/************************************************************************
*
*    PRODUCT:         Screen saver config dialog box
*
*    FILE NAME:       BaseCfg.h
*
*    DESCRIPTION:     Base class for the config dialog box
*
*    IMPLEMENTATION:  Functions and features of all config dialog boxes
*
*    Copyright(c) 2000 by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __BASE_CONFIG_H__
#define __BASE_CONFIG_H__

#include "Settings.h"        // User settings for this screen saver

enum
{
    REG_NUM_ERROR=0,
    FIRST_TMP_REG_NUM,
    SECOND_TMP_REG_NUM,
};

enum
{
    THANK_YOU_FOR_REG = 200,
    FIRST_TMP_NUM_ACT,
    SECOND_TMP_NUM_ACT,
    TMP_NUM_ONLY_ONCE,
    REG_NUM_ERROR_MSG,
};


class TBaseConfigDlg
{
protected:
    // Handle to the parent window
    HWND hParentWnd;
    // This program's instance
    HINSTANCE hInst;
    // Flag that indicates if the user is a registered user
    BOOL fRegisteredUser;
    // User settings for this screen saver
    TSettings Settings;        

    // Check the registration number entered
    void CheckRegNumber( char *RegNum );
    // Setup the order form
    void SetupOrderFormInfo();
    // Displays the orderform dialog box
    void ShowOrderFormDlg( HWND hwnd );

public:
    // Constructor
    TBaseConfigDlg();
    // Destructor
    ~TBaseConfigDlg();
};


#endif  // __BASE_CONFIG_H__