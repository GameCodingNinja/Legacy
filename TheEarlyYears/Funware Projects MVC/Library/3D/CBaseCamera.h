/************************************************************************
*
*    FILE NAME:       CBaseCamera.h
*
*    DESCRIPTION:     Base Camera class
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef _BASE_CAMERA_H_
#define _BASE_CAMERA_H_

#include "CMatrix.h"

/************************************************************************
*
*    Class for doing camera work
*
************************************************************************/

class CBaseCamera
{
protected:

	// Matrix with transformations and y rotations only
    CMatrix transAndYRotOnly;
	// Matrix with X rotations only. No translations
    CMatrix xzRotationOnly;
    // Final merged matrix of the above two 
    CMatrix viewMatrix;

public:

	CBaseCamera();
    virtual ~CBaseCamera();

	// Do the translation
	void Translate( float x, float y, float z );
	// Do the rotation
	void Rotate( float x, float y, float z );
	// Update the view matrix
	void UpdateView();

	// Get the trans and y matrix only
	CMatrix & GetWorldSpaceMatrix(){ return transAndYRotOnly; };
	// Get the view matrix
	CMatrix & GetViewSpaceMatrix(){ return viewMatrix; };
};


#endif  // _BASE_CAMERA_H_