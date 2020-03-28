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
*    FUNCTION NAME:              Constructer                                                             
*
*    -------------------------------------------------------------------
*    H Rosenorn    05/02/06      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CBaseCamera::CBaseCamera()
{
}


/************************************************************************
*    FUNCTION NAME:              Destructer                                                             
*
*    -------------------------------------------------------------------
*    H Rosenorn    05/02/06      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CBaseCamera::~CBaseCamera()
{
}


/************************************************************************                                                            
*    DESCRIPTION:    Do the translation
*
*    Input:    float x - X movement
*              float y - Y movement
*              float z - Z movement
*
*    -------------------------------------------------------------------
*    H Rosenorn    05/02/06      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
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
*
*    -------------------------------------------------------------------
*    H Rosenorn    05/02/06      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/  
void CBaseCamera::Rotate( float x, float y, float z )
{
	// Do the Y rotation. We are using the difference between the center and the new offsets
    transAndYRotOnly.Rotate( 0, y, z );

    // The X rotation has to be done seperately and added into final matrix later
    xRotationOnly.Rotate( x, 0, 0 );

}	// Rotate


/************************************************************************
*    DESCRIPTION:  Update the view matrix
*
*    -------------------------------------------------------------------
*    H Rosenorn    05/05/06      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/  
void CBaseCamera::UpdateView()
{
	// Update the view matrix
    viewMatrix.InitilizeMatrix();
    viewMatrix.MergeMatrix( transAndYRotOnly.matrix );
    viewMatrix.MergeMatrix( xRotationOnly.matrix );

}	// UpdateView 