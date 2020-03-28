/************************************************************************
*
*    PRODUCT:         3D Engine
*
*    FILE NAME:       PolyObj.h
*
*    DESCRIPTION:     Header file for a panel object class
*
*    Author: John De Goes
*
*    All Rights Reserved
*
************************************************************************/


#ifndef POLYOBJ_H
#define POLYOBJ_H

#include "Matrix3D.h"
#include "Point3D.h"
#include "Panel3D.h"


// Camera Structure
struct TCamera
{
    float XPos, YPos, ZPos;
    int XRot, YRot, ZRot;
    TCamera(){ Clear(); }
    void Clear(){ XRot = YRot = ZRot = 0; XPos = YPos = ZPos = 0.0F; }
};
typedef TCamera *PTCamera;

#pragma pack(1)
struct PolyHeader
{
    // The vertex count
    unsigned int VertCount;
    // The panel count
    unsigned int PanelCount;
};
typedef PolyHeader *PPolyHeader;
#pragma pack()

// Transparency types
enum
{
	STANDARD_IF_NOT_0=0,
	PAINT_OR,
	SCAND_PAINT_AND_OR,
	NO_SHADED_TABLE,
};

// Z Buffering situations
enum
{
    POS_ZBUFFER=0,
    NEG_ZBUFFER,
	NO_ZBUFFERING,
};


// An object made of panels:
class TPolyObj
{
protected:
    // The current screen width and height
    int ScreenW, ScreenH;
    // Pointer to the Z buffer
    int *ZBuffer;
    // Temporary vertex list (used for loading)
    Point3D *TList;
    // The morphing list
    Point3D *MList;
    // Pointer to Video Buffer
    void *pVBuffer;
    // The list of vertices
    Point3D *VList;
    // The list of panels
    Panel3D *PList;
    // The vertex count
    unsigned int VCount;
    // The panel count
    unsigned int PCount;
    // A visible flag  
    WORD Visible;
    // A morphing flag
    WORD Morph, StartMorph;
    // The object's radius
    float RadiusXY, RadiusXZ, Radius;
    // Declare the textures for the polygons
    PImageData pTextDat;
    // Delcare the texture shade look-up table
    void *pTextShade;
    // The Light Source
    TLight Light;
    // Polygon Z sort Optimization
    bool DoPolyZSort;
    // The scale of the camera
    int CameraScale;
    // The Maxium Z Distance
    int MaxZDistance;
    // The Maxium Light Distance
    int MaxLightDistance;
	// Flag to indicate the light source has changed
	bool dynamicLighting;

    // Transforms vertex list
    void Transform( Matrix3D &M );
    // Loads a coordinate
    Point3D LoadCoord( FILE *InFile );
    // Counts 3DFACEs
    unsigned int CountPanels( char *FileName );
    // Loads vertex list
    bool LoadVerts( char *FileName );
    // Loads panel data   
    bool LoadPanelData();
    // Loads a panel               
    bool LoadPanel( int &VIndex, int Index );
    // Reads a line of text from a text file
    int GetLine( FILE *InFile, char *String ); 

public:

