/************************************************************************
*
*    PRODUCT:         3D Engine
*
*    FILE NAME:       PolyObj.cpp
*
*    DESCRIPTION:     Header file for a panel object class
*
*    Author: John De Goes
*
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include <Math.H>
#include <Stdio.H>

#include "PolyObj.h"
#include "Matrix3D.h"
#include "Point3D.h"
#include "TextType.h"
#include "Panel3D.h"

// Globals
extern int gIntScreenW, gIntScreenH;
extern int *gIntZBuffer;
extern void *pgVBuffer;
extern float gfCameraScale;
extern int gIntMaxZDistance;
extern int gIntMaxLightDistance;

// Don't show deprecation warnings
#pragma warning(disable : 4996)


/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TPolyObj::TPolyObj()
{
    Morph = 0;
    StartMorph = 1;
    VCount = PCount = 0;
    ScreenW = 0;
    ScreenH = 0;
    ZBuffer = NULL;
    pVBuffer = NULL;
    // Set the default lighting
    Light.SetLight( -100.0F, -100.0F, 500 );
    DoPolyZSort = false;
    CameraScale = DEF_SCALE;
    MaxZDistance = DEF_MAXZ;
    MaxLightDistance = DEF_MAXZ;
    Visible = 0;
    RadiusXY = RadiusXZ = Radius = 0.0F;
    VList = NULL;
    PList = NULL;
    MList = NULL;
    dynamicLighting = false;
}


/************************************************************************
*    FUNCTION NAME:              Destructor                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TPolyObj::~TPolyObj()
{
    Free();
}


/************************************************************************
*    FUNCTION NAME:         Free()                                                             
*
*    DESCRIPTION:           Free all allocations
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPolyObj::Free()
{
    if( PList )
    {
        delete [] PList;
        PList = NULL;
    }

    if( Morph )
    {
        delete [] MList;
        MList = NULL;
    }

    if( VList )
    {
        delete [] VList;
        VList = NULL;
    }
     
}   // Free


/************************************************************************
*    FUNCTION NAME:         GetLine()                                                             
*
*    DESCRIPTION:           Reads a line of text from a text file
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *InFile - File handle
*              char *String - String of file names
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int TPolyObj::GetLine ( FILE *InFile, char *String )
{
    // Reads a line of text from a text file:
    int NextByte;
    int Index = 0;

    // Reads a line of text from a text file:
    NextByte = fgetc ( InFile );

    // Loop until we reach a new-line character:
    while ( NextByte != '\n' )
    {
         // Check for end of file:
         if ( NextByte == EOF )
         {
            // If found, close off string
            // and return EOF:
            String[ Index ] = '\0';
            return EOF;
         }

         // If the next byte is not a space....
         if ( !isspace ( NextByte ) )
         {
            // ....record it:
            String [ Index++ ] = ( char ) NextByte;
         }
            
         // Get the next character:
         NextByte = fgetc( InFile );
    }

    // Close off the string and return success (true):
    String[ Index ] = '\0';

    return NextByte;

}   // GetLine


/************************************************************************
*    FUNCTION NAME:         Transform()                                                             
*
*    DESCRIPTION:           Translates/rotates/morphs entire vertex list
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
void TPolyObj::Transform( Matrix3D &M )
{
    // Transform the vertex list
    if( ( Morph ) && ( StartMorph ) )
    {
        for( unsigned int Count = 0; Count < VCount; ++Count )
        {
            M.Transform( VList[ Count ] );
            // Perform the morph:
            VList[ Count ] += MList[ Count ];
        }
    }
    else
    {
        for( unsigned int Count = 0; Count < VCount; ++Count )
            M.Transform( VList[ Count ] );
        
    }

    // Update the normal and other panel-specific data
    for( unsigned int Count = 0; Count < PCount; ++Count )
    {
        //PList[ Count ].Update( M );
        M.Transform( PList[ Count ].Normal );
    }


}   // Transform


/************************************************************************
*    FUNCTION NAME:         PaintWireFrame8to16()                                                             
*
*    DESCRIPTION:           Render the 3D object on screen buffer in wire frame.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int TPolyObj::PaintWireFrame8to16( Matrix3D &M, bool DontBackFaceCul )
{
    int VisiblePanCount = 0;
    unsigned int Timer = timeGetTime();

    // Transform all the vertices
    M.Transform( VList, VCount );

    // Transform all the panel normals
    M.Transform( PList, PCount );

    // Display selected panels in the off-screen buffer:
    for( unsigned int Count = 0; Count < PCount; ++Count )
    {
        // Update the frame counter if this is an animated polygon
        PList[ Count ].UpdateAnimatedPoly( Timer );

        // Eliminate panels that are not seen but show the ones
        // that have their backs to us
        if( PList[ Count ].CalcVisible3D( DontBackFaceCul ) )
        {                   
            // ....project it:
            PList[ Count ].Project();

            // Eliminate any panels that are not seen in a 2D space
            if( PList[ Count ].CalcVisible2D() )
            {
                // Draw the line                
                PList[ Count ].WireFramePolygon8to16Pos();

                // Inc the counter
                ++VisiblePanCount;
            }
        }
    }
       
    return VisiblePanCount;

}   // PaintWireFrame8to16


/************************************************************************
*    FUNCTION NAME:         PaintWireFrame8to8()                                                             
*
*    DESCRIPTION:           Render the 3D object on screen buffer in wire frame.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int TPolyObj::PaintWireFrame8to8( Matrix3D &M, bool DontBackFaceCul )
{
    int VisiblePanCount = 0;
    unsigned int Timer = timeGetTime();

    // Transform all the vertices
    M.Transform( VList, VCount );

    // Transform all the panel normals
    M.Transform( PList, PCount );

    // Display selected panels in the off-screen buffer:
    for( unsigned int Count = 0; Count < PCount; ++Count )
    {
        // Update the frame counter if this is an animated polygon
        PList[ Count ].UpdateAnimatedPoly( Timer );

        // Eliminate panels that are not seen but show the ones
        // that have their backs to us
        if( PList[ Count ].CalcVisible3D( DontBackFaceCul ) )
        {                   
            // ....project it:
            PList[ Count ].Project();

            // Eliminate any panels that are not seen in a 2D space
            if( PList[ Count ].CalcVisible2D() )
            {
                // Draw the line
                PList[ Count ].WireFramePolygon8to8();

                // Inc the counter
                ++VisiblePanCount;
            }
        }
    }
       
    return VisiblePanCount;

}   // PaintWireFrame8to8


/************************************************************************
*    FUNCTION NAME:         PaintVBuffer8To8()                                                             
*
*    DESCRIPTION:           Render the 3D object on screen buffer. 8 bit
*                           graphics to an 8 bit buffer
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int TPolyObj::PaintVBuffer8To8( Matrix3D &M, int ZBufferOption, int Start, int End, bool DontBackFaceCul, int RenderType )
{
    int Count, Range;
    int VisiblePanCount = 0;
    unsigned int Timer = timeGetTime();

    // Transform all the vertices
    M.Transform( VList, VCount );

    // Transform all the panel normals
    M.Transform( PList, PCount );

    // Set the range of polygons to render
    if( End )
        Range = End;
    else
        Range = PCount;

    // Display selected panels in the off-screen buffer:
    for( Count = Start; Count < Range; ++Count )
    {
        // Update the frame counter if this is an animated polygon
        PList[ Count ].UpdateAnimatedPoly( Timer );

        // Eliminate panels that are not seen or have their backs to us
        if( PList[ Count ].CalcVisible3D( DontBackFaceCul ) )
        {
            // Relight the panel if need be. This is a constant
            // moving light source and has to be calulated after
            // all the points have been traslated but before the
            // points have been projected.
            if( dynamicLighting )
                PList[ Count ].CalcDynamicPolyInten( Light );
                                               
            // ....project it:
            PList[ Count ].Project();

            // Eliminate any panels that are not seen in a 2D space
            if( PList[ Count ].CalcVisible2D() )
            {
                if( ZBufferOption == POS_ZBUFFER )
                {
                    if( PList[ Count ].GetTransValue() )
                    {
                        if( RenderType == STANDARD_IF_NOT_0 )
                            PList[ Count ].RenderPolygon8To8PosTrans();
                        else if( RenderType == PAINT_OR )
                            PList[ Count ].RenderPolygon8To8PosOr();
                    }
                    else
                    {
                        // See if we are texturing this with a solid color
                        if( PList[ Count ].GetSolidColor() )
                            PList[ Count ].RenderPolygon8To8PosSolid();
                        // See if we are checking the texture bounds
                        else if( PList[ Count ].GetCheckBounds() )
                            PList[ Count ].RenderPolygon8To8PosCheck();
                        else
                            PList[ Count ].RenderPolygon8To8Pos();
                    }
                }
                else if( ZBufferOption == NEG_ZBUFFER )
                {
                    if( PList[ Count ].GetTransValue() )
                    {
                        if( RenderType == STANDARD_IF_NOT_0 )
                            PList[ Count ].RenderPolygon8To8NegTrans();
                        else if( RenderType == PAINT_OR )
                            PList[ Count ].RenderPolygon8To8NegOr();
                    }
                    else
                    {
                        // See if we are texturing this with a solid color
                        if( PList[ Count ].GetSolidColor() )
                            PList[ Count ].RenderPolygon8To8NegSolid();
                        else
                            PList[ Count ].RenderPolygon8To8Neg();
                    }
                }
                else if( ZBufferOption == NO_ZBUFFERING )
                {
                    if( PList[ Count ].GetTransValue() )
                    {
                        if( RenderType == STANDARD_IF_NOT_0 )
                            PList[ Count ].RenderPolygon8To8NoZBufferingTrans();
                        else if( RenderType == PAINT_OR )
                            PList[ Count ].RenderPolygon8To8NoZBufferingOr();
                    }
                    else
                    {
                        PList[ Count ].RenderPolygon8To8NoZBuffering();
                    }
                }

                // Inc the counter
                ++VisiblePanCount;
            }
        }
    }
       
    return VisiblePanCount;

}   // PaintVBuffer8To8


/************************************************************************
*    FUNCTION NAME:         PaintVBuffer8To16()                                                             
*
*    DESCRIPTION:           Render the 3D object on screen buffer. 8 bit
*                           graphics to an 16 bit buffer
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

#ifdef IDENTIFY_BAD_POLYS
    short int identifiedBadPolyIndex;
#endif

int TPolyObj::PaintVBuffer8To16( Matrix3D &M, int ZBufferOption, int Start, int End, bool DontBackFaceCul, int RenderType )
{
    int Count, Range;
    int VisiblePanCount = 0;
    unsigned int Timer = timeGetTime();

    // Transform all the vertices
    M.Transform( VList, VCount );

    // Transform all the panel normals
    M.Transform( PList, PCount );

    // Set the range of polygons to render
    if( End )
        Range = End;
    else
        Range = PCount;

    // Display selected panels in the off-screen buffer:
    for( Count = Start; Count < Range; ++Count )
    {
        #ifdef IDENTIFY_BAD_POLYS
            if( POLYGON_COUNT_TO_CHECK == PCount )
                identifiedBadPolyIndex = Count;
            else
                identifiedBadPolyIndex = DONT_CHECK_PANEL;
        #endif

        // Update the frame counter if this is an animated polygon
        PList[ Count ].UpdateAnimatedPoly( Timer );

        // Eliminate panels that are not seen or have their backs to us
        if( PList[ Count ].CalcVisible3D( DontBackFaceCul ) )
        {
            // Relight the panel if need be. This is a constant
            // moving light source and has to be calulated after
            // all the points have been traslated but before the
            // points have been projected.
            if( dynamicLighting )
                PList[ Count ].CalcDynamicPolyInten( Light );
                                               
            // ....project it:
            PList[ Count ].Project();

            // Eliminate any panels that are not seen in a 2D space
            if( PList[ Count ].CalcVisible2D() )
            {
                if( ZBufferOption == POS_ZBUFFER )
                {
                    if( PList[ Count ].GetTransValue() )
                    {
                        if( RenderType == STANDARD_IF_NOT_0 )
                            PList[ Count ].RenderPolygon8To16PosTrans();
                        else if( RenderType == PAINT_OR )
                            PList[ Count ].RenderPolygon8To16PosOr();
                    }
                    else
                    {
                        // See if we are texturing this with a solid color
                        if( PList[ Count ].GetSolidColor() )
                        {
                            PList[ Count ].RenderPolygon8To16PosSolid();
                        }
                        // See if we are checking the texture bounds
                        else if( PList[ Count ].GetCheckBounds() )
                            PList[ Count ].RenderPolygon8To16PosCheck();
                        else
                            PList[ Count ].RenderPolygon8To16Pos();
                    }
                }
                else if( ZBufferOption == NEG_ZBUFFER )
                {
                    if( PList[ Count ].GetTransValue() )
                    {
                        if( RenderType == STANDARD_IF_NOT_0 )
                            PList[ Count ].RenderPolygon8To16NegTrans();
                        else if( RenderType == PAINT_OR )
                            PList[ Count ].RenderPolygon8To16NegOr();
                    }
                    else
                    {
                        // See if we are texturing this with a solid color
                        if( PList[ Count ].GetSolidColor() )
                        {
                            PList[ Count ].RenderPolygon8To16NegSolid();
                        }
                        else
                            PList[ Count ].RenderPolygon8To16Neg();
                    }
                }
                else if( ZBufferOption == NO_ZBUFFERING )
                {
                    if( PList[ Count ].GetTransValue() )
                    {
                        if( RenderType == STANDARD_IF_NOT_0 )
                            PList[ Count ].RenderPolygon8To16NoZBufferingTrans();
                        else if( RenderType == PAINT_OR )
                            PList[ Count ].RenderPolygon8To16NoZBufferingOr();
                    }
                    else
                    {
                        PList[ Count ].RenderPolygon8To16NoZBuffering();
                    }
                }

                // Inc the counter
                ++VisiblePanCount;
            }
        }
    }
       
    return VisiblePanCount;

}   // PaintVBuffer8To16


/************************************************************************
*    FUNCTION NAME:         PaintVBuffer16To16()                                                             
*
*    DESCRIPTION:           Render the 3D object on screen buffer. 16 bit
*                           graphics to an 16 bit buffer
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int TPolyObj::PaintVBuffer16To16( Matrix3D &M, int ZBufferOption, int Start, int End, bool DontBackFaceCul, int RenderType )
{
    int Range;
    int VisiblePanCount = 0;
    unsigned int Timer = timeGetTime();

    // Transform all the vertices
    M.Transform( VList, VCount );

    // Transform all the panel normals
    M.Transform( PList, PCount );

    // Set the range of polygons to render
    if( End )
        Range = End;
    else
        Range = PCount;

    // Display selected panels in the off-screen buffer:
    for( int Count = Start; Count < Range; ++Count )
    {
        // Update the frame counter if this is an animated polygon
        PList[ Count ].UpdateAnimatedPoly( Timer );

        // Eliminate panels that are not see or have their backs to us
        if( PList[ Count ].CalcVisible3D( DontBackFaceCul ) )
        {
            // Relight the panel if need be. This is a constant
            // moving light source and has to be calulated after
            // all the points have been traslated but before the
            // points have been projected.
            if( dynamicLighting )
                PList[ Count ].CalcDynamicPolyInten( Light );
                                                   
            // ....project it:
            PList[ Count ].Project16();

            // Eliminate any panels that are not seen in a 2D space
            if( PList[ Count ].CalcVisible2D() )
            {                
                if( ZBufferOption == POS_ZBUFFER )
                {
                    if( PList[ Count ].GetTransValue() )
                    {
                        if( RenderType == STANDARD_IF_NOT_0 )
                            PList[ Count ].RenderPolygon16To16PosTrans();
                        else if( RenderType == PAINT_OR )
                            PList[ Count ].RenderPolygon16To16PosOr();
                    }
                    else
                    {
                        // See if we are texturing this with a solid color
                        if( PList[ Count ].GetSolidColor() )
                            PList[ Count ].RenderPolygon16To16PosSolid();
                        else
                            PList[ Count ].RenderPolygon16To16Pos();
                    }
                }
                else if( ZBufferOption == NEG_ZBUFFER )
                {
                    if( PList[ Count ].GetTransValue() )
                    {
                        if( RenderType == STANDARD_IF_NOT_0 )
                            PList[ Count ].RenderPolygon16To16NegTrans();
                        else if( RenderType == PAINT_OR )
                            PList[ Count ].RenderPolygon16To16NegOr();
                    }
                    else
                    {
                        // See if we are texturing this with a solid color
                        if( PList[ Count ].GetSolidColor() )
                            PList[ Count ].RenderPolygon16To16NegSolid();
                        else
                            PList[ Count ].RenderPolygon16To16Neg();
                    }
                }
                else if( ZBufferOption == NO_ZBUFFERING )
                {
                    if( PList[ Count ].GetTransValue() )
                    {
                        if( RenderType == STANDARD_IF_NOT_0 )
                            PList[ Count ].RenderPolygon16To16NoZBufferingTrans();
                        else if( RenderType == PAINT_OR )
                            PList[ Count ].RenderPolygon16To16NoZBufferingOr();
                    }
                    else
                    {
                        if( RenderType == NO_SHADED_TABLE )
                            PList[ Count ].RenderPolygon16To16NoZBufNoShadedTbl();
                        else
                            PList[ Count ].RenderPolygon16To16NoZBuffering();
                    }
                }

                // Inc the counter
                ++VisiblePanCount;
            }
        }
    }
   
    return VisiblePanCount;

}   // PaintVBuffer16To16


/************************************************************************
*    FUNCTION NAME:         RenderSinglePoly()                                                             
*
*    DESCRIPTION:           Render a singlr polygon
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPolyObj::RenderSinglePoly( int Polygon, int ZBufferOption )
{
    PList[ Polygon ].RenderPolygon8To8Pos();

}   // PaintVBuffer
 

/************************************************************************
*    FUNCTION NAME:         GetSelectedPolygon()                                                             
*
*    DESCRIPTION:           Return the index to the selected polygon
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int TPolyObj::GetSelectedPolygon( int X, int Y )
{
    int Result = -1;
    double zDist = 0xFFFFFFF;

    // Cycle through all the polygons and weed out all that don't apply
    for( unsigned int Count = 0; Count < PCount; ++Count )
    {
        // Eliminate panels that are not see or have their backs to us
        if( PList[ Count ].CalcVisible3D() )
        {
            // Eliminate any panels that are not see in a 2D space
            if( PList[ Count ].CalcVisible2D() )
            {
                if( PList[ Count ].IsPointInsidePolygon( X, Y ) )
                {
                    // Find the lowest index
                    if( zDist > PList[ Count ].CalcCenterZ() )
                    {
                        zDist = PList[ Count ].CalcCenterZ();
                        Result = Count;
                    }
                }
            }
        }
    }
       
    return Result;

}   // GetSelectedPolygon


/************************************************************************
*    FUNCTION NAME:         OutlinePolygon8to8()                                                             
*
*    DESCRIPTION:           Outline the polygon in a color
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPolyObj::OutlinePolygon8to8( int Polygon )
{
    if( PList[ Polygon ].CalcVisible3D( false ) )
        if( PList[ Polygon ].CalcVisible2D() )
            PList[ Polygon ].WireFramePolygon8to8Xor();

}   // OutlinePolygon8to8


/************************************************************************
*    FUNCTION NAME:         OutlinePolygon8to8()                                                             
*
*    DESCRIPTION:           Outline the polygon in a color
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPolyObj::OutlinePolygon8to16( int Polygon )
{
    if( PList[ Polygon ].CalcVisible3D( false ) )
        if( PList[ Polygon ].CalcVisible2D() )
            PList[ Polygon ].WireFramePolygon8to16Xor();

}   // OutlinePolygon8to8


/************************************************************************
*    FUNCTION NAME:         MoveText()                                                             
*
*    DESCRIPTION:           function for moving the texture around in the panel
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
/*
void TPolyObj::MoveText( int Index, int Ox, int Oy, int Tx, int Ty )
{
    // Make sure selection is in allowed range:
    PList[ Index ].MoveText( Ox, Oy, Tx, Ty );

}   // MoveText
*/

