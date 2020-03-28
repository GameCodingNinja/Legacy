
/************************************************************************
*
*    FILE NAME:       CMesh.h
*
*    DESCRIPTION:     base mesh class with all general mesh attributes
*
************************************************************************/

#ifndef __CMESH_H__
#define __CMESH_H__

#include "CPolygon.h"      // Polygon class
#include "CMatrix.h"       // Matrix class
#include "CLight.h"        // Header file for this *.cpp file.
#include <vector>


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
    float radiusX;
    float radiusY;
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
    // Points for bounding box
    CPoint boundingBox[8];
    // Flag that indicates this object's cull state
    bool culled;
    // Flag indicating cull test performed
    bool cullTestDone;
    // ID of this object
    int id;
    // What this object is
    EOT_OBJECT_TYPE type;
    // Flag that indicates this mesh has been rendered
    bool rendered;
	// Flag indicating transform was done
	bool transformed;
	// Max point of this mesh
	CPoint maxPoint;
	// Min point of this mesh
	CPoint minPoint;
     
    // CAlculate a vertex normal for a grid.
    void CalcGridVertexNormals( int width );
    // Calculate a bounding sphere around this object.
    virtual void CalBoundingSphereBox();
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

    // Render the 3D object on screen buffer
    int RenderOpenGL( CMatrix &m );
    int RenderOpenGL( CMatrix &m, float portRadius, CPoint *pPortalCenter );
	int RenderOpenGLShadow( CPoint &offset );
    int RenderSoftware( CMatrix &m );
    int RenderSoftware( CMatrix &m, float portRadius, CPoint *pPortalCenter );
    int RenderDirectX( CMatrix &m );
    int RenderDirectX( CMatrix &m, float portRadius, CPoint *pPortalCenter );
	int RenderDirectXShadow( CPoint &offset );

	// Translate the light list
	virtual void TransformLightList( CMatrix &m );

