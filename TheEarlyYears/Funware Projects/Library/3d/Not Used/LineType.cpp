//
// File name: LineType.cpp
//
// Description: A fixed-point implementation of a polygon edge 
//             algorithm
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size

#include "LineType.h"


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

CeilLine::CeilLine()
{
    X1 = X2 = Y1 = Y2 = 0;
    X = StepX = StepZ = Z = 0;
    U = V = StepU = StepV = I = StepI = 0;
    EdgeHeight = Y = 0;

}   // Constructer

/************************************************************************
*    FUNCTION NAME:         Step()                                                             
*
*    DESCRIPTION:           Step the edge
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

inline void CeilLine::Step()
{
    X += StepX;
    ++Y;
    Z += StepZ;
    U += StepU;
    V += StepV;
    I += StepI;
    --EdgeHeight;

}   // Step


/************************************************************************
*    FUNCTION NAME:         Step()                                                             
*
*    DESCRIPTION:           Step the edge by "Amount"
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

inline void CeilLine::Step( int Amount )
{
    X += ( StepX * Amount );
    Y += ( Amount );
    Z += ( StepZ * Amount );
    U += ( StepU * Amount );
    V += ( StepV * Amount );
    I += ( StepI * Amount );
    EdgeHeight -= ( Amount );

}   // Step