/************************************************************************
*    FUNCTION NAME:         ReInitPanels()                                                             
*
*    DESCRIPTION:           This function sets up the panels with a default
*                           texture and texture information.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPolyObj::ReInitPanels()
{
    // Init the globals
    gIntScreenW = ScreenW;
    gIntScreenH = ScreenH;
    gIntZBuffer = ZBuffer;
    pgVBuffer = pVBuffer;
    gfCameraScale = CameraScale;
    gIntMaxZDistance = MaxZDistance;
    gIntMaxLightDistance = MaxLightDistance;
    
    for( unsigned int Count = 0; Count < PCount; ++Count )
    {
        // Initilize the panel object
        PList[ Count ].InitPanel( pTextDat, pTextShade );
    }

}   // ReInitPanels


/************************************************************************
*    FUNCTION NAME:         InitDefText()                                                             
*
*    DESCRIPTION:           When loading a new map, this function sets up
*                           the panels with a default texture and texture
*                           information.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPolyObj::InitDefText()
{
    // Init the globals
    gIntScreenW = ScreenW;
    gIntScreenH = ScreenH;
    gIntZBuffer = ZBuffer;
    pgVBuffer = pVBuffer;
    gfCameraScale = CameraScale;
    gIntMaxZDistance = MaxZDistance;
    gIntMaxLightDistance = MaxLightDistance;

    for( unsigned int Count = 0; Count < PCount; ++Count )
    {
        // Initilize the panel object
        PList[ Count ].InitPanel( pTextDat, pTextShade );

        // Set the attributes
        PList[ Count ].SetTextAttrib( Count % 2, 0, 0, 
                                        pTextDat->TMap[ 0 ].Width,
                                        pTextDat->TMap[ 0 ].Height );
    }
}   // InitDefText


/************************************************************************
*    FUNCTION NAME:         MorphTo()                                                             
*
*    DESCRIPTION:           Function for morphing
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool TPolyObj::MorphTo( char *FileName, float Steps )
{
    int VertCount, PanelCount, Count;
    Point3D TVert;
    FILE *InFile;

    StartMorph = 1;

    InFile = fopen( FileName, "rb" );
    if( !InFile )
        return false;

    fread( &VertCount, sizeof VertCount, 1, InFile );
    fread( &PanelCount, sizeof PanelCount, 1, InFile );

    if( VertCount != VCount )
    {
        fclose( InFile );
        return 0;
    }

    // Allocate memory
    TList = new Point3D[ VertCount ];
    if( !TList )
        return false;

    if( !Morph )
    {
        Morph = 1;
        MList = new Point3D [ VertCount ];
        if( !MList )
            return false;
    }

    // Load vertices:
    for ( Count = 0; Count < VertCount; ++Count )
        TList [ Count ].Read ( InFile );

    // Generate morphing data to morph VList to TList:
    for ( Count = 0; Count < VertCount; ++Count )
        MList [ Count ] = ( TList [ Count ] - VList [ Count ] ) / Steps;

    // Delete memory:
    delete [] TList;

    // Close file:
    fclose( InFile );

    // Return success:
    return true;

}   // MorphTo


/************************************************************************
*    FUNCTION NAME:         MorphRev()                                                             
*
*    DESCRIPTION:           Reverse the direction of the morph
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPolyObj::MorphRev()
{
    unsigned int Count;

    for( Count = 0; Count < VCount; ++Count )
        MList[ Count ] = MList[ Count ] * -1.0F;

}   // MorphRev


/************************************************************************
*    FUNCTION NAME:         WRITE()                                                             
*
*    DESCRIPTION:           Reverse the direction of the morph
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void WRITE( char *Message, float Value )
{
    static int Opened = 0;
    FILE *File;

    if( !Opened )
    {
        File = fopen ( "Debug.DAT", "wt" );
        Opened = 1;
    }
    else
        File = fopen ( "Debug.DAT", "at" );

    fprintf( File, Message, Value );
    fclose( File );
}


/************************************************************************
*    FUNCTION NAME:         Collide()                                                             
*
*    DESCRIPTION:           Check cor collision
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

PPanel3D TPolyObj::Collide( int &index, float &X, float &Y, float &Z, float Rad )
{
    PPanel3D result = NULL;

    for( unsigned int count = index; count < PCount; ++count )
    {
        if( PList[ count ].Collide( X, Y, Z, Rad ) )
        {
            result = &PList[ count ];
            index = count;
            break;
        }
    }

    return result;

}   // Collide


/************************************************************************
*    FUNCTION NAME:         LoadCoord()                                                             
*
*    DESCRIPTION:           Loads a single coordinate from a text file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

Point3D TPolyObj::LoadCoord( FILE *InFile )
{
    int PCount = 0;
    char String [ 100 ] = "";
    Point3D Coord;

    while( PCount < 3 )
    {
        if( GetLine( InFile, String ) == EOF )
        {
            Coord.Lx = 0;
            Coord.Ly = 0;
            Coord.Lz = 0;
            break;
        }

        // Check for the first X coordinate:
        if( !strcmp( String, "10" ) )
        {
            GetLine( InFile, String );
            Coord.Lx = atof( String );
            ++PCount;
        }

        // Check for the second X coordinate:
        else if( !strcmp( String, "11" ) )
        {
            GetLine( InFile, String );
            Coord.Lx = atof( String );
            ++PCount;
        }
        // Check for the third X coordinate:
        else if( !strcmp( String, "12" ) )
        {
            GetLine( InFile, String );
            Coord.Lx = atof( String );
            ++PCount;
        }
        // Check for the fourth X coordinate:
        else if( !strcmp( String, "13" ) )
        {
            GetLine( InFile, String );
            Coord.Lx = atof( String );
            ++PCount;
        }
        // Check for the first Y coordinate:
        else if( !strcmp( String, "20" ) )
        {
            GetLine( InFile, String );
            Coord.Ly = atof( String );
            ++PCount;
        }
        // Check for the second Y coordinate:
        else if( !strcmp( String, "21" ) )
        {
            GetLine( InFile, String );
            Coord.Ly = atof( String );
            ++PCount;
        }
        // Check for the third Y coordinate:
        else if( !strcmp( String, "22" ) )
        {
            GetLine( InFile, String );
            Coord.Ly = atof( String );
            ++PCount;
        }
        // Check for the fourth Y coordinate:
        else if( !strcmp( String, "23" ) )
        {
            GetLine( InFile, String );
            Coord.Ly = atof( String );
            ++PCount;
        }
        // Check for the first Z coordinate:
        else if( !strcmp( String, "30" ) )
        {
            GetLine( InFile, String );
            Coord.Lz = atof( String );
            ++PCount;
        }
        // Check for the second Z coordinate:
        else if( !strcmp( String, "31" ) )
        {
            GetLine( InFile, String );
            Coord.Lz = atof( String );
            ++PCount;
        }
        // Check for the third Z coordinate:
        else if( !strcmp( String, "32" ) )
        {
            GetLine( InFile, String );
            Coord.Lz = atof( String );
            ++PCount;
        }
        // Check for the fourth Z coordinate:
        else if( !strcmp( String, "33" ) )
        {
            GetLine( InFile, String );
            Coord.Lz = atof( String );
            ++PCount;
        }
    }

/*  if(pCount == 0)  TEST CODE
    {
        sprintf( String, "%d.\r\n", pCount );
        WriteDataToFile( "c:\\temp1\\test.txt", String, strlen(String), OPEN_ALWAYS );
        ++pCount;
    }

    sprintf( String, "%f, %f, %f\r\n", Coord.Lx, Coord.Ly, Coord.Lz );
    WriteDataToFile( "c:\\temp1\\test.txt", String, strlen(String), OPEN_ALWAYS );
    gCount = (gCount + 1) % 4;

    if(gCount == 0)
    {
        sprintf( String, "%d.\r\n", pCount );
        WriteDataToFile( "c:\\temp1\\test.txt", String, strlen(String), OPEN_ALWAYS );
        ++pCount;
    }
*/
    // Return a copy of the loaded vertex:
    return Coord;

}   // LoadCoord


