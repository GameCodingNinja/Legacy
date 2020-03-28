/************************************************************************
*
*    FILE NAME:       CPolygon.h
*
*    DESCRIPTION:     Polygon class
*
************************************************************************/

#ifndef __CPOLYGON_H__
#define __CPOLYGON_H__

#include "CPoint.h"     // class holding all the point classes
#include "CScrnPoint.h" // 2D screen points for 3d software rendering
#include "CTextLib.h"   // Texture library class
#include "CLight.h"     // Header file for this *.cpp file.
#include "types.h"

class CPolygon
{
protected:

    // radius of polygon
    float viewRadius;
	float collideRadius;
    // Texture Index
    int tIndex;
    // collision type - so that we know what we are hitting
    ECT_COLLISION_TYPE collisionType;
    // Number of vertexes for this polygon. Could be a tri or a quad
    int vertCount;
	// Inverse of vertCount so that we can multiply instead of divide
	float inverseVertCount;
    // Pointer to the surface normal
    CPoint *pNormal;
    // 4 pointers to a vertex list
    CPoint *pVPoint[QUAD];
    // UN coordinates, vertex normal and rgb color per vertex
    CUVRGB uvrgb[QUAD];
    // Projected point array
    CScrnPoint projectPoint[ MAX_CLIPPED_POINTS ];
    // Projection count
    int projCount;
    // clipped flag
    bool clipped;

    // Load extra polygon data
    virtual bool LoadExtraPolygonData( FILE *hFile );
    // Save extra polygon data
    virtual bool SaveExtraPolygonData( FILE *hFile );

public:
    
    CPolygon();
    virtual ~CPolygon();

    // Calculate light intensity
    void _fastcall CalcLightIntenStatic( CLightList *pLightList );
    void _fastcall CalcLightIntenDynamic( CLightList *pLightList );
    // Use plane equation to determine the orientation of the surface
    void CalcSurfaceNormal();
    // Calculate a bounding sphere around the polygon
    void CalcBoundingPoly();
    // Check for collision using bounding spheres
    ECT_COLLISION_TYPE _fastcall Collide_BoundSphere( CPoint &pt, double &distance, float rad );
	ECT_COLLISION_TYPE _fastcall Collide_BoundSphereSt( CPoint &pt, double &distance, float rad );
    // Get the a specific point of the polygon
    void CopyPoint( CPoint &pt, int index );
    // Get a copy of the normal
    void CopyNormal( CPoint &pt );
    // Is the supplied vertice the same
    bool _fastcall HasVert( CPoint &P );
	// Is the supplied vertice similar within a range
	bool _fastcall HasCloseVert( CPoint &P, float range );
    // Load the polygon data
    virtual bool LoadPolygonData( FILE *hFile, CPoint *vertLst );
    // Load all the normals from file
    bool LoadNormalsFromFile( FILE *hFile );
    // Save the polygon data
    virtual bool SavePolygonData( FILE *hFile, int vertexCount, CPoint *vertLst );
    // Save all the normals to a file
    bool SaveNormalsToFile( FILE *hFile );
    // Check if the polygon is visible. Perform 3D culling
    virtual bool IsVisable();
    bool _fastcall IsVisable( float portRadius, CPoint *pPortalCenter );
	bool IsShadowVisable();
    // Copy the polygon
    void CopyPolygon( CPolygon &poly, CPoint *vertLst, int vertexCount );
	// Adjust the radius depending on the need
	void AdjustViewRadius( float scaler );
	void AdjustCollideRadius( float scaler );
	// Calculate the center point of the polygon
	void CalcPolyCenterPointSt( CPoint &centerPt );
	void CalcPolyCenterPointDy( CPoint &centerPt );

    /////////////////
    // Set functions
    /////////////////

