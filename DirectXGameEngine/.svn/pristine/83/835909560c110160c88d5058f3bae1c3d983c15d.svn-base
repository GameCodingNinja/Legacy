/************************************************************************
*    FILE NAME:       camera.cpp
*
*    DESCRIPTION:     3D camera class
************************************************************************/

// Physical component dependency
#include <common/camera.h>

/************************************************************************
*    desc:  Constructor                                                             
************************************************************************/
CCamera::CCamera()
{
}   // Constructor


/************************************************************************
*    desc:  Destructor                                                             
************************************************************************/
CCamera::~CCamera()
{

}   // Destructor


/************************************************************************
*    desc:  Reset the camera
************************************************************************/  
void CCamera::Reset()
{
    moveMatrix.InitilizeMatrix();
    finalMatrix.InitilizeMatrix();
    rotation.Clear();
    position.Clear();

}	// Reset


/************************************************************************
*    desc:  Reset the 2D camera
************************************************************************/  
void CCamera::Reset2D()
{
    moveMatrix2D.InitilizeMatrix();
    finalMatrix2D.InitilizeMatrix();
    rotation2D.Clear();

}	// Reset2D


/************************************************************************
*    desc:  Rotate the camera
*
*    param: CPoint & point - rotation point
************************************************************************/  
void CCamera::Rotate( const CPoint & point )
{
    // Inc the rotation point but save it for later
    rotation += point;

    rotation.Cap(360);

    // Create a temporary point and clear out the X and Z rotations
    CPoint tmpPoint = point;

    tmpPoint.ClearX();
    tmpPoint.ClearZ();
    
    moveMatrix.Rotate( tmpPoint );

}   // Rotate


/************************************************************************
*    desc:  Rotate the 2D camera
*
*    param: CPoint & point - rotation point
************************************************************************/  
void CCamera::Rotate2D( const CPoint & point )
{
    // Inc the rotation point but save it for later
    rotation2D += point;

    rotation2D.Cap(360);

    // Create a temporary point and clear out the X and Z rotations
    CPoint tmpPoint = point;

    tmpPoint.ClearX();
    tmpPoint.ClearY();
    
    moveMatrix2D.Rotate( tmpPoint );

}   // Rotate2D


/************************************************************************
*    desc:  Translate the camera
*
*    param: CPoint & point - translation point
************************************************************************/
void CCamera::Translate( const CPoint & point )
{
    // Flip the signs to make outside values simulate a camera moving instead
    // of the world moving. Makes things easier to conceptualize.
    moveMatrix.Translate( point * -1 );

    CMatrix tmpMatrix;
    tmpMatrix.Rotate( CPoint(0, -rotation.y, 0) );

    CPoint tmpPint = point * tmpMatrix;
    //point *= -1;

    position += point;

}   // Translate


/************************************************************************
*    desc:  Translate the 2D camera
*
*    param: CPoint & point - translation point
************************************************************************/
void CCamera::Translate2D( const CPoint & point )
{
    // Flip the signs to make outside values simulate a camera moving instead
    // of the world moving. Makes things easier to conceptualize.
    moveMatrix2D.Translate( point * -1 );

}   // Translate2D


/************************************************************************
*    desc:  Finalize the translations and rotations into one matrix
************************************************************************/  
void CCamera::Finalize()
{
    // Reset the final matrix
    finalMatrix.InitilizeMatrix();

    CMatrix tmpMatrix;
    tmpMatrix.Translate( position * -1 );

    tmpMatrix.Rotate( CPoint(0, rotation.y, 0) );
    

    // Merge in the translation and y rotation
    //finalMatrix.MergeMatrix( moveMatrix );

    // Create a temporary point and clear out the Y rotation
    CPoint tmpPoint = rotation;

    tmpPoint.ClearY();

    tmpMatrix.Rotate( tmpPoint );
    // Add in the remaining rotations
    //finalMatrix.Rotate( tmpPoint );
    finalMatrix = tmpMatrix;

}	// Finalize


/************************************************************************
*    desc:  Finalize the 2D translations and rotations into one matrix
************************************************************************/  
void CCamera::Finalize2D()
{
    // Reset the final matrix
    finalMatrix2D.InitilizeMatrix();

    // Merge in the translation and y rotation
    finalMatrix2D *= moveMatrix2D;

    // Create a temporary point and clear out the Y rotation
    CPoint tmpPoint = rotation2D;

    tmpPoint.ClearY();

    // Add in the remaining rotations
    finalMatrix2D.Rotate( tmpPoint );

}	// Finalize2D


/************************************************************************
*    desc:  Finalize the translations and rotations into one matrix
*
*    param: CPoint & point - translation point 
************************************************************************/  
void CCamera::Finalize(const CPoint & trans)
{
    // Finalize the matrix
    Finalize();

    // Add in any final transformations
    finalMatrix.Translate( trans );

}	// Finalize


/************************************************************************
*    desc:  Finalize the 2D translations and rotations into one matrix
*
*    param: CPoint & point - translation point 
************************************************************************/  
void CCamera::Finalize2D(const CPoint & trans)
{
    // Finalize the matrix
    Finalize2D();

    // Add in any final transformations
    finalMatrix2D.Translate( trans );

}	// Finalize2D


/************************************************************************
*    desc:  Get the final matrix
*
*	 ret: CMatrix & - Final Matrix
************************************************************************/  
CMatrix & CCamera::GetFinalMatrix()
{
    return finalMatrix;

}	// GetFinalMatrix

/************************************************************************
*    desc:  Get the 2D final matrix
*
*	 ret: CMatrix & - 2D final matrix
************************************************************************/  
CMatrix & CCamera::GetFinalMatrix2D()
{
    return finalMatrix2D;

}	// GetFinalMatrix2D


/************************************************************************
*    desc:  Get the move matrix. This matrix is used for movement around 
*           the map. Does not include any X or Z rotations. Perfect for
*           using on collision meshes.
*
*	 ret: CMatrix & - move matrix
************************************************************************/  
CMatrix & CCamera::GetMoveMatrix()
{
    return moveMatrix;

}	// GetMoveMatrix


/************************************************************************
*    desc:  Get the 2D move matrix. This matrix is used for movement around 
*           the map. Does not include any X or Z rotations. Perfect for
*           using on collision meshes.
*
*	 ret: CMatrix & - 2D move matrix
************************************************************************/  
CMatrix & CCamera::GetMoveMatrix2D()
{
    return moveMatrix2D;

}	// GetMoveMatrix2D


/************************************************************************
*    desc:  Get the position
*
*	 ret: CPoint & - position
************************************************************************/  
const CPoint & CCamera::GetPos() const
{
    return position;

}	// GetPos


/************************************************************************
*    desc:  Get the rotation
*
*	 ret: CPoint & - rotation
************************************************************************/  
const CPoint & CCamera::GetRotation() const
{
    return rotation;

}	// GetRotation


/************************************************************************
*    desc:  Get the 2D rotation
*
*	 ret: CPoint & - 2D rotation
************************************************************************/  
const CPoint & CCamera::GetRotation2D() const
{
    return rotation2D;

}	// GetRotation2D
