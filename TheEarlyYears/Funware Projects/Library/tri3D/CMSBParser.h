
/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CMSBParser.h
*
*    DESCRIPTION:     Parse 3D OBJ files
*
*    IMPLEMENTATION:  Load in an MilkShape file and parse out the data.
*                     If there is a texture library to be made, there is a
*                     MLT file listed in the OBJ file.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __OBJ_FILE_PARSER_H__
#define __OBJ_FILE_PARSER_H__

#include "CObjListLt.h"        // Header file for the object list
#include <stdlib.h>          // Declares several commonly used routines such as atio, random, abs, malloc etc.
#include <stdio.h>           // Defines types and macros needed for the standard I/O. Needed for vprintf
#include "CTextLib.h"        // Texture library class


// Ascii characters
#define QUOTE_CHR                0x22


enum EPM_ParseState
{
    EPM_NONE=0,
    EMP_MESH,
    EMP_MATERIALS,
    EMP_BONES,
};

#define VERT_NULL_COUNT           6
#define FACE_NULL_COUNT           7
#define BONE_OFFSET_NULL_COUNT    6
#define VECTOR_NULL_COUNT         3


//////////////////////////////////
// Simple UV class for holding
// parsed data
//////////////////////////////////
class CUV
{
public:
    // U, V assumed
    float uv[2];

    CUV(){ uv[0]=0.0; uv[1]=0.0; }
};

typedef CUV *PCUV;


//////////////////////////////////
// Simple vertex class for holding 
// parsed data
//////////////////////////////////
class CVertice
{
public:
    // x, y, z is assumed
    float vert[3];
    // Index to bone
    int boneIndex;
    // U, V assumed
    CUV crd;
    

    CVertice(){ vert[0]=0.0; vert[1]=0.0; vert[2]=0.0; boneIndex=0; }
};

typedef CVertice *PCVertice;


//////////////////////////////////
// Face class for holding parsed
// information by face (triangle)
//////////////////////////////////
class CFace
{
public:
    // index into main vertice list
    int vIndex[3];
    // U, V assumed
    CUV crd[3];
    // texture index
    int tIndex;

    // x, y, z is assumed
    CVertice point[3];

    CFace(){ tIndex = 0; }

    float CalZCenter()
    { 
        return (point[0].vert[2] + point[1].vert[2] + point[2].vert[2]) / 3;
    }
};

typedef CFace *PCFace;

//////////////////////////////////
// Triangles are put into groups.
// What determins a group is
// the texture being used
//////////////////////////////////
class CGroup
{
public:
    CObjList vertLst;
    int textureIndex;
    char groupName[100];
    char materialName[100];

    CGroup()
    {
        groupName[0] = 0;
        textureIndex = 0;
        materialName[0] = 0;
    }
};

typedef CGroup *PCGroup;


class CVector
{
public:

    float time;
    float vec[3];

    CVector()
    {
        time = 0.0;
    }
};

typedef CVector *PCVector;


//////////////////////////////////
//  Bones
//////////////////////////////////
class CBones
{
public:
    char boneName[100];
    char parentName[100];
    int parentIndex;
    
    CBones()
    {
        boneName[0] = 0;
        parentName[0] = 0;
        parentIndex = -1;
    }
};

typedef CBones *PCBones;


//////////////////////////////////
//  Bones Animation
//////////////////////////////////
class CBonesAnim
{
public:
    char boneName[100];
	char parentName[100];
    int boneIndex;
	int parentIndex;

    // initial offsets
    CVector intTran;
    CVector intRot;

    // translation/rotation list
    CObjList transLst;
    CObjList rotLst;
    
    CBonesAnim()
    {
        boneName[0] = 0;
		parentName[0] = 0;
        boneIndex = -1;
		parentIndex = -1;
    }
};

typedef CBonesAnim *PCBonesAnim;


