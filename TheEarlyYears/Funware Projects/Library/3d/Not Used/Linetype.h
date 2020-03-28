//
// File name: LineType.h
//
// Description: A fixed-point implementation of a polygon edge 
//             algorithm
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef LINETYPEHPP
#define LINETYPEHPP

#include <Math.H>
#include "Point2D.h"

const XSTEP_PREC = 10;

// TSTEP_PREC and ZSTEP_PREC must be equal:
const ZSTEP_PREC = 26;
const TSTEP_PREC = ZSTEP_PREC;
const LSTEP_PREC = 16;
const ISTEP_PREC = 8;
const TMASK      = ( 0xFFFFFFFF << LSTEP_PREC );
const IMASK      = ( 0xFFFFFFFF << ISTEP_PREC );


// Ceiling step constant:
const int CEIL_FRACT = ( ( 1 << XSTEP_PREC ) - 1 );


struct CeilLine
{

    int X1, X2, Y1, Y2;
    int X, StepX, StepZ;
    int Z, U, V, StepU;
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


// A class that steps on the ceiling of integers - always steps
// on Y, rendering it useless for anything but polygon
// rasterization:
/*
class CeilLine
{
protected:

    //inline void Step();
    //inline void Step( int Amount );

public:
    int X1, X2, Y1, Y2;
    int X, StepX, StepZ, Z, U, V, StepU, StepV, I, StepI;
    int EdgeHeight, Y;

    // The constructor
    CeilLine()
	{
	    X1 = X2 = Y1 = Y2 = 0;
	    X = StepX = StepZ = Z = 0;
	    U = V = StepU = StepV = I = StepI = 0;
	    EdgeHeight = Y = 0;
	}

	
    // The initialize function
    inline void Init( Point2D &P1, Point2D &P2 );
    // Object-precision clip against top of viewport
    inline void ClipTop( const int Top );   
    // Steps polygon edge 
    inline void operator ++ ()
    {   X += StepX;
        ++Y;
        Z += StepZ;
        U += StepU;
        V += StepV;
        I += StepI;
        --EdgeHeight; 
    }
    // Steps polygon edge        
    inline void operator ++ ( int )
    {   X += StepX;
        ++Y;
        Z += StepZ;
        U += StepU;
        V += StepV;
        I += StepI;
        --EdgeHeight; 
    }
    // Steps edge by "Amount"
    inline void operator += ( int Amount ) 
    {
        X += ( StepX * Amount );
        Y += ( Amount );
        Z += ( StepZ * Amount );
        U += ( StepU * Amount );
        V += ( StepV * Amount );
        I += ( StepI * Amount );
        EdgeHeight -= ( Amount );
    }
    // Returns a point P + Amount
    inline Point2D operator + ( int Amount );
    // Returns the current X coordinate
    inline int GetX() { return ( X >> XSTEP_PREC ); }
    // Returns the current Y coordinate
    inline int GetY() { return Y; }
    // Returns the current 1/Z coordinate 
    inline int GetZ() { return Z; }
    // Returns the current 1/U texture coordinate 
    inline int GetU() { return U; }
    // Returns the current 1/V texture coordinate
    inline int GetV() { return V; }
    // Return the current intensity
    inline int GetI() { return I; }
    // Returns the current poly height
    inline int Height() { return EdgeHeight; }
	
};
*/

/************************************************************************
*    FUNCTION NAME:         operator()                                                             
*
*    DESCRIPTION:           Return a point P + Amount
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
inline Point2D CeilLine::operator + ( int Amount )
{
    Point2D Temp;
    Temp.X = X + ( StepX * Amount );
    Temp.Y = Y + ( Amount );
    Temp.Z = Z + ( StepZ * Amount );
    Temp.U = U + ( StepU * Amount );
    Temp.V = V + ( StepV * Amount );
    Temp.I = I + ( StepI * Amount );

    return Temp;
}
*/
/************************************************************************
*    FUNCTION NAME:         Init()                                                             
*
*    DESCRIPTION:           Calculate initial values for polygon edge
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
inline void CeilLine::Init( Point2D &P1, Point2D &P2 )
{
    X1 = P1.X; X2 = P2.X;
    Y1 = P1.Y; Y2 = P2.Y;

    EdgeHeight = ( Y2 - Y1 );

    X = ( X1 << XSTEP_PREC ) + CEIL_FRACT;
    Y = Y1;
    Z = P1.Z;

    // Store U/Z and V/Z values:
    U = P1.U;
    V = P1.V;

    // Store intensity value:
    I = P1.I;

    if( EdgeHeight )
    {
        StepX = ( ( X2 - X1 ) << XSTEP_PREC ) / EdgeHeight;
        StepZ = ( P2.Z - P1.Z ) / EdgeHeight;
        // Calculate U/Z and V/Z deltas:
        StepU = ( P2.U - P1.U ) / EdgeHeight;
        StepV = ( P2.V - P1.V ) / EdgeHeight;
        // Calculate intensity delta
        StepI = ( P2.I - P1.I ) / EdgeHeight;
    }
    else
    {
        StepX = 0;
        StepZ = 0;
        StepU = 0;
        StepV = 0;
        StepI = 0;
    }
}   // Init
*/

/************************************************************************
*    FUNCTION NAME:         ClipTop()                                                             
*
*    DESCRIPTION:           Clip the polygon edge against the top of the 
*                           viewport. Note: must be called directly after
*                           edge initialization
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
inline void CeilLine::ClipTop( const int Top )
{
    //double SlopeX;
    //int Height( Y2 - Y1 );

    if( ( Y < Top ) && ( Y2 - Y1 ) )
    {
        int Stepit( Top - Y ), Height( Y2 - Y1 );
        double SlopeX( ( double ) ( X2 - X1 ) / ( double ) Height);

        X = ( X1 << XSTEP_PREC ) + 
        ( ( int ) ( SlopeX * ( double ) Stepit ) << XSTEP_PREC ) + CEIL_FRACT;

        Y = Top;
        Z += StepZ * Stepit;
        U += StepU * Stepit;
        V += StepV * Stepit;
        I += StepI * Stepit;
        EdgeHeight -= Stepit;
    }
}   // ClipTop
*/
#endif