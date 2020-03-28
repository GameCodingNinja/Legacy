
/************************************************************************
*
*    PRODUCT:         TBase
*
*    FILE NAME:       TGBase.h
*
*    DESCRIPTION:     Base class for all graphics objects
*
*    IMPLEMENTATION:  
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __TGBASE_H__
#define __TGBASE_H__


enum
{
    OBJECT_UNDEFINED=0,
    BITMAP_OBJECT,
    TEXT_OBJECT,
    METAFILE_OBJECT,
};


/************************************************************************
*                         TDIB Object
************************************************************************/

class TGBase
{
protected:
    // The type of object this is
    int ObjectType;
    // Size of the object in bytes
    int ObjectDataSizeInBytes;

public:
    // Constructor
    TGBase(){};
    // Destructor
    ~TGBase(){};

    // Get a pointer to a block of memory that is a dib
    virtual BOOL LoadFromBuffer( BYTE *pBuffer, DWORD Size = 0 ){ return 0; };
    // Load bitmap from file
    virtual BOOL LoadFromFile( char *FileName, BOOL showErrorMsg = TRUE ){ return 0; };
    // Load bitmap from resource
    virtual BOOL LoadFromResource( HINSTANCE hInst, char *name, char *type ){ return 0; };
    // Get the object type
    int GetObjectType(){ return ObjectType; };
    // Get the size of the object in bytes
    int GetObjectSizeInBytes(){ return ObjectDataSizeInBytes; };   
};

typedef TGBase *PTGBase;


#endif  /* __TBASE_H__ */