
/************************************************************************
*
*    PRODUCT:         Polygon Object list
*
*    FILE NAME:       TPolygonObjList.h
*
*    DESCRIPTION:     Object for holding lists of polygon objects and their
*                     animations.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __POLYGONOBJ_LIST_H__
#define __POLYGONOBJ_LIST_H__ 

#include "TObjList.h"       // Parent object class
#include "TStrList.h"       // Object file for holding lists of strings
#include "TCamLst.h"        // Camera movement list

/*******************************
*    3D Engine include files 
*******************************/

#include "TextType.h"
#include "PalShade.h"
#include "PolyObj.h"


#define MAX_ALIAS_SIZE       100
#define FUTURE_ST_DATA_SIZE  5
#define FUTURE_STA_DATA_SIZE 10


class TPolygon
{
public:
    char PolyObjFileName[MAX_PATH];    // Polygon object File Name
    char AnimFileName[MAX_PATH];       // Animation Offset File Name
    BOOL LiveCRD;                      // Indicates if the CRD file needs saving
	BOOL LivePlyObj;				   // Indicates if the Polygon Object file needs saving
    BOOL LiveTxtLibary;                // Indicates if the Texture library needs saving
    BOOL LiveObjMoveList;              // Indicates an animation has been setup
    BOOL AllowTextCtrls;               // Allow texture map controls
    int FrameCounter;                  // Frame counter for animating the movements
    BOOL Trails;                       // Trails for this object used for plotting a cource
    BOOL RotatePt;                     // Flag to indicate we are rotating from a selected point
	BOOL WireFrame;					   // Render as wire frame
	BOOL UseAsLightSource;			   // Flag used to take the objects coordinates and use them as a light souce information

    TCameraOffset PlyOffset;           // Active Polygon object movement
    TCameraOffset RotatePoint;         // Holds the point to do the rotation from
	TCameraOffset RotatePointSave;     // Holds the result of the point to do the rotation from
    TPolyObj PolyObj;                  // Polygon object class
    ImageData TextDat;                 // Bitmap texture array class
    ShadeDat TextShade;                // Shaded lookup table class
    TCameraList PolyOffsetList;        // List of Polygon object movement offsets
    TStringList TextureLst;            // List of bitmap file names of tetures

    // Init the data members on allocation
    TPolygon()
    {
        PolyObjFileName[0] = 0;
        AnimFileName[0] = 0;
        LiveCRD = FALSE;
		LivePlyObj = FALSE;
        LiveTxtLibary = FALSE;
        LiveObjMoveList = FALSE;
        AllowTextCtrls = FALSE;
        FrameCounter = 0;
        WireFrame = FALSE;
        UseAsLightSource = FALSE;  
    }
};
typedef TPolygon * PTPolygon;



class TPolygonList : public TObjList
{

public:
    // Common Functions

    // Constructor
    TPolygonList();
    // Destructor - Put all your cleanup code here
    ~TPolygonList();
    // Add a string
    virtual BOOL Add();
    // Frees the pointer from the object class
    void Free();
    // Get the size of the data needed for the Obj class
    // Save everything but the TStoryList
    virtual int GetObjSize( int Index ){ return 0; };
    // A little more work needs to be done to do a correct delete
    virtual void Delete( int Index );

