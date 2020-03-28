
/************************************************************************
*
*    PRODUCT:         Camera list
*
*    FILE NAME:       TCamLst.h
*
*    DESCRIPTION:     Object for holding lists of camera movements.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __CAMERAOBJ_LIST_H__
#define __CAMERAOBJ_LIST_H__ 

#include "TObjList.h"        // Parent object class
#include "TStrList.h"        // Object file for holding lists of strings


/***************************************************************
*
*  TCameraOffset - Structure to handle coordinate movements
*
****************************************************************/
class  TCameraOffset
{
public:

    int XPos, YPos, ZPos;
    int XRot, YRot, ZRot;

    // Init the data members on allocation
    TCameraOffset()
    {
        XPos = YPos = ZPos = 0;
        XRot = YRot = ZRot = 0;
    }
};
typedef TCameraOffset * PTCameraOffset;


typedef struct tagTCameraByte
{
    char XPos, YPos, ZPos;
    char XRot, YRot, ZRot;
} TCameraByte, *PTCameraByte;

typedef struct tagTCameraByteCmp
{
    char XPos, YPos, ZPos;
    char XRot, YRot, ZRot;
    WORD count;
} TCameraByteCmp, *PTCameraByteCmp;

typedef struct tagTCameraWord
{
    short XPos, YPos, ZPos;
    short XRot, YRot, ZRot;
} TCameraWord, *PTCameraWord;

typedef struct tagTCameraWordCmp
{
    short XPos, YPos, ZPos;
    short XRot, YRot, ZRot;
    WORD count;
} TCameraWordCmp, *PTCameraWordCmp;

typedef struct tagTCameraInt
{
    int XPos, YPos, ZPos;
    int XRot, YRot, ZRot;
} TCameraInt, *PTCameraInt;

typedef struct tagTCameraIntCmp
{
    int XPos, YPos, ZPos;
    int XRot, YRot, ZRot;
    WORD count;
} TCameraIntCmp, *PTCameraIntCmp;

enum
{
    BYTE_SIZE=0,
    WORD_SIZE,
    INT_SIZE,
};

enum
{
    UNCOMPRESSED=0,
    COMPRESSED,
};

class TCameraList : public TObjList
{
protected:
    WORD counter;

public:
    // Common Functions

    // Constructor
    TCameraList();
    // Destructor - Put all your cleanup code here
    ~TCameraList();
    // Add a string
    virtual BOOL Add();
    // Frees the pointer from the object class
    void Free();
    // Get the size of the data needed for the Obj class
    // We don't want to save the file name
    virtual int GetObjSize( int Index ){ return sizeof( TCameraOffset ); };
    // Export data in the size of a byte
    BOOL ExportSimpleList( char *FileName, int DataSize, BOOL compressType );
    // Export data in the size of a byte
    BOOL WriteByteList( int index, HANDLE hFile, BOOL compressType );
    // Export data in the size of a word
    BOOL WriteWordList( int index, HANDLE hFile, BOOL compressType );
    // Export data in the size of a int
    BOOL WriteIntList( int index, HANDLE hFile, BOOL compressType );

    // Get the X Pos Offset
    PTCameraOffset GetCameraOffset( int Index ){ return ( Count() ? PTCameraOffset(GetObj( Index )) : NULL); };

    // Get the X Pos Offset
    int GetXPos( int Index ){ return ( Count() ? PTCameraOffset(GetObj( Index ))->XPos : 0); };
    // Get the Y Pos Offset
    int GetYPos( int Index ){ return ( Count() ? PTCameraOffset(GetObj( Index ))->YPos : 0); };
    // Get the Z Pos Offset
    int GetZPos( int Index ){ return ( Count() ? PTCameraOffset(GetObj( Index ))->ZPos : 0); };

    // Get the X Rot Offset
    int GetXRot( int Index ){ return ( Count() ? PTCameraOffset(GetObj( Index ))->XRot : 0); };
    // Get the Y Rot Offset
    int GetYRot( int Index ){ return ( Count() ? PTCameraOffset(GetObj( Index ))->YRot : 0); };
    // Get the Z Rot Offset
    int GetZRot( int Index ){ return ( Count() ? PTCameraOffset(GetObj( Index ))->ZRot : 0); };

    // Set the X Pos Offset
    void SetXPos( int Index, int Value ){ if( Count() ) PTCameraOffset(GetObj( Index ))->XPos = Value; };
    // Set the Y Pos Offset
    void SetYPos( int Index, int Value ){ if( Count() ) PTCameraOffset(GetObj( Index ))->YPos = Value;  };
    // Set the Z Pos Offset
    void SetZPos( int Index, int Value ){ if( Count() ) PTCameraOffset(GetObj( Index ))->ZPos = Value; };

    // Set the X Rot Offset
    void SetXRot( int Index, int Value ){ if( Count() ) PTCameraOffset(GetObj( Index ))->XRot = Value; };
    // Set the Y Rot Offset
    void SetYRot( int Index, int Value ){ if( Count() ) PTCameraOffset(GetObj( Index ))->YRot = Value; };
    // Set the Z Rot Offset
    void SetZRot( int Index, int Value ){ if( Count() ) PTCameraOffset(GetObj( Index ))->ZRot = Value; };

};

typedef TCameraList *PTCameraList;


#endif  /* __CAMERAOBJ_LIST_H__ */