
/************************************************************************
*
*    PRODUCT:         Screen Saver
*
*    FILE NAME:       GDefines.h
*
*    DESCRIPTION:     Global defines
*
*    IMPLEMENTATION:  Header file for the project files 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __GDefines_H__
#define __GDefines_H__

// Testing defines
//#define FULL_SCREEN_DEBUG
//#define DEBUG
//#define ON_OFF_TEST  // Causes the screen saver to turn on and off.

// Define the windows version
#define _WIN32_WINNT 0x0400
 
// Define packed structure, no padding
//#pragma pack(push, 1)

// Define a padded structure
//#pragma pack(pop)

// Screen Saver Specific
#define INI_FILE_NAME            "3001 Space Oddities-Prefs.ini"
#define PRODUCT_NAME             "3001 Space Oddities Screen Saver"
#define PROGRAM                  "3001 Space Oddities"
#define PRODUCT_VERSION          "3001 Space Oddities  Ver 1.2.1"
#define ORDER_FORM_TITLE         "3001 Space Oddities Order Form       www.howiesfunware.com"
#define AOL_ADDRESS              "funware"
#define WEB_ADDRESS              "www.howiesfunware.com"
#define ONLINE_ORDER_WEB_ADDRESS "https://www.howiesfunware.com/horder.html"
#define EMAIL_ADDRESS            "funware@aol.com"
#define MAIL_ORDER_ONLY          "(Mail Order Only)"
#define CO_NAME                  "Howie's Funware"
#define CO_ADDRESS1              "1822 Forest Bend Lane"
#define CO_CITY                  "Keller"
#define CO_ZIP                   "76248"
#define NUM_US_STATES            51
#define STATE_ORDER_INDEX        44
#define FLOPPY_DISK_INFO         "Include 3-1/2 floppy disk ($%.2lf extra)"
#define CREDIT_CARD_INFO         "To pay by credit card, fill out the information below. Please double-check your credit card number and expiration date before printing."


#define FULL_VERSION_NUMBER      "192799-23446"
#define FAKE_VERSION_NUMBER      FULL_VERSION_NUMBER
#define FIRST_TEMP_NUMBER        "16K-231-9A33"
#define SECOND_TEMP_NUMBER       "3W1-5F1-B861"
#define FIRST_CHECK_FILE         "c1wljxq9.dli"
#define SECOND_CHECK_FILE        "l3qevuwt.dli"
#define SHAREWAEW_TMP_RUN_MAX    200

// String Sizes
#define MAX_TITLE                150
#define MAX_INFO                 200
#define MAX_REG_FEE              50
#define MAX_SALES_TAX            50
#define MAX_PREF_STRS            1
#define MAX_PAGE_STRS            4
#define SMALL_ARRAY_50           50
#define SMALL_ARRAY_100          100
#define MED_ARRAY_300            300
#define LARGE_ARRAY_500          500
#define MAX_STR_PATH             (MAX_PATH*2)  // Twice the string length just in case
#define MAX_OF_NAME              150
#define MAX_OF_ADDRESS           150
#define MAX_OF_CITY              70
#define MAX_OF_ZIP               50
#define MAX_OF_EMAIL             100

// Key Codes
#define RETURN_KEY               0xD
#define LINEFEED_KEY             0xA 

#define ORDER_FORM_INFO          "To register the %s ($%.2lf), fill out the order form, print it, then mail it in with your payment."

// Change these to change the install messages
#define TYPEOFPROGRAM            "Screen saver"
#define TYPEOFPROGRAM2           "screen saver"

// Costs
#define PRODUCT_COST             10.00
#define SALES_TAX                0.08125
#define FLOPPY_DISK_COST         3.50


//Misc
#define SEMAPHORE                "HowiesFunwareScreenSavers"
#define ENABLE                   TRUE
#define DISABLE                  FALSE
#define CENTER_WND_TO_PARENT     TRUE


////////////////////////////
//    Dialog Defines
////////////////////////////

// Dialog box Defines
#define DLG_QUIT_BOX             2

// Button Defines
#define BTN_DONE                 100
#define BTN_NO_PRINTER           101
#define BTN_OK                   102
#define BTN_CANCEL               103
#define BTN_ORDER_FORM           104
#define BTN_PRINT_ORDERFORM      105
#define BTN_PLAY_SOUND           106
#define BTN_ORDER_ONLINE         107
#define BTN_ORDER_THRO_MAIL      108

// Text label control defines
#define TXT_REG_NUM              200
#define TXT_SHIP_HAND_COST       201
#define TXT_SHIP_HAND_STATIC     202
#define TXT_TITLE                203
#define TXT_IL_RESIDENT          204
#define TXT_TOTAL_COST           205
#define TXT_PRINTING             206
#define TXT_COUNT                208
#define TXT_HLP_TITLE            209
#define TXT_MEDIA_DIR            210
#define TXT_CREDIT_CARD_EXP      211
#define TXT_CREDIT_CARD_NO       212
#define TXT_CREDIT_CARD_INFO     213
#define TXT_STATE_TAX            214

// Edit field defines
#define EDT_REG_NUMBER           300
#define EDT_HELPFUL_HINT         301
#define EDT_CITY                 302
#define EDT_EMAIL                303
#define EDT_ADDRESS_2            304
#define EDT_ADDRESS_1            305
#define EDT_NAME                 306
#define EDT_CREDIT_CARD_NO       307
#define EDT_EXP_DATE             308
#define EDT_ZIP                  309
#define BTN_WEB_SITE             310

// Check box defines
#define CHK_DONT_SHOW_AGAIN      400
#define CHK_IL_RESIDENT          401
#define CHK_FLOPPY_DISK          402
#define CHK_PLANETS              403
#define CHK_SPACE_SHIPS          404
#define CHK_RAND_MUSIC           405
#define CHK_FRONT_OF_STARS       406
#define CHK_CLOUDS               407

// Scroll bars and boxes
#define SCRBAR_AMOUNT            500
#define LSTBOX_DIRECTORY         502
#define CBOBOX_DRIVE             503
#define CBOBOX_NUM_PLANETS       504
#define CBOBOX_NUM_SHIPS         505
#define CBO_STATE                506

// Radio Buttons
#define RDOBTN_MOVE_LEFT         601
#define RDOBTN_MOVE_RIGHT        602
#define RDOBTN_MOVE_UP           603
#define RDOBTN_MOVE_DOWN         604
#define RDOBTN_MOVE_RANDOM       605
#define RDOBTN_VISA              606
#define RDOBTN_MASTERCARD        607
#define RDOBTN_DISCOVER          608
#define RDOBTN_AMER_EXP          609
#define RDO_MUSIC_OFF            610
#define RDO_DEFAULT_MUSIC        611
#define RDO_LOAD_YOUR_OWN        612

// Misc
#define SHOW_HELPHINT_WND        900
#define GRPBOX_STAR_DIRECTION    901
#define GRPBOX_MUSIC             902


#endif  // __GDefines__