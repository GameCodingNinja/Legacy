/************************************************************************
*
*    FILE NAME:       CBaseCamera.cpp
*
*    DESCRIPTION:     Base Camera class
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#include "CBaseCamera.h"


/************************************************************************
*    FUNCTION NAME:  Constructer                                                             
************************************************************************/
CBaseCamera::CBaseCamera()
{
}


/************************************************************************
*    FUNCTION NAME:  Destructer                                                             
************************************************************************/
CBaseCamera::~CBaseCamera()
{
}


/************************************************************************                                                            
*    DESCRIPTION:  Do the translation
*
*    Input:    float x - X movement
*              float y - Y movement
*              float z - Z movement
************************************************************************/
void CBaseCamera::Translate( float x, float y, float z )
{
	// Add in the translation
    transAndYRotOnly.Translate( x, y, z );

}	// Translate


/************************************************************************
*    DESCRIPTION:  Do the rotation
*
*    Input:    float x - X rotation
*              float y - Y rotation
*              float z - Z rotation
************************************************************************/  
void CBaseCamera::Rotate( float x, float y, float z )
{
	// Do the Y rotation. We are using the difference between the center and the new offsets
    transAndYRotOnly.Rotate( 0, y, 0 );

    // The X & Z rotation has to be done seperately and added into final matrix later
    xzRotationOnly.Rotate( x, 0, z );

}	// Rotate


/************************************************************************
*    DESCRIPTION:  Update the view matrix
************************************************************************/  
void CBaseCamera::UpdateView()
{
	// Update the view matrix
    viewMatrix.InitilizeMatrix();
    viewMatrix.MergeMatrix( transAndYRotOnly.matrix );
    viewMatrix.MergeMatrix( xzRotationOnly.matrix );

}	// UpdateView 