//
// File name: Panel3D.CPP
//
// Description: The support files for the Panel3D.h header
//
// Author: John De Goes
//
// Project: Cutting SPoint 3D Game Programming
//

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

#include <Math.H>
#include <Stdlib.H>
#include <String.H>

#include "Panel3D.h"
#include "FixASM.h"
#include "TextType.h"
#include "PalShade.h"

// test
//#include "gtools.h"


const MINX  =  0;
const MINY  =  0;
const MINZ  =  100.0F;
const COLOR_RANGE = 8;
const COLOR_START = 8;

// TSTEP_PREC and ZSTEP_PREC must be equal:
const ZSTEP_PREC = 26;
const TSTEP_PREC = ZSTEP_PREC;
const TSTEP_PREC_SL1 = ( 1 << TSTEP_PREC );
const ISTEP_PREC = 8;
const TMASK      = ( 0xFFFFFFFF << 16 );


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

Panel3D::Panel3D()
{
    SPCount = 0;
    ScreenW = 0;
    ScreenH = 0;
    PreMultShift = 0;
    FrameCounter = 0;
    FrameTimer = 0;
    MaxLightDistance = 30000;
    MaxZDistance = 30000;

}   // Constructer


/************************************************************************
*    FUNCTION NAME:         ClipHLine()                                                             
*
*    DESCRIPTION:           Clip a horizontal "Z-buffered" line
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
void Panel3D::ClipHLine( int &X1, int &X2, int &Z, int ZStep,
                         int &U, int UStep, int &V, int VStep,
                         int &I, int IStep )
{
    //int register Step;

    // Clip a horizontal "Z-buffered" line:
    if( X1 < MINX )
    {
        int register Step ( MINX - X1 );
        // Take advantage of the fact that ( a * ( b * f ) / f ) 
        // is equal to ( a * b );
        Z += ZStep * Step;
        U += UStep * Step;
        V += VStep * Step;
        I += IStep * Step;
        X1 = MINX;
    }
    else if( X1 > ScreenW )
        X1 = ScreenW;

    if ( X2 < MINX )
        X2 = MINX;

    else if ( X2 > ScreenW )
        X2 = ScreenW;

}   // ClipHLine
*/

/************************************************************************
*    FUNCTION NAME:         CalcRadius()                                                             
*
*    DESCRIPTION:           Calculate the radius of the panel
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

void Panel3D::CalcRadius()
{
    #ifdef COLLISION_DETECTION

    Point3D TempPoint[ 4 ], Center;
    int Count;
    double Distance[ 4 ], Dist;

    Center.Lx = ( VPoint[ 0 ]->Lx +
                 VPoint[ 1 ]->Lx +
                 VPoint[ 2 ]->Lx +
                 VPoint[ 3 ]->Lx ) / 4.0F;

    Center.Ly = ( VPoint[ 0 ]->Ly +
                 VPoint[ 1 ]->Ly +
                 VPoint[ 2 ]->Ly +
                 VPoint[ 3 ]->Ly ) / 4.0F;

    Center.Lz = ( VPoint[ 0 ]->Lz +
                 VPoint[ 1 ]->Lz +
                 VPoint[ 2 ]->Lz +
                 VPoint[ 3 ]->Lz ) / 4.0F;

    // Translate the panel to the origin and store it in TempPoint:
    for( Count = 0; Count < 4; ++Count )
        TempPoint[ Count ] = ( *VPoint[ Count ] ) - Center;

    // Calculate the distance to each of the vertices:
    for( Count = 0; Count < 4; ++Count )
    {
        // Calculate distance to the panel:
        Dist = TempPoint[ Count ].Mag ();

        // Store it:
        Distance[ Count ] = Dist;
    }

    // Determine the maximum distance:
    Dist = Distance [ 0 ];

    for( Count = 1; Count < 4; ++Count )
    {
        if( Distance[ Count ] > Dist )
            Dist = Distance[ Count ];
    }

    Radius = Dist * 1.1;

    // Calculate the distance to each of the vertices:
    for( Count = 0; Count < 4; ++Count )
    {
        // Calculate distance to object ON THE X/Z PLANE!
        Dist = 
        sqrt( TempPoint[ Count ].Lx * TempPoint[ Count ].Lx +
        TempPoint[ Count ].Lz * TempPoint[ Count ].Lz );
        Distance[ Count ] = Dist;
    }

    // Determine the maximum distance:
    Dist = Distance [ 0 ];

    for ( Count = 1; Count < 4; ++Count )
    {
        if ( Distance [ Count ] > Dist )
            Dist = Distance [ Count ];
    }

   // Dist holds the maximum X/Z extent of the panel:
   RadiusXZ = Dist;

    // Calculate the distance to each of the vertices:
    for ( Count = 0; Count < 4; ++Count )
    {
        // Calculate distance to object ON THE Y AXIS!
        Dist = TempPoint [ Count ].Ly;
        Distance [ Count ] = Dist;
    }

    // Determine the maximum distance:
    Dist = Distance [ 0 ];

    for ( Count = 1; Count < 4; ++Count )
    {
        if ( Distance [ Count ] > Dist )
            Dist = Distance [ Count ];
    }

    // Dist holds the maximum Y extent of the panel:
    RadiusY = Dist;

    #endif

}   // CalcRadius


/************************************************************************
*    FUNCTION NAME:         CalcInten()                                                             
*
*    DESCRIPTION:           Calculates panel's intensity. This function is
*                           usually called after we calculate the normal.
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

void Panel3D::CalcInten( PTLight pLight )
{
    // Assign a shade based on normal:
    double CosA = ( ( Normal.X - VPoint[ 0 ]->Lx ) * pLight->GetX() +
                    ( Normal.Y - VPoint[ 0 ]->Ly ) * pLight->GetY() +
                    ( Normal.Z - VPoint[ 0 ]->Lz ) * pLight->GetZ() ) / pLight->GetMag();

    int Intensity = ( ( CosA * ( double ) COLOR_RANGE ) + COLOR_START );

    Behavior.Attrib[ 0 ].I = Intensity;
    Behavior.Attrib[ 1 ].I = Intensity;
    Behavior.Attrib[ 2 ].I = Intensity;
    Behavior.Attrib[ 3 ].I = Intensity;

}   // CalcInten


/************************************************************************
*    FUNCTION NAME:         CalcIntenForObjMove()                                                             
*
*    DESCRIPTION:           Dynamicly Calculates panel's intensity for moving
*                           the panel around.
*                           For this we use transformed normals and vpoints
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

void Panel3D::CalcIntenForObjMove( PTLight pLight )
{
    // Assign a shade based on a transformed normal and vpoint:
    double CosA = ( ( Normal.Tx - VPoint[ 0 ]->Wx ) * pLight->GetX() +
                    ( Normal.Ty - VPoint[ 0 ]->Wy ) * pLight->GetY() +
                    ( Normal.Tz - VPoint[ 0 ]->Wz ) * pLight->GetZ() ) / pLight->GetMag();

    int Intensity = ( ( CosA * ( double ) COLOR_RANGE ) + COLOR_START );

    Behavior.Attrib[ 0 ].I = Intensity;
    Behavior.Attrib[ 1 ].I = Intensity;
    Behavior.Attrib[ 2 ].I = Intensity;
    Behavior.Attrib[ 3 ].I = Intensity;

}   // CalcIntenForObjMove


/************************************************************************
*    FUNCTION NAME:         Project()                                                             
*
*    DESCRIPTION:           Perform front Z-clipping and project the panel's
*                           3-dimensional points onto the screen
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

void Panel3D::Project()
{
    SPCount = 4;
    int Count, OutC(0), StartI, EndI, NewI;
    double UOverZ, VOverZ, OneOverZ, DeltaZ, t;
    // Maximum of 6 clipped Z points
    Point3D ZClip[ MAX_CLIPPED_POINTS ];
    // Maximum of 6 clipped detail values 
    Detail3D NewA[ MAX_CLIPPED_POINTS ];
    Detail3D temp;
    const double SHADE_DIV( ( double ) ( MaxLightDistance - MINZ ) / ( double ) SHADE_COUNT );
    WORD ScreenWDiv2(ScreenW >> 1), ScreenHDiv2(ScreenH >> 1); 

    // Initialize pointer to last vertex:
    StartI = SPCount - 1;

    // Loop through all edges of panel using S&H algorithm:
    for( EndI = 0; EndI < SPCount; ++EndI )
    {
        if( VPoint[ StartI ]->Wz >= MINZ )
        {
            if( VPoint[ EndI ]->Wz >= MINZ )
            {
                // Entirely inside front view volume (case 1) - output unchanged 
                // vertex:
                ZClip[ OutC ].Wx = VPoint[ EndI ]->Wx;
                ZClip[ OutC ].Wy = VPoint[ EndI ]->Wy;
                ZClip[ OutC ].Wz = VPoint[ EndI ]->Wz;

                // Output unchanged detail values:
                NewA[ OutC ] = Behavior.Attrib[ EndI ];

                // Update index:
                ++OutC;
            }
            else
            {    

                // SPoint is leaving view volume (case 2) -
                // clip using parametric form of line:
                DeltaZ = VPoint[ EndI ]->Wz - VPoint[ StartI ]->Wz;
                
                t = ( MINZ - VPoint[ StartI ]->Wz ) / DeltaZ;

                ZClip[ OutC ].Wx = VPoint[ StartI ]->Wx + ( double )
                                   ( VPoint[ EndI ]->Wx - 
                                     VPoint[ StartI ]->Wx ) * t;
                ZClip[ OutC ].Wy = VPoint[ StartI ]->Wy + ( double )
                                   ( VPoint[ EndI ]->Wy - 
                                     VPoint[ StartI ]->Wy ) * t;

                ZClip[ OutC ].Wz = MINZ;

                // Had to break up the below commented line because it caused a compiler error
                temp = Behavior.Attrib[ EndI ] - Behavior.Attrib[ StartI ];
                temp = temp * t;
                NewA[ OutC ] = Behavior.Attrib[ StartI ] + temp;

                // Calculate new surface detail values:
                //NewA[ OutC ] = Behavior.Attrib[ StartI ] + (  ( Behavior.Attrib[ EndI ] - Behavior.Attrib[ StartI ] ) * t );
                // Update index:
                ++OutC;

            }
        }   
        else
        {
            if( VPoint [ EndI ]->Wz >= MINZ ) 
            {
                // SPoint is entering view volume (case 3) - clip
                // using parametric form of line:
                DeltaZ = VPoint[ EndI ]->Wz - VPoint[ StartI ]->Wz;

                t = ( MINZ - VPoint[ StartI ]->Wz ) / DeltaZ;

                ZClip [ OutC ].Wx = VPoint[ StartI ]->Wx + ( double )
                                   ( VPoint[ EndI ]->Wx - VPoint[ StartI ]->Wx ) * t;

                ZClip [ OutC ].Wy = VPoint[ StartI ]->Wy + ( double )
                                   ( VPoint[ EndI ]->Wy - VPoint[ StartI ]->Wy ) * t;

                ZClip [ OutC ].Wz =  MINZ;

                // Had to break up the below commented line because it caused a compiler error
                temp = Behavior.Attrib[ EndI ] - Behavior.Attrib[ StartI ];
                temp = temp * t;
                NewA[ OutC ] = Behavior.Attrib[ StartI ] + temp;

                // Calculate new surface detail values: 
                //NewA [ OutC ] = Behavior.Attrib[ StartI ] + (  ( Behavior.Attrib[ EndI ] - Behavior.Attrib[ StartI ] ) * t );

                // Update index:
                ++OutC;
                
                // Add an extra edge to list:
                ZClip[ OutC ].Wx =  VPoint[ EndI ]->Wx;
                ZClip[ OutC ].Wy =  VPoint[ EndI ]->Wy;
                ZClip[ OutC ].Wz =  VPoint[ EndI ]->Wz;

                // Add an additional surface detail point to 
                // the list:
                NewA[ OutC ] = Behavior.Attrib[ EndI ];

                ++OutC;
                
            }
            //else
            //{
                // No operation is necessary for case 4
            //}
        }
           
        // Advance to next vertex:
        StartI = EndI;
    }
    /*
    // Test
    if( OutC > MAX_CLIPPED_POINTS )
    {
        char test[100];
        sprintf( test, "Greater then 6 - %d\r\n", OutC );
        WriteDataToFile( "C:\\pointtest.txt", test, strlen(test), OPEN_ALWAYS );
    }
    */

    // Store the number of vertices in OutC:
    SPCount = OutC;

    // Project panel's points:
    for( Count = 0; Count < OutC; ++Count )
    {
        OneOverZ = 1.0F / ZClip[ Count ].Wz;

        SPoint[ Count ].X = ((ZClip[ Count ].Wx * CameraScale) * OneOverZ) + ScreenWDiv2;  // ScreenW / 2;   * OneOverZ

        SPoint[ Count ].Y = ((ZClip[ Count ].Wy * (-CameraScale)) * OneOverZ) + ScreenHDiv2;  // ScreenH / 2;

        // Store in Z fixed point values of the floating point values
        SPoint[ Count ].Z = ( ( double ) OneOverZ *
                            ( ( double ) ( TSTEP_PREC_SL1 ) ) );  // was 1 << ZSTEP_PREC

        UOverZ = ( double ) NewA[ Count ].U * OneOverZ;
        VOverZ = ( double ) NewA[ Count ].V * OneOverZ;

        // Store in U and V fixed point values of the floating point values
        SPoint[ Count ].U = UOverZ * ( double )( TSTEP_PREC_SL1 );  // was 1 << TSTEP_PREC

        SPoint[ Count ].V = VOverZ * ( double )( TSTEP_PREC_SL1 );  // was 1 << TSTEP_PREC

        // Perform depth shading:
        NewI = NewA [ Count ].I + ZClip[ Count ].Wz / SHADE_DIV;

        if( NewI >= SHADE_COUNT )  // was  if( NewI > ( SHADE_COUNT - 1 ) )
            NewI = ( SHADE_COUNT - 1 );

        SPoint[ Count ].I = NewI << ISTEP_PREC;
    }
    
}   // Project


