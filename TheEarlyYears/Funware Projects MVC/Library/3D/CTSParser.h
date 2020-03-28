
/************************************************************************
*
*    FILE NAME:       CTSParser.h
*
*    DESCRIPTION:     Parse True Space 3D files
*
************************************************************************/

#ifndef __TRUESPACE_FILE_PARSER_H__
#define __TRUESPACE_FILE_PARSER_H__

#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include "CTextLib.h"        // Texture library class
#include "CMeshMap.h"        // Map specific mesh class
#include "types.h"           // Object & Collision type specifiers
#include <vector>
#include <string>
#include <algorithm>
#include "deletefuncs.h"

// uncomment to generate the ply file
//#define GENERATE_PLY_FILE

#define MAX_LINE                200
#define VERT_NORM_RANGE         0.3f

typedef unsigned int uint;
// Make a quick string list class for file paths
typedef std::vector<std::string> CTextStrLst;

//////////////////////////////////
// Simple vertex class list for  
// holding parsed data
//////////////////////////////////

typedef std::vector<CVertLoad *> CVertLst;
typedef CVertLoad *PCVertice;

//////////////////////////////////
// Simple UV class list for 
// holding parsed data
//////////////////////////////////

typedef std::vector<CUVLoad *> CUVLst;
typedef CUVLoad *PCUV;

#pragma pack(1)

//////////////////////////////////
// Face class for holding parsed
// information by face (triangle)
//////////////////////////////////
class CFace : public CFaceLoad
{
public:
    // x, y, z is assumed
    CVertLoad point[QUAD];
    // radus
    float radius;
    // offset point
    CPoint offsetPt;
	// Normal
	CPoint normal;

    CFace()
    { 
        radius = 0.0;
    }

    void Radius()
    {
        CPoint centerPt;
        long double curDistance;
        long double maxDistance = 0.0;
        

        // Average out each vertex
        for( int i = 0; i < vertCount; ++i )
        {
            offsetPt.x += point[i].vert[0];
            offsetPt.y += point[i].vert[1];
            offsetPt.z += point[i].vert[2];
        }

        // Multiplication is faster then division.
        offsetPt.x /= vertCount;
        offsetPt.y /= vertCount;
        offsetPt.z /= vertCount;

        // Find the largest distance
        for( int i = 0; i < vertCount; ++i )
        {
            // Calculate a new position where the center is 0,0,0
            centerPt.x = point[i].vert[0] - offsetPt.x;
            centerPt.y = point[i].vert[1] - offsetPt.y;
            centerPt.z = point[i].vert[2] - offsetPt.z;
            
            // Add the vertex
            curDistance = (centerPt.x * centerPt.x) +
                          (centerPt.y * centerPt.y) +
                          (centerPt.z * centerPt.z);

            // Have we found the longest distance?
            if( curDistance > maxDistance )
                maxDistance = curDistance;
        }

        // compute the radius by getting the vector length
		// The radious is used for collision detection to associate objects to rooms
		// so only half the radius is used to narrow the search.
        radius = (float) sqrt( maxDistance );

    }   // Radus

	/************************************************************************
	*    DESCRIPTION:  Calculate the Surface Normal that is then
	*                  normalized. This is the normal of the polygon.
	*                  This function assumes all vertex points are unique.
	************************************************************************/
	void CalcSurfaceNormal()
	{
	    double X1( point[ 0 ].vert[0] );
	    double Y1( point[ 0 ].vert[1] );
	    double Z1( point[ 0 ].vert[2] );

	    double X2( point[ 1 ].vert[0] );
	    double Y2( point[ 1 ].vert[1] );
	    double Z2( point[ 1 ].vert[2] );

	    double X3( point[ 2 ].vert[0] );
	    double Y3( point[ 2 ].vert[1] );
	    double Z3( point[ 2 ].vert[2] );

	    // Calculate the cross product
	    double x( Y1 * ( Z2 - Z3 ) + Y2 * ( Z3 - Z1 ) + Y3 * ( Z1 - Z2 ) );
	    double y( Z1 * ( X2 - X3 ) + Z2 * ( X3 - X1 ) + Z3 * ( X1 - X2 ) );
	    double z( X1 * ( Y2 - Y3 ) + X2 * ( Y3 - Y1 ) + X3 * ( Y1 - Y2 ) );

	    // Calculate the length of the vector
	    double length( sqrt( (x*x) + (y*y) + (z*z) ) );

	    // length can't be zero otherwise you'll get a divided by 0 error.

	    // The normalizing part is the (n.x / length). Notice we are also
	    // adding in the point (pVPoint[ vIndex ]->x). What this does is
	    // allow you to translate the normal like a point. A true normalized
	    // value has the value of -1 to 1. After we translate our new type
	    // of normal we just subtract it from the same point to get our
	    // true normal value back (pNormal->tx - pVPoint[ 0 ]->tx). Cool hu?
	    if( length != 0.0 )
	    {
	        // Normalize the normal. Sounds silly hu?
	        normal.tx = normal.x = (float)( x / length );
	        normal.ty = normal.y = (float)( y / length );
	        normal.tz = normal.z = (float)( z / length );
	    }

	}   // CalcSurfaceNormal
};

