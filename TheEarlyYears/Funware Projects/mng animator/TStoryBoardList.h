
/************************************************************************
*
*    PRODUCT:         Storyboard list
*
*    FILE NAME:       TStoryBoardList.h
*
*    DESCRIPTION:     Object for holding lists of storyboards
*
*    IMPLEMENTATION:  
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __STORYBOARD_LIST_H__
#define __STORYBOARD_LIST_H__ 

#include "TObjList.h"        // Parent object class
#include "mng.h"             // MNG header file
#include "GDefines.h"        // Header file with #defines
#include "TStrList.h"        // List object for storing lists of strings
#include "mediaply.h"        // Header file for media player

#define DEF_SND_VOLUME  100
#define DEF_SND_CHANNEL 0

/************************************************************************
*
*                        Sound list list object
*
************************************************************************/

struct SSoundList
{
    int ID;          // index into the sound list
    int Volume;      // Sound volume
    int Channel;     // Channel to play the sound on

    TMediaPlayer MediaPlayer;  // For playing the selected media files

    SSoundList()
    {
        ID = -1;
        Volume = DEF_SND_VOLUME;
        Channel = DEF_SND_CHANNEL; 
    };
};
typedef SSoundList * PSSoundList;


class CSoundListAry : public TObjList
{

public:
    // Common Functions

    // Constructor
    CSoundListAry();
    // Destructor - Put all your cleanup code here
    ~CSoundListAry();
    // Add a string
    virtual BOOL Add();
    // Frees the pointer from the object class
    void Free();
    // A little more work needs to be done to do a correct delete
    virtual void Delete( int Index );

    // Get the Sound ID
    int GetID( int Index ){ return ( Count() ? PSSoundList(GetObj( Index ))->ID : 0); };
    // Get the sound Volume
    int GetVolume( int Index ){ return ( Count() ? PSSoundList(GetObj( Index ))->Volume : 0); };
    // Get the sound Channel
    int GetChannel( int Index ){ return ( Count() ? PSSoundList(GetObj( Index ))->Channel : 0); };

    // Set the Sound ID
    void SetID( int Index, int Value ){ if( Count() ) PSSoundList(GetObj( Index ))->ID = Value; };
    // Set the sound Volume
    void SetVolume( int Index, int Value ){ if( Count() ) PSSoundList(GetObj( Index ))->Volume = Value; };
    // Set the sound Channel
    void SetChannel( int Index, int Value ){ if( Count() ) PSSoundList(GetObj( Index ))->Channel = Value; };

    // Get the media player
    PTMediaPlayer GetMP( int Index ){ return ( Count() ? &PSSoundList(GetObj( Index ))->MediaPlayer : 0); };
};

typedef CSoundListAry *PCSoundListAry;


/************************************************************************
*
*                        Storyboard list object
*
************************************************************************/


/***************************************************************
*
*  TStoryBoard - Structure to handle game animation data.
*                Holds all the relevant animation data for a 
*                frame of animation.
*
****************************************************************/
struct TStoryBoard 
{
    int X;                      // X Coord
    int Y;                      // Y Coord
    int FrameIndex;             // The image index into the MNG
    int FrameRepeat;            // Number of times the same storyboard id displayed
    int ZOrder;                 // The Z order

    CSoundListAry SndLst;       // The sound list

    TStoryBoard()
    {
        X = Y = ZOrder = 0;
        FrameRepeat = 0;
        FrameIndex = 1; 
    };
};
typedef TStoryBoard * PTStoryBoard;


class TStoryList : public TObjList
{

public:
    // Common Functions

    // Constructor
    TStoryList();
    // Destructor - Put all your cleanup code here
    ~TStoryList();
    // Add a string
    virtual BOOL Add();
    // Frees the pointer from the object class
    void Free();
    // A little more work needs to be done to do a correct delete
    virtual void Delete( int Index );
    // Get the size of the data needed for the Obj class
    virtual int GetObjSize( int Index ){ return sizeof( TStoryBoard ); };

    // Get the storyboard
    PTStoryBoard GetStoryBrd( int Index ){ return PTStoryBoard(GetObj( Index )); };

