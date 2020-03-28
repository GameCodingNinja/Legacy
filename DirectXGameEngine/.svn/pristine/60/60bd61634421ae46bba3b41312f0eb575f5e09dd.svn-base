/************************************************************************
*    FILE NAME:       camera.h
*
*    DESCRIPTION:     Camera class
************************************************************************/

#ifndef __camera_h__
#define __camera_h__

// Game lib dependencies
#include <common/matrix.h>
#include <common/point.h>
#include <common/worldpoint.h>

class CCamera
{
public:

    // Get the instance of the singleton class
    static CCamera & Instance()
    {
        static CCamera camera;
        return camera;
    }

    // Rotate the camera
    void Rotate( const CPoint & point );
    void Rotate2D( const CPoint & point );

    // Translate the camera
    void Translate( const CPoint & point );
    void Translate2D( const CPoint & point );

    // Reset the camera
    void Reset();
    void Reset2D();

    // Finalize the translates and rotations
    void Finalize();
    void Finalize2D();
    void Finalize(const CPoint & trans);
    void Finalize2D(const CPoint & trans);

    // Get the matrix
    CMatrix & GetFinalMatrix();
    CMatrix & GetFinalMatrix2D();

    // Get the move matrix.
    CMatrix & GetMoveMatrix();
    CMatrix & GetMoveMatrix2D();

    // Get the camera's position
    const CPoint & GetPos() const;

    // Get the rotation
    const CPoint & GetRotation() const;
    const CPoint & GetRotation2D() const;

protected:

    // Constructor
    CCamera();

    // Destructor
    ~CCamera();

    // Holds translation and y rotation
    CMatrix moveMatrix;
    CMatrix moveMatrix2D;

    // Final 3D matrix 
    CMatrix finalMatrix;
    CMatrix finalMatrix2D;

    // Holds the accumulated rotations
    CPoint rotation;
    CPoint rotation2D;

    CPoint position;
    //CWorldPoint worldPosition;

};

#endif  // __camera_h__