#pragma pack()

typedef std::vector<CFace *> CFaceLst;
typedef CFace *PCFace;

//////////////////////////////////
// Triangles are put into groups.
// What determins a group is
// the texture being used
//////////////////////////////////
class CGroup
{
public:
    EOT_OBJECT_TYPE objectType;	 // object type
    int objectID;				 // array index
    EMD_MAP_DATA mapData[EMD_MAX_MAP_DATA];
    CVertLst vertLst;
    CFaceLst faceLst;
    CUVLst uvLst;
    std::map<int, std::string> textStrMap;
	float radius;
	CPoint centerPt;

    CGroup()
    {
        objectType = (EOT_OBJECT_TYPE)0;
        objectID = 50;
        mapData[EMD_ROOM_1] = (EMD_MAP_DATA)-1;
        mapData[EMD_ROOM_2] = (EMD_MAP_DATA)-1;
		mapData[EMD_MAP_ID] = (EMD_MAP_DATA)-1;
    }

	~CGroup()
	{
		DeleteVectorPointers( vertLst );
		DeleteVectorPointers( faceLst );
		DeleteVectorPointers( uvLst );
	}

    /************************************************************************
	*    DESCRIPTION:  Calculate a bounding sphere & rect around this object.
	*                  Handy for full object culling against the 
	*                  view frustum.
	************************************************************************/
	void CalBoundingSphere()
	{
	    long double x(0.0), y(0.0), z(0.0);
		long double inverseVertCount = 1.0 / (float)vertLst.size();
		double maxDistance = 0.0;
	    double maxDistanceX = 0.0;
	    double maxDistanceY = 0.0;
	    double curDistance;
	    CPoint pt;
	    CPoint closeDist, maxDist, dist;

	    // Average out each vertex
	    for( uint i = 0; i < vertLst.size(); ++i )
		{
			x += vertLst[i]->vert[0];
			y += vertLst[i]->vert[1];
			z += vertLst[i]->vert[2];
		}

	    // Record the center point
	    centerPt.x = (float)(x * inverseVertCount);
	    centerPt.y = (float)(y * inverseVertCount);
	    centerPt.z = (float)(z * inverseVertCount);

	    // init
	    closeDist.x = vertLst[0]->vert[0];
	    maxDist.x = vertLst[0]->vert[0];
	    closeDist.y = vertLst[0]->vert[1];
	    maxDist.y = vertLst[0]->vert[1];
	    closeDist.z = vertLst[0]->vert[2];
	    maxDist.z = vertLst[0]->vert[2];

	    // Find the closest and farthest distances
	    // for each axis.
	    for( uint i = 1; i < vertLst.size(); ++i )
	    {
	        if( vertLst[i]->vert[0] < closeDist.x )
	            closeDist.x = vertLst[i]->vert[0];

	        else if( vertLst[i]->vert[0] > maxDist.x )
	            maxDist.x = vertLst[i]->vert[0];

	        if( vertLst[i]->vert[1] < closeDist.y )
	            closeDist.y = vertLst[i]->vert[1];

	        else if( vertLst[i]->vert[1] > maxDist.y )
	            maxDist.y = vertLst[i]->vert[1];

	        if( vertLst[i]->vert[2] < closeDist.z )
	            closeDist.z = vertLst[i]->vert[2];

	        else if( vertLst[i]->vert[2] > maxDist.z )
	            maxDist.z = vertLst[i]->vert[2];
	    }
	    
	    // Reguardless of where the object is,
	    // we need to calculate the radius as if the
	    // object's center was at 0,0,0
	    for( uint i = 0; i < vertLst.size(); ++i )
	    {
	        // Calculate a new position where the center is 0,0,0
	        pt.x = vertLst[i]->vert[0] - centerPt.x;
	        pt.y = vertLst[i]->vert[1] - centerPt.y;
	        pt.z = vertLst[i]->vert[2] - centerPt.z;
	        
	        // Add the vertex
	        curDistance = (pt.x * pt.x) +
	                      (pt.y * pt.y) +
	                      (pt.z * pt.z);

	        // Have we found the longest distance?
	        if( curDistance > maxDistance )
	            maxDistance = curDistance;

	        curDistance = (pt.x * pt.x) + (pt.z * pt.z);

	        if( curDistance > maxDistanceX )
	            maxDistanceX = curDistance;

	        curDistance = (pt.y * pt.y) + (pt.z * pt.z);

	        if( curDistance > maxDistanceY )
	            maxDistanceY = curDistance;
	    }

	    // computer the radius
	    radius = (float)sqrt( maxDistance );

	}   // CalBoundingSphere


