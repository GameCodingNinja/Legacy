
/************************************************************************
*
*    PRODUCT:         Mesh List class template
*
*    FILE NAME:       CMeshList.h
*
*    DESCRIPTION:     This list class is designed to only call members
*                     of the base mesh class. Inherit from this class
*                     to call specialized inherited mesh class members.  
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef _CMESH_LIST_H_
#define _CMESH_LIST_H_

#include "ObjectTypes.h"     // Object type specifiers
#include "CMesh.h"           // Mesh class

class CMesh;
class CMatrix;
class CLightList;

#define MESH_LST_FORMAT_STR_ID "mesh list"


/************************************************************************
*
*    Class for handling lists of meshes
*
************************************************************************/

template <class mesh_type>
class CMeshList
{
protected:

    // Mesh array
    mesh_type *pMesh;
    // Number of allocated meshes
    int meshCount;

public:

    CMeshList();
    virtual ~CMeshList(){ Free(); }

    // Free the allocations
    void Free();
    // Get the count
    int Count(){ return meshCount; };
    // Render all the meshes
    int RenderAll( CMatrix &m );
    // Load mesh and normal data from file
    virtual bool LoadFromFile( char *filePath );
    virtual bool LoadFromFile( FILE *hFile );
    // Load mesh from file
    virtual bool LoadMeshFromFile( char *filePath );
	virtual bool LoadMeshFromFile( FILE *hFile );
    // Save mesh and normal data to file
    virtual bool SaveToFile( char *filePath );
    virtual bool SaveToFile( FILE *hFile );
    // Calculate normals
    void CalNormals();
    // Set the list of light data to all meshes
    void SetLightList( CLightList &lightList );
    // Set the list of light data to a specific mesh
    void SetLightList( int index, CLightList &lightList );
    // Scale the mesh classes
    void Scale( float x, float y, float z );
    // Set the texture library to all meshes
    void SetTextureLibrary( CTextureLib &textLib );
};


/////////////////////////////////////////
//  Must include this here or you will
//  get a bunch of "Unresolved External"
//  errors. Templates are a pain to code
/////////////////////////////////////////
#include "CMeshList.cpp"


#endif  // _CMESH_LIST_H_