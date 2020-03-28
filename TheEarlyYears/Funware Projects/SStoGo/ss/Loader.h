
/************************************************************************
*
*    PRODUCT:         Screen Saver To Go
*
*    FILE NAME:       Loader.h
*
*    DESCRIPTION:     Code for loading the graphics from the back of the file 
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __LOADER_H__
#define __LOADER_H__

#include "TBitmap.h"         // Dib and Bitmap class for displaying and animating bitmaps
#include "S2GStrct.h"		 // Screen Savers To Go structure header file
#include "PageList.h"        // Dynamic Allocation of pages

// Open this applications exe file and read in the header information
BOOL Loader_OpenEXEandReadHeader( PTFileHeader pFileHeader, PTProjectPrefs pProjPrefsData, PTPageList pPageList, char *pPath );
// Load the bitmap
BOOL Loader_LoadFile( HWND hwnd, PTBitmap pBitmap, DWORD DataOffset, char *pPath );
// Loads the imgage data from the exe
BOOL Loader_LoadFileDataFromEXEfile( DWORD Offset, PTLoadBuffer pLoadBuffer, char *pPath );


#endif  // __LOADER_H__