	/************************************************************************
	*    DESCRIPTION:  Check for collision. A bounding sphere for
	*                  the mesh is used to check for collision
	*                  against the supplied mesh.
	*
	*    Input:    CMesh &mesh - mesh to check for collion
	*
	*    Output:   bool - true on success, false on fail
	************************************************************************/
	bool Collide_BoundSphere( CGroup * pGrp )
	{
	    double dist;
	    bool result = false;

	    // Calculate the distance between the center points of both objects
	    dist = sqrt( (( centerPt.x - pGrp->centerPt.x ) * ( centerPt.x - pGrp->centerPt.x )) +
	                 (( centerPt.y - pGrp->centerPt.y ) * ( centerPt.y - pGrp->centerPt.y )) +
	                 (( centerPt.z - pGrp->centerPt.z ) * ( centerPt.z - pGrp->centerPt.z )) );

	    // Check for possible collision by using bounding sphere
	    if( dist <= pGrp->radius + radius )
	    {
	        result = true;
	    }
	    
	    return result;

	}   // Collide_BoundSphere
};

typedef std::vector<CGroup *> CGroupLst;
typedef CGroup *PCGroup;


// Error list
enum
{
    NO_ERROR_DETECTED = 0,
    ERROR_ALLOCATION_FAIL,
    ERROR_OBJ_FILE_HANDLE_FAIL,
    ERROR_PLY_FILE_HANDLE_FAIL,
    ERROR_H_FILE_HANDLE_FAIL,
    ERROR_TXT_LIB_FILE_HANDLE_FAIL,
    ERROR_MAP_FILE_HANDLE_FAIL,
    ERROR_DATA_OUT_OF_RANGE,
    ERROR_EMPTY_GROUP,
    ERROR_MATCH_NOT_FOUND_VERTICES,
    ERROR_MATCH_NOT_FOUND_TEXTURE,
    ERROR_FILE_WRITE,
    ERROR_TEXT_LIB_ALLOC,
    ERROR_LOAD_TEXTURE,
    ERROR_CONVERT_TEXTLIB_TO_16,
    ERROR_CAL_NORMALS_FILE,
    ERROR_SAVE_NORMALS_FILE,
    ERROR_VERTEX_OUT_OF_RANGE,
	ERROR_OBJ_CENTER_AXIS_USED,
	ERROR_DOOR_NOT_ATTACHED_TO_ROOM
};

enum EPS_ParseState
{
    EPS_NO_STATE=0,
    EPS_TRANSFORM,
	EPS_CENTER,
    EPS_VERTEX,
    EPS_UV,
    EPS_FACE,
    EPS_COLLISION_TYPE, 
};