    // Get the X Offset
    int GetXOffset( int Index ){ return ( Count() ? PTStoryBoard(GetObj( Index ))->X : 0); };
    // Get the Y Offset
    int GetYOffset( int Index ){ return ( Count() ? PTStoryBoard(GetObj( Index ))->Y : 0); };
    // Get the Frame Index
    int GetFrameIndex( int Index ){ return ( Count() ? PTStoryBoard(GetObj( Index ))->FrameIndex : 0); };
    // Get the Display Count
    int GetFrameRepeat( int Index ){ return ( Count() ? PTStoryBoard(GetObj( Index ))->FrameRepeat : 0); };
    // Get the Z Order
    int GetZOrder( int Index ){ return ( Count() ? PTStoryBoard(GetObj( Index ))->ZOrder : 0); };

    // Set the X Offset
    void SetXOffset( int Index, int Value ){ if( Count() ) PTStoryBoard(GetObj( Index ))->X = Value; };
    // Set the Y Offset
    void SetYOffset( int Index, int Value ){ if( Count() ) PTStoryBoard(GetObj( Index ))->Y = Value; };
    // Set the Sound Clip
    void SetFrameIndex( int Index, int Value ){ if( Count() ) PTStoryBoard(GetObj( Index ))->FrameIndex = Value; };
    // Set the Display Count
    void SetFrameRepeat( int Index, int Value ){ if( Count() ) PTStoryBoard(GetObj( Index ))->FrameRepeat = Value; };
    // Set the Z Order
    void SetZOrder( int Index, int Value ){ if( Count() ) PTStoryBoard(GetObj( Index ))->ZOrder = Value; };

    // Get the count of storylist
    PCSoundListAry GetSndLst( int Index ){ return ( Count() ? (PCSoundListAry)&PTStoryBoard(GetObj( Index ))->SndLst : 0 ); };

};

typedef TStoryList *PTStoryList;



/************************************************************************
*
*                        Storyboard array object
*
************************************************************************/


struct TStoryBoardArry
{
    char ScriptName[MAX_PATH*2];      // Name of selected Script
    char mngFileName[MAX_PATH*2];     // Name of MNG file the script points to
    int DisplayTimer;                 // Display counter timer
    int FrameCounter;                 // Counter for the animation frames
    int BaseX, BaseY, BaseZ;          // Base offsets for the whole animation
    int AnimType;                     // Animation type
    BOOL KillAnim;                    // Kill the animation flag
    int PingPongDir;                  // The direction the animation moving in a ping pong

    CMNG mng;                         // MNG Class
    TStoryList StoryList;             // The storyboard array
    TStringList soundList;            // Sound list array

    TStoryBoardArry()
    {
        BaseX = BaseY = BaseZ = 0;
        AnimType = PLAY_FORWARD_LOOP;
        KillAnim = FALSE;
		PingPongDir = 1;
    }
};
typedef TStoryBoardArry * PTStoryBoardArry;



class TStoryListArry : public TObjList
{

public:
    // Common Functions

    // Constructor
    TStoryListArry();
    // Destructor - Put all your cleanup code here
    ~TStoryListArry();
    // Add a string
    virtual BOOL Add();
    // Frees the pointer from the object class
    void Free();
    // Get the size of the data needed for the Obj class
    // Save everything but the TStoryList
    virtual int GetObjSize( int Index ){ return sizeof( TStoryBoardArry ); };
    // Save the object data to a file. We need our own for this class.
    BOOL SaveToFile( int index );
    // Save the object data to a file. We need our own for this class.
    BOOL SaveToFile( HANDLE hFile, int index, int animsDirOffset );
    // Load the object data from a file. We need our own for this class.
    BOOL LoadFromFile( char *FileName );
    // Load the object data from a file. We need our own for this class.
    BOOL LoadFromFile( HANDLE hFile, char *ScriptName );
    // A little more work needs to be done to do a correct delete
    virtual void Delete( int Index );
    // Parse the data from the loaded file
    void ParseData( char *pData, int size, char *fileName );
    // Convert the path to a unix or a dos path
    void ConvertToDosUnixPath( char *path );

