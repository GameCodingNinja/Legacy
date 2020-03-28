//
// File name: Panel3D.h
//
// Description: Header file for a 3D panel class
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef PANEL3DHPP
#define PANEL3DHPP

#include "Point3D.h"
#include "Point2D.h"
#include "Vector3D.h"
#include "Detail3D.h"
#include "TextType.h"
#include "PalShade.h"
#include "LightP.h"


#define MAX_CLIPPED_POINTS   6
#define MAX_ROT              4
#define DEF_SCALE            150
#define DEF_MAXZ             100000.0F

struct CeilLine
{

    int X1, X2, Y1, Y2;
    int X, StepX, StepZ, StepU;
    int Z, U, V;
    int StepV, I, StepI;
    int EdgeHeight, Y;

    // The constructor
    CeilLine()
    {
        X1 = X2 = Y1 = Y2 = 0;
        X = StepX = StepZ = Z = 0;
        U = V = StepU = StepV = I = StepI = 0;
        EdgeHeight = Y = 0;
    }
};


struct PanelBehavior
{
    // Index of the texture into the texture library
    WORD TextHan;
    // Number of frames in animated texture
    WORD FrameCount;
    // Frame rate for animated textures
    unsigned int FrameRate;
    // The panel's detail attributes
    Detail3D Attrib[ 4 ];

    unsigned char Trans : 1;
    unsigned char Rot : 2;
    unsigned char NoCheckExten : 1;
    unsigned char SolidColor : 1;
    unsigned char CheckBounds : 1;
	unsigned char PropertySet : 1; // This flag indicates to check for the below items
	unsigned char GroupID : 8;
	unsigned char PannelID : 8;
	unsigned char CollisionCheck : 1;
    unsigned int Future1 : 8;
    unsigned int Future2;
    unsigned int Future3;
    unsigned int Future4;

    PanelBehavior()
    {
        TextHan = FrameCount = 0;
        Trans = Rot = NoCheckExten = 0;
        SolidColor = CheckBounds = FrameRate = 0;
        GroupID = PannelID = Future1 = Future2 = Future3 = Future4 = 0;  
    }
};
typedef PanelBehavior *PPanelBehavior;

// Because of the above bit fields, size of doesn't report correctly
// This structure is used with sizeof
struct PanelBehaviorForLoad
{
    // Index of the texture into the texture library
    WORD TextHan;
    // Number of frames in animated texture
    WORD FrameCount;
    // Frame rate for animated textures
    unsigned int FrameRate;
    // The panel's detail attributes
    Detail3D Attrib[ 4 ];

    unsigned int Future1;
    unsigned int Future2;
    unsigned int Future3;
    unsigned int Future4;
};


// The panel class:
class Panel3D
{
protected:

    // Declare the textures for the polygons:
    PImageData pTextDat;
    // Pointer to the shaded table
    void *pTextShade;
    // Screen point count & invisible flag
    int SPCount;
    // Panel Animation Variables
    unsigned char FrameCounter;
    // Timers
    unsigned int FrameTimer;
    // Maximum of 6 on-screen points
    Point2D SPoint[ MAX_CLIPPED_POINTS ];

    // Determines if panel is backface        
    bool inline CalcBFace();
    // Determines if panel lies within the Z extents     
    //bool CheckExtents();     
    // Clip a horizontal "Z-buffered" line
    /*void inline ClipHLine( int &X1, int &X2, int &Z, int ZStep,
                           int &U, int UStep, int &V, int VStep,
                           int &I, int IStep ); */
    // Rotate textures
    void RotText();

public:
    Vector Normal;
	// Pannel intensity
	int panelIntensity;
	// 4 pointers to a vertex list     
    Point3D *VPoint[ 4 ];
	// Panel behavior data
    PanelBehavior Behavior;
	#ifdef COLLISION_DETECTION
    // The panel's X/Z extent
    float RadiusXZ;
    // The panel's Y extent         
    float RadiusY;
    float Radius;
    #endif
                      
