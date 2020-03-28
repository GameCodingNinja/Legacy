
/************************************************************************
*
*    PRODUCT:         ists of Art 4 Sale Page List
*
*    FILE NAME:       TPageList.h
*
*    DESCRIPTION:     Object for holding lists of Art 4 Sale pages.
*                     Inherited from the object list class
*
*    IMPLEMENTATION:  
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __PAGELIST_H__
#define __PAGELIST_H__ 

#include "TObjList.h"        // Base class header file
#include "GDefines.h"        // Header file with #defines


struct TPage
{
    DWORD PageCheckID;          // Special number to check to validate data
    DWORD MainDataOffset;       // Data offset to image or text for this page
    DWORD SoundDataOffset;      // Data offset to sound for this page
    DWORD AVIDataOffset;        // Data offset to AVI for this page
    WORD  NumActiveClickRgns;   // Total number of click regions used for this page
	#ifdef SHARE_THE_MEMORIES
	char  Caption[MAX_PAGE_CAPTION+1]; // Text Caption for each page
	#endif

    // This holds the enum of the type of data the file is
    BYTE MainDataType;          // Data type of main image or text file 
    BYTE SoundDataType;         // Data type of sound file
    BYTE AVIDataType;           // Data type of video file

    // Tool version specific
    #ifdef EBOOK_TOOL
    // Flags that indicate that the data is a duplicate
    BOOL MainDataDuplicate;
    BOOL SoundDataDuplicate;
    BOOL AVIDataDuplicate;

    // The tool version needs to save the file paths for the project file
    // Below is the MAX_PAGE_STRS
    char MainData[MAX_STR_PATH];   // Path to main image file
    char SoundData[MAX_STR_PATH];  // Path to sound file
    char AVIData[MAX_STR_PATH];    // Path to video file
    #endif

    // Future datatypes here

    #ifdef EBOOK_TOOL
    BYTE FutureData[ PAGE_DATA_FUTURE ];  // Future data
    #endif
};
typedef TPage *PTPage;


// This structure is never used to hold data. It's used for easy calculating
// the size of the view programs TPage version of the structure. So this structure
// is the same as the TPage structure minus the tool data
#ifdef EBOOK_TOOL
struct TPageView
{
    DWORD PageCheckID;          // Special number to check to validate data
    DWORD MainDataOffset;       // Data offset to image or text for this page
    DWORD SoundDataOffset;      // Data offset to sound for this page
    DWORD AVIDataOffset;        // Data offset to AVI for this page
    WORD  NumActiveClickRgns;   // Total number of click regions used for this page
	#ifdef SHARE_THE_MEMORIES
	char  Caption[MAX_PAGE_CAPTION+1]; // Text Caption for each page
	#endif

    // This holds the enum of the type of data the file is
    BYTE MainDataType;          // Data type of main image or text file 
    BYTE SoundDataType;         // Data type of sound file
    BYTE AVIDataType;           // Data type of video file
};
#endif


class TPageList : public TObjList
{
protected:
    WORD CurrentPage;

public:
    // Common Functions

    // Constructor
    TPageList();
    // Destructor - Put all your cleanup code here
    ~TPageList();

    // Add a string
    virtual BOOL Add();
    // Get the size of the data
    virtual int GetObjSize( int Index ){ return sizeof(TPage); };

    // Returns a pointer to a string at the given index
    PTPage GetPage( WORD Index );
    // Free the whole object
    void Free();
    // Return the current page
    WORD GetCurrentPage(){ return CurrentPage; };
    // Advance the counter to the next page
    void NextPage(){ if( CurrentPage < Count()-1 ) ++CurrentPage; };
    // Advance the counter to the next page
    void PreviousPage(){ if( CurrentPage > 0 ) --CurrentPage; }
    // Set the current page
    void SetCurrentPage( WORD CurrPage ){ if( CurrPage < Count() ) CurrentPage = CurrPage; };

    // Get the page check ID
    DWORD GetPageCheckID( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->PageCheckID : 0); };

	#ifdef SHARE_THE_MEMORIES
    // Get the pointer o the caption text
    char *GetPtrToCaption( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->Caption : NULL); };
	#endif

    #ifdef EBOOK_TOOL
    // Set the MainDataOffset
    void SetMainDataOffset( WORD Index, DWORD Offset ){ if( Count() ) PTPage(GetObj( Index ))->MainDataOffset = Offset; };
    // Set the SoundDataOffset
    void SetSoundDataOffset( WORD Index, DWORD Offset ){ if( Count() ) PTPage(GetObj( Index ))->SoundDataOffset = Offset; };
    // Set the AVIDataOffset
    void SetAVIDataOffset( WORD Index, DWORD Offset ){ if( Count() ) PTPage(GetObj( Index ))->AVIDataOffset = Offset; };
    // Set/Check for duplicate data
    // Set the MainDataOffset if this is a duplicate
    void SetMainDataDuplicate( WORD Index, BOOL Dup ){ if( Count() ) PTPage(GetObj( Index ))->MainDataDuplicate = Dup; };
    // Set the SoundDataOffset if this is a duplicate
    void SetSoundDataDuplicate( WORD Index, BOOL Dup ){ if( Count() ) PTPage(GetObj( Index ))->SoundDataDuplicate = Dup; };
    // Set the AVIDataOffset if this is a duplicate
    void SetAVIDataDuplicate( WORD Index, BOOL Dup ){ if( Count() ) PTPage(GetObj( Index ))->AVIDataDuplicate = Dup; };
    // Set the MainDataType
    void SetMainDataType( WORD Index, BYTE DataType ){ if( Count() ) PTPage(GetObj( Index ))->MainDataType = DataType; };
    // Set the SoundDataType
    void SetSoundDataType( WORD Index, BYTE DataType ){ if( Count() ) PTPage(GetObj( Index ))->SoundDataType = DataType; };
    // Set the AVIDataType
    void SetAVIDataType( WORD Index, BYTE DataType ){ if( Count() ) PTPage(GetObj( Index ))->AVIDataType = DataType; };

    // Get pointer to MainData path
    char *GetMainDataPath( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->MainData : NULL); };
    // Get pointer to SoundData path
    char *GetSoundDataPath( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->SoundData : NULL); };
    // Get pointer to AVIData path
    char *GetAVIDataPath( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->AVIData : NULL); };
    // Get if the MainDataOffset is a duplicate
    BOOL GetMainDataDuplicate( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->MainDataDuplicate : 0); };
    // Get if the SoundDataOffset is a duplicate
    BOOL GetSoundDataDuplicate( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->SoundDataDuplicate : 0); };
    // Get if the AVIDataOffset is a duplicate
    BOOL GetAVIDataDuplicate( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->AVIDataDuplicate : 0); };

    #endif
    // Get the MainDataOffset
    DWORD GetMainDataOffset( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->MainDataOffset : 0); };
    // Get the SoundDataOffset
    DWORD GetSoundDataOffset( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->SoundDataOffset : 0); };
    // Get the AVIDataOffset
    DWORD GetAVIDataOffset( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->AVIDataOffset : 0); };
    
    // Get the MainDataType
    BYTE GetMainDataType( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->MainDataType : 0); };
    // Get the SoundDataType
    BYTE GetSoundDataType( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->SoundDataType : 0); };
    // Get the AVIDataType
    BYTE GetAVIDataType( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->AVIDataType : 0); }; 

    // Free up this classes memory
    //void FreePages();
    // Delete string
    //virtual void Delete( WORD Index );
};

typedef TPageList *PTPageList;


#endif  /* __PAGELIST__ */