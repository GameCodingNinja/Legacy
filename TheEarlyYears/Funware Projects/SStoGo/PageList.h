
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
    DWORD MainDataOffset;       // Data offset to image for this page
    DWORD BackDataOffset;       // Data offset to image for this page
    DWORD SoundDataOffset;      // Data offset to sound for this page
    DWORD AVIDataOffset;        // Data offset to AVI for this page
    char  TopCaption[MAX_PAGE_CAPTION+1]; // Text Caption for top of page
    char  BotCaption[MAX_PAGE_CAPTION+1]; // Text Caption for bottom page
    BOOL  SizeToFit;                      // Size the main image to fit the screen
	BOOL  SizeToFitBackup;				  // This data is set and never changed

    // This holds the enum of the type of data the file is
    BYTE MainDataType;          // Data type of main image
    BYTE BackDataType;          // Data type of back image 
    BYTE SoundDataType;         // Data type of sound file
    BYTE AVIDataType;           // Data type of video file

    LOGFONT TopFont;            // Font data for top font
    LOGFONT BotFont;            // Font data for bottom font

    COLORREF TopFontColor;      // Top font color
    COLORREF BotFontColor;      // Bottom font color
    COLORREF BackgroundColor;   // Background color

    BYTE TopTxtJust;            // Justification type of top text
    BYTE BotTxtJust;            // Justification type of bottom text

    BYTE DelayInSec;            // The delay amount to leave the current screen visible
    BYTE TransType;             // The transition type used to draw this screen

    // Tool version specific
    #ifdef SS2GO_TOOL
    // Flags that indicate that the data is a duplicate
    BOOL MainDataDuplicate;
    BOOL BackDataDuplicate;
    BOOL SoundDataDuplicate;
    BOOL AVIDataDuplicate;

    // The tool version needs to save the file paths for the project file
    // Below is the MAX_PAGE_STRS
    char MainData[MAX_STR_PATH];   // Path to main image file
    char BackData[MAX_STR_PATH];   // Path to background image file
    char SoundData[MAX_STR_PATH];  // Path to sound file
    char AVIData[MAX_STR_PATH];    // Path to video file
    #endif

    // Future datatypes here

    #ifdef SS2GO_TOOL
    BYTE FutureData[ PAGE_DATA_FUTURE ];  // Future data
    #endif
};
typedef TPage *PTPage;


// This structure is never used to hold data. It's used for easy calculating
// the size of the view programs TPage version of the structure. So this structure
// is the same as the TPage structure minus the tool data
// This structure needs to be divisible by 4
#ifdef SS2GO_TOOL
struct TPageView
{
    DWORD PageCheckID;          // Special number to check to validate data
    DWORD MainDataOffset;       // Data offset to image for this page
    DWORD BackDataOffset;       // Data offset to image for this page
    DWORD SoundDataOffset;      // Data offset to sound for this page
    DWORD AVIDataOffset;        // Data offset to AVI for this page
    char  TopCaption[MAX_PAGE_CAPTION+1]; // Text Caption for top of page
    char  BotCaption[MAX_PAGE_CAPTION+1]; // Text Caption for bottom page
    BOOL  SizeToFit;                      // Size the main image to fit the screen
	BOOL  SizeToFitBackup;				  // This data is set and never changed

    // This holds the enum of the type of data the file is
    BYTE MainDataType;          // Data type of main image
    BYTE BackDataType;          // Data type of main image 
    BYTE SoundDataType;         // Data type of sound file
    BYTE AVIDataType;           // Data type of video file

    LOGFONT TopFont;            // Font data for top font
    LOGFONT BotFont;            // Font data for bottom font

    COLORREF TopFontColor;      // Top font color
    COLORREF BotFontColor;      // Bottom font color
    COLORREF BackgroundColor;   // Background color

    BYTE TopTxtJust;            // Justification type of top text
    BYTE BotTxtJust;            // Justification type of bottom text

    BYTE DelayInSec;            // The delay amount to leave the current screen visible
    BYTE TransType;             // The transition type used to draw this screen  
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

    // Add a an object
    virtual BOOL Add();
    // Get the size of the data
    virtual int GetObjSize( int Index ){ return sizeof(TPage); };