    // Constructor
    Panel3D();
    // Initilize the panel
    bool InitPanel( PImageData pImageData, void *pShadeDat );
    // Write panel data to the file
    void WritePanelData( FILE *OutFile );
    // Read panel data from the file
    void ReadPanelData( FILE *InFile, int TextureCount );
    // Update normal and misc. information
    //void Update( Matrix3D &M );
    
    /************************************************************************
    *
    *  Why are there so many "RenderPolygon" functions?
    *
    *  In software 3D rendering, speed is a HUGE issue. Adding one "if" to the 
    *  very long function will result in milliseconds lost. That is why there
    *  are so many flavors of this one function to suite every need. It's not
    *  pretty, but this is the only time we break the rules for the need for speed. 
    *                                 
    ************************************************************************/
              
    // Blit panel texture to video buffer. 8 bit graphics to an 8 bit buffer
    void _fastcall RenderPolygon8To8Pos();
    void _fastcall RenderPolygon8To8Neg();
    void _fastcall RenderPolygon8To8PosTrans();
    void _fastcall RenderPolygon8To8NegTrans();
    // This flavor of the function uses an OR to simulate transparence
    void _fastcall RenderPolygon8To8PosOr();
    void _fastcall RenderPolygon8To8NegOr();
    // No Z buffering functions
    void _fastcall RenderPolygon8To8NoZBuffering();
    void _fastcall RenderPolygon8To8NoZBufferingOr();
    void _fastcall RenderPolygon8To8NoZBufferingTrans();
    // No Z buffering & No Shaded Table
    void _fastcall RenderPolygon16To16NoZBufNoShadedTbl();
    // Solid color rendering
    void _fastcall RenderPolygon8To8PosSolid();
    void _fastcall RenderPolygon8To8NegSolid();
    // Render with bounds checking
    void _fastcall RenderPolygon8To8PosCheck();

    // Blit panel texture to video buffer. 8 bit graphics to a 16 bit buffer
    void _fastcall RenderPolygon8To16Pos();
    void _fastcall RenderPolygon8To16Neg();
    void _fastcall RenderPolygon8To16PosTrans();
    void _fastcall RenderPolygon8To16NegTrans();
    // This flavor of the function uses an OR to simulate transparence
    void _fastcall RenderPolygon8To16PosOr();
    void _fastcall RenderPolygon8To16NegOr();
    // No Z buffering functions
    void _fastcall RenderPolygon8To16NoZBuffering();
    void _fastcall RenderPolygon8To16NoZBufferingOr();
    void _fastcall RenderPolygon8To16NoZBufferingTrans();
    // Solid color rendering
    void _fastcall RenderPolygon8To16PosSolid();
    void _fastcall RenderPolygon8To16NegSolid();
    // Render with bounds checking
    void _fastcall RenderPolygon8To16PosCheck();

    // Blit panel texture to video buffer. 16 bit graphics to a 16 bit buffer
    void _fastcall RenderPolygon16To16Pos();
    void _fastcall RenderPolygon16To16Neg();
    void _fastcall RenderPolygon16To16PosTrans();
    void _fastcall RenderPolygon16To16NegTrans();
    // This flavor of the function uses an OR to simulate transparence
    void _fastcall RenderPolygon16To16PosOr();
    void _fastcall RenderPolygon16To16NegOr();
    // No Z buffering functions
    void _fastcall RenderPolygon16To16NoZBuffering();
    void _fastcall RenderPolygon16To16NoZBufferingOr();
    void _fastcall RenderPolygon16To16NoZBufferingTrans();
    // Solid color rendering
    void _fastcall RenderPolygon16To16PosSolid();
    void _fastcall RenderPolygon16To16NegSolid();
          
