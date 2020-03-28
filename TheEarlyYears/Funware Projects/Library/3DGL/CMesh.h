
/************************************************************************
*
*    PRODUCT:         Mesh class
*
*    FILE NAME:       CMesh.h
*
*    DESCRIPTION:     base mesh class with all general mesh attributes
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef __CMESH_H__
#define __CMESH_H__

#include "CPolygon.h"      // Polygon class
#include "CMatrix.h"       // Matrix class
#include "CLight.h"        // Header file for this *.cpp file.


#define MESH_RAW_FORMAT_STR_ID   "raw mesh"
#define NORMAL_RAW_FORMAT_STR_ID "raw normal"


class CMesh
{
protected:

    // The vertex count
    int vCount;
    // The polygon count
    int polyCount;
    // Bounding sphere radius
    float radius;
    // Allocated list of vertices
    CPoint *vList;
    // Allocated list of polygons
    CPolygon *pList;
    // Allocated list of normals
    CPoint *nList;
    // Texture library used for this mesh
    CTextureLib *pTextLib;
    // Pointer to the light list
    CLightList *pLightList;
    // Object center point for culling this object
    CPoint centerPt;
    // Flag that indicates this object was culled
    bool culled;
    // Distance of bounding sphere
    float distance;
    // ID of this object
    int id;
    // What this object is
    int type;
     
    // Calculate a surface normal for each vertex.
    void CalcVertexNormals();
    // CAlculate a vertex normal for a grid.
    void CalcGridVertexNormals( int width );
    // Calculate a bounding sphere around this object.
    void CalBoundingSphere();
    // Allocate the polygon array
    virtual CPolygon * AllocatePolygonArray();
    // Virtual function for loading inherited class specific data
    virtual bool LoadExtraMeshData( FILE *hFile );
    // Virtual function for saving inherited class specific data
    virtual bool SaveExtraMeshData( FILE *hFile );
    // Load the polygon data
    virtual bool LoadPolygonData( FILE *hFile );
    // Save the polygon data
    virtual bool SavePolygonData( FILE *hFile );

public:

    CMesh();
    virtual ~CMesh();

    // Free all allocations
    void Free();
    // Render the 3D object on screen buffer
    int Render( CMatrix &m );
    int Render( CMatrix &m,  float portRadiusX, float portRadiusY, CPoint *pPortalCenter );

    // Load the polygon object file
    bool LoadFromFile( char *filePath );
    bool LoadFromFile( FILE *hFile );
    // Load mesh from file
    bool LoadMeshFromFile( char *filePath );
    bool LoadMeshFromFile( FILE *hFile );
    // Load the normals from a file
    bool LoadNormalsFromFile( char *filePath );
    bool LoadNormalsFromFile( FILE *hFile );
    
    // Load the polygon object file
    bool SaveToFile( char *filePath );
    bool SaveToFile( FILE *hFile );
    // Save the mesh to file
    bool SaveMeshToFile( char *filePath );
    bool SaveMeshToFile( FILE *hFile );
    // Save the normals to file
    bool SaveNormalsToFile( char *filePath );
    bool SaveNormalsToFile( FILE *hFile );  

    // Load an object from data pointer
    //bool LoadFromPointer( unsigned char *pData );    
    // Generate a Grid
    bool GenerateGrid( int x, int z, int yOffset, float multiplyer, float size, unsigned char * hightData );
    // Final Check for collision using intersection
    bool IsCollision_Intersection( CMatrix &m, int triIndex, float padding, CPoint &pt,
                                   CPoint &normal, float &pointD );
    // Check for collision with polygons using bound spheres
    int IsCollision_BoundSphere( int &index, double &distance, CPoint &pt, float rad );
    // Test position of object in frustrum to see if it can be culled
    bool _fastcall CullMesh_BoundSphere( CMatrix &m );
    bool _fastcall CullMesh_BoundSphere( CMatrix &m, float portRadiusX, float portRadiusY,
                                         CPoint *pPortalCenter );
    // Check for collision using bounding spheres
    bool _fastcall Collide_BoundSphere( CMesh *mesh );
    // Check for collision with point
    bool _fastcall Collide_BoundSphere( CPoint &pt, float rad );
    // Was this object culled?
    bool WasCulled(){ return culled; };
    // Calculate bounding polygon
    void CalBoundingPoly();
    // Scale mesh
    void Scale( float x, float y, float z );
    // Calculate normals
    void CalNormals();
	// Copy Mesh
	bool CopyMesh( CMesh &mesh );
	// Merge two meshes into this one
	void InterpulateMesh( CMesh &mesh1, CMesh &mesh2, float amount );

    /////////////////
    // Set functions
    /////////////////

    // Set the texture library
    void SetTextureLibrary( CTextureLib &textLib ){ pTextLib = &textLib; };
    // Set the pointer to the light list
    void SetLightList( CLightList &lightList );
    // Set an ambient light level for this object
    void SetAmbientLightLevel( int rColor, int gColor, int bColor );
    // Set the id of the object
    void SetID( int value ){ id = value; };
    // Set the type
    void SetType( int value ){ type = value; };

    /////////////////
    // Get functions
    /////////////////

    // Get the value of the radius
    float GetRadius(){ return radius; };
    float GetRadiusX();
    float GetRadiusY();
    // Get polygon count
    int GetPolyCount(){ return polyCount; };
    // Get vertex count
    int GetVertCount(){ return vCount; };
    // Get the object distance
    float GetDistance(){ return distance; };
    // Get the index
    int GetID(){ return id; };
    // Get the type
    int GetType(){ return type; };
    // Get the center point
    CPoint * GetCenterPt(){ return &centerPt; };
	// Get the vertex list
    const CPoint * GetVertexList(){ return vList; };
    // Get the trianglr list
    CPolygon * GetPolyList(){ return pList; };
	// Get the vertex list
    const CPoint * GetNormalList(){ return nList; };
	// Get the light list
	CLightList * GetLightList(){ return pLightList; };
	// Get the texture library
	CTextureLib * GetTextLibary(){ return pTextLib; };
};


/************************************************************************
*
*    Simple classes for loading/saving of data from binary files.
*
************************************************************************/

class CVertLoad
{
public:
    // x, y, z is assumed
    float vert[3];

    CVertLoad(){ vert[0]=0.0; vert[1]=0.0; vert[2]=0.0; }
};

#endif // __CMESH_H__