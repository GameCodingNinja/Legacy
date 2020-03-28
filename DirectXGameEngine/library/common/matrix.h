/************************************************************************
*    FILE NAME:       matrix.h
*
*    DESCRIPTION:     4x4 Matrix math class
************************************************************************/

#ifndef __matrix_h__
#define __matrix_h__

// Game lib dependencies
#include <common/normal.h>
#include <common/quaternion.h>
#include <common/rect.h>
#include <common/quad.h>
#include <common/defs.h>

enum
{
    m00, m01, m02, m03,
    m10, m11, m12, m13,
    m20, m21, m22, m23,
    m30, m31, m32, m33,
    mMax
};

// The 3D view matrix class:
class CMatrix
{
public:
    // Constructor
    CMatrix();

    // Copy contructor
    CMatrix( const CMatrix & obj );
    CMatrix::CMatrix( float mat[mMax] );

    // Reset the matrix to the identity matrix
    void InitilizeMatrix();

    // Clear translation data from the matrix
    void ClearTranlate();

    // Generate 3D rotation matrix
    void Rotate( const CPoint & point );
    void Rotate( const CRadian & point );

    // Get the Z rotation of the matrix
    float GetZRot( bool inDegrees = true ) const;

    // Create 3D translation matrix
    void Translate( const CPoint & point );

    // Function designed to merge scaling matrix with master matrix
    void Scale( const CPoint & point );
    void Scale( float scale );

    // Functions designed to transform using the master matrix
    void Transform( CPoint & dest, const CPoint & source ) const;
    void Transform( CPoint * pDest, const CPoint * pSource ) const;
    void Transform( CNormal & dest, const CNormal & source ) const;
    void Transform( CRect<float> & dest, const CRect<float> & source ) const;
    void Transform( CQuad & dest, const CQuad & source ) const;
    void Transform3x3( CPoint & dest, const CPoint & source ) const;

    // Get the transpose of a matrix
    CMatrix GetTransposeMatrix() const;

    // Get the matrix point in space
    CPoint GetMatrixPoint();

    // Get matrix rotation
    CRadian GetMatrixRotation();

    // Inverses this matrix.
    void InverseZ();
    bool Inverse();

    // Set quaternion
    void Set( const CQuaternion & quat );

    // Create the matrix based on where the camera is looking
    void LookAt( const CPoint & pos, const CPoint & target, const CPoint & cameraUp );

    // Multiply the matrices only using the rotation/scale portion
    void Multiply3x3( const CMatrix & obj );

    // Overloaded operators
    CMatrix operator * ( const CMatrix & obj ) const;
    CMatrix operator *= ( const CMatrix & obj );
    CMatrix operator = ( const float mat[4][4] );
    const float operator [] ( uint index ) const;
    float & operator [] ( uint index );
    const float * operator () () const;

private:

    // Initializes a specific matrix to the identity matrix
    void InitIdentityMatrix( float mat[mMax] );

    // Merge source matrix into destination matrix
    void MergeMatrices( float dest[mMax], const float source[mMax] );

    // Merge matrix into master matrix
    void MergeMatrix( const float newMatrix[mMax] );
    void ReverseMergeMatrix( const float newMatrix[mMax] );

    // Separate axis rotation functions
    void RotateZRad( float dest[mMax], float value, int rotFlags );
    //void RotateZDeg( float dest[mMax], float value, int rotFlags );
    void RotateYRad( float dest[mMax], float value, int rotFlags );
    //void RotateYDeg( float dest[mMax], float value, int rotFlags );
    void RotateXRad( float dest[mMax], float value, int rotFlags );
    //void RotateXDeg( float dest[mMax], float value, int rotFlags );

private:

    // Master matrix
    // a single dimension array (array[]) is much faster
    // then a multi-dimension array (array[][])
    float matrix[mMax];

};


#endif  // __matrix_h__