///////////////////////////////////
// WaveFront OBJ ASCII Parse Class
///////////////////////////////////
class CTrueSpaceParser
{
protected:
    // Holds the error code
    int errorCode;
    // Holds the external file path responsible for error
    char errorFilePath[500];
    //parse state
    EPS_ParseState state;
    // transform matrix
    float transMat[16];
	// center matrix
    float centerMat[9];
    // matrix index counter
    int matIndex;
    // Center point
    CPoint centerPoint;
    // Object type
    EOT_OBJECT_TYPE objectType;
	// map ID
	int mapID;
    // Collision type
    int collisionType;
    // Map type counters
    int typeCounter[EOT_MAX_TYPES];
	// Floor counter
	int floorCounter;
	// Material index
	int maternialIndex;

	CTextureLib textLib;
    CGroupLst groupLst;    
	CTextStrLst textStrLst;
	CFaceLst faceLst;
    CVertLst vertLst;    

    // Add a new group to the list
    bool AddGroup();
    // decode the string of vertices
    bool Decode_Vertice( char *strData, int strCount );
    // decode the string of UV values
    bool Decode_UV( char *strData, int strCount );
    // Remove all the duplicate verticies from the vertice list
    bool RemoveDupVerticies();
	bool RemoveDupVerticies( CVertLst &vertLst );
    // Add a new face to the group.
    bool AddFaceToGroup( char *strData, int strCount );
    // Decode the string of face
    bool Decode_Face( char *strData, int strCount );
    // Add the material name to the group
    bool AddMateralNameToGroup( char *strData, int strCount );
    // Search a string and replace a character with a different character
    void RelpaceChar( char *charString, char find, char replace, int strCount );
    // Match texture index to face
    bool MatchTextureIndexToFace();
    // Build the texture library file
    bool BuildTextureLibrary( char *filePath );
    // Reads a line of text from a text file
    int GetLine( FILE *inFile, char *str );
    // Match vertices to global list
    bool MatchVertsToGlobalList();
    // Match vertices to local list
    bool MatchVertsToLocalList();
    // Save polygon object data to file
    bool SaveToFile_PLY(char *fileName );
    // Save polygon object data to file
    bool SaveToFile_MAP( char *fileName );
    // Strip the file name from the path and add new file name
    void CropOutPathAddFileName( char *sourcePath, char *destPath, char *newFileName );
    // Build a map file to show the data organization
    bool BuildMapFile( char *filePath );
    // Strip the file name from the file path
    void CropFileNameFromPath( char *SourceFilePath, char *DestFileName, bool KeepExtension = true );
    // Sort all the polygons by their Z order
    void SortByTextIndex( CFaceLst &faceLst );
    // Generate the normal file
    bool GenerateNormals_PLY( char *fileName );
	bool GenerateNormals_MAP( char *fileName );
    // Decode the matrix
    void Decode_TransMatrix( char *strData, int strCount );
	// Decode the matrix
    void Decode_CenterMatrix( char *strData, int strCount );
    // Decode the center
    void Decode_Center( FILE *hFile, char *strData, int strCount );
    // Create a global texture list
    void CreateTextureList();
    // Decode the object name
    void Decode_ObjectName( char *strData, int strCount );
    // Sort the objects by object name
    //void SortByObjectName( CGroupLst &groupLst );
    // Make a global vert list
    bool MakeGlobalVertLst();
    // Make a global face list
    bool MakeGlobalFaceLst();
    // Decode the collision type
    bool Decode_CollisionType( char *strData, int strCount );
    // Match up all the doors with ajoining rooms
    bool MatchDoorsToRooms();
	// Match up all the objects with rooms
	void MatchObjectsToRooms();
	// Objects of no specific type are to be added to the
    // room mesh they are associated with.
	bool CombineNoTypeObjectsToRoomMesh(PCGroup pObjectGroup,
                                        PCGroup pRoomGroup);
    // Check to see if faces collide
    bool DoFacesCollide( PCFace pFace1, PCFace pFace2 );
	// Set the collision types
	void SetWallFloorCollisionType();
	// Calculate the bounding sphere for each oject
	void CalBoundingSphere();

public:
    CTrueSpaceParser();
    ~CTrueSpaceParser();

    // Load OBJ text file and parse it
    bool LoadFromFile( const char *filePath );
	// Build the map
	bool BuildMap( const char *filePath );
    // get the error code
    int GetErrorCode(){ return errorCode; };
    // get the error file path code
    char *GetErrorFilePath(){ return errorFilePath; };
};


#endif  // __TRUESPACE_FILE_PARSER_H__