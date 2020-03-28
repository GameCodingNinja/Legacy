
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
#include "CObjList.h"        // Header file for the template list object.

#define MAX_LINE                200

// Make a quick string list class for file paths
typedef CObjLst<char> CTextStrLst;

//////////////////////////////////
// Simple vertex class list for  
// holding parsed data
//////////////////////////////////

typedef CObjLst<CVertLoad> CVertLst;
typedef CVertLoad *PCVertice;

//////////////////////////////////
// Simple UV class list for 
// holding parsed data
//////////////////////////////////

typedef CObjLst<CUVLoad> CUVLst;
typedef CUVLoad *PCUV;

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
        radius = (float) sqrt( maxDistance );

    }   // Radus
};

typedef CObjLst<CFace> CFaceLst;
typedef CFace *PCFace;

//////////////////////////////////
// Triangles are put into groups.
// What determins a group is
// the texture being used
//////////////////////////////////
class CGroup
{
public:
    int objectType;
    int objectID;
    int mapData[MAX_MAP_DATA];
    CVertLst vertLst;
    CFaceLst faceLst;
    CUVLst uvLst;
    CTextStrLst textLst;

    CGroup()
    {
        objectType = 0;
        objectID = 50;
        mapData[0] = -1;
        mapData[1] = -1;
    }
};

typedef CObjLst<CGroup> CGroupLst;
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
    // Collision type
    int collisionType;
    // Map type counters
    int typeCounter[EOT_MAX_TYPES];

    // Add a new group to the list
    bool AddGroup( CGroupLst &groupLst );
    // decode the string of vertices
    bool Decode_Vertice( CGroupLst &groupLst, char *strData, int strCount );
    // decode the string of UV values
    bool Decode_UV( CGroupLst &groupLst, char *strData, int strCount );
    // Remove all the duplicate verticies from the vertice list
    bool RemoveDupVerticies( CGroupLst &groupLst );
	bool RemoveDupVerticies( CVertLst &vertLst );
    // Add a new face to the group.
    bool AddFaceToGroup( CGroupLst &groupLst, char *strData, int strCount );
    // Decode the string of face
    bool Decode_Face( CGroupLst &groupLst, char *strData, int strCount );
    // Add the material name to the group
    bool AddMateralNameToGroup( CGroupLst &groupLst, char *strData, int strCount );
    // Search a string and replace a character with a different character
    void RelpaceChar( char *charString, char find, char replace, int strCount );
    // Match texture index to face
    bool MatchTextureIndexToFace( CGroupLst &groupLst, CTextStrLst &textLst );
    // Build the texture library file
    bool BuildTextureLibrary( CTextureLib &textLib, CTextStrLst &textLst, char *filePath );
    // Reads a line of text from a text file
    int GetLine( FILE *inFile, char *str );
    // Match vertices to global list
    bool MatchVertsToGlobalList( CVertLst &vertLst, CFaceLst &faceLst );
    // Match vertices to local list
    bool MatchVertsToLocalList( CGroupLst &groupLst );
    // Save polygon object data to file
    bool SaveToFile_Condenced( CVertLst &vertLst, CFaceLst &faceLst, char *fileName );
    // Save polygon object data to file
    bool SaveToFile_Seperated( CGroupLst &groupLst, char *fileName );
    // Strip the file name from the path and add new file name
    void CropOutPathAddFileName( char *sourcePath, char *destPath, char *newFileName );
    // Build a map file to show the data organization
    bool BuildMapFile( CGroupLst &groupLst, CTextStrLst &textLst, 
                       CTextureLib &textLib, char *filePath );
    // Strip the file name from the file path
    void CropFileNameFromPath( char *SourceFilePath, char *DestFileName, bool KeepExtension = true );
    // Sort all the polygons by their Z order
    void SortByTextIndex( CFaceLst &faceLst );
    // Generate the normal file
    bool GenerateNormals( char *fileName );
    // Decode the matrix
    void Decode_TransMatrix( char *strData, int strCount );
	// Decode the matrix
    void Decode_CenterMatrix( char *strData, int strCount );
    // Decode the center
    void Decode_Center( FILE *hFile, char *strData, int strCount );
    // Create a global texture list
    void CreateTextureList( CGroupLst &groupLst, CTextStrLst &textLst );
    // Decode the object name
    void Decode_ObjectName( char *strData, int strCount );
    // Sort the objects by object name
    //void SortByObjectName( CGroupLst &groupLst );
    // Make a global vert list
    bool MakeGlobalVertLst( CGroupLst &groupLst, CVertLst &vertLst );
    // Make a global face list
    bool MakeGlobalFaceLst( CGroupLst &groupLst, CFaceLst &faceLst );
    // Decode the collision type
    bool Decode_CollisionType( CGroupLst &groupLst, char *strData, int strCount );
    // Match up all the doors with ajoining rooms
    void MatchDoorsToRooms( CGroupLst &groupLst );
	// Match up all the objects with rooms
	void MatchObjectsToRooms( CGroupLst &groupLst );
	// Objects of no specific type are to be added to the
    // room mesh they are associated with.
	bool CombineNoTypeObjectsToRoomMesh( CGroupLst &groupLst );
    // Check to see if faces collide
    bool DoFacesCollide( PCFace pFace1, PCFace pFace2 );

public:
    CTrueSpaceParser();
    ~CTrueSpaceParser();

    // Load OBJ text file and parse it
    bool LoadFromFile( char *filePath );
    // get the error code
    int GetErrorCode(){ return errorCode; };
    // get the error file path code
    char *GetErrorFilePath(){ return errorFilePath; };
};


#endif  // __TRUESPACE_FILE_PARSER_H__