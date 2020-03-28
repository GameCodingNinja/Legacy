
/************************************************************************
*
*    FILE NAME:       CMSAnimParser.h
*
*    DESCRIPTION:     Parse 3D ascii files in to an animation lis
*
*    IMPLEMENTATION:  Load in an MilkShape file and parse out the data.
*
************************************************************************/

#ifndef __MS_ANIM_FILE_PARSER_H__
#define __MS_ANIM_FILE_PARSER_H__

#include "CObjList.h"        // Header file for the object list
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include "CTextLib.h"        // Texture library class
#include "CAnimMesh.h"       // Animated mesh class
#include "CMeshList.h"       // mesh list class

// Ascii characters
#define QUOTE_CHR                0x22


enum EPM_ParseState
{
    EPM_NONE=0,
    EMP_MESH,
    EMP_MATERIALS,
};

#define VERT_NULL_COUNT           6
#define FACE_NULL_COUNT           7
#define MAX_POSSIBLE_TEXTURES     256
#define MAX_POSSIBLE_VERTICES     0x7FFF

#pragma pack(1)

//////////////////////////////////
// Simple vertex class for holding 
// parsed data
//////////////////////////////////
class CVertice : public CVertLoad
{
public:
    // U, V assumed
    CUVLoad crd;
};

typedef CObjLst<CVertice> CVertLst;
typedef CVertice *PCVertice;


//////////////////////////////////
// Face class for holding parsed
// information by face (triangle)
//////////////////////////////////
class CFace : public CAnimFaceLoad
{
public:
    // x, y, z is assumed
    CVertice point[3];
};

#pragma pack()

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
    // List for this group
    CVertLst vertLst;
    CFaceLst faceList;
    int textureIndex;
    char textStr[100];

    CGroup()
    {
        textureIndex = -1;
    }
};

typedef CObjLst<CGroup> CGroupLst;
typedef CGroup *PCGroup;


/////////////////////////////
// Frame List Class
/////////////////////////////

class CFrame
{
public:
    // Combination of all verticies
    CVertLst gVertLst;
    // The groups used to make the global lists
    CGroupLst groupLst;
    // local texture list
    CObjLst<char> textStrLst;
    
    CFrame()
    {
        // Don't have the global lists free data
        // because we are reusing pointers
        gVertLst.DontFreeData();
    }   
};

typedef CObjLst<CFrame> CFrameLst;
typedef CFrame *PCFrame;


/////////////////////////////
// Animation List Class
/////////////////////////////

class CAnim
{
public:
    CFrameLst frameLst;
    char animNameStr[50];
};

typedef CObjLst<CAnim> CAnimLst;
typedef CAnim *PCAnim;


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
    ERROR_CAL_NORMALS_FILE,
    ERROR_SAVE_NORMALS_FILE,
    ERROR_VERTEX_OUT_OF_RANGE,
    ERROR_TOO_MANY_TEXTURES,
    ERROR_TOO_MANY_VERTEXES,
    ERROR_FRAME_VERTEX_COUNT_MISMATCH,
    ERROR_FRAME_FACE_COUNT_MISMATCH,
};


///////////////////////////////////
// MilkShape ASCII Parse Class
///////////////////////////////////
class CMSBjParser
{
protected:
    // Holds the error code
    int errorCode;
    // Holds the external file path responsible for error
    char errorFilePath[500];
    // file name
    char fileName[300];
    // The parse state
    EPM_ParseState parseState;
    // Texture library class
    CTextureLib textLib;
    // Object classes
    CAnimLst animLst;
    // Texture string list
    CObjLst<char> textStrLst;
    // Running indexes
    int animIndex;
    int frmIndex;
    int grpIndex;

    // Strip the file name from the path and add new file name
    void CropOutPathAddFileName( char *sourcePath, char *destPath, char *newFileName );
    // Decode the vertices, UV and bone index
    bool Decode_VerticeUV( char *strData, int strCount );
    // Add a new group to the list
    bool Add_Group( char *strData, int strCount );
    // decode the string of face values
    bool Decode_Faces( char *strData, int strCount );
    // Remove all the duplicate verticies from the vertice list
    bool RemoveDupVerticies();
    // Match up all the vertices in the faces to the verstices in the list.
    bool MatchVerticies();
    // Get the texture name and add it to the list
    bool Decode_TextureName( char *strData, int strCount );
    // Reads a line of text from a text file
    int GetLine( FILE *inFile, char *str, int &nullCount );

public:
    CMSBjParser();
    ~CMSBjParser();

    // Decode Verts, Faces, UV, textures and bones
    bool LoadFromFile( char *filePath );
    // get the error code
    int GetErrorCode(){ return errorCode; };
    void SetErrorCode( int value ){ errorCode = value; };
    // get the error file path code
    char *GetErrorFilePath(){ return errorFilePath; };
    // Save polygon object data to file
    bool SaveToFile();
    // Build a map file to show the data organization
    bool BuildMapFile();
    // Generate the normal file
    bool GenerateNormalFile();
    // Build the texture library file
    bool BuildTextureLibrary();
    // Match the group texture to the global texture
    bool MatchGroupTextToGlobalText();
    // Final error checks
    bool DoFinalErrorChecks();
};


#endif  // __MS_ANIM_FILE_PARSER_H__