/************************************************************************
*    FUNCTION NAME:         CalcNormal()                                                             
*
*    DESCRIPTION:           Calculate the normal of the panel
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
   
void Panel3D::CalcNormal()
{
    long double X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3;
    long double Distance, A, B, C, D;
    Point3D UniqueVerts [ 4 ], TVert;
    int Count, Range = 0;

    // Create a list of unique vertices:
    for( Count = 0; Count < 4; ++Count )
    {
        TVert = *VPoint[ Count ];

        if( Range == 0 )
            UniqueVerts[ Range++ ] = TVert;
        else if( UniqueVert( TVert, UniqueVerts, Range ) )
            UniqueVerts[ Range++ ] = TVert;
    }

    X1 = UniqueVerts[ 0 ].Lx;
    Y1 = UniqueVerts[ 0 ].Ly;
    Z1 = UniqueVerts[ 0 ].Lz;

    X2 = UniqueVerts[ 1 ].Lx;
    Y2 = UniqueVerts[ 1 ].Ly;
    Z2 = UniqueVerts[ 1 ].Lz;

    X3 = UniqueVerts[ 2 ].Lx;
    Y3 = UniqueVerts[ 2 ].Ly;
    Z3 = UniqueVerts[ 2 ].Lz;

    // Use plane equation to determine plane's orientation:
    A = Y1 * ( Z2 - Z3 ) + Y2 * ( Z3 - Z1 ) + Y3 * ( Z1 - Z2 );
    B = Z1 * ( X2 - X3 ) + Z2 * ( X3 - X1 ) + Z3 * ( X1 - X2 );
    C = X1 * ( Y2 - Y3 ) + X2 * ( Y3 - Y1 ) + X3 * ( Y1 - Y2 );
    D = -X1 * ( Y2 * Z3 - Y3 * Z2 )  -X2 * ( Y3 * Z1 - Y1 * Z3 )
        -X3 * ( Y1 * Z2 - Y2 * Z1 );


    // Get the distance to the vector:
    Distance = sqrt( (A*A) + (B*B) + (C*C) );

    // Normalize the normal to 1 and create a point:
    Normal.X = ( A / Distance ) + VPoint [ 0 ]->Lx;
    Normal.Y = ( B / Distance ) + VPoint [ 0 ]->Ly;
    Normal.Z = ( C / Distance ) + VPoint [ 0 ]->Lz;
    Normal.D = ( D );
    Normal.Td = Distance;

}   // CalcNormal


/************************************************************************
*    FUNCTION NAME:         CalcBFace()                                                             
*
*    DESCRIPTION:           Determine if polygon is a backface
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

BOOL inline Panel3D::CalcBFace()
{
    BOOL Visible = TRUE; //Invis = 0;
    /*Point3D V = *VPoint [ 0 ];
    

    double Direction = ( V.Wx * ( Normal.Tx - VPoint [ 0 ]->Wx ) + 
                       V.Wy * ( Normal.Ty - VPoint [ 0 ]->Wy ) + 
                       V.Wz * ( Normal.Tz - VPoint [ 0 ]->Wz ) );*/


    double Direction = ( (VPoint[ 0 ]->Wx * ( Normal.Tx - VPoint[ 0 ]->Wx )) + 
                         (VPoint[ 0 ]->Wy * ( Normal.Ty - VPoint[ 0 ]->Wy )) + 
                         (VPoint[ 0 ]->Wz * ( Normal.Tz - VPoint[ 0 ]->Wz )) );

    if( Direction > 0.0F )
    {
        // Get the cosine of the angle between the viewer and the polygon 
        // normal:
        //Direction /= V.Mag();
        // Assume panel will remain a time proportional to the angle between
        // the viewer to the polygon normal:
        //Invis = ( double ) Direction * ( double ) 10.0f;  // 25.0F;
        // Set the invisible flag for this frame:
        Visible = FALSE;
    }

    return Visible;
}


/************************************************************************
*    FUNCTION NAME:         CalcVisible3D()                                                             
*
*    DESCRIPTION:           Check if the poly is visible. Perform 3D culling
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

BOOL Panel3D::CalcVisible3D( BOOL DontBackFaceCul )
{
    BOOL Visible;
    double Direction;

    // Skip the back face culling for what ever reason.
    if( DontBackFaceCul )
    {
        Direction = 0.0F;
    }
    else  // Perform 3D culling:
    {
        // Check to see if the back is facing us
        Direction = (VPoint[ 0 ]->Wx * ( Normal.Tx - VPoint[ 0 ]->Wx )) + 
                          (VPoint[ 0 ]->Wy * ( Normal.Ty - VPoint[ 0 ]->Wy )) + 
                          (VPoint[ 0 ]->Wz * ( Normal.Tz - VPoint[ 0 ]->Wz ));
    }

    // If we have a positive number, then the panel has it's back to us.
    if( Direction > 0.0F )
    {
        Visible = FALSE;
    }
    else
    {
        // unroll for loop
        // Eliminate panel if it is behind us

        if( VPoint[ 0 ]->Wz > MINZ )
            Visible = TRUE;
        else if( VPoint[ 1 ]->Wz > MINZ )
            Visible = TRUE;
        else if( VPoint[ 2 ]->Wz > MINZ )
            Visible = TRUE;
        else if( VPoint[ 3 ]->Wz > MINZ )
            Visible = TRUE;
        else
            Visible = FALSE;

        // Check if the panel is too far ahead to see
        if( Visible )
        {
            double MinZ( VPoint[ 0 ]->Wz );

            // Unroll the for loop
            if( VPoint[ 1 ]->Wz < MinZ )
                MinZ = VPoint[ 1 ]->Wz;

            if( VPoint[ 2 ]->Wz < MinZ )
                MinZ = VPoint[ 2 ]->Wz;

            if( VPoint[ 3 ]->Wz < MinZ )
                MinZ = VPoint[ 3 ]->Wz;

            // Panel is too far ahead to see
            if( MinZ > MaxZDistance )
                Visible = FALSE;
        }
    }

    return Visible;

}   // CalcVisible3D


/************************************************************************
*    FUNCTION NAME:         CalcCenterZ()                                                             
*
*    DESCRIPTION:           Calculate the polygon's center Z
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

int Panel3D::CalcCenterZ()
{
    // Calculate the polygon's center Z:
    int SummedComponents( ( double ) VPoint[ 0 ]-> Wz + 
                            ( double ) VPoint[ 1 ]-> Wz +
                            ( double ) VPoint[ 2 ]-> Wz +
                            ( double ) VPoint[ 3 ]-> Wz );

    int CenterZ( SummedComponents  >> 2 );

    return CenterZ;

}   // CalcCenterZ


/************************************************************************
*    FUNCTION NAME:         CalcVisible2D()                                                             
*
*    DESCRIPTION:           Perform 2D culling
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

BOOL Panel3D::CalcVisible2D()
{
    BOOL Result(TRUE);

    // Make sure the panel has more than two points
    if( SPCount > 2 )
    {
        int XMinInVis(0), XMaxInVis(0), YMinInVis(0), YMaxInVis(0), i(0);

        // Determine location of panel's 2D points
        for( ;i < SPCount; ++i )
        {
            if( SPoint[ i ].X < 0 ) // 0 was MINX
                ++XMinInVis;

            if( SPoint[ i ].X > ScreenW )
                ++XMaxInVis;

            if( SPoint[ i ].Y < 0 )  // 0 was MINY
                ++YMinInVis;

            if( SPoint[ i ].Y > ScreenH )
                ++YMaxInVis;
        }

        if( XMinInVis >= SPCount || YMinInVis >= SPCount || 
            XMaxInVis >= SPCount || YMaxInVis >= SPCount )
            Result = FALSE;
    }

    return Result;

}   // CalcVisible2D


/************************************************************************
*    FUNCTION NAME:         CheckExtents()                                                             
*
*    DESCRIPTION:           Determine if panel is in the range of MINZ to MAXZ
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
BOOL Panel3D::CheckExtents()
{
    double MinZ;
    BOOL Visible = FALSE;
*/
    // Eliminate panel if it is behind us
    /*for( Count = 0; Count < 4; ++Count )
    {
        if( VPoint[ Count ]->Wz > MINZ )
        {
            Visible = TRUE;
            //Invis = 0;
            break;
        }
    }*/

    // unroll for loop
    // Eliminate panel if it is behind us
/*
    if( VPoint[ 0 ]->Wz > MINZ )
        Visible = TRUE;
    else if( VPoint[ 1 ]->Wz > MINZ )
        Visible = TRUE;
    else if( VPoint[ 2 ]->Wz > MINZ )
        Visible = TRUE;
    else if( VPoint[ 3 ]->Wz > MINZ )
        Visible = TRUE;


    if( Visible )
    {
        MinZ = VPoint[ 0 ]->Wz;

        // Unroll the for loop
        if( VPoint[ 1 ]->Wz < MinZ )
            MinZ = VPoint[ 1 ]->Wz;

        if( VPoint[ 2 ]->Wz < MinZ )
            MinZ = VPoint[ 2 ]->Wz;

        if( VPoint[ 3 ]->Wz < MinZ )
            MinZ = VPoint[ 3 ]->Wz;
*/

        /*for( Count = 1; Count < 4; ++Count )
        {
            if( VPoint[ Count ]->Wz < MinZ )
                MinZ = VPoint[ Count ]->Wz;
        }*/
/*
        // Check if the panel is too far ahead to see
        if( MinZ > MaxZDistance )
        {
            // Set the invisible flag for this frame:
            Visible = FALSE;

            // Assume panel will remain invisible for a time 
            // proportional to the distance from the viewer:
            //Invis = ( WORD ) ( ( MinZ - MaxZDistance ) / 50 );
        }
    }

    return Visible;

}   // CheckExtents
*/

/************************************************************************
*    FUNCTION NAME:         Update()                                                             
*
*    DESCRIPTION:           Transform the normal
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

void Panel3D::Update( Matrix3D &M )
{
    M.Transform( Normal );

    // Update the invisibility flag:
    //if( Invis > 0 )
        //--Invis;

}   // Update


/************************************************************************
*
*  Why are there so many "RenderPolygon" functions?
*
*  In software 3D rendering, speed is a HUGE issue. Adding one "if" to the 
*  very long function will result in milliseconds lost. That is why there
*  are so many flavors of this one function to suite every need. It's not
*  pretty, but this is the ONLY TIME we break the rules because the need 
*  for speed is so critical in this one function. This is why you'll find 
*  hard coded values and other bad programming habits. 
*                                 
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To8Pos()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to an 8 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.                                 
************************************************************************/

