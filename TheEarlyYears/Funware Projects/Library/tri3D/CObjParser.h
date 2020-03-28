
/************************************************************************
*
*    PRODUCT:         3D Engine based on triangles only.
*
*    FILE NAME:       CObjParser.h
*
*    DESCRIPTION:     Parse 3D OBJ files
*
*    IMPLEMENTATION:  Load in an WaveFront OBJ file and parse out the data.
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

//////////////////////////////////
// Simple vertex class for holding 
// parsed data
//////////////////////////////////
class CVertice
{
public:
    // x, y, z is assumed
    float vert[3];

	CVertice(){ vert[0]=0.0; vert[1]=0.0; vert[2]=0.0; }
};

typedef CVertice *PCVertice;

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
    char groupName[100];
    char materialName[100];
    CObjList faceLst;

	CGroup()
	{
		groupName[0] = 0;
		materialName[0] = 0;
	}
};

typedef CGroup *PCGroup;

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
};


///////////////////////////////////
// WaveFront OBJ ASCII Parse Class
///////////////////////////////////
class CObjParser
{
protected:
    // Holds the error code
    int errorCode;
    // Holds the external file path responsible for error
    char errorFilePath[500];

    // Reads a line of text from a text file
    int GetLine( FILE *inFile, char *str );
    // decode the string of vertices
    bool Decode_Vertice( CObjList &objLst, char *strData, int strCount );
    // Get the name of the ASCII file that holds 
    // all the texture file names
    void Decode_Name( char *strName, char *strData, int strCount );
    // decode the string of UV values
    bool Decode_UV( CObjList &objLst, char *strData, int strCount );
    // Add a new group to the list
    bool AddGroup( CObjList &objLst, char *strData, int strCount );
	// Add the material name to the group
	bool AddMateralNameToGroup( CObjList &groupLst, char *strData, int strCount );
    // setup the group list with the vertices and uv data.
    bool AddFaceToGroup( CObjList &groupLst, CObjList &vLst, CObjList &uvLst, char *strData, int strCount );
    // Remove all the duplicate verticies from the vertice list
    bool RemoveDupVerticies( CObjList &objLst );
    // Match up all the vertices in the faces to the verstices in the list.
    bool MatchVerticies( CObjList &vertLst, CObjList &groupLst );
    // Save polygon object data to file
    bool SaveToFile( CObjList &vertLst, CObjList &groupLst, char *fileName );
    // Strip the ".ext" of the end of a file path or
    // file and replace with different extension
    void RelpaceFileExtension( char *filePath, char *Ext );
    // Strip the file name from the path and add new file name
    void CropOutPathAddFileName( char *sourcePath, char *destPath, char *newFileName );
    // Parse the texture library class
    bool ParseTextLibFile( CObjList &objLst, char *filePath, char *fileName );
    // Get the material name and add it to the list
    bool Decode_MaterialName( CObjList &objLst, char *strData, int strCount );
    // Get the texture name and add it to the list
    bool Decode_TextureName( CObjList &objLst, char *strData, int strCount );
    // Check to see if we have  any duplicate textures in the mix.
    bool FlagDuplicateTexturess( CObjList &tLst, CObjList &noDupTxtLst );
    // Match texture index to face
    bool MatchTextureIndexToFace( CObjList &groupLst, CObjList &textLst, CObjList &noDupTxtLst );
    // Build the texture library file
    bool BuildTextureLibrary( CTextureLib &textLib, CObjList &noDupTxtLst, char *filePath );
    // Convert the non-specific coordinates to coordinates specific to the library textures
    void ConvertUVtoSpecificCoordinates( CTextureLib &textLib, CObjList &groupLst );
    // Build a header file related to the image library
    bool Build_H_File( CObjList &noDupTxtLst, char *filePath );
    // Build a map file to show the data organization
    bool BuildMapFile( CObjList &groupLst, CObjList &vLst, CObjList &tLst, 
                       CTextureLib &textLib, CObjList &noDupTxtLst, char *filePath );
    // Strip the file name from the file path
    void CropFileNameFromPath( char *SourceFilePath, char *DestFileName, bool KeepExtension = true );
    // Search a string and replace a character with a different character
    void CObjParser::RelpaceChar( char *charString, char find, char replace );
	// Sort all the polygons by their Z order
	void SortByZCenters( CObjList &tmpFaceLst );
	// Generate the normal file
    bool GenerateNormalFile( char *fileName );

public:
    CObjParser();
    ~CObjParser();

    // Load OBJ text file and parse it
    bool LoadFromFile( char *filePath );
    // get the error code
    int GetErrorCode(){ return errorCode; };
    // get the error file path code
    char *GetErrorFilePath(){ return errorFilePath; };
};


#endif  // __COBJ_FILE_PARSER_H__