    // Get pointer to the script name
    char *GetPtrToScriptName( int Index ){ return ( Count() ? PTStoryBoardArry(GetObj( Index ))->ScriptName : NULL); };
    // Get pointer to the MNG file name
    char *GetPtrToMngName( int Index ){ return ( Count() ? PTStoryBoardArry(GetObj( Index ))->mngFileName : NULL); };
    // Get pointer to MNG class
    PCMNG GetPtrToMng( int Index ){ return ( Count() ? &PTStoryBoardArry(GetObj( Index ))->mng : NULL); };

    // Get the X Offset
    int GetBaseXOffset( int Index ){ return ( Count() ? PTStoryBoardArry(GetObj( Index ))->BaseX : 0); };
    // Get the Y Offset
    int GetBaseYOffset( int Index ){ return ( Count() ? PTStoryBoardArry(GetObj( Index ))->BaseY : 0); };
    // Get the Z Offset
    int GetBaseZOffset( int Index ){ return ( Count() ? PTStoryBoardArry(GetObj( Index ))->BaseZ : 0); };
    // Get the DisplayTimer
    int GetDisplayTimer( int Index ){ return ( Count() ? PTStoryBoardArry(GetObj( Index ))->DisplayTimer : 0); };
    // Get the FrameCounter
    int GetFrameCounter( int Index ){ return ( Count() ? PTStoryBoardArry(GetObj( Index ))->FrameCounter : 0); };
    // Get the Animation Type
    int GetAnimType( int Index ){ return ( Count() ? PTStoryBoardArry(GetObj( Index ))->AnimType : 0); };
    // Get the Kill Animation Flag
    BOOL GetKillAnim( int Index ){ return ( Count() ? PTStoryBoardArry(GetObj( Index ))->KillAnim : 0); };
    // Get the Ping Pong Direction
    int GetPingPongDir( int Index ){ return ( Count() ? PTStoryBoardArry(GetObj( Index ))->PingPongDir : 0); };

    // Set the X Offset
    void SetBaseXOffset( int Index, int Value ){ if( Count() ) PTStoryBoardArry(GetObj( Index ))->BaseX = Value; };
    // Set the Y Offset
    void SetBaseYOffset( int Index, int Value ){ if( Count() ) PTStoryBoardArry(GetObj( Index ))->BaseY = Value; };
    // Set the Z Offset
    void SetBaseZOffset( int Index, int Value ){ if( Count() ) PTStoryBoardArry(GetObj( Index ))->BaseZ = Value; };
    // Set the DisplayTimer
    void SetDisplayTimer( int Index, int Value ){ if( Count() ) PTStoryBoardArry(GetObj( Index ))->DisplayTimer = Value; };
    // Set the FrameCounter
    void SetFrameCounter( int Index, int Value ){ if( Count() ) PTStoryBoardArry(GetObj( Index ))->FrameCounter = Value; };
    // Set the Animation Type
    void SetAnimType( int Index, int Value ){ if( Count() ) PTStoryBoardArry(GetObj( Index ))->AnimType = Value; };
    // Set the Kill Animation Flag
    void SetKillAnim( int Index, BOOL Value ){ if( Count() ) PTStoryBoardArry(GetObj( Index ))->KillAnim = Value; };
    // Set the Ping Pong Direction
    void SetPingPongDir( int Index, int Value ){ if( Count() ) PTStoryBoardArry(GetObj( Index ))->PingPongDir = Value; };

    // Get the storylist
    PTStoryList GetStoryList( int Index ){ return ( Count() ? (PTStoryList)&PTStoryBoardArry(GetObj( Index ))->StoryList : 0 ); };
    // Get the sound list
    PTStringList GetSoundListStr( int Index ){ return ( Count() ? (PTStringList)&PTStoryBoardArry(GetObj( Index ))->soundList : 0 ); };
};

typedef TStoryListArry * PTStoryListArry;


#endif  /* __STORYBOARD_LIST_H__ */