//////////////////////////////////
//  Bones Animation list
//////////////////////////////////
class CAnim
{
public:
    char animName[100];
    int frameCount;
    int currentFrame;
    CObjList boneAnimLst;

    CAnim()
    {
        animName[0] = 0;
        frameCount = 0;
        currentFrame = 0;
    }
};
typedef CAnim *PCAnim;


//////////////////////////////////
// Simple class for holding parsed
// texture library data
//////////////////////////////////
class CTextLibrary
{
public:
    char materialName[200];
    char txtFileName[200];
    bool duplicate;
    bool used;

    CTextLibrary()
    {
        duplicate = used = false;
        materialName[0] = 0;
        txtFileName[0] = 0; 
    }
};

typedef CTextLibrary *PCTextLibrary;

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
    ERROR_SAVE_NO_ANIMATION_FRAMES,
    ERROR_SAVE_INVALID_CURRENT_FRAME_NO,
    ERROR_NO_BONE_INDEX_NOT_FOUND,
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
    // The parse state
    EPM_ParseState parseState;
    // Texture ligrary class
    CTextureLib textLib;
    // Object classes
    CObjList groupLst, vertLst, faceLst, textLst, boneLst, animLst;
    // first file name
    char fileName[400];

    // Strip the ".ext" of the end of a file path or
    // file and replace with different extension
    void RelpaceFileExtension( char *filePath, char *Ext );
    // Strip the file name from the path and add new file name
    void CropOutPathAddFileName( char *sourcePath, char *destPath, char *newFileName );

    // Strip the file name from the file path
    void CropFileNameFromPath( char *SourceFilePath, char *DestFileName, bool KeepExtension = true );
    // Search a string and replace a character with a different character
    void RelpaceChar( char *charString, char find, char replace );
    

    // Get the number of frames in this animation
    bool Decode_FrameCount( char *strData, int strCount, int &frameCount );
    // Get the current frame
    bool Decode_CurrentFrame( char *strData, int strCount, int &currentFrame, int &frameCount );
    // Decode the vertices, UV and bone index
    bool Decode_VerticeUV( CObjList &groupVertLst, CObjList &vertLst, char *strData, int strCount );
    // Add a new group to the list
    bool Add_Group( CObjList &groupLst, char *strData, int strCount );
    // decode the string of face values
    bool Decode_Faces( CObjList &vertLst, int textureIndex, CObjList &faceLst, char *strData, int strCount );
    // Remove all the duplicate verticies from the vertice list
    bool RemoveDupVerticies( CObjList &vertLst );
    // Match up all the vertices in the faces to the verstices in the list.
    bool MatchVerticies( CObjList &vertLst, CObjList &faceLst );
    // Get the texture name and add it to the list
    bool Decode_TextureName( CObjList &textLst, char *strData, int strCount );
    // Build the texture library file
    bool BuildTextureLibrary( CTextureLib &textLib, CObjList &textLst, char *filePath );
    // Convert the non-specific coordinates to coordinates specific to the library textures
    void ConvertUVtoSpecificCoordinates( CTextureLib &textLib, CObjList &faceLst );
    // Reads a line of text from a text file
    int GetLine( FILE *inFile, char *str, int &nullCount );
    // Add a new bone to the list
    bool Add_Bone( FILE *hFile, CObjList &boneLst, char *strData, int strCount );
    // Add bone animation
    bool Add_BoneAnim( FILE *hFile, CObjList &boneAnimLst, char *strData, int strCount );
    // Decode the animation offset
    bool Decode_Animation( FILE *hFile, CObjList &boneAnimLst, char *strData, int strCount );

public:
    CMSBjParser();
    ~CMSBjParser();

    // Decode Verts, Faces, UV, textures and bones
    bool Decode_VertsFacesUVTextBones( char *filePath );
    // Decode bones animations
    bool Decode_BonesAnimation( char *filePath );
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
	// Build a header file related to the image library
    bool Build_H_File();
};


#endif  // __COBJ_FILE_PARSER_H__