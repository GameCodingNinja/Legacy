//
// File name: Point3D.h
//
// Description: The header file a 3D point class
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

#ifndef POINT3DHPP
#define POINT3DHPP

#include <Math.H>
#include <Stdio.H>

// Turn off the data type conversion warning (ie. int to float, float to int etc.)
// We do this all the time in 3D. Don't need to be bugged by it all the time.
#pragma warning(disable : 4244)

struct PointLoad
{
	float Lx, Ly, Lz;
};
typedef PointLoad *PPointLoad;

// The 3-dimensional point class:
class Point3D
{

public:
    float Lx, Ly, Lz;  // The local X, Y and Z
    float Wx, Wy, Wz;  // The world X, Y and Z
    Point3D() { Wx = Wy = Wz = Lx = Ly = Lz = 0.0F; }
    // The equality operator:
    inline int operator == ( Point3D &V );
    // The inequality operator:
    inline int operator != ( Point3D &V );
    // The subtraction operator:
    inline Point3D operator - ( Point3D &V );
    // The addition operator:  
    inline Point3D operator + ( Point3D &V );
    // The multiplication operator:  
    inline Point3D operator * ( Point3D &V );
    // The division operator:  
    inline Point3D operator / ( Point3D &V );
    // The subtraction/assignment operator:  
    inline Point3D &operator -= ( Point3D &V );
    // The addition/assignment operator:  
    inline Point3D &operator += ( Point3D &V );
    // The multiplication/assignment operator:  
    inline Point3D &operator *= ( Point3D &V );
    // The division/assignment operator:  
    inline Point3D &operator /= ( Point3D &V );
    inline Point3D operator - ( double V );
    // The addition operator: 
    inline Point3D operator + ( double V );
    // The multiplication operator:  
    inline Point3D operator * ( double V );
    // The division operator:  
    inline Point3D operator / ( double V );
    // The subtraction/assignment operator:  
    inline Point3D &operator -= ( double V );
    // The addition/assignment operator:  
    inline Point3D &operator += ( double V );
    // The multiplication/assignment operator:  
    inline Point3D &operator *= ( double V );
    // The division/assignment operator:  
    inline Point3D &operator /= ( double V );
    inline float Mag(); // Returns the magnitute of the 3D point
    inline float DotUnit ( Point3D &V );
    inline float DotNonUnit ( Point3D &V );
    void Read( FILE *File );
    void Write( FILE *File );
	// Load the 3D point from a pointer
	unsigned char *LoadFromPointer( unsigned char *pData );
};

int UniqueVert( Point3D &V, Point3D *List, int Range );
int GetVertIndex( Point3D &V, Point3D *List, int Range );



/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           equality operator
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