void Panel3D::RenderPolygon8To8Pos()
{
#ifndef NO_8_TO_8_POS

    unsigned char register *IPtr;
    int register *ZPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be unsigned char
    unsigned char register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (unsigned char *)pTextShade;

    // Init the video buffer pointer
    pVidBuf = (unsigned char *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )    // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y; // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;
                       
                        if( *ZPtr < Z )
                        {
                            // Update the Z buffer
                            *ZPtr = Z;

                            *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Update the Z buffer
                            *ZPtr = Z;

                            *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To8Pos


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To8NoZBufferingOr()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to an 8 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.                                 
************************************************************************/

void Panel3D::RenderPolygon8To8NoZBufferingOr()
{
#ifndef NO_8_TO_8_NO_Z_BUF_OR

    unsigned char register *IPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be unsigned char
    unsigned char register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (unsigned char *)pTextShade;

    // Init the video buffer pointer
    pVidBuf = (unsigned char *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )    // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y; // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                DPtr = &pVidBuf[ YIndex + XStart ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        *DPtr |= pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        *DPtr |= pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }
                }
            }

            //++RightSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To8NoZBufferingOr


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To8NoZBufferingTrans()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to an 8 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.                                 
************************************************************************/

void Panel3D::RenderPolygon8To8NoZBufferingTrans()
{
#ifndef NO_8_TO_8_NO_Z_BUF_TRANS

    unsigned char register *IPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep, IPos;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be unsigned char
    unsigned char register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (unsigned char *)pTextShade;

    // Init the video buffer pointer
    pVidBuf = (unsigned char *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )    // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y; // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                DPtr = &pVidBuf[ YIndex + XStart ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        // Get the color position in the texture
                        IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                        // Use the position to index into the shaded table
                        // to paint a pixel to the buffer. This will give the
                        // pixel a shaded color depending on distance and light source.
                        // Also, the zero color is considered to be the transparent color.
                        if( IPtr[ IPos ] )
                        {
                            *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ IPos ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        // Get the color position in the texture
                        IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                        // Use the position to index into the shaded table
                        // to paint a pixel to the buffer. This will give the
                        // pixel a shaded color depending on distance and light source.
                        // Also, the zero color is considered to be the transparent color.
                        if( IPtr[ IPos ] )
                        {
                            *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ IPos ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }
                }
            }

            //++RightSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To8NoZBufferingTrans


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To8NoZBuffering()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to an 8 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.                                 
************************************************************************/

void Panel3D::RenderPolygon8To8NoZBuffering()
{
#ifndef NO_8_TO_8_NO_Z_BUF

    unsigned char register *IPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be unsigned char
    unsigned char register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (unsigned char *)pTextShade;

    // Init the video buffer pointer
    pVidBuf = (unsigned char *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )    // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y; // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                DPtr = &pVidBuf[ YIndex + XStart ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }
                }
            }

            //++RightSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To8NoZBuffering


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To8PosOr()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to an 8 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.                                 
************************************************************************/

void Panel3D::RenderPolygon8To8PosOr()
{
#ifndef NO_8_TO_8_POS_OR

    unsigned char register *IPtr;
    int register *ZPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be unsigned char
    unsigned char register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (unsigned char *)pTextShade;

    // Init the video buffer pointer
    pVidBuf = (unsigned char *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )    // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y; // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;
                       
                        if( *ZPtr < Z )
                        {
                            // Update the Z buffer
                            *ZPtr = Z;

                            *DPtr |= pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Update the Z buffer
                            *ZPtr = Z;

                            *DPtr |= pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To8PosOr


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To8Pos()                                                             
*
*    DESCRIPTION:           This function renders the textured transparent polygon 
*                           to the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to an 8 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.                                 
************************************************************************/

void Panel3D::RenderPolygon8To8PosTrans()
{
#ifndef NO_8_TO_8_POS_TRANS

    unsigned char register *IPtr;
    int register *ZPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep, IPos;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be unsigned char
    unsigned char register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (unsigned char *)pTextShade;

    // Init the video buffer pointer
    pVidBuf = (unsigned char *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )    // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y; // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;
                       
                        if( *ZPtr < Z )
                        {
                            // Get the color position in the texture
                            IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                            // Use the position to index into the shaded table
                            // to paint a pixel to the buffer. This will give the
                            // pixel a shaded color depending on distance and light source.
                            // Also, the zero color is considered to be the transparent color.
                            if( IPtr[ IPos ] )
                            {
                                *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ IPos ] ];

                                // Update the Z buffer only if we are to blit the pixel
                                *ZPtr = Z;
                            }
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Get the color position in the texture
                            IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                            // Use the position to index into the shaded table
                            // to paint a pixel to the buffer. This will give the
                            // pixel a shaded color depending on distance and light source.
                            // Also, the zero color is considered to be the transparent color.
                            if( IPtr[ IPos ] )
                            {
                                *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ IPos ] ];

                                // Update the Z buffer only if we are to blit the pixel
                                *ZPtr = Z;
                            }
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To8PosTrans


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To8Neg()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to an 8 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.                                 
************************************************************************/