public:

    CMesh();
    virtual ~CMesh();

    // Free all allocations
    void Free();
    // Render the 3D object on screen buffer
    int Render( CMatrix &m, bool transform = true );
    int Render( CMatrix &m,  float portRadius, CPoint *pPortalCenter, bool transform = true, bool shadow = false );
	int RenderShadow( CMatrix &m, CPoint &offset, bool transform = true );

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
    
    // Generate a Grid
    bool GenerateGrid( int x, int z, int yOffset, float multiplyer, float size, unsigned char * hightData );
    // Final Check for collision using intersection
    bool IsCollision_Intersection( CPoint &normal, CPoint &triVert, CPoint &pt,
                                   float padding, float &pointD );
	bool IsCollision_IntersectionSt( int triIndex, float padding, CPoint &pt,
                                     CPoint &normal, float &pointD );
    // Check for collision with polygons using bound spheres
    ECT_COLLISION_TYPE _fastcall IsCollision_BoundSphere( int &index, double &colDistance, CPoint &pt,
                                                          float rad, ECT_COLLISION_TYPE checkSpecificType=ECT_NO_TYPE );
	ECT_COLLISION_TYPE _fastcall IsCollision_BoundSphereSt( int &index, double &colDistance, CPoint &pt,
                                                            float rad, ECT_COLLISION_TYPE checkSpecificType=ECT_NO_TYPE );
    // Test position of object in frustrum to see if it can be culled
    bool _fastcall CullMesh_BoundSphere( float _radius );
	bool CullMesh_BoundSphere();
    bool _fastcall CullMesh_BoundSphere( float portRadius, CPoint *pPortalCenter );
	bool _fastcall CullMesh_BoundSphere( float _radius, float portRadius, CPoint *pPortalCenter );
    bool CullMesh_BoundBox();
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
	// Calculate a surface normal for each vertex.
    void CalcVertexNormals( float range = 0.0f );
    // Copy Mesh
    bool CopyMesh( CMesh &mesh );
    // Merge two meshes into this one
    void InterpulateMesh( CMesh &mesh1, CMesh &mesh2, float amount );
    // Depending on the angle of the normal, set the polygon
    // Collision type to wall or floor.
    virtual void SetWallFloorCollisionType();
	// Transform all the points	in this mesh
	void Transform( CMatrix &m );
	// Transform the center point
	void TransformCenterPt( CMatrix &m );
	// Transform the bounding box
	void TransformBoundingBox( CMatrix &m );
	// Adjust the radius depending on the need
	void AdjustRadius( float scaler, ECT_COLLISION_TYPE colType );
	// Adjust the radius depending on the need 
	void SetRadius( float rad, ECT_COLLISION_TYPE colType );
	// Find shared indexes
	void _fastcall FindSharedNorms( float range, CPoint * vPoint, std::vector<CNormAdd> &sharedNormLst );
	// Add the shared normals to create vertex normal
	void _fastcall AverageSharedNorms( int pIndex, int vIndex, std::vector<CNormAdd> &sharedNormLst );
	// Find the closest light
	CLight * FindClosestLight( CLightList * pLiteLst );
	// Convert the transformed point to a flat shadow
	void ConvertToShadow( CMesh &mesh,
						  CMatrix &m,
						  CPoint &light,
						  CPoint &projectDir,
						  CPoint &areaMinPoint,
						  CPoint &areaMaxPoint );
	// Bind a texture into video memory
	void BindTexture( unsigned int polyIndex = 0 );
	// NULL the light list
	void ClearLightLst(){ pLightList = NULL; };
	// NULL the texture library
	void ClearTextureLibrary(){ pTextLib = NULL; };

    /////////////////
    // Set functions
    /////////////////

    // Set the texture library
    void SetTextureLibrary( CTextureLib &textLib ){ pTextLib = &textLib; };
    // Set the pointer to the light list
    void SetLightList( CLightList * pLiteLst );
    // Set an ambient light level for this object
    void SetAmbientLightLevel( int rColor, int gColor, int bColor );
    // Set the id of the object
    void SetID( int value ){ id = value; };
    // Set the type
    void SetType( EOT_OBJECT_TYPE value ){ type = value; };
    // Reset the flag that indicates this mesh was rendered
    void ResetRenderFlag(){ rendered = false; };
    // Reset the flag that indicates this mesh was cull tested
    void ResetCullTestFlag(){ cullTestDone = false; };
	// Reset the flag that indicates this mesh was transformed
    void ResetTransformedFlag(){ transformed = false; };
	// Set the radius
	void SetRadius( float value ){ radius = value; };
	// Set the texture index
	void SetTextIndex( int index );

    /////////////////
    // Get functions
    /////////////////

    // Get the value of the radius
    float GetRadius(){ return radius; };
    float GetRadiusX(){ return radiusX; };
    float GetRadiusY(){ return radiusY; };
    // Get polygon count
    int GetPolyCount(){ return polyCount; };
    // Get vertex count
    int GetVertCount(){ return vCount; };
    // Get the object distance
    float GetDistance();
    // Get the index
    int GetID(){ return id; };
    // Get the type
    EOT_OBJECT_TYPE GetType(){ return type; };
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
    // Was this mesh already rendered?
    bool WasRendered(){ return rendered; };
    // Was this mesh already tested for culling?
    bool WasCullTested(){ return cullTestDone; };
	// Was this mesh already transformed
    bool WasTransformed(){ return transformed; };
    // Get the average height of the polygon
    float GetHeightOfPolygon( int triIndex );
    // Get the max point
    CPoint & GetMaxPoint(){ return maxPoint; };
    // Get the min point
    CPoint & GetMinPoint(){ return minPoint; };	
};


/************************************************************************
*
*    Simple classes for loading/saving of data from binary files.
*
************************************************************************/
#pragma pack(1)
class CVertLoad
{
public:
    // x, y, z is assumed
    float vert[3];

    CVertLoad(){ vert[0]=0.0; vert[1]=0.0; vert[2]=0.0; }
};
#pragma pack()

// Generate a 16 bit shaded table
void Generate16BitShadedTable( unsigned short int *shadeTbl );

#endif // __CMESH_H__