inline int Point3D::operator == ( Point3D &V )
{
    int RValue = 0;

    if( V.Lx == Lx )
        if( V.Ly == Ly )
            if( V.Lz == Lz )
                RValue = 1;

    return RValue;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The inequality operator
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

inline int Point3D::operator != ( Point3D &V )
{
    int RValue = 0;

    if( ( V.Lx != Lx ) || ( V.Ly != Ly ) || ( V.Lz != Lz ) )
        RValue = 1;

    return RValue;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The subtraction operator
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

inline Point3D Point3D::operator - ( Point3D &V )
{
    Point3D Temp;
    Temp.Lx = Lx - V.Lx;
    Temp.Ly = Ly - V.Ly;
    Temp.Lz = Lz - V.Lz;

    return Temp;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The addition operator
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
  
inline Point3D Point3D::operator + ( Point3D &V )
{
    Point3D Temp;

    Temp.Lx = Lx + V.Lx;
    Temp.Ly = Ly + V.Ly;
    Temp.Lz = Lz + V.Lz;

    return Temp;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The multiplication operator
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
  
inline Point3D Point3D::operator * ( Point3D &V )
{
    Point3D Temp;

    Temp.Lx = Lx * V.Lx;
    Temp.Ly = Ly * V.Ly;
    Temp.Lz = Lz * V.Lz;

    return Temp;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The division operator
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
  
inline Point3D Point3D::operator / ( Point3D &V )
{
    Point3D Temp;

    Temp.Lx = Lx / V.Lx;
    Temp.Ly = Ly / V.Ly;
    Temp.Lz = Lz / V.Lz;

    return Temp;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The subtraction/assignment operator
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
  
inline Point3D &Point3D::operator -= ( Point3D &V )
{
    Lx -= V.Lx;
    Ly -= V.Ly;
    Lz -= V.Lz;

    return *this;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The addition/assignment operator
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
  
inline Point3D &Point3D::operator += ( Point3D &V )
{
    Lx += V.Lx;
    Ly += V.Ly;
    Lz += V.Lz;

    return *this;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The multiplication/assignment operator:
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
  
inline Point3D &Point3D::operator *= ( Point3D &V )
{
    Lx *= V.Lx;
    Ly *= V.Ly;
    Lz *= V.Lz;

    return *this;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The division/assignment operator
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
  
inline Point3D &Point3D::operator /= ( Point3D &V )
{
    Lx /= V.Lx;
    Ly /= V.Ly;
    Lz /= V.Lz;

    return *this;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           Subtraction operator
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

inline Point3D Point3D::operator - ( double V )
{
    Point3D Temp;

    Temp.Lx = Lx - V;
    Temp.Ly = Ly - V;
    Temp.Lz = Lz - V;

    return Temp;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The addition operator
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
  
inline Point3D Point3D::operator + ( double V )
{
    Point3D Temp;

    Temp.Lx = Lx + V;
    Temp.Ly = Ly + V;
    Temp.Lz = Lz + V;

    return Temp;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The multiplication operator
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
  
inline Point3D Point3D::operator * ( double V )
{
    Point3D Temp;

    Temp.Lx = Lx * V;
    Temp.Ly = Ly * V;
    Temp.Lz = Lz * V;

    return Temp;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The division operator
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
  
inline Point3D Point3D::operator / ( double V )
{
    Point3D Temp;

    Temp.Lx = Lx / V;
    Temp.Ly = Ly / V;
    Temp.Lz = Lz / V;

    return Temp;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The subtraction/assignment operator
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
  
inline Point3D &Point3D::operator -= ( double V )
{
    Lx -= V;
    Ly -= V;
    Lz -= V;

    return *this;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The addition/assignment operator
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
  
inline Point3D &Point3D::operator += ( double V )
{
    Lx += V;
    Ly += V;
    Lz += V;

    return *this;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The multiplication/assignment operator
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
  
inline Point3D &Point3D::operator *= ( double V )
{
    Lx *= V;
    Ly *= V;
    Lz *= V;

    return *this;
}


/************************************************************************
*    FUNCTION NAME:                                                             
*
*    DESCRIPTION:           The division/assignment operator
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
  
inline Point3D &Point3D::operator /= ( double V )
{
    Lx /= V;
    Ly /= V;
    Lz /= V;

    return *this;
}


/************************************************************************
*    FUNCTION NAME:         Mag                                                    
*
*    DESCRIPTION:           
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

float inline Point3D::Mag()
{
    return sqrt ( Lx * Lx + Ly * Ly + Lz * Lz );
}


/************************************************************************
*    FUNCTION NAME:         DotUnit                                                    
*
*    DESCRIPTION:           
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

inline float Point3D::DotUnit( Point3D &V )
{
    return ( Lx * V.Lx + Ly * V.Ly + Lz * V.Lz );
}


/************************************************************************
*    FUNCTION NAME:         DotNonUnit                                                    
*
*    DESCRIPTION:           
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

inline float Point3D::DotNonUnit( Point3D &V )
{
    float Dot = ( Lx * V.Lx +
                Ly * V.Ly +
                Lz * V.Lz ) / ( Mag () * V.Mag () );
    return Dot;
}


#endif
