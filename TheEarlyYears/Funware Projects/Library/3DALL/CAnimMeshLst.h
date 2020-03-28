
/************************************************************************
*
*    PRODUCT:         Animated mesh list
*
*    FILE NAME:       CAnimMeshLst.h
*
*    DESCRIPTION:     Handle mesh frames 
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

#ifndef _CANIM_MESH_LST_H_
#define _CANIM_MESH_LST_H_

#include "CMeshList.h"       // Mesh list class
#include "CAnimMesh.h"       // Animation specific mesh class
#include "CMapMgr.h"

enum EAPM_ANIM_PLAY_MODE
{
    EAPM_FORWARD_STOP=0,
	EAPM_BACKWARDS_STOP,
    EAPM_LOOP,
};


/************************************************************************
*
*    Class for handling lists of meshes
*
************************************************************************/

template <class mesh_type>
class CAnimMeshLst : public CMeshList<mesh_type>
{
protected:

    // Pointer to animation table info
    CAnimTable *pAnimTable;
    // Mesh array for interpolation
    mesh_type *pInterpMesh;
    // Total number of animations
    int animCount;
    // Number of frames in animation
    int maxFrames;
    // frame counter
    int frameCounter;
    // Current frame index
    int frameIndex;   
    // Offset in to frame list
    int frameOffset;
    // value to inc the frame from the offset
    float tweenVector;
    // Animation play mode
    EAPM_ANIM_PLAY_MODE animPlayMode;
    // Active animation index
    int curAnimIndex;
    // tween counter
    int tweenCounter;
    // max tweens
    int maxTweens;
    // matrix position of this animated mesh
    CMatrix matrix, rotMatrix, matrixMrg;

public:

    CAnimMeshLst();
    virtual ~CAnimMeshLst();

    // Free the allocations
    void Free();
    // Load mesh and normal data from file
    virtual bool LoadFromFile( char *filePath );
    virtual bool LoadFromFile( FILE *hFile );
    // Load mesh from file
    virtual bool LoadMeshFromFile( char *filePath );
    virtual bool LoadMeshFromFile( FILE *hFile );
    // // Save mesh and normal data to file
    virtual bool SaveToFile( char *filePath );
    virtual bool SaveToFile( FILE *hFile );
    // Set the animation to play
    void SetAnimation( char *animStr, int twCount = 0, EAPM_ANIM_PLAY_MODE playMode = EAPM_LOOP );
    // Inc the animation frame
    void IncFrame();
    // Render the animation frame
    virtual int Render( CMatrix &m );
    // Setup the class for tweening
    void SetUpTweens();
	// get the matrix for this animated mesh
	CMatrix *GetMatrix(){ return &matrix; };
	// Do the world transformation
	void DoWorldTransformation();
	// Do the view transformation
	void DoViewTransformation( CMatrix &m );	
};


/////////////////////////////////////////
//  Must include this here or you will
//  get a bunch of "Unresolved External"
//  errors. Templates are a pain to code
/////////////////////////////////////////
#include "CAnimMeshLst.cpp"

#endif  // _CANIM_MESH_LST_H_