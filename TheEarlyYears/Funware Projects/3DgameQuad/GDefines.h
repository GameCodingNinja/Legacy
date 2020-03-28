/************************************************************************
*
*    PRODUCT:         Map texture editor
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

// This keeps us from having to compile all the different render flavor functions.
// Uncomment the ones you don't need and comment the ones you use


#define REDEFINE_MINZ  20.0F
#define REDEFINE_DEGREECOUNT 2048
#define MOVEMENT_SPEED      6.5
#define COLLISION_DETECTION 

//#define NO_16_TO_16_POS
#define NO_16_TO_16_POS_OR
//#define NO_16_TO_16_POS_TRANS
//#define NO_16_TO_16_NEG
#define NO_16_TO_16_NEG_OR
//#define NO_16_TO_16_NEG_TRANS
#define NO_16_TO_16_NO_Z_BUF
#define NO_16_TO_16_NO_Z_BUF_NO_SHTBL
#define NO_16_TO_16_NO_Z_BUF_OR
#define NO_16_TO_16_NO_Z_BUF_TRANS
#define NO_16_TO_16_POS_SOLID
#define NO_16_TO_16_NEG_SOLID

#define NO_8_TO_16_POS
#define NO_8_TO_16_POS_CHECK
#define NO_8_TO_16_POS_OR
#define NO_8_TO_16_POS_TRANS
#define NO_8_TO_16_NEG
#define NO_8_TO_16_NEG_OR
#define NO_8_TO_16_NEG_TRANS
#define NO_8_TO_16_NO_Z_BUF
#define NO_8_TO_16_NO_Z_BUF_OR
#define NO_8_TO_16_NO_Z_BUF_TRANS
#define NO_8_TO_16_POS_SOLID
#define NO_8_TO_16_NEG_SOLID

#define NO_8_TO_8_POS
#define NO_8_TO_8_POS_CHECK
#define NO_8_TO_8_POS_OR
#define NO_8_TO_8_POS_TRANS
#define NO_8_TO_8_NEG
#define NO_8_TO_8_NEG_OR
#define NO_8_TO_8_NEG_TRANS
#define NO_8_TO_8_NO_Z_BUF
#define NO_8_TO_8_NO_Z_BUF_OR
#define NO_8_TO_8_NO_Z_BUF_TRANS
#define NO_8_TO_8_POS_SOLID
#define NO_8_TO_8_NEG_SOLID


// Misc defines
#define WINDOW_NAME            "3DGame"
#define WINDOW_TITLE           "3D Game 1.0.0"   

// Command defines
#define MNU_ABOUT              1
#define MNU_EXIT               2  // Also the same value when the little X box is clicked
#define MNU_START              3
#define MNU_STOP               4                


#endif // __GDefines_H__