/************************************************************************
*    FUNCTION NAME:         CountPanels()                                                             
*
*    DESCRIPTION:            Count the number of panels in a text file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

unsigned int TPolyObj::CountPanels ( char *FileName )
{
    char String[ 100 ] = "";
    unsigned int PanelCount = 0;
    FILE *InFile;

    // Open the file
    InFile = fopen ( FileName, "rt" );
    if( !InFile )
        return 0;

    // Loop until we come to the end of the file:
    for( ;; )
    {
        // Get a line of text:
        if( GetLine( InFile, String ) == EOF )
            break;

        // Search for a panel (3DFACE):
        if( !strcmp ( String, "3DFACE" ) )
            ++PanelCount;
    }

    // Close the file:
    fclose( InFile );

    // Return number of vertices:
    return PanelCount;

}   // CountPanels


/************************************************************************
*    FUNCTION NAME:         LoadVerts()                                                             
*
*    DESCRIPTION:           Load all vertices from a DXF text file and
*                           Declare/initialize variables
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
   
bool TPolyObj::LoadVerts( char *FileName )
{
    int VIndex = 0, Count;
    FILE *InFile;
    char String [ 100 ] = "";

    PCount = CountPanels ( FileName );
    if( !PCount )
        return false;

    // Allocate memory for temporary list
    TList = new Point3D[ PCount * 4 ];
    if( !TList )
        return false;

    // Open file - abort if error
    InFile = fopen ( FileName, "rt" );
    if( !InFile )
        return false;
  
    // Loop until we come to the end of the file:
    for( ;; )
    {
        // Get a line of text:
        if( GetLine ( InFile, String ) == EOF )
            break;

        // If a 3DFACE entity is encountered....
        if( !strcmp( "3DFACE", String ) )
        {
            // Load four vertices:
            for( Count = 0; Count < 4; ++Count )
            {
                // Load the next coordinate:
                Point3D TPoint3D = LoadCoord ( InFile );
                TList [ VIndex++ ] = TPoint3D;   
            }
        }
    }

    // Close the file:
    fclose( InFile );

    // Set the vertex count at zero:
    VCount = 0;

    // Allocate memory for vertex list - abort if error
    VList = new Point3D[ VIndex ];
    if( !VList )
        return false;

    // Copy TList to VList:
    for( Count = 0; Count < VIndex; ++Count )
    {
        if( UniqueVert( TList [ Count ], VList, VCount ) )
        {
            VList [ VCount ] = TList [ Count ];

            // Increment the vertex count/index:
            ++VCount;
        }
    }

    return true;

}   // LoadVerts


/************************************************************************
*    FUNCTION NAME:         LoadPanelData()                                                             
*
*    DESCRIPTION:           Read the panel data from a DXF file. Note: Assumes 
*                           all vertices have been loaded via member function "LoadVerts"
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool TPolyObj::LoadPanelData()
{
    int VIndex = 0;
    bool Result = true;

    // Allocate memory for files
    PList = new Panel3D[ PCount ];

    if( !TList || !PList )
        return false;

    // Loop until we come to the end of the file:
    for( unsigned int i = 0; i < PCount; ++i )
    {
        // Load an entire panel:
        Result = LoadPanel( VIndex, i );

        if( !Result )
            break;
    }

    // Deallocate temporary list data
    delete [] TList;

    return Result;

}   // LoadPanelData


/************************************************************************
*    FUNCTION NAME:         LoadPanel()                                                             
*
*    DESCRIPTION:           Load a panel from a DXF text file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool TPolyObj::LoadPanel( int &VIndex, int Index )
{
    // 3DFACE has been found - load four coordinates:
    for( int Count = 0; Count < 4; ++Count )
    {
        // Assign a vertex to the panel:
        PList[ Index ].SetVPoint( Count, &VList[ GetVertIndex (  TList[ VIndex ], VList, VCount ) ] );

        // Increment the vertex index:
        ++VIndex;
    }

    // Calculate panel normal:
    PList[ Index ].CalcNormal();

    // Calculate panel's intensity:
    PList[ Index ].CalcStaticPolyInten( Light );

    return true;

}   // LoadPanel


/************************************************************************
*    FUNCTION NAME:         LoadDXF()                                                             
*
*    DESCRIPTION:           Load a panel from a DXF text file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool TPolyObj::LoadDXF( char *FileName )
{
    bool Result = false;

    // Load an object from a DXF text file:
    if( LoadVerts( FileName ) && LoadPanelData() )
    {
        // Sort by Z order
        Sort();
        Result = true;
    }

    return Result;

}   // LoadDXF


/************************************************************************
*    FUNCTION NAME:         LoadPolygonObjFile()                                                             
*
*    DESCRIPTION:           Load an object from a BIN binary file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool TPolyObj::LoadPolygonObjFile( char *FileName )
{
    // Data types for the file format. Don't change.
    unsigned int VertCount, PanelCount, PIndex;

    FILE *InFile;

    // Free all allocations
    Free();

    if( ( InFile = fopen ( FileName, "rb" ) ) == NULL )
        return false;

    // Read in the first two ints for the number panels and verticies
    fread( &VertCount, sizeof VertCount, 1, InFile );
    fread( &PanelCount, sizeof PanelCount, 1, InFile );

    VCount = VertCount;
    PCount = PanelCount;

    // Allocate memory
    VList = new Point3D [ VertCount ];
    PList = new Panel3D [ PanelCount ]; 

    // Load vertices:
    for( unsigned int Count = 0; Count < VertCount; ++Count )
        VList [ Count ].Read ( InFile );

    // Load panel data:
    for( unsigned int Count = 0; Count < PCount; ++Count )
    {
        for( unsigned int VIndex = 0; VIndex < 4; ++VIndex )
        {
            fread( &PIndex, sizeof PIndex, 1, InFile );
            PList[ Count ].SetVPoint( VIndex, &VList[ PIndex ] );
        }

        // Read the panel normal
        fread( PList[ Count ].GetNormal(), sizeof( Vector ), 1, InFile );

        // Calculate the panel radious
        #ifdef COLLISION_DETECTION
            PList[ Count ].CalcRadius();
        #endif

        // Calculate the panel's intensity:
        PList[ Count ].CalcStaticPolyInten( Light );
    }

    fclose( InFile );

    return true;

}   // LoadPolygonObjFile


/************************************************************************
*    FUNCTION NAME:         CalculateShading()                                                             
*
*    DESCRIPTION:           Calculate the shading for all vertexes
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPolyObj::CalculateShading()
{
    unsigned int SharedIndexCount;
    unsigned int SharedColor;
    unsigned int SharedIndex [ 100 ];
	Point3D TVert;

   // Calculate Gouraud information for all polygons:
   for( unsigned int  Count = 0; Count < PCount; Count++ )
   {
       // Calculate an intensity value for each of the vertices:
       for( unsigned int  VIndex = 0; VIndex < 4; VIndex++ )
       {
           SharedIndexCount = 0;
           SharedColor = 0;
           TVert = *PList [ Count ].VPoint [ VIndex ];
           // Search through the entire list of panels looking 
           // for shared vertices:
           for( unsigned int  N = 0; N < PCount; N++ )
           {
               if ( PList [ N ].HasVert ( TVert ) )
               {
                  SharedIndex [ SharedIndexCount++ ] = N;
               }
           }

           for( unsigned int  N = 0; N < SharedIndexCount; N++ )
           {
               SharedColor += PList [ SharedIndex [ N ] ].panelIntensity;
           }

           if ( !SharedIndexCount )
              SharedIndexCount = 1;

           SharedColor /= SharedIndexCount;
           PList[ Count ].Behavior.Attrib [ VIndex ].I = ( WORD ) SharedColor;
       }
   }
}


/************************************************************************
*    FUNCTION NAME:         SavePolygonObjFile()                                                             
*
*    DESCRIPTION:           Write a BIN binary file
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool TPolyObj::SavePolygonObjFile( char *FileName )
{
    // Write a BIN binary file:
    // Data types for the file format. Don't change.
    unsigned int VertCount = VCount, PanelCount = PCount, PIndex;

    Point3D *PVert;
    FILE *OutFile;

    if( ( OutFile = fopen ( FileName, "wb" ) ) == NULL )
        return false;

    fwrite( &VertCount, sizeof VertCount, 1, OutFile );
    fwrite( &PanelCount, sizeof PanelCount, 1, OutFile );

    // Write vertex data:
    for( unsigned int Count = 0; Count < VCount; ++Count )
        VList [ Count ].Write ( OutFile );

    // Write panel data:
    for( unsigned int Count = 0; Count < PCount; ++Count )
    {
        for( unsigned int VIndex = 0; VIndex < 4; ++VIndex )
        {
            PVert = PList[ Count ].GetVPoint( VIndex );
            PIndex = GetVertIndex( *PVert, VList, VCount );
            fwrite ( &PIndex, sizeof PIndex, 1, OutFile );
        }

        // Write the panel normal
        fwrite( PList[ Count ].GetNormal(), sizeof( Vector ), 1, OutFile );
    }

    fclose( OutFile );

    return true;

}   // SavePolygonObjFile


/************************************************************************
*    FUNCTION NAME:         ReadText()                                                             
*
*    DESCRIPTION:           Read in the texture coordinates
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool TPolyObj::ReadTextCoord( char *FileName )
{
    FILE *InFile;

    if( ( InFile = fopen ( FileName, "rb" ) ) == 0 )
        return false;

    // Init the globals
    gIntScreenW = ScreenW;
    gIntScreenH = ScreenH;
    gIntZBuffer = ZBuffer;
    pgVBuffer = pVBuffer;
    gfCameraScale = CameraScale;
    gIntMaxZDistance = MaxZDistance;
    gIntMaxLightDistance = MaxLightDistance;

    // Load the texture coordinates and handles:
    for( unsigned int Count = 0; Count < PCount; ++Count )
    {
        // Load the panel data from file
        PList[ Count ].ReadPanelData( InFile, pTextDat->TCount );

        // Initilize the panel object
        PList[ Count ].InitPanel( pTextDat, pTextShade );
    }

    fclose( InFile );

    return true;

}   // ReadText


/************************************************************************
*    FUNCTION NAME:         LoadCrdFromPointer()                                                             
*
*    DESCRIPTION:           Read in the texture coordinates from a pointer
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPolyObj::LoadCrdFromPointer( unsigned char *pData )
{
    // Init the globals
    gIntScreenW = ScreenW;
    gIntScreenH = ScreenH;
    gIntZBuffer = ZBuffer;
    pgVBuffer = pVBuffer;
    gfCameraScale = CameraScale;
    gIntMaxZDistance = MaxZDistance;
    gIntMaxLightDistance = MaxLightDistance;

    // Load the texture coordinates and handles:
    for( unsigned int Count = 0; Count < PCount; ++Count )
    {
        // Load the panel data from file
        pData = PList[ Count ].ReadPanelDataFromPointer( pData );

        // Initilize the panel object
        PList[ Count ].InitPanel( pTextDat, pTextShade );
    }

}   // ReadText


/************************************************************************
*    FUNCTION NAME:         WriteTextCoord()                                                             
*
*    DESCRIPTION:           Write out the texture coordinates
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool TPolyObj::WriteTextCoord( char *FileName )
{
    FILE *OutFile;

    if( ( OutFile = fopen ( FileName, "wb" ) ) == 0 )
        return false;

    // Save the texture coordinates, data and handles
    for( unsigned int Count = 0; Count < PCount; ++Count )
    {        
        PList[ Count ].WritePanelData( OutFile );
    }

    fclose( OutFile );

    return true;

}   // WriteText


/************************************************************************
*    FUNCTION NAME:         RotText()                                                             
*
*    DESCRIPTION:           Rotate the texture in the panel
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPolyObj::RotateTexture( int Polygon )
{
    if( Polygon > -1 )
        PList[ Polygon ].RotTexture();

}   // RotText


/************************************************************************
*    FUNCTION NAME:         SetTexture()                                                             
*
*    DESCRIPTION:           Selects a texture for a given polygon
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPolyObj::SetTexture( int Polygon, int TextIndex, int X, int Y, int Width, int Height )
{
    if( Polygon > -1 && Polygon < (int)PCount )
    {
        // Set the value of the texture handle
        PList[ Polygon ].SetTextureHandle( TextIndex );

        // Set the width
        if( (Width == 0) || (X + Width > pTextDat->TMap[ TextIndex ].Width) )
            Width = pTextDat->TMap[ TextIndex ].Width;
        else
            Width = X + Width;

        // Set the height
        if( (Height == 0) || (Y + Height > pTextDat->TMap[ TextIndex ].Height) )
            Height = pTextDat->TMap[ TextIndex ].Height;
        else
            Height = Y + Height;

        // Set the attributes
        PList[ Polygon ].SetTextAttrib( Polygon % 2, X, Y, Width, Height );
    }

}   // SetTexture

/************************************************************************
*    FUNCTION NAME:         LoadFromPointer                                                             
*
*    DESCRIPTION:           Load an object from data pointer
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

bool TPolyObj::LoadPlyFromPointer( unsigned char *pData )
{
    unsigned int PIndex;
    PPolyHeader pPolyHeader;
    unsigned int Count, VIndex;

    // Free all allocations
    Free();

    // Get the pointer to the poly header
    pPolyHeader = PPolyHeader( pData );

    VCount = pPolyHeader->VertCount;
    PCount = pPolyHeader->PanelCount;

    // inc the data pointer
    pData += sizeof( PolyHeader );

    // Allocate memory
    VList = new Point3D[ VCount ];
    PList = new Panel3D[ PCount ];
    
    if( !VList || !PList )
        return false; 

    // Load vertices:
    for( Count = 0; Count < VCount; ++Count )
        pData = VList[ Count ].LoadFromPointer( pData );

    // Load panel data:
    for( Count = 0; Count < PCount; ++Count )
    {
        for( VIndex = 0; VIndex < 4; ++VIndex )
        {
            // Get the index value
            PIndex = *(unsigned int *)pData;
            // Save the index
            PList[ Count ].SetVPoint( VIndex, &VList[ PIndex ] );
            // Inc the pointer
            pData += sizeof( unsigned int );
        }

        // Read the panel normal
        memcpy( PList[ Count ].GetNormal(), pData, sizeof( Vector ) );

        // Calculate the panel radious
        #ifdef COLLISION_DETECTION
            PList[ Count ].CalcRadius();
        #endif

        // Inc the pointer
        pData += sizeof( Vector );
    }

    return true;

}   // LoadPlyFromPointer


/************************************************************************
*    FUNCTION NAME:         SetLightSource                                                             
*
*    DESCRIPTION:           Set the light source. ReCalcInten
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPolyObj::SetLightSource( float X, float Y, float Z, bool enambleDynamicLighting )
{
    // Set the default lighting
    // This also does calculations
    Light.SetLight( X, Y, Z );

    // Calculate panel's intensity:
    if( enambleDynamicLighting )
    {
        // This type of lighting change needs to be done
        // after all the points have been transformed but
        // before the polygon is projected. So we set a flag here
        // so that only the visable polygons will be calcualetd.
        dynamicLighting = true;
    }
    else
    {
        // This calculation can be done at any time because
        // it deals with the constant polygon data that doesn't change
        for( unsigned int Index = 0; Index < PCount; ++Index )
            PList[ Index ].CalcStaticPolyInten( Light );
    }

}   // SetLightSource


/************************************************************************
*    FUNCTION NAME:         Sort                                                             
*
*    DESCRIPTION:           Sort all the panels
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TPolyObj::Sort()
{
    int offset, j, x=0;
    bool inOrder;
    Panel3D tmpPanel;

    // Make sure we have something to sort
    if( PCount > 1 )
    {
        inOrder = false;
        offset = PCount;

        while( x < offset && inOrder == false )
        {
            ++x;
            inOrder = true;
            for( j = 0; j < offset - x; ++j )
            {
                if( PList[j].CalcCenterZ() > PList[j+1].CalcCenterZ()  )
                {
                    inOrder = false;
                    memcpy( &tmpPanel, &PList[j], sizeof( Panel3D ) );
                    memcpy( &PList[j], &PList[j+1], sizeof( Panel3D ) );
                    memcpy( &PList[j+1], &tmpPanel, sizeof( Panel3D ) );
                }
            }
        }
    }

}   // Sort