void Panel3D::RenderPolygon8To8Neg()
{
#ifndef NO_8_TO_8_NEG

    unsigned char register *IPtr;
    int register *ZPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be unsigned char
    unsigned char register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (unsigned char *)pTextShade;

    // Init the video buffer pointer
    pVidBuf = (unsigned char *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )     // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y; // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;   // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;
                       
                        if( *ZPtr > -Z )
                        {
                            // Update the Z buffer
                            *ZPtr = -Z;

                            *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Update the Z buffer
                            *ZPtr = -Z;

                            *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To8Neg


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To8NegOr()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to an 8 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.                                 
************************************************************************/

void Panel3D::RenderPolygon8To8NegOr()
{
#ifndef NO_8_TO_8_NEG_OR

    unsigned char register *IPtr;
    int register *ZPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be unsigned char
    unsigned char register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (unsigned char *)pTextShade;

    // Init the video buffer pointer
    pVidBuf = (unsigned char *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )     // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y; // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;   // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;
                       
                        if( *ZPtr > -Z )
                        {
                            // Update the Z buffer
                            *ZPtr = -Z;

                            *DPtr |= pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Update the Z buffer
                            *ZPtr = -Z;

                            *DPtr |= pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To8NegOr


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To8NegTrans()                                                             
*
*    DESCRIPTION:           This function renders the textured transparent polygon 
*                           to the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to an 8 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.                                 
************************************************************************/

void Panel3D::RenderPolygon8To8NegTrans()
{
#ifndef NO_8_TO_8_NEG_TRANS

    unsigned char register *IPtr;
    int register *ZPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep, IPos;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be unsigned char
    unsigned char register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (unsigned char *)pTextShade;

    // Init the video buffer pointer
    pVidBuf = (unsigned char *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )     // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y; // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;   // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;
                       
                        if( *ZPtr > -Z )
                        {
                            // Get the color position in the texture
                            IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                            // Use the position to index into the shaded table
                            // to paint a pixel to the buffer. This will give the
                            // pixel a shaded color depending on distance and light source.
                            // Also, the zero color is considered to be the transparent color.
                            if( IPtr[ IPos ] )
                            {
                                *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ IPos ] ];

                                // Update the Z buffer only if we are to blit the pixel
                                *ZPtr = -Z;
                            }
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Get the color position in the texture
                            IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                            // Use the position to index into the shaded table
                            // to paint a pixel to the buffer. This will give the
                            // pixel a shaded color depending on distance and light source.
                            // Also, the zero color is considered to be the transparent color.
                            if( IPtr[ IPos ] )
                            {
                                *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ IPos ] ];

                                // Update the Z buffer only if we are to blit the pixel
                                *ZPtr = -Z;
                            }
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg;  // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To8NegTrans


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To16Pos()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon8To16Pos()
{
#ifndef NO_8_TO_16_POS

    unsigned char register *IPtr;
    int register *ZPtr;
    int Top(0);

    int TxStep, TyStep, Tx1, Ty1, Tx2, Ty2;
    int ZStep, UStep, VStep, IStep;
    int RunLoops, Length;
    int Z, I, RZ, U, V;    
    int NewRightPos, NewLeftPos; 
    int Height, Width, YIndex, XStart, XEnd;   
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10), RunShift(4);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    ///////////////////////////////////////////////////
    //  Because polygons can be not completely plainer
    //  Check the bounds of the texture to make sure
    //  we are not blowing past our texture size
    //////////////////////////////////////////////////
    #ifdef CHECK_TEXT_CEILING
    DWORD pos, textureSize = (pTextDat->TMap[ Behavior.TextHan + FrameCounter ].Width * 
                              pTextDat->TMap[ Behavior.TextHan + FrameCounter ].Height) - 1;
    #endif

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf =  (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////

    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint[ N ].Y < SPoint[ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 ) // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;

                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = 0x10;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U += SubUStep;
                    V += SubVStep;
                    RZ = Z + SubZStep;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> 0x4; // RunShift
                    TyStep = ( Ty2 - Ty1 ) >> 0x4; // RunShift

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Update the Z buffer
                            *ZPtr = Z;

                            ///////////////////////////////////////////////////
                            //  Because polygons can be not completely plainer
                            //  Check the bounds of the texture to make sure
                            //  we are not blowing past our texture size
                            //////////////////////////////////////////////////
                            #ifdef CHECK_TEXT_CEILING
                                // Get the position in the texture we are referencing
                                pos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                                // Make sure we are not blowing past our range
                                if( pos > textureSize )
                                    pos = textureSize;

                                // Write the pixel
                                *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ pos ] ];
                            #else
                                // Write the pixel
                                *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];

                            #endif
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & 0xF; // was ( RunLen - 1 ) = 15;

                if( Length > 0 )
                {                    
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );
                    
                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Update the Z buffer
                            *ZPtr = Z;

                            ///////////////////////////////////////////////////
                            //  Because polygons can be not completely plainer
                            //  Check the bounds of the texture to make sure
                            //  we are not blowing past our texture size
                            //////////////////////////////////////////////////
                            #ifdef CHECK_TEXT_CEILING
                                // Get the position in the texture we are referencing
                                pos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                                // Make sure we are not blowing past our range
                                if( pos > textureSize )
                                    pos = textureSize;

                                // Write the pixel
                                *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ pos ] ];
                            #else
                                // Write the pixel
                                *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];

                            #endif
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To16Pos


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To16PosTrans()                                                             
*
*    DESCRIPTION:           This function renders the transparent textured polygon 
*                           to the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon8To16PosTrans()
{
#ifndef NO_8_TO_16_POS_TRANS

    unsigned char register *IPtr;
    int register *ZPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep, IPos;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf =  (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////

    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();


    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 ) // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Get the color position in the texture
                            IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                            // Use the position to index into the shaded table
                            // to paint a pixel to the buffer. This will give the
                            // pixel a shaded color depending on distance and light source.
                            // Also, the zero color is considered to be the transparent color.
                            if( IPtr[ IPos ] )
                            {
                                *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ IPos ] ];

                                // Update the Z buffer only if we are to blit the pixel
                                *ZPtr = Z;
                            }
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );
                    
                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Get the color position in the texture
                            IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                            // Use the position to index into the shaded table
                            // to paint a pixel to the buffer. This will give the
                            // pixel a shaded color depending on distance and light source.
                            // Also, the zero color is considered to be the transparent color.
                            if( IPtr[ IPos ] )
                            {
                                *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ IPos ] ];

                                // Update the Z buffer only if we are to blit the pixel
                                *ZPtr = Z;
                            }
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To16PosTrans


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To16Neg()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon8To16Neg()
{
#ifndef NO_8_TO_16_NEG

    unsigned char register *IPtr;
    int register *ZPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf = (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();


    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )    // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;       // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;       // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Update the Z buffer
                            *ZPtr = -Z;

                            *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Update the Z buffer
                            *ZPtr = -Z;

                            *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To16Neg


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To16NegTrans()                                                             
*
*    DESCRIPTION:           This function renders the textured transparent polygon 
*                           to the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon8To16NegTrans()
{
#ifndef NO_8_TO_16_NEG_TRANS

    unsigned char register *IPtr;
    int register *ZPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep, IPos;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf = (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();


    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )    // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;       // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;       // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Get the color position in the texture
                            IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                            // Use the position to index into the shaded table
                            // to paint a pixel to the buffer. This will give the
                            // pixel a shaded color depending on distance and light source.
                            // Also, the zero color is considered to be the transparent color.
                            if( IPtr[ IPos ] )
                            {
                                *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ IPos ] ];

                                // Update the Z buffer only if we are to blit the pixel
                                *ZPtr = -Z;
                            }
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Get the color position in the texture
                            IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                            // Use the position to index into the shaded table
                            // to paint a pixel to the buffer. This will give the
                            // pixel a shaded color depending on distance and light source.
                            // Also, the zero color is considered to be the transparent color.
                            if( IPtr[ IPos ] )
                            {
                                *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ IPos ] ];

                                // Update the Z buffer only if we are to blit the pixel
                                *ZPtr = -Z;
                            }
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To16NegTrans


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To16PosOr()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           This flavor of the function uses an OR to simulate transparence
*
*                           8 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon8To16PosOr()
{
#ifndef NO_8_TO_16_POS_OR

    unsigned char register *IPtr;
    int register *ZPtr;
    int Top(0);

    int TxStep, TyStep, Tx1, Ty1, Tx2, Ty2;
    int ZStep, UStep, VStep, IStep;
    int RunLoops, Length;
    int Z, I, RZ, U, V;    
    int NewRightPos, NewLeftPos; 
    int Height, Width, YIndex, XStart, XEnd;   
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf =  (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////

    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint[ N ].Y < SPoint[ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 ) // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;

                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Update the Z buffer
                            *ZPtr = Z;

                            // Write the pixel
                            *DPtr |= pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];

                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {                    
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );
                    
                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Update the Z buffer
                            *ZPtr = Z;

                            // Write the pixel
                            *DPtr |= pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To16PosOr


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To16NegOr()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           This flavor of the function uses an OR to simulate transparence
*
*                           8 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon8To16NegOr()
{
#ifndef NO_8_TO_16_NEG_OR

    unsigned char register *IPtr;
    int register *ZPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf = (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();


    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )    // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;       // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;       // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Update the Z buffer
                            *ZPtr = -Z;

                            *DPtr |= pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Update the Z buffer
                            *ZPtr = -Z;

                            *DPtr |= pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To16NegOr


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To16NoZBuffering()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           There are situations where you don't need a Z buffer 
*                           so this function does it's magic without a z buffer.
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon8To16NoZBuffering()
{
#ifndef NO_8_TO_16_NO_Z_BUF

    unsigned char register *IPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf = (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();


    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )    // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;       // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;       // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                DPtr = &pVidBuf[ YIndex + XStart ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To16NoZBuffering


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To16NoZBufferingOr()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           This flavor of the function uses an OR to simulate transparence
*
*                           8 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           There are situations where you don't need a Z buffer 
*                           so this function does it's magic without a z buffer.
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon8To16NoZBufferingOr()
{
#ifndef NO_8_TO_16_NO_Z_BUF_OR

    unsigned char register *IPtr;
    int Top(0);

    int TxStep, TyStep, Tx1, Ty1, Tx2, Ty2;
    int ZStep, UStep, VStep, IStep;
    int RunLoops, Length;
    int Z, I, RZ, U, V;    
    int NewRightPos, NewLeftPos; 
    int Height, Width, YIndex, XStart, XEnd;   
    int SubUStep, SubVStep, SubZStep, Step;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf =  (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////

    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint[ N ].Y < SPoint[ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 ) // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;

                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                DPtr = &pVidBuf[ YIndex + XStart ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        // Write the pixel
                        *DPtr |= pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );
                    
                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        // Write the pixel
                        *DPtr |= pShade[ ( I & 0xFFFFFF00 ) | IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ] ];

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To16NoZBufferingOr


/************************************************************************
*    FUNCTION NAME:         RenderPolygon8To16NoZBufferingTrans()                                                             
*
*    DESCRIPTION:           This function renders the transparent textured polygon 
*                           to the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           8 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           There are situations where you don't need a Z buffer 
*                           so this function does it's magic without a z buffer.
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon8To16NoZBufferingTrans()
{
#ifndef NO_8_TO_16_NO_Z_BUF_TRANS

    unsigned char register *IPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, I, IStep, Tx1, Ty1, Tx2, Ty2;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep, IPos;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *pShade, *pVidBuf;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf =  (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////

    // Get pointer to the texture map
    IPtr = pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();


    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 ) // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                DPtr = &pVidBuf[ YIndex + XStart ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        // Get the color position in the texture
                        IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                        // Use the position to index into the shaded table
                        // to paint a pixel to the buffer. This will give the
                        // pixel a shaded color depending on distance and light source.
                        // Also, the zero color is considered to be the transparent color.
                        if( IPtr[ IPos ] )
                        {
                            *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ IPos ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );
                    
                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        // Get the color position in the texture
                        IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                        // Use the position to index into the shaded table
                        // to paint a pixel to the buffer. This will give the
                        // pixel a shaded color depending on distance and light source.
                        // Also, the zero color is considered to be the transparent color.
                        if( IPtr[ IPos ] )
                        {
                            *DPtr = pShade[ ( I & 0xFFFFFF00 ) | IPtr[ IPos ] ];
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon8To16NoZBufferingTrans


/************************************************************************
*    FUNCTION NAME:         RenderPolygon16To16Pos()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           16 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon16To16Pos()
{
#ifndef NO_16_TO_16_POS

    int register *ZPtr;
    int Top(0);

    int TxStep, TyStep;
    int Z, RZ, U, V, Tx1, Ty1, Tx2, Ty2, I, IStep;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;
    WORD pixel, result, shadeR, shadeG, shadeB, *pVidBuf;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    ///////////////////////////////////////////////////
    //  Because polygons can be not completely plainer
    //  Check the bounds of the texture to make sure
    //  we are not blowing past our texture size
    //////////////////////////////////////////////////
    #ifdef CHECK_TEXT_CEILING
    DWORD pos, textureSize = (pTextDat->TMap[ Behavior.TextHan + FrameCounter ].Width * 
                              pTextDat->TMap[ Behavior.TextHan + FrameCounter ].Height) - 1;
    #endif

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *IPtr, *pShade;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf =  (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////

    // Get pointer to the texture map
    IPtr = (WORD *)pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint[ N ].Y < SPoint[ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 ) // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;

                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Get the value of the shade for each color. This value ia also designed to help index into the shaded table.
                // Normally this is calculated on each pixel written to the video buffer. Placing it out here
                // and doing the calculation only once, helps to gain back some speed we loose with doing the 16 bit color.
                // Mask out the junk bits and shift it just enough to use it as an index into the the first color
                shadeB = ( I & 0xFFFFFF00 ) >> 3;
                // Shift it 5 more bits so that we have the actual value of the intensity.
                // Then add the offset into the shaded table to the beginning of the green values of the shaded table.
                shadeG = (shadeB >> 5) + 0x400;
                // For the red part of the shaded table, just add in another offset
                shadeR = shadeG + 0x400;

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;
                    
                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Update the Z buffer
                            *ZPtr = Z;

                            ///////////////////////////////////////////////////
                            //  Because polygons can be not completely plainer
                            //  Check the bounds of the texture to make sure
                            //  we are not blowing past our texture size
                            //////////////////////////////////////////////////
                            #ifdef CHECK_TEXT_CEILING
                                // Get the position in the texture we are referencing
                                pos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                                // Make sure we are not blowing past our range
                                if( pos > textureSize )
                                    pos = textureSize;

                                // Write the pixel
                                pixel = IPtr[ pos ];

                                // Generate the new shaded pixel
                                result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                         pShade[ shadeG | (pixel & 0x3E0) ] | 
                                         (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                                
                                
                                // Save the resulted pixel
                                *DPtr = result;
                            #else
                                
                            // Get the pixel in question
                            pixel = IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ];

                            result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                     pShade[ shadeG | (pixel & 0x3E0) ] | 
                                     (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                            
                            
                            // Save the resulted pixel
                            *DPtr = result;

                            #endif
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {                    
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );
                    
                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Update the Z buffer
                            *ZPtr = Z;

                            ///////////////////////////////////////////////////
                            //  Because polygons can be not completely plainer
                            //  Check the bounds of the texture to make sure
                            //  we are not blowing past our texture size
                            //////////////////////////////////////////////////
                            #ifdef CHECK_TEXT_CEILING
                                // Get the position in the texture we are referencing
                                pos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                                // Make sure we are not blowing past our range
                                if( pos > textureSize )
                                    pos = textureSize;

                                // Write the pixel
                                pixel = IPtr[ pos ];

                                // Generate the new shaded pixel
                                result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                         pShade[ shadeG | (pixel & 0x3E0) ] | 
                                         (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                                
                                
                                // Save the resulted pixel
                                *DPtr = result;
                            #else
                                
                            // Get the pixel in question
                            pixel = IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ];

                            result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                     pShade[ shadeG | (pixel & 0x3E0) ] | 
                                     (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                            
                            // Save the resulted pixel
                            *DPtr = result;

                            #endif
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon16To16Pos


/************************************************************************
*    FUNCTION NAME:         RenderPolygon16To16Neg()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           16 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon16To16Neg()
{
#ifndef NO_16_TO_16_NEG

    int register *ZPtr;
    int Top(0);

    int TxStep, TyStep;
    int Z, RZ, U, V, Tx1, Ty1, Tx2, Ty2, I, IStep;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;
    WORD pixel, result, shadeR, shadeG, shadeB, *pVidBuf;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *IPtr, *pShade;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf =  (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////

    // Get pointer to the texture map
    IPtr = (WORD *)pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint[ N ].Y < SPoint[ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 ) // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;

                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Get the value of the shade for each color. This value ia also designed to help index into the shaded table.
                // Normally this is calculated on each pixel written to the video buffer. Placing it out here
                // and doing the calculation only once, helps to gain back some speed we loose with doing the 16 bit color.
                // Mask out the junk bits and shift it just enough to use it as an index into the the first color
                shadeB = ( I & 0xFFFFFF00 ) >> 3;
                // Shift it 5 more bits so that we have the actual value of the intensity.
                // Then add the offset into the shaded table to the beginning of the green values of the shaded table.
                shadeG = (shadeB >> 5) + 0x400;
                // For the red part of the shaded table, just add in another offset
                shadeR = shadeG + 0x400;

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;
                    
                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Update the Z buffer
                            *ZPtr = -Z;

                            // Get the pixel in question
                            pixel = IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ];

                            result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                     pShade[ shadeG | (pixel & 0x3E0) ] | 
                                     (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                            
                            
                            // Save the resulted pixel
                            *DPtr = result;
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );
                    
                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Update the Z buffer
                            *ZPtr = -Z;
                                
                            // Get the pixel in question
                            pixel = IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ];

                            result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                     pShade[ shadeG | (pixel & 0x3E0) ] | 
                                     (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                            
                            // Save the resulted pixel
                            *DPtr = result;
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon16To16Neg


/************************************************************************
*    FUNCTION NAME:         RenderPolygon16To16PosTrans()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           16 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon16To16PosTrans()
{
#ifndef NO_16_TO_16_POS_TRANS

    int register *ZPtr;
    int Top(0);

    int TxStep, TyStep;
    int Z, RZ, U, V, Tx1, Ty1, Tx2, Ty2, I, IStep;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep, IPos;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;
    WORD pixel, result, shadeR, shadeG, shadeB, *pVidBuf;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *IPtr, *pShade;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf =  (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////

    // Get pointer to the texture map
    IPtr = (WORD *)pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint[ N ].Y < SPoint[ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 ) // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;

                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Get the value of the shade for each color. This value ia also designed to help index into the shaded table.
                // Normally this is calculated on each pixel written to the video buffer. Placing it out here
                // and doing the calculation only once, helps to gain back some speed we loose with doing the 16 bit color.
                // Mask out the junk bits and shift it just enough to use it as an index into the the first color
                shadeB = ( I & 0xFFFFFF00 ) >> 3;
                // Shift it 5 more bits so that we have the actual value of the intensity.
                // Then add the offset into the shaded table to the beginning of the green values of the shaded table.
                shadeG = (shadeB >> 5) + 0x400;
                // For the red part of the shaded table, just add in another offset
                shadeR = shadeG + 0x400;

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;
                    
                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Get the position in the texture we are referencing
                            IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                            // Use the position to index into the shaded table
                            // to paint a pixel to the buffer. This will give the
                            // pixel a shaded color depending on distance and light source.
                            // Also, in 16 bit, the brite green color is considered to 
                            // be the transparent color.
                            if( IPtr[ IPos ] ^ 0x3E0 )
                            {
                                // Update the Z buffer only if we are to blit the pixel
                                *ZPtr = Z;

                                // Write the pixel
                                pixel = IPtr[ IPos ];

                                // Generate the new shaded pixel
                                result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                         pShade[ shadeG | (pixel & 0x3E0) ] | 
                                         (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                                                                
                                // Save the resulted pixel
                                *DPtr = result;
                            }
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {                    
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );
                    
                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Get the position in the texture we are referencing
                            IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                            // Use the position to index into the shaded table
                            // to paint a pixel to the buffer. This will give the
                            // pixel a shaded color depending on distance and light source.
                            // Also, in 16 bit, the brite green color is considered to 
                            // be the transparent color.
                            if( IPtr[ IPos ] ^ 0x3E0 )
                            {
                                // Update the Z buffer only if we are to blit the pixel
                                *ZPtr = Z;

                                // Write the pixel
                                pixel = IPtr[ IPos ];

                                // Generate the new shaded pixel
                                result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                         pShade[ shadeG | (pixel & 0x3E0) ] | 
                                         (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                                                                
                                // Save the resulted pixel
                                *DPtr = result;
                            }
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon16To16PosTrans


/************************************************************************
*    FUNCTION NAME:         RenderPolygon16To16NegTrans()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           16 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon16To16NegTrans()
{
#ifndef NO_16_TO_16_NEG_TRANS

    int register *ZPtr;
    int Top(0);

    int TxStep, TyStep;
    int Z, RZ, U, V, Tx1, Ty1, Tx2, Ty2, I, IStep;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep, IPos;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;
    WORD pixel, result, shadeR, shadeG, shadeB, *pVidBuf;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *IPtr, *pShade;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf =  (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////

    // Get pointer to the texture map
    IPtr = (WORD *)pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint[ N ].Y < SPoint[ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 ) // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;

                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Get the value of the shade for each color. This value ia also designed to help index into the shaded table.
                // Normally this is calculated on each pixel written to the video buffer. Placing it out here
                // and doing the calculation only once, helps to gain back some speed we loose with doing the 16 bit color.
                // Mask out the junk bits and shift it just enough to use it as an index into the the first color
                shadeB = ( I & 0xFFFFFF00 ) >> 3;
                // Shift it 5 more bits so that we have the actual value of the intensity.
                // Then add the offset into the shaded table to the beginning of the green values of the shaded table.
                shadeG = (shadeB >> 5) + 0x400;
                // For the red part of the shaded table, just add in another offset
                shadeR = shadeG + 0x400;

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;
                    
                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Get the position in the texture we are referencing
                            IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                            // Use the position to index into the shaded table
                            // to paint a pixel to the buffer. This will give the
                            // pixel a shaded color depending on distance and light source.
                            // Also, in 16 bit, the brite green color is considered to 
                            // be the transparent color.
                            if( IPtr[ IPos ] ^ 0x3E0 )
                            {
                                // Update the Z buffer only if we are to blit the pixel
                                *ZPtr = -Z;

                                // Write the pixel
                                pixel = IPtr[ IPos ];

                                // Generate the new shaded pixel
                                result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                         pShade[ shadeG | (pixel & 0x3E0) ] | 
                                         (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                                                                
                                // Save the resulted pixel
                                *DPtr = result;
                            }
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {                    
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );
                    
                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Get the position in the texture we are referencing
                            IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                            // Use the position to index into the shaded table
                            // to paint a pixel to the buffer. This will give the
                            // pixel a shaded color depending on distance and light source.
                            // Also, in 16 bit, the brite green color is considered to 
                            // be the transparent color.
                            if( IPtr[ IPos ] ^ 0x3E0 )
                            {
                                // Update the Z buffer only if we are to blit the pixel
                                *ZPtr = -Z;

                                // Write the pixel
                                pixel = IPtr[ IPos ];

                                // Generate the new shaded pixel
                                result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                         pShade[ shadeG | (pixel & 0x3E0) ] | 
                                         (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                                                                
                                // Save the resulted pixel
                                *DPtr = result;
                            }
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon16To16NegTrans


/************************************************************************
*    FUNCTION NAME:         RenderPolygon16To16PosOr()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           16 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon16To16PosOr()
{
#ifndef NO_16_TO_16_POS_OR

    int register *ZPtr;
    int Top(0);

    int TxStep, TyStep;
    int Z, RZ, U, V, Tx1, Ty1, Tx2, Ty2;//, I, IStep;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *IPtr, *pVidBuf;

    // Init the video buffer pointer
    pVidBuf =  (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////

    // Get pointer to the texture map
    IPtr = (WORD *)pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint[ N ].Y < SPoint[ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            //RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                //RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                //RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 ) // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    //RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            //LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                //LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                //LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    //LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                //I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                //IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    //I += IStep * Step;

                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Update the Z buffer
                            *ZPtr = Z;

                            // Write the pixel
                            *DPtr |= IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ];

                        }

                        Z  += ZStep;
                        //I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {                    
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );
                    
                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr < Z )
                        {
                            // Update the Z buffer
                            *ZPtr = Z;

                            // Write the pixel
                            *DPtr |= IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ];
                        }

                        Z  += ZStep;
                        //I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            //RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            //LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon16To16PosOr


/************************************************************************
*    FUNCTION NAME:         RenderPolygon16To16NegOr()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           16 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon16To16NegOr()
{
#ifndef NO_16_TO_16_NEG_OR

    int register *ZPtr;
    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, Tx1, Ty1, Tx2, Ty2;//, I, IStep;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step, offset;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *IPtr, *pVidBuf;

    // Init the video buffer pointer
    pVidBuf = (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = (WORD *)pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();


    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            //RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                //RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                //RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )    // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;       // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    //RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            //LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                //LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                //LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;       // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    //LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                //I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                //IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    //I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                // Calculate this offset only once for speed
                offset = YIndex + XStart;

                DPtr = &pVidBuf[ offset ];
                ZPtr = &ZBuffer[ offset ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Update the Z buffer
                            *ZPtr = -Z;

                            *DPtr |= IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ];
                        }

                        Z  += ZStep;
                        //I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        if( *ZPtr > -Z )
                        {
                            // Update the Z buffer
                            *ZPtr = -Z;

                            *DPtr |= IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ];
                        }

                        Z  += ZStep;
                        //I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                        ++ZPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            //RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            //LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon16To16NegOr


/************************************************************************
*    FUNCTION NAME:         RenderPolygon16To16NoZBuffering()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           16 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon16To16NoZBuffering()
{
#ifndef NO_16_TO_16_NO_Z_BUF

    int Top(0);

    int TxStep, TyStep;
    int Z, RZ, U, V, Tx1, Ty1, Tx2, Ty2, I, IStep;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step;
    WORD pixel, result, shadeR, shadeG, shadeB, *pVidBuf;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *IPtr, *pShade;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf =  (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////

    // Get pointer to the texture map
    IPtr = (WORD *)pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint[ N ].Y < SPoint[ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 ) // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;

                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////
;

                DPtr = &pVidBuf[ YIndex + XStart ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Get the value of the shade for each color. This value ia also designed to help index into the shaded table.
                // Normally this is calculated on each pixel written to the video buffer. Placing it out here
                // and doing the calculation only once, helps to gain back some speed we loose with doing the 16 bit color.
                // Mask out the junk bits and shift it just enough to use it as an index into the the first color
                shadeB = ( I & 0xFFFFFF00 ) >> 3;
                // Shift it 5 more bits so that we have the actual value of the intensity.
                // Then add the offset into the shaded table to the beginning of the green values of the shaded table.
                shadeG = (shadeB >> 5) + 0x400;
                // For the red part of the shaded table, just add in another offset
                shadeR = shadeG + 0x400;

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;
                    
                    while( Length > 0 )
                    {
                        --Length;
                                
                        // Get the pixel in question
                        pixel = IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ];

                        result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                 pShade[ shadeG | (pixel & 0x3E0) ] | 
                                 (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                        
                        
                        // Save the resulted pixel
                        *DPtr = result;

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {                    
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );
                    
                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;
                                
                        // Get the pixel in question
                        pixel = IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ];

                        result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                 pShade[ shadeG | (pixel & 0x3E0) ] | 
                                 (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                        
                        // Save the resulted pixel
                        *DPtr = result;

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon16To16NoZBuffering


/************************************************************************
*    FUNCTION NAME:         RenderPolygon16To16NoZBufferingOr()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           16 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon16To16NoZBufferingOr()
{
#ifndef NO_16_TO_16_NO_Z_BUF_OR

    int Top(0);
    
    int TxStep, TyStep;
    int Z, RZ, U, V, Tx1, Ty1, Tx2, Ty2;//, I, IStep;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *IPtr, *pVidBuf;

    // Init the video buffer pointer
    pVidBuf = (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////


    // Get pointer to the texture map
    IPtr = (WORD *)pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();


    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint [ N ].Y < SPoint [ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            //RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                //RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                //RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 )    // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;       // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    //RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            //LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                //LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                //LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;       // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    //LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////


                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                //I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                //IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );
                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    //I += IStep * Step;
                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                DPtr = &pVidBuf[ YIndex + XStart ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        *DPtr |= IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ];

                        Z += ZStep;
                        //I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        *DPtr |= IPtr[ ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10 ];

                        Z += ZStep;
                        //I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            //RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            //LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon16To16NoZBufferingOr


/************************************************************************
*    FUNCTION NAME:         RenderPolygon16To16NoZBufferingTrans()                                                             
*
*    DESCRIPTION:           This function renders the textured polygon to
*                           the buffer. Because speed is the highest priority,
*                           this function uses every trick I can think of to
*                           speed up the process. Each trick has been tested
*                           to ensure that a speed increase has been achieved.
*                           Also, to cut down on the number of "IF" statements,
*                           there are slightly different version of this same
*                           function. That is why these functions break the rules.
*
*                           16 bit graphics to a 16 bit buffer
*                           This function is the work horse because it
*                           blits each pixel to the buffer
*
*                           When your plotting all the pixels to the screen, 
*                           you can elimate clearing the Z buffer by filp
*                           flopping the Z buffer from positive to negative
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
*    Howie        12/20/01      In a desprate effort to speed up this
*                               function, I removed all function calls
*                               and placed all the code from the function
*                               calls in this function. It makes the
*                               function much longer, but it also contributed
*                               to a considerable speed improvement.
*
*   Howie         3/16/02       To once again speed up this function, I have
*                               broken out the pos and neg z buffer flip flopping
*                               into their own seperate function to elimate repeated
*                               IF statement calls.
*
*   Howie         3/22/02       Yet again to speed up this function, I have made
*                               all varaibles that can be local to this function local.
*                               Local variables seem to be faster then all the rest.  
************************************************************************/

void Panel3D::RenderPolygon16To16NoZBufferingTrans()
{
#ifndef NO_16_TO_16_NO_Z_BUF_TRANS

    int Top(0);

    int TxStep, TyStep;
    int Z, RZ, U, V, Tx1, Ty1, Tx2, Ty2, I, IStep;
    int NewRightPos, NewLeftPos; 
    int Height, YIndex, Width, XStart, XEnd;  
    int ZStep, UStep, VStep, IPos;
    int RunLoops, Length; 
    int SubUStep, SubVStep, SubZStep, Step;
    WORD pixel, result, shadeR, shadeG, shadeB, *pVidBuf;

    /////  Use local varables for speed
    int EdgeCount(SPCount), tmpSPCount(SPCount);
    const int ScrnW(ScreenW), ScrnH(ScreenH), MultShift(PreMultShift);
    const DWORD NewTMask( TMASK * pTextDat->TMap[ Behavior.TextHan ].Width );
    const int XSTEP_PREC(10);
    const int RunLen(16), RunShift(4), RunLenMinus1(15);
    const int CEIL_FRACT( ( 1 << XSTEP_PREC ) - 1 );    // Ceiling step constant:
    /////  Use local varables for speed

    CeilLine LeftSeg, RightSeg;

    /////////////////////////////////////////////
    // Color depth specific data types
    // These should all be words
    WORD register *DPtr, *IPtr, *pShade;

    // Init the shaded table pointer
    pShade = (WORD *)pTextShade;

    // Init the video buffer pointer
    pVidBuf =  (WORD *)pVBuffer;
    // Color depth specific data types
    ///////////////////////////////////////////

    // Get pointer to the texture map
    IPtr = (WORD *)pTextDat->TMap[ Behavior.TextHan + FrameCounter ].GetTexturePtr();

    // Search for lowest Y coordinate (top of polygon):
    for( int N = 1; N < EdgeCount; ++N )
    {
        if( SPoint[ N ].Y < SPoint[ Top ].Y )
            Top = N;
    }

    // This will initilize faster
    int RightPos(Top), LeftPos(Top);

    // Calculate the index to the buffer:
    YIndex = SPoint[ Top ].Y * ScrnW;

    // Loop for all polygon edges:
    while( EdgeCount > 0 )
    {
        // Determine if the right side of the polygon needs 
        // (re)initializing:
        if( RightSeg.EdgeHeight <= 0 )
        {
            NewRightPos = RightPos + 1;

            if( NewRightPos >= tmpSPCount )
                NewRightPos = 0;

            //RightSeg.Init( SPoint[ RightPos ], SPoint[ NewRightPos ] );

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X1 = SPoint[ RightPos ].X;
            RightSeg.X2 = SPoint[ NewRightPos ].X;
            RightSeg.Y1 = SPoint[ RightPos ].Y;
            RightSeg.Y2 = SPoint[ NewRightPos ].Y;

            RightSeg.EdgeHeight = ( RightSeg.Y2 - RightSeg.Y1 );

            RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            RightSeg.Y = RightSeg.Y1;
            RightSeg.Z = SPoint[ RightPos ].Z;

            // Store U/Z and V/Z values:
            RightSeg.U = SPoint[ RightPos ].U;
            RightSeg.V = SPoint[ RightPos ].V;

            // Store intensity value:
            RightSeg.I = SPoint[ RightPos ].I;

            if( RightSeg.EdgeHeight )
            {
                RightSeg.StepX = ( ( RightSeg.X2 - RightSeg.X1 ) << XSTEP_PREC ) / RightSeg.EdgeHeight;
                RightSeg.StepZ = ( SPoint[ NewRightPos ].Z - SPoint[ RightPos ].Z ) / RightSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                RightSeg.StepU = ( SPoint[ NewRightPos ].U - SPoint[ RightPos ].U ) / RightSeg.EdgeHeight;
                RightSeg.StepV = ( SPoint[ NewRightPos ].V - SPoint[ RightPos ].V ) / RightSeg.EdgeHeight;
                // Calculate intensity delta
                RightSeg.StepI = ( SPoint[ NewRightPos ].I - SPoint[ RightPos ].I ) / RightSeg.EdgeHeight;
            }
            else
            {
                RightSeg.StepX = 0;
                RightSeg.StepZ = 0;
                RightSeg.StepU = 0;
                RightSeg.StepV = 0;
                RightSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            /////////////////////////////////////////////////////////////////////////////////////


            RightPos = NewRightPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( RightSeg.Y < 0 ) // 0 was MINY
            {
                //RightSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( RightSeg.Y2 != RightSeg.Y1 )
                {
                    Step = -RightSeg.Y;  // 0 was MINY

                    RightSeg.X = ( RightSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( RightSeg.X2 - RightSeg.X1 ) / 
                        ( double ) (RightSeg.Y2 - RightSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    RightSeg.Y = 0;    // 0 was MINY
                    RightSeg.Z += RightSeg.StepZ * Step;
                    RightSeg.U += RightSeg.StepU * Step;
                    RightSeg.V += RightSeg.StepV * Step;
                    RightSeg.I += RightSeg.StepI * Step;
                    RightSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW
            }
        }

        // Determine if the left side of the polygon needs 
        // (re)initializing:
        if( LeftSeg.EdgeHeight <= 0 )
        {
            NewLeftPos = LeftPos - 1;

            if( NewLeftPos < 0 )
                NewLeftPos = ( tmpSPCount - 1 );

            //LeftSeg.Init( SPoint[ LeftPos ], SPoint[ NewLeftPos ] );

            /////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X1 = SPoint[ LeftPos ].X;
            LeftSeg.X2 = SPoint[ NewLeftPos ].X;
            LeftSeg.Y1 = SPoint[ LeftPos ].Y;
            LeftSeg.Y2 = SPoint[ NewLeftPos ].Y;

            LeftSeg.EdgeHeight = ( LeftSeg.Y2 - LeftSeg.Y1 );

            LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + CEIL_FRACT;
            LeftSeg.Y = LeftSeg.Y1;
            LeftSeg.Z = SPoint[ LeftPos ].Z;

            // Store U/Z and V/Z values:
            LeftSeg.U = SPoint[ LeftPos ].U;
            LeftSeg.V = SPoint[ LeftPos ].V;

            // Store intensity value:
            LeftSeg.I = SPoint[ LeftPos ].I;

            if( LeftSeg.EdgeHeight )
            {
                LeftSeg.StepX = ( ( LeftSeg.X2 - LeftSeg.X1 ) << XSTEP_PREC ) / LeftSeg.EdgeHeight;
                LeftSeg.StepZ = ( SPoint[ NewLeftPos ].Z - SPoint[ LeftPos ].Z ) / LeftSeg.EdgeHeight;
                // Calculate U/Z and V/Z deltas:
                LeftSeg.StepU = ( SPoint[ NewLeftPos ].U - SPoint[ LeftPos ].U ) / LeftSeg.EdgeHeight;
                LeftSeg.StepV = ( SPoint[ NewLeftPos ].V - SPoint[ LeftPos ].V ) / LeftSeg.EdgeHeight;
                // Calculate intensity delta
                LeftSeg.StepI = ( SPoint[ NewLeftPos ].I - SPoint[ LeftPos ].I ) / LeftSeg.EdgeHeight;
            }
            else
            {
                LeftSeg.StepX = 0;
                LeftSeg.StepZ = 0;
                LeftSeg.StepU = 0;
                LeftSeg.StepV = 0;
                LeftSeg.StepI = 0;
            }
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            ////////////////////////////////////////////////////////////////////////////////////


            LeftPos = NewLeftPos;
            --EdgeCount;

            // Perform object-precision clip on top of edge 
            // (if necessary):
            if( LeftSeg.Y < 0 ) // 0 was MINY
            {
                //LeftSeg.ClipTop( MINY );

                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                if( LeftSeg.Y2 != LeftSeg.Y1 )
                {
                    Step = -LeftSeg.Y;  // 0 was MINY

                    LeftSeg.X = ( LeftSeg.X1 << XSTEP_PREC ) + 
                        ( ( int ) ( (( double ) ( LeftSeg.X2 - LeftSeg.X1 ) / 
                        ( double ) (LeftSeg.Y2 - LeftSeg.Y1)) * 
                        ( double ) Step ) << XSTEP_PREC ) + CEIL_FRACT;

                    LeftSeg.Y = 0;   // 0 was MINY
                    LeftSeg.Z += LeftSeg.StepZ * Step;
                    LeftSeg.U += LeftSeg.StepU * Step;
                    LeftSeg.V += LeftSeg.StepV * Step;
                    LeftSeg.I += LeftSeg.StepI * Step;
                    LeftSeg.EdgeHeight -= Step;
                }
                // Pulled the function from the above CeilLine class to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                YIndex = 0; // was MINY * ScrnW;
            }
        }

        // Subdivide polygon into trapezoid:
        if( LeftSeg.EdgeHeight < RightSeg.EdgeHeight )
        {
            Height = LeftSeg.EdgeHeight;

            if( ( LeftSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - LeftSeg.Y;
                EdgeCount = 0;
            }
        }
        else 
        {
            Height = RightSeg.EdgeHeight;

            if( ( RightSeg.Y + Height ) > ScrnH )
            {
                Height = ScrnH - RightSeg.Y;
                EdgeCount = 0;
            }
        }

        // Loop for the height of the trapezoid:
        while( Height > 0 )
        {
            --Height;

            // Calculate initial values:
            XStart = LeftSeg.X >> XSTEP_PREC;
            XEnd = RightSeg.X >> XSTEP_PREC;
            Width = XEnd - XStart;

            if( ( Width > 0 ) && ( XEnd > 0 ) && ( XStart < ScrnW ) )
            {
                U = LeftSeg.U;
                V = LeftSeg.V;
                Z = LeftSeg.Z;
                I = LeftSeg.I;

                // Note: The following steps are linear in 
                // screen space and are constant throughout 
                // the entire polygon. Given enough Z 
                // precision (which reduces the efficiency
                // of the clear-reduction algorithm) they 
                // can be calculated a single time and 
                // used throughout the remainder of the 
                // rasterization:

                UStep = ( RightSeg.U - U ) / Width;
                VStep = ( RightSeg.V - V ) / Width;
                ZStep = ( RightSeg.Z - Z ) / Width;
                IStep = ( RightSeg.I - I ) / Width;

                // Clip the scan-line:
                //ClipHLine( XStart, XEnd, Z, ZStep, U, UStep,
                //           V, VStep, I, IStep );


                //////////////////////////////////////////////////////////////////////////////////
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be

                // Clip a horizontal "Z-buffered" line:
                if( XStart < 0 )
                {
                    Step = -XStart;
                    // Take advantage of the fact that ( a * ( b * f ) / f ) 
                    // is equal to ( a * b );

                    Z += ZStep * Step;
                    U += UStep * Step;
                    V += VStep * Step;
                    I += IStep * Step;

                    XStart = 0;
                    Width = XEnd;
                }
                //else if( XStart > ScreenW )  This evaluation is never done because to get into this
                //    XStart = ScreenW;        if statement, XStart has to be less then ScreenW

                //if( XEnd < MINX )            This evaluation is never done because to get into this
                //    XEnd = MINX;             if statement, XEnd has to be greater then MINX

                //else if( XEnd > ScreenW )

                if( XEnd > ScrnW )
                {
                    XEnd = ScrnW;
                    Width = XEnd - XStart;
                }
                // Pulled the function from the above ClipHLine function to help speed up the process.
                // Makes the function longer and messy, but it is also faster because
                // we are not making a function call and passing data on the stack.
                // We need this to be as fast as it can be
                /////////////////////////////////////////////////////////////////////////////////

                DPtr = &pVidBuf[ YIndex + XStart ];

                // Calculate subdivision loops:
                RunLoops = Width >> RunShift;

                if( RunLoops > 0 )
                {
                    // Premultiply steps for subdivision:
                    SubUStep = UStep << RunShift;
                    SubVStep = VStep << RunShift;
                    SubZStep = ZStep << RunShift;
                }

                // Get the value of the shade for each color. This value ia also designed to help index into the shaded table.
                // Normally this is calculated on each pixel written to the video buffer. Placing it out here
                // and doing the calculation only once, helps to gain back some speed we loose with doing the 16 bit color.
                // Mask out the junk bits and shift it just enough to use it as an index into the the first color
                shadeB = ( I & 0xFFFFFF00 ) >> 3;
                // Shift it 5 more bits so that we have the actual value of the intensity.
                // Then add the offset into the shaded table to the beginning of the green values of the shaded table.
                shadeG = (shadeB >> 5) + 0x400;
                // For the red part of the shaded table, just add in another offset
                shadeR = shadeG + 0x400;

                // Loop for number of subdivisions:
                for( int Count = 0; Count < RunLoops; ++Count )
                {
                    Length = RunLen;

                    // Calculate affine mapping coordinates
                    // based on perspective subdivision:

                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += SubUStep;
                    V  += SubVStep;
                    RZ = SubZStep + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );

                    TxStep = ( Tx2 - Tx1 ) >> RunShift;
                    TyStep = ( Ty2 - Ty1 ) >> RunShift;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply texture's Y coordinate:
                    Ty1 <<= MultShift;
                    
                    while( Length > 0 )
                    {
                        --Length;

                        // Get the position in the texture we are referencing
                        IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                        // Use the position to index into the shaded table
                        // to paint a pixel to the buffer. This will give the
                        // pixel a shaded color depending on distance and light source.
                        // Also, in 16 bit, the brite green color is considered to 
                        // be the transparent color.
                        if( IPtr[ IPos ] ^ 0x3E0 )
                        {
                            // Write the pixel
                            pixel = IPtr[ IPos ];

                            // Generate the new shaded pixel
                            result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                     pShade[ shadeG | (pixel & 0x3E0) ] | 
                                     (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                                                            
                            // Save the resulted pixel
                            *DPtr = result;
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }          
                }

                // Calculate remainder of scan-line left:
                // to rasterize:
                Length = Width & RunLenMinus1; // was ( RunLen - 1 );

                if( Length > 0 )
                {                    
                    fixdiv( Tx1, U, Z );
                    fixdiv( Ty1, V, Z );

                    // Jump ahead to next subdivision:
                    U  += ( UStep * Length );
                    V  += ( VStep * Length );
                    RZ = ( ZStep * Length ) + Z;

                    fixdiv( Tx2, U, RZ );
                    fixdiv( Ty2, V, RZ );
                    
                    TxStep = ( Tx2 - Tx1 ) / Length;
                    TyStep = ( Ty2 - Ty1 ) / Length;

                    // Premultiply the texture's Y step:
                    TyStep <<= MultShift;

                    // Premultiply the texture's Y coordinate:
                    Ty1 <<= MultShift;

                    while( Length > 0 )
                    {
                        --Length;

                        // Get the position in the texture we are referencing
                        IPos = ( ( Ty1 & NewTMask ) + Tx1 ) >> 0x10;

                        // Use the position to index into the shaded table
                        // to paint a pixel to the buffer. This will give the
                        // pixel a shaded color depending on distance and light source.
                        // Also, in 16 bit, the brite green color is considered to 
                        // be the transparent color.
                        if( IPtr[ IPos ] ^ 0x3E0 )
                        {
                            // Write the pixel
                            pixel = IPtr[ IPos ];

                            // Generate the new shaded pixel
                            result = pShade[ shadeB | (pixel & 0x1F) ] | 
                                     pShade[ shadeG | (pixel & 0x3E0) ] | 
                                     (pShade[ shadeR | ((pixel & 0x7C00) >> 0x5) ]);
                                                            
                            // Save the resulted pixel
                            *DPtr = result;
                        }

                        Z  += ZStep;
                        I  += IStep;
                        Tx1 += TxStep;
                        Ty1 += TyStep;
                        ++DPtr;
                    }
                }
            }

            //++RightSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            RightSeg.X += RightSeg.StepX;
            ++RightSeg.Y;
            RightSeg.Z += RightSeg.StepZ;
            RightSeg.U += RightSeg.StepU;
            RightSeg.V += RightSeg.StepV;
            RightSeg.I += RightSeg.StepI;
            --RightSeg.EdgeHeight; 

            //++LeftSeg; // Yes, this is a function. Believe it or not

            //////////////////////////////////////////////////////////////////////////////////
            // Pulled the function from the above CeilLine class to help speed up the process.
            // Makes the function longer and messy, but it is also faster because
            // we are not making a function call and passing data on the stack.
            // We need this to be as fast as it can be
            LeftSeg.X += LeftSeg.StepX;
            ++LeftSeg.Y;
            LeftSeg.Z += LeftSeg.StepZ;
            LeftSeg.U += LeftSeg.StepU;
            LeftSeg.V += LeftSeg.StepV;
            LeftSeg.I += LeftSeg.StepI;
            --LeftSeg.EdgeHeight;

            YIndex += ScrnW;
        }
    }
#endif
}   // RenderPolygon16To16NoZBufferingTrans


/************************************************************************
*    FUNCTION NAME:         WritePanelData()                                                             
*
*    DESCRIPTION:           Write panel data to the file
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

void Panel3D::WritePanelData( FILE *OutFile )
{
    // Save the panel behavior data
    fwrite( &Behavior, sizeof( PanelBehavior ), 1, OutFile );

}   // WritePanelData


/************************************************************************
*    FUNCTION NAME:         SetTextAttrib()                                                             
*
*    DESCRIPTION:           Set the textures attributes
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

void Panel3D::SetTextAttrib( int Odd, int X, int Y, int Width, int Height )
{
    int Tri = 0, i;

    if( ( *VPoint[ 0 ] ) == ( *VPoint[ 1 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 0 ] ) == ( *VPoint[ 2 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 0 ] ) == ( *VPoint[ 3 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 1 ] ) == ( *VPoint[ 2 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 1 ] ) == ( *VPoint[ 3 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 2 ] ) == ( *VPoint[ 3 ] ) )
        Tri = 1;

    if( Tri )
    {
        if( !Odd )
        {
            Behavior.Attrib[ 3 ].U = X;
            Behavior.Attrib[ 3 ].V = Y;

            Behavior.Attrib[ 2 ].U = Width - 1;
            Behavior.Attrib[ 2 ].V = Y;

            Behavior.Attrib[ 1 ].U = Width - 1;
            Behavior.Attrib[ 1 ].V = Height - 1;

            Behavior.Attrib[ 0 ].U = Width - 1;
            Behavior.Attrib[ 0 ].V = Height - 1;
        }

        if( Odd )
        {
            Behavior.Attrib[ 3 ].U = X;
            Behavior.Attrib[ 3 ].V = Y;

            Behavior.Attrib[ 2 ].U = Width - 1;
            Behavior.Attrib[ 2 ].V = Height - 1;

            Behavior.Attrib[ 1 ].U = X;
            Behavior.Attrib[ 1 ].V = Height - 1;

            Behavior.Attrib[ 0 ].U = X;
            Behavior.Attrib[ 0 ].V = Height - 1;
        }
    }
    else
    {
        Behavior.Attrib[ 0 ].U = X;
        Behavior.Attrib[ 0 ].V = Y;

        Behavior.Attrib[ 1 ].U = Width - 1;
        Behavior.Attrib[ 1 ].V = Y;

        Behavior.Attrib[ 2 ].U = Width - 1;
        Behavior.Attrib[ 2 ].V = Height - 1;

        Behavior.Attrib[ 3 ].U = X;
        Behavior.Attrib[ 3 ].V = Height - 1;
    }

    // Rotate the texture how ever many times we need to
    for( i = 0; i < Behavior.Rot; ++i )
        RotText();

}   // SetTextAttrib


/************************************************************************
*    FUNCTION NAME:         GetU()                                                             
*
*    DESCRIPTION:           Get the U value
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

int Panel3D::GetU()
{
    int Result;

    Result = Behavior.Attrib[ 3 - Behavior.Rot ].U;

    return Result;

}   // GetU


/************************************************************************
*    FUNCTION NAME:         GetV()                                                             
*
*    DESCRIPTION:           Get the V value
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

int Panel3D::GetV()
{
    int Tri = 0, Result, offset[4] = { 0,3,2,1 };

    if( ( *VPoint[ 0 ] ) == ( *VPoint[ 1 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 0 ] ) == ( *VPoint[ 2 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 0 ] ) == ( *VPoint[ 3 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 1 ] ) == ( *VPoint[ 2 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 1 ] ) == ( *VPoint[ 3 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 2 ] ) == ( *VPoint[ 3 ] ) )
        Tri = 1;

    if( Tri )
    {
        Result = Behavior.Attrib[ 3 - Behavior.Rot ].V;
    }
    else
    {
        Result = Behavior.Attrib[ offset[ Behavior.Rot ] ].V; // 0 original index
    }

    return Result;

}   // GetV


/************************************************************************
*    FUNCTION NAME:         GetUWidth()                                                             
*
*    DESCRIPTION:           Get the Width value
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

int Panel3D::GetUWidth()
{
    int Result, offset[4] = { 2,1,0,3 };

    Result = Behavior.Attrib[ offset[ Behavior.Rot ] ].U + 1;
    Result -= Behavior.Attrib[ 3 - Behavior.Rot ].U;

    return Result;

}   // GetUWidth


/************************************************************************
*    FUNCTION NAME:         GetVHeight()                                                             
*
*    DESCRIPTION:           Get the Height value
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

int Panel3D::GetVHeight()
{
    int Tri = 0, Result, offset1[4] = { 2,1,0,3 }, offset2[4] = { 1,0,3,2 }, offset3[4] = { 0,3,2,1 };

    if( ( *VPoint[ 0 ] ) == ( *VPoint[ 1 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 0 ] ) == ( *VPoint[ 2 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 0 ] ) == ( *VPoint[ 3 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 1 ] ) == ( *VPoint[ 2 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 1 ] ) == ( *VPoint[ 3 ] ) )
        Tri = 1;

    else if( ( *VPoint[ 2 ] ) == ( *VPoint[ 3 ] ) )
        Tri = 1;

    if( Tri )
    {
        Result = Behavior.Attrib[ offset2[ Behavior.Rot ] ].V + 1;;
        Result -= Behavior.Attrib[ 3 - Behavior.Rot ].V;
    }
    else
    {
        Result = Behavior.Attrib[ offset1[ Behavior.Rot ] ].V + 1;
        Result -= Behavior.Attrib[ offset3[ Behavior.Rot ] ].V; 
    }

    return Result;

}   // GetVHeight


/************************************************************************
*    FUNCTION NAME:         SetIntensity()                                                             
*
*    DESCRIPTION:           Set the intensenity of the texture
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

void Panel3D::SetIntensity( WORD Value )
{    
    Behavior.Attrib[ 0 ].I = Value;
    Behavior.Attrib[ 1 ].I = Value;
    Behavior.Attrib[ 2 ].I = Value;
    Behavior.Attrib[ 3 ].I = Value;

}   // SetInten


/************************************************************************
*    FUNCTION NAME:         Collide()                                                             
*
*    DESCRIPTION:           Check for collision
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

BOOL Panel3D::Collide( float &X, float &Y, float &Z, float Rad )
{
    BOOL Result = FALSE;

    #ifdef COLLISION_DETECTION

    float Dist, X2, Y2, Z2, A, B, C, D, Sign, MinDist, NormY;
    const float NOSE = 300.0F, VIEW_HEIGHT = 700.0F;

    Y -= VIEW_HEIGHT;

    X2 = ( VPoint [ 0 ]->Wx +
           VPoint[ 1 ]->Wx +
           VPoint[ 2 ]->Wx +
           VPoint[ 3 ]->Wx ) / 4.0F;

    Y2 = ( VPoint [ 0 ]->Wy +
           VPoint[ 1 ]->Wy +
           VPoint[ 2 ]->Wy +
           VPoint[ 3 ]->Wy ) / 4.0F;

    Z2 = ( VPoint [ 0 ]->Wz +
           VPoint[ 1 ]->Wz +
           VPoint[ 2 ]->Wz +
           VPoint[ 3 ]->Wz ) / 4.0F;

    // Calculate distance to panel:
    Dist = sqrt( ( X2 - X ) * ( X2 - X ) +
                 ( Y2 - Y ) * ( Y2 - Y ) +
                 ( Z2 - Z ) * ( Z2 - Z ) );

    // Check for possible collision by using bounding sphere:
    MinDist = Rad + Radius;

    if( Dist <= MinDist )
    {
        // Confirm/reject collision warning by calculating
        // which side of the plane the viewer will be on:
        A = ( Normal.Tx - VPoint [ 0 ]->Wx );
        B = ( Normal.Ty - VPoint [ 0 ]->Wy );
        C = ( Normal.Tz - VPoint [ 0 ]->Wz );
        D = -( A * VPoint [ 0 ]->Wx + B * VPoint [ 0 ]->Wy + C * VPoint [ 0 ]->Wz );
        Sign = A * X + B * Y + C * ( Z + NOSE ) + D;

        if( Sign <= 0.0F )
        {
            // Collision is definite
            Result = TRUE;

            // Store the Y component of the panel normal:
            NormY = B;

            // If panel is a wall....
            if ( fabs ( NormY ) < sqrt ( 0.5F ) )
            {
                if ( A )
                {
                    Z = ( A * X + B * Y + D ) / ( -C ) + 1.0F - NOSE;
                    X = ( B * Y + C * ( Z + NOSE ) + D ) / ( -A );
                }
                else
                    X = Z = 0.0F;
            }
            // Else panel is a floor or ceiling....
            else
            {
                // Determine if panel is a floor:
                if ( NormY > 0.0F )
                {
                    Y = ( A * X + C * Z + D ) / ( - B );
                }
            }
        }
    }

    Y += VIEW_HEIGHT;

    #endif

    return Result;

}   // Collide


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
void Panel3D::MoveText( int Ox, int Oy, int Tx, int Ty )
{
    float Dist [ 5 ], X, Y;
    int Count, ScreenCount, MinDist;

    Project();
    ScreenCount = SPCount;

    if( ScreenCount > 4 )
        ScreenCount = 4;

    // Calculate the distance from each vertex to (Ox, Oy):
    for( Count = 0; Count < ScreenCount; ++Count )
    {
        X = SPoint [ Count ].X;
        Y = SPoint [ Count ].Y;
        Dist [ Count ] = sqrt ( ( X - Ox ) * ( X - Ox ) +
                          ( Y - Oy ) * ( Y - Oy ) );
    }

    // Select minimum distance:
    MinDist = 0;

    for( Count = 1; Count < ScreenCount; ++Count )
    {
        if ( Dist [ Count ] < Dist [ MinDist ] )
            MinDist = Count;
    }

    // Translate texture vertex by specified amount:
    Behavior.Attrib [ MinDist ].U += Tx;
    Behavior.Attrib [ MinDist ].V += Ty;

    // Clip selected translation:
    if( Behavior.Attrib[ MinDist ].U < 0 )
        Behavior.Attrib[ MinDist ].U = 0;

    if( Behavior.Attrib[ MinDist ].V < 0 )
        Behavior.Attrib[ MinDist ].V = 0;

    if( Behavior.Attrib[ MinDist ].U >= pTextDat->TMap[ Behavior.TextHan ].Width )
    {
        Behavior.Attrib[ MinDist ].U = pTextDat->TMap[ Behavior.TextHan ].Width - 1;
    }

    if( Behavior.Attrib[ MinDist ].V >= pTextDat->TMap[ Behavior.TextHan ].Height )
    {
        Behavior.Attrib[ MinDist ].V = pTextDat->TMap[ Behavior.TextHan ].Height - 1;
    }

}   // MoveText
*/

/************************************************************************
*    FUNCTION NAME:         ReadPanelData()                                                             
*
*    DESCRIPTION:           Read panel data from the file
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

void Panel3D::ReadPanelData( FILE *InFile, int TextureCount )
{
    // Read the panel behavior data
    fread( &Behavior, sizeof( PanelBehavior ), 1, InFile );

    // Double check that the TextHan is not greater then the number of
    // textures in the library. If so set it to the first texture handle
    if( Behavior.TextHan >= TextureCount )
        Behavior.TextHan = 0;    

}   // ReadPanelData


/************************************************************************
*    FUNCTION NAME:         ReadPanelDataFromPointer()                                                             
*
*    DESCRIPTION:           Read panel data from a pointer
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

BYTE *Panel3D::ReadPanelDataFromPointer( BYTE *pData )
{   
    PPanelBehavior pPanelData;

    // init the pointer
    pPanelData = PPanelBehavior( pData );

    // Read the panel behavior data
    memcpy( &Behavior, pPanelData, sizeof( PanelBehavior ) );
    
    // Inc the pointer
    pData += sizeof( PanelBehavior );
    
    return pData;    

}   // ReadPanelDataFromPointer


/************************************************************************
*    FUNCTION NAME:         InitPanel()                                                             
*
*    DESCRIPTION:           Init the panel with pointer from PolyObj
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

BOOL Panel3D::InitPanel( int Width, int Height, int *pZBuffer,
                         PImageData pImageData, void *pShadeDat, void *VBuffer, 
                         int aCameraScale, int ZDist, int LightDist )
{ 
    ScreenW = Width; 
    ScreenH = Height; 
    ZBuffer = pZBuffer;
    pTextDat = pImageData;
    pTextShade = pShadeDat;
    CameraScale = aCameraScale;
    MaxZDistance = ZDist;
    MaxLightDistance = LightDist;
    pVBuffer = VBuffer;

    // Get the total size of the textured buffer. We need to check
    // that we are not over running the buffer when rendering the video buffer.
    //ImageBufferSize = pTextDat->TMap[ Behavior.TextHan ].GetBufferSize();

    // Do the PreMultShift now so it doesn't have to be done later
    switch( pTextDat->TMap[ Behavior.TextHan ].Width )
    {
        case ( 2 ):   PreMultShift = 1; break;
        case ( 4 ):   PreMultShift = 2; break;
        case ( 8 ):   PreMultShift = 3; break;
        case ( 16 ):  PreMultShift = 4; break;
        case ( 32 ):  PreMultShift = 5; break;
        case ( 64 ):  PreMultShift = 6; break;
        case ( 128 ): PreMultShift = 7; break;
        case ( 256 ): PreMultShift = 8; break;
        case ( 512 ): PreMultShift = 9; break;
    }

    return TRUE;
    
}   // InitPanel
 

/************************************************************************
*    FUNCTION NAME:         HasVert()                                                             
*
*    DESCRIPTION:           Checks for veritcies
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

int Panel3D::HasVert( Point3D &P )
{
    if( ( *VPoint [ 0 ] ) == P )
        return 1;
    if( ( *VPoint [ 1 ] ) == P )
        return 1;
    if( ( *VPoint [ 2 ] ) == P )
        return 1;
    if( ( *VPoint [ 3 ] ) == P )
        return 1;

    return 0;

}   // HasVert


/************************************************************************
*    FUNCTION NAME:         RotText()                                                             
*
*    DESCRIPTION:           Rotate textures
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

void Panel3D::RotText()
{
    Detail3D Temp;
    Temp = Behavior.Attrib [ 0 ];
    Behavior.Attrib [ 0 ] = Behavior.Attrib [ 1 ];
    Behavior.Attrib [ 1 ] = Behavior.Attrib [ 2 ];
    Behavior.Attrib [ 2 ] = Behavior.Attrib [ 3 ];
    Behavior.Attrib [ 3 ] = Temp;

}   // RotText


/************************************************************************
*    FUNCTION NAME:         IsPointInsidePolygon()                                                             
*
*    DESCRIPTION:           Check to see if the point is inside the polygon
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

BOOL Panel3D::IsPointInsidePolygon( int X, int Y )
{
    POINT points[6];
    BOOL Result = FALSE;
    HRGN PolyRgn;

    // Init a point array to the points in this polygon
    InitPolygonPoints( points );

    // Create a polygon reagon to test if the point is in that regoin
    PolyRgn = CreatePolygonRgn( &points[0], SPCount+1, WINDING );

    if( PolyRgn )
    {
        // Is the point in this region?
        Result = PtInRegion( PolyRgn, X, Y );

        // Delete the region
        DeleteObject( PolyRgn );
    }

    /*int i, j, minX, maxX, minY, maxY, Count=0;
    BOOL FirstInsideLine, InsideLine;
    float LinePoint, NextX, NextY, CurX, CurY, PointX, PointY;

    // Need to use all floats
    CurX = X;
    CurY = Y;

    // Make sure the panel has more than two points
    if( SPCount > 2 )
    {
        // Determine location of panel's 2D points
        for( i = 0; i < SPCount; ++i )
        {
            // Get two sets of point to create a line
            PointX = SPoint[ i ].X;
            PointY = SPoint[ i ].Y;
            NextX = SPoint[ ( i + 1 ) % SPCount ].X;
            NextY = SPoint[ ( i + 1 ) % SPCount ].Y;

            // Get the value of the two line points divided
            LinePoint = PointY - NextY / PointX - NextX;

            // Take the first set of points and test if the point we are looking for
            // is on the inside or outside of the line
            if( i == 0 )
            {
                if( NextY - CurY > LinePoint * ( PointX - CurX ) )
                    FirstInsideLine = TRUE;
                else
                    FirstInsideLine = FALSE;
            }
            else
            {
                if( NextY - CurY > LinePoint * ( PointX - CurX ) )
                    InsideLine = TRUE;
                else
                    InsideLine = FALSE;

                // The return value of (abs( NextY - Y ) > LinePoint * abs( SPoint[ i ].X - NextX ))
                // will not be the same if the point is within the polygon. All other polygons will return
                // all TRUE or all FALSE;
                if( FirstInsideLine != InsideLine )
                {
                    ++Count;

                    // Now check the bounding rect
                    for( j = 0; j < SPCount; ++j )
                    {
                        if( j == 0 )
                        {
                            minX = SPoint[ j ].X;
                            minY = SPoint[ j ].Y;
                            maxX = SPoint[ j ].X;
                            maxY = SPoint[ j ].Y;
                        }
                        else
                        {
                            if( minX > SPoint[ j ].X )
                                minX = SPoint[ j ].X;

                            if( minY > SPoint[ j ].Y )
                                minY = SPoint[ j ].Y;

                            if( maxX < SPoint[ j ].X )
                                maxX = SPoint[ j ].X;

                            if( maxY < SPoint[ j ].Y )
                                maxY = SPoint[ j ].Y;
                        }
                    }

                    // Testthe rect
                    if( X >= minX && X <= maxX &&
                        Y >= minY && Y <= maxY )
                        Result = TRUE;

                    InitPolygonPoints( points );

                    // Create a polygon reagon to test if the point is in that regoin
                    PolyRgn = CreatePolygonRgn( &points[0], SPCount+1, WINDING );

                    if( PolyRgn )
                    {
                        // Is the point in this region?
                        Result = PtInRegion( PolyRgn, X, Y );

                        // Delete the region
                        DeleteObject( PolyRgn );
                    }

                    break;
                }
            }
        }
    }*/

    return Result;

}   // IsPointInsidePolygon


/************************************************************************
*    FUNCTION NAME:         OutlinePolygon()                                                             
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

void Panel3D::OutlinePolygon( HDC DC )
{
    POINT points[6];

    // Make sure the panel has more than two points
    if( SPCount > 2 )
    {
        // Fill out the point structure with all the points
        if( InitPolygonPoints( points ) )

            // Draw the points to the screen
            Polyline( DC, &points[0], SPCount+1 );
    }

}   // IsPointInsidePolygon


/************************************************************************
*    FUNCTION NAME:         InitPolygonPoints()                                                             
*
*    DESCRIPTION:           Init the point array with the polygon points
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   FALSE - Pointes are outside the viewing area
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL Panel3D::InitPolygonPoints( POINT *points )
{
    int i, BehindZ = 0;
    BOOL Result = TRUE;

    // Fill out the point structure with all the points
    for( i = 0; i < SPCount; ++i )
    {
        points[ i ].x = SPoint[ i ].X;
        points[ i ].y = SPoint[ i ].Y;

        // Check to see if we are behind the minimum Z
        if( SPoint[ i ].Z < MINZ )
            ++BehindZ;
    }

    // Add the first points again to close the polygon
    points[ i ].x = SPoint[ 0 ].X;
    points[ i ].y = SPoint[ 0 ].Y;

    // If all the points are behind the minimum Z, then 
    // don't let it draw the line
    if( BehindZ == SPCount )
        Result = FALSE;

    return Result;

}   // InitPolygonPoints


/************************************************************************
*    FUNCTION NAME:         UpdateAnimatedPoly()                                                             
*
*    DESCRIPTION:           Inc the animated polgon's frame counter
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

void Panel3D::UpdateAnimatedPoly( DWORD MSTime )
{
    // If FrameCount has been set, then this is an animated panel
    if( Behavior.FrameCount )
    {
        // If time has elapsed, inc to the next frame
        if( FrameTimer < MSTime )
        {
            // Inc the frame counter
            FrameCounter = ( FrameCounter + 1 ) % Behavior.FrameCount;

            // Reset the frame timer
            FrameTimer = MSTime + Behavior.FrameRate;
        }
    }
}   // UpdateAnimatedPoly



/*

My attemps at assembly that didn't yeald faster code

// We need to do some assembly here to gain some speed
asm
{
    // Get the intensity
    mov eax, I          // Get the fixed point math intensity value
    and eax, 0xFFFFFF00 // Mask out the junk
    shr eax, 1          // Shift it right to get the value which indexes into the first color of the array
                        // Shifting it twice leaves the number big enough to offsets in byte numbers

    mov shade, eax      // save the offset into the first part of the color table

    // Set the shaded table pointer into edi
    mov edi, [pShade]

    // Seperate out the red component
    mov eax, pixel   // Copy the pixel value into eax register
    and eax, 0x1F    // Use the and to clear out all the bits except the last 5
    shl eax, 2       // Quad the value (value * 4) to index into the shaded table
    add eax, shade   // add in the shaded table value to get the proper index

    add edi, eax     // Add the offset to the shaded table pointer for the next color
    mov eax, [edi]   // Have to transfer the value to eax first
    mov result, eax  // Save the value to the result

    add shade, 0x1000  // inc the shaded table for the next color

    // Seperate out the green component
    mov eax, pixel   // Copy the pixel value into eax register
    and eax, 0x3E0   // Use the and to clear out all the bits except the middle 5
    shr eax, 3       // Shift it so that the bits we care about is the last 6
    add eax, shade   // add in the shaded table value to get the proper index

    // Set the shaded table pointer
    add edi, eax     // Index to the Green element of the shaded table
    mov eax, [edi]   // Save the value from the shaded table
    shl eax, 5       // Shift the value 5 time to the left for the green color
    or result, eax   // OR the value to the result

    add shade, 0x1000  // inc the shaded table for the next color

    // Seperate out the green component
    mov eax, pixel   // Copy the pixel value into eax register
    and eax, 0x7C00  // Use the and to clear out all the bits except the last 5
    shr eax, 8       // Shift it so that the bits we care about is the last 6
    add eax, shade   // add in the shaded table value to get the proper index

    // Set the shaded table pointer
    add edi, eax     // Index to the Blue element of the shaded table
    mov eax, [edi]   // Save the value from the shaded table
    shl eax, 10      // Shift the value 10 time to the left for the blue color
    or result, eax   // OR the value to the result
}


// We need to do some assembly here to gain some speed
asm
{
    mov eax, I       // Get the fixed point math intensity value
    shr eax, 8       // Shift it right to get the value which is 0 to 31
    mov shade, eax   // save the value in the local variable
    
    // Seperate out the red component
    mov eax, pixel   // Copy the pixel value into eax register
    and eax, 0x1F    // Use the and to clear out all the bits except the last 5                               
    sub eax, shade   // Subtract the shade from this value
    jc SHORT NEG1    // If the cary flag is set, we rolled over then number so jump to zero it out
    jmp SHORT SAVE1  // If we didn't roll the number, jump to where the result is saved

    NEG1:
    mov result, 0    // The number was negative so just zero out
    jmp SHORT FIN1 

    SAVE1:
                                      
    mov result, eax  // Save the subtracted result

    FIN1:            // Finish
    
    // Seperate out the green component
    mov eax, pixel   // Copy the pixel value into eax register
    shr eax, 5       // Shift it so that the bits we care about is the last 5
    and eax, 0x1F    // Use the and to clear out all the bits except the last 5                               
    sub eax, shade   // Subtract the shade from this value
    jc SHORT FIN2    // If the cary flag is set, we rolled over then number so jump to finish because we don't need to use what is in eax
    shl eax, 5       // Since we have a proper value, shift it left 5 times to put it back into the green place                             
    or result, eax   // OR the eax into result to put the bits into it's proper place

    FIN2:
    
    // Seperate out the blue component
    mov eax, pixel   // Copy the pixel value into eax register
    shr eax, 10      // Shift it so that the bits we care about is the last 5
    and eax, 0x1F    // Use the and to clear out all the bits except the last 5                               
    sub eax, shade   // Subtract the shade from this value
    jc SHORT FIN3    // If the cary flag is set, we rolled over then number so jump to finish because we don't need to use what is in eax
    shl eax, 10      // Since we have a proper value, shift it left 10 times to put it back into the blue place
    or result, eax   // OR the eax into result to put the bits into it's proper place

    FIN3:
    
    // Set the video pointer
    //mov edi, [DPtr]
    //mov [WORD PTR edi], eax  
}


WORD shaTbl[3072] = { 

0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,
0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,
0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,
0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,
0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,
0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,
0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,
0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,
0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,
0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,
0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                   
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,
21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,
22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,
23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,
24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,
25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,
26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,
27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,
28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,
29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,
30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,                      
31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, 
                      
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,0,
21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,0,
22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,0,
23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,0,
24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,0,
25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0,
26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,
27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,0,
28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,0,
29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,0,
30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0,0,                      
31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0, };

*/