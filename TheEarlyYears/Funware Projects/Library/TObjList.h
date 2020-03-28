
/************************************************************************
*
*    PRODUCT:         Object List
*
*    FILE NAME:       TObjList.h
*
*    DESCRIPTION:     Base class for managing lists of pointers. Inherit
*                     from this class to manage lists of specific data types.
*
*    IMPLEMENTATION:  
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __OBJECTLIST_H__
#define __OBJECTLIST_H__ 

#define OBJECT_CHECK_HEADER  0xFEDCBAFE

struct TObjFileHeader
{
    DWORD ObjForamtID;
    DWORD ObjCount;
    DWORD Future1;
    DWORD Future2;
    TObjFileHeader()
        { ObjForamtID = ObjCount = Future1 = Future2 = 0; };
};
typedef TObjFileHeader *PTObjFileHeader;

class TObjList
{
protected:
    // Number of current objects
    int count;
    // For allocating an array of voided pointers
    LPVOID *ppPointer;
    // If it is set to TRUE, then all allocations stop
    BOOL fAllocationError;
    // File header for file read and write
    TObjFileHeader ObjFileHeader;
    // Check Data that is between each set of data
    DWORD ObjCheckHeader;

    // Save object data to a file
    virtual BOOL LoadFromFile( HANDLE hFile );
    // Load object data from a file
    virtual BOOL SaveToFile( HANDLE hFile );

    // Get the object check data incase we want to change it
    virtual DWORD GetObjDataHeader( int Index ){ return ObjCheckHeader; };
    // Load the object data from a file. This function  is virtual so that  
    // reading in the object data can be customized for a given situation.
    virtual BOOL LoadObjectData( HANDLE hFile, int Index );
    // Load the object from a pointer.
    virtual BYTE *LoadObjectDataFromPointer( BYTE *pData, int Index );

public:
    // Common Functions

    // Constructor
    TObjList();
    // Destructor - Put all your cleanup code here
    ~TObjList();
    // Add a nother pointer
    BOOL AddObj( LPVOID Obj );
    // Delete pointer to the object at the given index
    virtual void Delete( int Index );
    // Return the number of objects
    int Count(){ return count; };
    // Free up all the memory
    void FreeObj();
    // Mix up all the objects into a random order
    void MixUp();
    // Returns a pointer to the object at the given index.
    LPVOID GetObj( int Index );
    // Sets an object pointer to the object array
    void SetObj( LPVOID t, int Index ){ ppPointer[Index] = t; };
    // Moves an index to a new position
    void MoveToNewPos( int CurrentIndex, int NewIndex );
    // Save object data to a file
    BOOL SaveToFile( char *FileName, DWORD Flags = OPEN_ALWAYS );
    // Load object data from a file
    BOOL LoadFromFile( char *FileName, DWORD Flags = OPEN_ALWAYS );
    // Get the size of the data
    virtual int GetObjSize( int Index ){ return 0; };
    // Allocate data for loading in file data
    virtual BOOL Add(){ return FALSE; };
    // Load the object data from a pointer
    BOOL LoadFromPointer( BYTE *pData );
};

typedef TObjList *PTObjList;


#endif  /* __OBJLIST__ */