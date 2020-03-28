
/************************************************************************
*
*    PRODUCT:         Debuging Functions
*
*    FILE NAME:       DebugTools.h
*
*    DESCRIPTION:     Screen Saver Tools
*
*    IMPLEMENTATION:  This module holds misc funstions for debugging
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __DEBUGTOOLS_H__
#define __DEBUGTOOLS_H__


// For debugging if we need to write info to a file
void Debug_TextToFile( char *FilePath, char *S, ... );

// For debugging if we need to write info to a file
void Debug_DataToFile( char *FilePath, void *data, int Size );

#endif  /* __DEBUGTOOLS__ */