    // Returns a pointer to the object at the given index
    PTPage GetPage( WORD Index );
    // Free the whole object
    void Free();
    // Return the current page
    WORD GetCurrentPage(){ return CurrentPage; };
    // Advance the counter to the next page
    void NextPage(){ CurrentPage = ( CurrentPage + 1 ) % Count(); };
    // Advance the counter to the next page
    void PreviousPage(){ if( CurrentPage > 0 ) --CurrentPage; }
    // Set the current page
    void SetCurrentPage( WORD CurrPage ){ if( CurrPage < Count() ) CurrentPage = CurrPage; };
    // Get the pointer o the caption text
    char *GetPtrToTopCaption( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->TopCaption : NULL); };
    // Get the pointer o the caption text
    char *GetPtrToBotCaption( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->BotCaption : NULL); };

    // Get the page check ID
    DWORD GetPageCheckID( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->PageCheckID : 0); };

    // Has text been entered into the top caption?
    BOOL IsTopCaptionText( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->TopCaption[0] : NULL); };
    // Has text been entered into the bottom caption?
    BOOL IsBotCaptionText( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->BotCaption[0] : NULL); };

    // Get the MainDataOffset
    DWORD GetMainDataOffset( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->MainDataOffset : 0); };
    // Get the BackDataOffset
    DWORD GetBackDataOffset( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->BackDataOffset : 0); };
    // Get the SoundDataOffset
    DWORD GetSoundDataOffset( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->SoundDataOffset : 0); };
    // Get the AVIDataOffset
    DWORD GetAVIDataOffset( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->AVIDataOffset : 0); };
    
    // Get the MainDataType
    BYTE GetMainDataType( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->MainDataType : 0); };
    // Get the BackDataType
    BYTE GetBackDataType( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->BackDataType : 0); };
    // Get the SoundDataType
    BYTE GetSoundDataType( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->SoundDataType : 0); };
    // Get the AVIDataType
    BYTE GetAVIDataType( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->AVIDataType : 0); };

    // Get the Top Logfont
    LOGFONT * GetPtrToTopLogfont( WORD Index ){ return ( Count() ? &PTPage(GetObj( Index ))->TopFont : NULL); };
    // Get the Top Logfont
    LOGFONT * GetPtrToBotLogfont( WORD Index ){ return ( Count() ? &PTPage(GetObj( Index ))->BotFont : NULL); };

    // Get the Top Font Color
    COLORREF GetTopFontColor( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->TopFontColor : 0); };
    // Get the Bottom Font Color
    COLORREF GetBotFontColor( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->BotFontColor : 0); };
    // Get the Bottom Font Color
    COLORREF GetBackColor( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->BackgroundColor : 0); };

    // Get justification type of top text
    BYTE GetTopTxtJust( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->TopTxtJust : 0); };
    // Get justification type of bottom text
    BYTE GetBotTxtJust( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->BotTxtJust : 0); };

    // Get the delay in seconds
    BYTE GetDelayInSec( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->DelayInSec : 0); };
    // Get the Size To Fit
    BOOL GetSizeToFit( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->SizeToFit : 0); };
    // Get the Size To Fit backup
    BOOL GetSizeToFitBackup( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->SizeToFitBackup : 0); };

	#ifndef SS2GO_TOOL
    // Set Get the delay in seconds
    void SetSizeToFit( WORD Index, BOOL SizeFit ){ if( Count() ) PTPage(GetObj( Index ))->SizeToFit = SizeFit; };
	#endif

    #ifdef SS2GO_TOOL

    // Set Get the delay in seconds
    void SetSizeToFit( WORD Index, BOOL SizeFit ){ if( Count() ){ PTPage(GetObj( Index ))->SizeToFit = SizeFit; PTPage(GetObj( Index ))->SizeToFitBackup = SizeFit; }};
    // Set the MainDataOffset
    void SetMainDataOffset( WORD Index, DWORD Offset ){ if( Count() ) PTPage(GetObj( Index ))->MainDataOffset = Offset; };
    // Set the BackDataOffset
    void SetBackDataOffset( WORD Index, DWORD Offset ){ if( Count() ) PTPage(GetObj( Index ))->BackDataOffset = Offset; };
    // Set the SoundDataOffset
    void SetSoundDataOffset( WORD Index, DWORD Offset ){ if( Count() ) PTPage(GetObj( Index ))->SoundDataOffset = Offset; };
    // Set the AVIDataOffset
    void SetAVIDataOffset( WORD Index, DWORD Offset ){ if( Count() ) PTPage(GetObj( Index ))->AVIDataOffset = Offset; };
    // Set/Check for duplicate data
    // Set the MainDataOffset if this is a duplicate
    void SetMainDataDuplicate( WORD Index, BOOL Dup ){ if( Count() ) PTPage(GetObj( Index ))->MainDataDuplicate = Dup; };
    // Set the BackDataOffset if this is a duplicate
    void SetBackDataDuplicate( WORD Index, BOOL Dup ){ if( Count() ) PTPage(GetObj( Index ))->BackDataDuplicate = Dup; };
    // Set the SoundDataOffset if this is a duplicate
    void SetSoundDataDuplicate( WORD Index, BOOL Dup ){ if( Count() ) PTPage(GetObj( Index ))->SoundDataDuplicate = Dup; };
    // Set the AVIDataOffset if this is a duplicate
    void SetAVIDataDuplicate( WORD Index, BOOL Dup ){ if( Count() ) PTPage(GetObj( Index ))->AVIDataDuplicate = Dup; };
    // Set the MainDataType
    void SetMainDataType( WORD Index, BYTE DataType ){ if( Count() ) PTPage(GetObj( Index ))->MainDataType = DataType; };
    // Set the BackDataType
    void SetBackDataType( WORD Index, BYTE DataType ){ if( Count() ) PTPage(GetObj( Index ))->BackDataType = DataType; };
    // Set the SoundDataType
    void SetSoundDataType( WORD Index, BYTE DataType ){ if( Count() ) PTPage(GetObj( Index ))->SoundDataType = DataType; };
    // Set the AVIDataType
    void SetAVIDataType( WORD Index, BYTE DataType ){ if( Count() ) PTPage(GetObj( Index ))->AVIDataType = DataType; };

    // Set the Top Font color
    void SetTopFontColor( WORD Index, COLORREF Color ){ if( Count() ) PTPage(GetObj( Index ))->TopFontColor = Color; };
    // Set the Bottom Font color
    void SetBotFontColor( WORD Index, COLORREF Color ){ if( Count() ) PTPage(GetObj( Index ))->BotFontColor = Color; };
    // Set the Background color
    void SetBackColor( WORD Index, COLORREF Color ){ if( Count() ) PTPage(GetObj( Index ))->BackgroundColor = Color; };

    // Get pointer to MainData path
    char *GetMainDataPath( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->MainData : NULL); };
    // Get pointer to BackData path
    char *GetBackDataPath( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->BackData : NULL); };
    // Get pointer to SoundData path
    char *GetSoundDataPath( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->SoundData : NULL); };
    // Get pointer to AVIData path
    char *GetAVIDataPath( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->AVIData : NULL); };
    // Get if the MainDataOffset is a duplicate
    BOOL GetMainDataDuplicate( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->MainDataDuplicate : 0); };
    // Get if the BackDataOffset is a duplicate
    BOOL GetBackDataDuplicate( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->BackDataDuplicate : 0); };
    // Get if the SoundDataOffset is a duplicate
    BOOL GetSoundDataDuplicate( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->SoundDataDuplicate : 0); };
    // Get if the AVIDataOffset is a duplicate
    BOOL GetAVIDataDuplicate( WORD Index ){ return ( Count() ? PTPage(GetObj( Index ))->AVIDataDuplicate : 0); };

    // Set justification type of top text
    void SetTopTxtJust( WORD Index, BYTE TxtJust ){ if( Count() ) PTPage(GetObj( Index ))->TopTxtJust = TxtJust; };
    // Set justification type of bottom text
    void SetBotTxtJust( WORD Index, BYTE TxtJust ){ if( Count() ) PTPage(GetObj( Index ))->BotTxtJust = TxtJust; };

    // Set Get the delay in seconds
    void SetDelayInSec( WORD Index, BYTE Delay ){ if( Count() ) PTPage(GetObj( Index ))->DelayInSec = Delay; };

    #endif
    
    // Free up this classes memory
    //void FreePages();
    // Delete string
    //virtual void Delete( int Index ); 
};

typedef TPageList *PTPageList;


#endif  /* __PAGELIST__ */