    // 3-dimensional points onto the screen
    void Project();
	void Project16();
    // Calculates panel's intensity
    void _fastcall CalcStaticPolyInten( TLight &light );
    // Dynamicly Calculates panel's intensity
    void _fastcall CalcDynamicPolyInten( TLight &light );
    // Determines panel's visibility in 3D
    bool CalcVisible3D( bool DontBackFaceCul = false );     
    // Determines panel's visibility in 2D
    bool CalcVisible2D();
    // Checks for veritcies
    int HasVert( Point3D &P );
    // function for moving the texture around in the panel
    //void MoveText( int Ox, int Oy, int Tx, int Ty );
    // Rotate textures
    // inc the rotation index. It's only 2 bits so it
    // only goes to 3 then starts again
    void RotTexture(){ RotText(); ++Behavior.Rot; };
    // Calculates center Z
    double CalcCenterX();
	double CalcCenterY();
	double CalcCenterZ();
    // Check to see if the point is inside the polygon
    bool IsPointInsidePolygon( int X, int Y );
    // Outline the polygon in a color
    void WireFramePolygon8to8();
    void WireFramePolygon8to16Pos();
	void WireFramePolygon8to16Neg();
    void WireFramePolygon8to8Xor();
    void WireFramePolygon8to16Xor();
    // Init the point array with the polygon points
    void InitPolygonPoints( POINT *points );
    // Inc the animated polgon's frame counter
    void UpdateAnimatedPoly( unsigned int MSTime );
    // Set the textures attributes
    void SetTextAttrib( int Odd, int X, int Y, int Width, int Height );
    // Set the Texture Handle
    void SetTextureHandle( WORD Value ){ Behavior.TextHan = Value; };
    // Get the Texture Handle
    WORD GetTextureHandle(){ return Behavior.TextHan; };
    // Set the Frame Count
    void SetFrameCount( WORD Value ){ Behavior.FrameCount = Value; };
    // Get the Frame Count
    WORD GetFrameCount(){ return Behavior.FrameCount; };
    // Set the Frame Rate
    void SetFrameRate( WORD Value ){ Behavior.FrameRate = Value; };
    // Get the Frame Rate
    unsigned int GetFrameRate(){ return Behavior.FrameRate; };
    // Set if the back of the polygon is backfaced
    void SetNoCheckExten( bool Value ){ Behavior.NoCheckExten = Value; };
    // Get if the back of the polygon backface is checked
    bool GetNoCheckExten(){ return Behavior.NoCheckExten; };
    // Set if the polygon is set for solid color
    void SetSolidColor( bool Value ){ Behavior.SolidColor = Value; };
    // Get if the polygon is set for solid color
    inline bool GetSolidColor(){ return Behavior.SolidColor; };
    // Set if the polygon is to have it's bounds checked
    void SetCheckBounds( bool Value ){ Behavior.CheckBounds = Value; };
    // Get if the polygon is to have it's bounds checked
    bool GetCheckBounds(){ return Behavior.CheckBounds; };
    // Set the VPoint
    void SetVPoint( int Index, Point3D *Value ){ VPoint[ Index ] = Value; };
    // Get the VPoint
    Point3D *GetVPoint( int Index ){ return VPoint[ Index ]; };
    // Get the normal
    Vector *GetNormal(){ return &Normal; };
    // Get the Intensity of the panel
    WORD GetIntensity(){ return Behavior.Attrib[ 0 ].I; };
    // Set the intensenity of the texture
    void SetIntensity( WORD Value );
    // Read panel data from a pointer
    unsigned char *ReadPanelDataFromPointer( unsigned char *pData );
    // Calculates the panel normal 
    void CalcNormal();
    // Get the transparent value
    inline bool GetTransValue(){ return Behavior.Trans; };
    // Set the transparent value
    void SetTransValue( bool Value ){ Behavior.Trans = Value; };
    // Check for collision
    bool Collide( float &X, float &Y, float &Z, float Rad );
    // Calculates panel radius
    void CalcRadius();
    // Get the U value
    int GetU();
    // Get the V value
    int GetV();
    // Get the width value
    int GetUWidth();
    // Get the height value
    int GetVHeight();
};

typedef Panel3D *PPanel3D;

#endif