    // Constructor
    TPolyObj();
    // Destructor
    ~TPolyObj();
    // Render the 3D object on screen buffer. 8 bit	graphics to an 8 bit buffer
    int PaintVBuffer8To8( Matrix3D &M, int ZBufferOption = POS_ZBUFFER, int Start = 0, int End = 0, bool DontBackFaceCul = false, int RenderType = STANDARD_IF_NOT_0 );
    // Render the 3D object on screen buffer. 8 bit	graphics to a 16 bit buffer
    int PaintVBuffer8To16( Matrix3D &M, int ZBufferOption = POS_ZBUFFER, int Start = 0, int End = 0, bool DontBackFaceCul = false, int RenderType = STANDARD_IF_NOT_0 );
    // Render the 3D object on screen buffer. 8 bit	graphics to a 16 bit buffer
    int PaintVBuffer16To16( Matrix3D &M, int ZBufferOption = POS_ZBUFFER, int Start = 0, int End = 0, bool DontBackFaceCul = false, int RenderType = STANDARD_IF_NOT_0 );
    // Loads DXF files
    bool LoadDXF( char *FileName );
    // Writes BIN files
    bool SavePolygonObjFile( char *FileName );
    // Reads BIN files
    bool LoadPolygonObjFile( char *FileName );
    // Loads textures/data
    bool ReadTextCoord( char *FileName );
    // Writes textures/data  
    bool WriteTextCoord( char *FileName ); 
    // Initializes default texture/shade
    void InitDefText(); 
    // coords
    //void MoveText( int Index, int Ox, int Oy, int Tx, int Ty );
    // Rotates the selected panel's texture
    void RotateTexture( int Polygon ); 
    // Morph function
    bool MorphTo( char *FileName, float Steps );
    void MorphStop() { StartMorph = 0; }
    void MorphStart() { StartMorph = 1; }
    void MorphRev();
    // Check for collision
    PPanel3D Collide( int &index, float &X, float &Y, float &Z, float Rad );
    // Get the lighting
    PTLight GetLightSource(){ return &Light; }
    // Set the scale of the camera
    void SetCameraScale( int Value ){ CameraScale = Value; }
    // Set the maxium Z distance from the camera
    void SetMaxZDistance( int Value ){ MaxZDistance = Value; }
    // Set the maxium light distance from the camera
    void SetMaxLightDistance( int Value ){ MaxLightDistance = Value; }    
    // Set pointer to video buffer
    void SetVideoBuffer( void *pValue ){ pVBuffer = pValue; }
    // Set the texture data
    void SetTextures( PImageData pTexture ){ pTextDat = pTexture; }
    // Set the texture shade look-up table
    void SetShadeLookUpTable( void *pShade ){ pTextShade = pShade; }
    // Set the ZBuffer
    void SetZBuffer( int *pZBuffer ){ ZBuffer = pZBuffer; }
    // Set the render size
    void SetRenderSize( int Width, int Height ){ ScreenW = Width; ScreenH = Height; };
	// Set the dynamic lighting flag
	void SetDynamicLighting( bool value ){ dynamicLighting = value; };
    // Free all allocations
    void Free();
    // Return the index to the selected polygon
    int GetSelectedPolygon( int X, int Y );
    // Outline the polygon in a color
    void OutlinePolygon8to8( int Polygon );
	void OutlinePolygon8to16( int Polygon );
    // Render just a singlr polygon
    void RenderSinglePoly( int Polygon, int ZBufferOption );
    // sets up the panels with a default texture and texture information
    void ReInitPanels();
    // Select a specific polygon to use a specific texture index
    void SetTexture( int Polygon, int TextIndex, int X = 0, int Y = 0, int Width = 0, int Height = 0 );
    // Get the total number of polygons/verts in this object
    unsigned int GetTotalPolyCount(){ return PCount; };
	unsigned int GetTotalVertCount(){ return VCount; };
    // Get the polygon panel
    PPanel3D GetPoly( int Index ){ return &PList[ Index ]; };
    // Load an object from data pointer
    bool LoadPlyFromPointer( unsigned char *pData );
    // Read in the texture coordinates  from a pointer
    void LoadCrdFromPointer( unsigned char *pData );
	// Get the handle index of the selected polygon
	WORD GetTextureIndex( int PolyIndex ){ return PList[ PolyIndex ].GetTextureHandle(); };
	// Render the 3D object on screen buffer in wire frame.
	int PaintWireFrame8to8( Matrix3D &M, bool DontBackFaceCul );
	int PaintWireFrame8to16( Matrix3D &M, bool DontBackFaceCul );
	// Set the light source
	void SetLightSource( float X, float Y, float Z, bool enambleDynamicLighting = false );
	// Set the texture handle
	void SetTextHandle( int Polygon, int TextIndex ){ PList[ Polygon ].SetTextureHandle( TextIndex ); };
	// Sort on Z orderr
	void Sort();
	// Calculate the shading for all vertexes
	void CalculateShading();
};

typedef TPolyObj *PTPolyObj;

#endif