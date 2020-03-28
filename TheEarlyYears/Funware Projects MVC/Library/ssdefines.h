
/************************************************************************
*
*    PRODUCT:         Screen Saver defines
*
*    FILE NAME:       ssDefines.h
*
*    DESCRIPTION:     Global defines
*
*    IMPLEMENTATION:  Header file for the project files 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __ssDefines_H__
#define __ssDefines_H__

// Key Codes
#define RETURN_KEY               0xD
#define LINEFEED_KEY             0xA

//Misc
#define SEMAPHORE                "HowiesFunwareScreenSavers"
#define MAX_STR_PATH             (MAX_PATH*2)  // Twice the string length just in case
#define WEB_ADDRESS              "www.howiesfunware.com"

// Dialog box Defines
#define DLG_QUIT_BOX             2

// Button Defines
#define BTN_DONE                 100
#define BTN_OK                   101
#define BTN_CANCEL               102
#define BTN_PLAY_SOUND           103
#define BTN_WEB_SITE             104

// Text label control defines
#define TXT_HLP_TITLE            110
#define TXT_MEDIA_DIR            111
#define TXT_SPEED                112

// Edit field defines
#define EDT_HELPFUL_HINT         120

// Check box defines
#define CHK_DONT_SHOW_AGAIN      130
#define CHK_RAND_MUSIC           131

// Scroll bars and boxes
#define SCRBAR_SPEED             140

// Radio Buttons
#define RDO_MUSIC_OFF            150
#define RDO_DEFAULT_MUSIC        151
#define RDO_LOAD_YOUR_OWN        152

// Misc
#define GRPBOX_MUSIC             160
#define SHOW_HELPHINT_WND        161
#define CBO_STATE                162


#endif  // __ssDefines__