    // Set the normal
    void SetNormal( CPoint * pNor ){ pNormal = pNor; };
    // Set the vertex point
    void SetVPoint( int index, CPoint * pVert ){ pVPoint[index] = pVert; };
    // Set vertex normal
    void SetVertexNormal( int index, CPoint &vNorm ){ uvrgb[index].n = vNorm; };
    // Set an ambient light level for this polygon
    void SetAmbientLightLevel( float rColor, float gColor, float bColor );
    // Set the vertex count
    void SetVertexCount( int value ){ vertCount = value; };
	// Set the collision type
	void SetCollisionType( ECT_COLLISION_TYPE value ){ collisionType = value; };
	// Set the radius
	void SetViewRadius( float value ){ viewRadius = value; };
	void SetCollideRadius( float value ){ collideRadius = value; };
	// Set the texture index
	void SetTextIndex( int index ){ tIndex = index; };

    /////////////////
    // Get functions
    /////////////////

    // Get the collision type
    ECT_COLLISION_TYPE GetCollisionType(){ return collisionType; };
    // Get the number of vertexes for this polygon
    int GetVertexCount(){ return vertCount; };
    // Get the texture index
    int GetTextIndex();
    // Get the normal
    CPoint * GetNormal(){ return pNormal; };
    // Get the vertex point
    CPoint * GetVPoint( int index ){ return pVPoint[index]; };
    // Get the uvrgb
    CUVRGB * GetUVRGB(){ return uvrgb; };
    // Get the radius
    float GetViewRadius(){ return viewRadius; };
	float GetCollideRadius(){ return collideRadius; };
    // Get if polygon was clipped
    bool GetClipped(){ return clipped; };
	// Get the average height of the polygon
	float GetAverageHeightSt();

    //////////////////////
    // Rendering functions
    //////////////////////

    // OpenGL functions
    void _fastcall RenderOpenGL();
    void _fastcall RenderOpenGLText( CTextureLib *pTextLib );
	void _fastcall BindOpenGLText( CTextureLib *pTextLib );
	void _fastcall RenderOpenGLShadow( CPoint &shadowOffset );

    // DirectX9 functions
    void _fastcall RenderDirectX();
    void _fastcall RenderDirectXText( CTextureLib *pTextLib );
	void _fastcall BindDirectXText( CTextureLib *pTextLib );
	void _fastcall RenderDirectXShadow( CPoint &shadowOffset );

    // Software rendering functions
    void _fastcall ClipProject( CTextureLib *pTextLib );
    void ClipProject();
    void _fastcall Render_WireFrame();
    void _fastcall RenderClipped( CTextureLib *pTextLib );
    void _fastcall Render( CTextureLib *pTextLib );
    void _fastcall RenderAlpha( CTextureLib *pTextLib );
    void _fastcall RenderClippedAlpha( CTextureLib *pTextLib );
	bool IsVisable2D();
	void BindSoftwareText();
    
    // Does polygon = polygon?
    bool operator == ( CPolygon &polygon );
    
    // copy the normals	from another polygon
    void SetNormals( CPolygon &polygon ); 
};


void _fastcall DrawDirectXPrimatives( int maxRenderAmount = 0 );


/************************************************************************
*
*    CeilLine class for plotting your way down the edge of the triangle
*
************************************************************************/

class CCeilLine
{
public:
    int Y, Z, U, V, r, g, b, x1, x2, y1, y2, I;
    int stepZ, stepU, stepV, edgeHeight;
    int stepR, stepG, stepB;
    int X_int, stepX_int;
    float X_float, stepX_float;

    // The constructor
    CCeilLine():edgeHeight(0)
    {
    }

