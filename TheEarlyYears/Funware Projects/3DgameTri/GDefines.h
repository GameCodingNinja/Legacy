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

#define MAX_MOVEMENT_SPEED    2.5
#define GRAVITY               2.0
#define DIST_FROM_FLOOR       15.0
#define DIST_FROM_WALL        16.0

// triangle type
enum ETT_TRI_TYPE
{
	NO_TYPE_SET=0,
	FLOOR,
	WALL,
	CEILING,
};


// Misc defines
#define WINDOW_NAME            "3DGame"
#define WINDOW_TITLE           "3D Game 1.0.0"   

// Command defines
#define MNU_ABOUT              1
#define MNU_EXIT               2  // Also the same value when the little X box is clicked
#define MNU_START              3
#define MNU_STOP               4                


#endif // __GDefines_H__