    // Get pointer to the polygon object file name
    char *GetPolyObjFileName( int Index ){ return ( Count() ? PTPolygon(GetObj( Index ))->PolyObjFileName : NULL); };
    // Get pointer to the offset animation file name
    char *GetMoveFileName( int Index ){ return ( Count() ? PTPolygon(GetObj( Index ))->AnimFileName : NULL); };
    // Get pointer to the polygon object class
    PTPolyObj GetPolyObjClass( int Index ){ return ( Count() ? &PTPolygon(GetObj( Index ))->PolyObj : NULL); };
    // Get pointer to the polygon texture class
    PImageData GetTextClass( int Index ){ return ( Count() ? &PTPolygon(GetObj( Index ))->TextDat : NULL); };
    // Get pointer to the polygon shaded table class
    PShadeDat GetShadedTableClass( int Index ){ return ( Count() ? &PTPolygon(GetObj( Index ))->TextShade : NULL); };
    // Get pointer to the polygon offset animation list
    PTCameraList GetPolyOffList( int Index ){ return ( Count() ? &PTPolygon(GetObj( Index ))->PolyOffsetList : NULL); };
    // Get pointer to the active polygon offsets
    PTCameraOffset GetOffset( int Index ){ return ( Count() ? &PTPolygon(GetObj( Index ))->PlyOffset : NULL); };
    // Set the live project flag for the Texture Library
    void SetTxtLibaryLive( int Index, BOOL Value ){ if( Count() ) PTPolygon(GetObj( Index ))->LiveTxtLibary = Value; };
    // Get the state of the active Texture Library
    BOOL GetTxtLibaryLive( int Index ){ return ( Count() ? PTPolygon(GetObj( Index ))->LiveTxtLibary : 0); };
    // Set the live project flag for the coordinate file
    void SetCDRLive( int Index, BOOL Value ){ if( Count() ) PTPolygon(GetObj( Index ))->LiveCRD = Value; };
    // Get the state of the active project
    BOOL GetCDRLive( int Index ){ return ( Count() ? PTPolygon(GetObj( Index ))->LiveCRD : 0); };
    // Set the live project flag for the coordinate file
    void SetPlyObjLive( int Index, BOOL Value ){ if( Count() ) PTPolygon(GetObj( Index ))->LivePlyObj = Value; };
    // Get the state of the active project
    BOOL GetPlyObjLive( int Index ){ return ( Count() ? PTPolygon(GetObj( Index ))->LivePlyObj : 0); };
    // Set the live project flag for the coordinate file
    void SetLiveObjMove( int Index, BOOL Value ){ if( Count() ) PTPolygon(GetObj( Index ))->LiveObjMoveList = Value; };
    // Get the state of the active project
    BOOL GetLiveObjMove( int Index ){ return ( Count() ? PTPolygon(GetObj( Index ))->LiveObjMoveList : 0); };
    // Set the flag to allow texture work
    void SetAllowTextCtrls( int Index, BOOL Value ){ if( Count() ) PTPolygon(GetObj( Index ))->AllowTextCtrls = Value; };
    // Get the flag to allow texture work
    BOOL GetAllowTextCtrls( int Index ){ return ( Count() ? PTPolygon(GetObj( Index ))->AllowTextCtrls : 0); };
    // Get pointer to the polygon object class
    PTStringList GetTextStrList( int Index ){ return ( Count() ? &PTPolygon(GetObj( Index ))->TextureLst : NULL); };
    // Set the frame counter
    void SetFrameCounter( int Index, int Value ){ if( Count() ) PTPolygon(GetObj( Index ))->FrameCounter = Value; };
    // Get the frame counter
    int GetFrameCounter( int Index ){ return ( Count() ? PTPolygon(GetObj( Index ))->FrameCounter : 0); };
    // Set the trails
    void SetTrails( int Index, BOOL Value ){ if( Count() ) PTPolygon(GetObj( Index ))->Trails = Value; };
    // Get the state of the trails
    BOOL GetTrails( int Index ){ return ( Count() ? PTPolygon(GetObj( Index ))->Trails : 0); };
    // Get pointer to the polygon offsets to rotate from
    PTCameraOffset GetRotatePt( int Index ){ return ( Count() ? &PTPolygon(GetObj( Index ))->RotatePoint : NULL); };
    // Get pointer to the polygon offsets to rotate from
    PTCameraOffset GetRotatePtSave( int Index ){ return ( Count() ? &PTPolygon(GetObj( Index ))->RotatePointSave : NULL); };
    // Set the rotate from point
    void SetRotatePoint( int Index, BOOL Value ){ if( Count() ) PTPolygon(GetObj( Index ))->RotatePt = Value; };
    // Get the rotate from point
    BOOL GetRotatePoint( int Index ){ return ( Count() ? PTPolygon(GetObj( Index ))->RotatePt : 0); };
    // Set the wire frame
    void SetWireFrame( int Index, BOOL Value ){ if( Count() ) PTPolygon(GetObj( Index ))->WireFrame = Value; };
    // Get the state of the wire frame
    BOOL GetWireFrame( int Index ){ return ( Count() ? PTPolygon(GetObj( Index ))->WireFrame : 0); };
    // Set the light source
    void SetUseAsLightSource( int Index, BOOL Value ){ if( Count() ) PTPolygon(GetObj( Index ))->UseAsLightSource = Value; };
    // Get the state of the light source
    BOOL GetUseAsLightSource( int Index ){ return ( Count() ? PTPolygon(GetObj( Index ))->UseAsLightSource : 0); };
};

typedef TPolygonList *PTPolygonList;


#endif  /* __POLYGONOBJ_LIST_H__ */