    // Init the class with the two points that form a line down the triangle
    inline void CCeilLine::Init( CScrnPoint &point2D_Pos, CScrnPoint &point2D_newPos )
    {
		// Turn off the data type conversion warning (ie. int to float, float to int etc.)       
		#pragma warning(disable : 4244)

		// The two points that makeup a line
        x1 = point2D_Pos.x;
        x2 = point2D_newPos.x;
        y1 = point2D_Pos.y;
        y2 = point2D_newPos.y;

        // Height of the two points
        edgeHeight = ( y2 - y1 );

        // Push the X to the Ceiling of the number so that
        // the number isn't truncated.
        X_int = (x1 << FIX_SHIFT_20_INT) + CEIL_FRACT;
        X_float = x1;
        Y = y1;
        Z = point2D_Pos.z;

        // Store U/Z and V/Z values:
        U = point2D_Pos.U;
        V = point2D_Pos.V;

        // Store color intensity value:
        r = point2D_Pos.r;
        g = point2D_Pos.g;
        b = point2D_Pos.b;

        if( edgeHeight )
        {
            // Create the step amount
            stepX_int = (( x2 - x1 ) << FIX_SHIFT_20_INT) / edgeHeight;
            stepX_float = ((double)(x2 - x1) / (double)edgeHeight);
            stepZ = ( point2D_newPos.z - point2D_Pos.z ) / edgeHeight;

            // Calculate U/Z and V/Z deltas
            stepU = ( point2D_newPos.U - point2D_Pos.U ) / edgeHeight;
            stepV = ( point2D_newPos.V - point2D_Pos.V ) / edgeHeight;

            // Calculate color intensity delta                
            stepR = (point2D_newPos.r - point2D_Pos.r) / edgeHeight;
            stepG = (point2D_newPos.g - point2D_Pos.g) / edgeHeight;
            stepB = (point2D_newPos.b - point2D_Pos.b) / edgeHeight;
        }
        else
        {
            stepX_float = 0;
            stepX_int = 0;
            stepZ = 0;
            stepU = 0;
            stepV = 0;
            stepR = 0;
            stepG = 0;
            stepB = 0;
        }
    }   // init

    // Perform object-precision clip on top of edge. 
    // Is the top edge of the polygon above the Y of the screen?
    // If so, recalculate the X offset and set the Y to 0
    inline void CCeilLine::ClipTop()
    {
        // Turn off the data type conversion warning (ie. int to float, float to int etc.)       
		#pragma warning(disable : 4244)

		float step(-Y);

        // calculate a new x offset based on the y being zero
        X_int = ((x1 + (int)(((float)(x2 - x1) / (float)(y2 - y1)) * step)) << FIX_SHIFT_20_INT ) + CEIL_FRACT;
        X_float = (double)x1 + ((double)(x2 - x1) / (double)(y2 - y1)) * step;

        // Set to zero
        Y = 0;
        Z += stepZ * step;
        U += stepU * step;
        V += stepV * step;
        r += stepR * step;
        g += stepG * step;
        b += stepB * step;

        // Take the current edge height and subtract the
        // original offscreen y offset
        edgeHeight -= step;

    }   // ClipTop
};


#define fixdiv( Result, Num, Denom, Prec )\
  { _asm { mov  eax, Num }\
    _asm { cdq }\
    _asm { mov  cl, Prec }\
    _asm { shld  edx, eax, cl }\
    _asm { sal  eax, cl }\
    _asm { idiv Denom }\
    _asm { mov  Result, eax } }


/************************************************************************
*
*    Simple classes for loading/saving of data from binary files.
*
************************************************************************/
#pragma pack(1)
class CUVLoad
{
public:
    // U, V assumed
    float uv[2];

    CUVLoad(){ uv[0]=0.0; uv[1]=0.0; }
};


class CFaceLoad
{
public:
    // index into main vertice list
    int vIndex[QUAD];
    // U, V assumed
    CUVLoad crd[QUAD];
    // texture index
    int tIndex;
    // Vert count
    unsigned char vertCount;
    // Collsion type flag
    unsigned char collisionType;

    CFaceLoad()
    {
        vIndex[0] = vIndex[1] = vIndex[2] = vIndex[3] = 0;
        tIndex = 0;
        vertCount = 3;
        collisionType = (unsigned char)ECT_NO_TYPE;
    }
};

class CNormal
{
public:
    // x, y, z is assumed
    float norm[3];

    CNormal(){ norm[0]=0.0; norm[1]=0.0; norm[2]=0.0; }
};

class CNormalLoad
{
public:
    // x, y, z is assumed
    float norm[3];

    CNormal vertNor[QUAD];

    CNormalLoad(){ norm[0]=0.0; norm[1]=0.0; norm[2]=0.0; }
};

class CLoadPoint
{
public:
    // x, y, z is assumed
    float pt[3];

    CLoadPoint(){ pt[0]=0.0; pt[1]=0.0; pt[2]=0.0; }
};
#pragma pack()

#endif // __CPOLYGON_H__