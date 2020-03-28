
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
#include "CMatrix.h"         // Matrix class
#include "CBaseCamera.h"

#include "tagfileparser/datadefnode.h"
#include "tagfileparser/tokenizer.h"
#include "tagfileparser/tagfileparser.h"

extern int giRenderDevice;

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
	// Mesh array for shadows
    mesh_type *pShadowMesh;
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
    CMatrix transMatrix, rotMatrix, matrixMrg;
	// The current animation
	char curAnimNameStr[50];
	// the average scale
	float scale;
	// Shadow flag
	bool shadow;
	// Shadow intensity
	unsigned int shadowInten;
	// Shadow projection direction
	CPoint shadowDirPt;
	// Light offset
	CPoint lightOffsetPt;

public:

    CAnimMeshLst();
	CAnimMeshLst( CAnimMeshLst &animMsh );
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
    int Render();
	virtual int Render( CLightList *pLightList,
						CMatrix &m,
						CPoint &minPoint,
						CPoint &maxPoint );
	int Render( CLightList *pLightList,
				CMatrix &m,
				float portRadius,
				CPoint *pPortalCenter,
				CPoint &minPoint,
				CPoint &maxPoint );
	int Render( float portRadius,
				CPoint *pPortalCenter );
    // Setup the class for tweening
    bool SetUpTweens();
	// get the matrix for this animated mesh
	CMatrix &GetWorldSpaceMatrix(){ return transMatrix; };
	// Do the world transformation
	void DoWorldTransformation();
	// Do the view transformation
	void DoViewTransformation( CMatrix &m );	
	// Is the animation cycle completed?
	bool IsAnimFinish(){ return (frameCounter == (maxFrames-2)) && (tweenCounter == (maxTweens-1)); };
	// Bind a given animation texture into memory
	void BindAnimationText( char *animStr, unsigned int polyIndex = 0 );
	// Find the texture index of the given poly index
	int GetTextIndex( char *animStr, unsigned int polyIndex );
	// Make the class ready for shadows
	bool SetUpShadows();

	/////////////////
	// Set functions
	/////////////////

	// Set the position of the object
	void SetPos( float x, float y, float z );
	void SetPos( CPoint &point, TIntVector &intVector );
	// Set the rotation of the object
	void SetRot( float x, float y, float z );
	// Set the size of the object
	void SetScale( float x, float y, float z );
	// Set the shadows
	void SetShadow( bool value ){ shadow = value; };
	// Set the shadow project
	void SetShadowProject( CPoint & value ){ shadowDirPt.Init( value ); };
	// Set the light Offset
	void SetLightOffset( CPoint & value ){ lightOffsetPt.Init( value ); };
	// Set the shadows intensity
	void SetShadowInten( unsigned int value ){ shadowInten = value; };
	
	/////////////////
	// Get functions
	/////////////////

    // Get the animation table
	CAnimTable * GetAnimTable(){ return pAnimTable; };
	// get the interpulated mesh
	mesh_type * GetInterpMesh(){ return pInterpMesh; };
	// get the shadow mesh
	mesh_type * GetShadowMesh(){ return pShadowMesh; };
	// Get the animation count
	int GetAnimCount(){ return animCount; };		
};


/************************************************************************
*    FUNCTION NAME:  Constructer                                                             
************************************************************************/
template <class mesh_type>
CAnimMeshLst<mesh_type>::CAnimMeshLst()
					   : pAnimTable(NULL),
					     pInterpMesh(NULL),
						 pShadowMesh(NULL),
						 animCount(0),
						 maxFrames(0),
						 frameCounter(0),
						 frameIndex(0),
						 frameOffset(0),
						 tweenVector(0.0),
						 animPlayMode(EAPM_LOOP),
						 curAnimIndex(0),
						 tweenCounter(0),
						 maxTweens(0),
						 scale(1.0),
						 shadow(false),
						 shadowInten(0)
{
	curAnimNameStr[0] = 0;
}


/************************************************************************
*    FUNCTION NAME:  Copy Constructer                                                             
************************************************************************/
template <class mesh_type>
CAnimMeshLst<mesh_type>::CAnimMeshLst( CAnimMeshLst &animMsh )
					   : CMeshList( animMsh ),
					     pAnimTable(animMsh.GetAnimTable()),
					     pInterpMesh(animMsh.GetInterpMesh()),
						 pShadowMesh(animMsh.GetShadowMesh()),
						 animCount(animMsh.GetAnimCount()),
						 maxFrames(0),
						 frameCounter(0),
						 frameIndex(0),
						 frameOffset(0),
						 tweenVector(0.0),
						 animPlayMode(EAPM_LOOP),
						 curAnimIndex(0),
						 tweenCounter(0),
						 maxTweens(0),
						 scale(1.0),
						 shadow(false),
						 shadowInten(0)
{
	curAnimNameStr[0] = 0;
}


/************************************************************************
*    FUNCTION NAME:  Destructer                                                             
************************************************************************/
template <class mesh_type>
CAnimMeshLst<mesh_type>::~CAnimMeshLst()
{
    Free();
}


/************************************************************************
*    DESCRIPTION:  Free the allocations
************************************************************************/
template <class mesh_type>
void CAnimMeshLst<mesh_type>::Free()
{
    if( pAnimTable != NULL )
    {
        if( meshAllocated )
        	delete [] pAnimTable;

        pAnimTable = NULL;
        animCount = 0;
    }

    if( pInterpMesh != NULL )
    {
        if( meshAllocated )
        	delete [] pInterpMesh;

        pInterpMesh = NULL;
    }

	if( pShadowMesh != NULL )
    {
        if( meshAllocated )
        	delete [] pShadowMesh;

        pShadowMesh = NULL;
    }

}   // Free


/************************************************************************                                                             
*    DESCRIPTION:  Load an object from a PLY binary file. This is
*                  our own simple file format for quickly loading
*                  vertices, face indexes, uv and texture indexes
*                  into the texture library.
*
*    Input:    char *filePath - Path to file to load
************************************************************************/
template <class mesh_type>
bool CAnimMeshLst<mesh_type>::LoadFromFile( char *filePath )
{
    return CMeshList<mesh_type>::LoadFromFile( filePath );

}   // LoadFromFile


/************************************************************************                                                             
*
*    DESCRIPTION:  Load an object from a PLY binary file. This is
*                  our own simple file for quickly loading
*                  vertices, face indexes, uv and texture indexes
*                  into the texture library.
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
************************************************************************/
template <class mesh_type>
bool CAnimMeshLst<mesh_type>::LoadFromFile( FILE *hFile )
{
	char formatType[FORMAT_SPEC_STR_COUNT];
    bool result = false;

    Free();

    // Load in the header so that we can check it
    if( fread( formatType, sizeof( formatType ), 1, hFile ) != 1 )
        return result;

    // Check the header to make sure this is not random data
    if( strcmp( formatType, MESH_ANIM_TBL_FORMAT_STR_ID ) != 0 )
        return result;

    // Read in the animation count
    if( fread( &animCount, sizeof( animCount ), 1, hFile ) != 1 )
        return result;

    // Allocate the animation table
    pAnimTable = new CAnimTable[ animCount ];
    if( pAnimTable == NULL )
        return result;

    // Read in the animation table
    if( fread( pAnimTable, sizeof( CAnimTable ), animCount, hFile ) != animCount )
        return result;
    
    result = CMeshList<mesh_type>::LoadFromFile( hFile );

    return result;

}   // LoadFromFile


/************************************************************************
*    DESCRIPTION:  Calling this function means the file only
*                  contains mesh data and we need to use this
*                  to calculate all our normals before we save
*                  the final file which is a combination of
*                  mesh and normal data.
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
************************************************************************/
template <class mesh_type>
bool CAnimMeshLst<mesh_type>::LoadMeshFromFile( FILE *hFile )
{
    char formatType[FORMAT_SPEC_STR_COUNT];
    bool result = false;

    Free();

    // Load in the header so that we can check it
    if( fread( formatType, sizeof( formatType ), 1, hFile ) != 1 )
        return result;

    // Check the header to make sure this is not random data
    if( strcmp( formatType, MESH_ANIM_TBL_FORMAT_STR_ID ) != 0 )
        return result;

    // Read in the animation count
    if( fread( &animCount, sizeof( animCount ), 1, hFile ) != 1 )
        return result;

    // Allocate the mesh list
    pAnimTable = new CAnimTable[ animCount ];
    if( pAnimTable == NULL )
        return result;

    // Read in the animation count
    if( fread( pAnimTable, sizeof( CAnimTable ), animCount, hFile ) != animCount )
        return result;
    
    result = CMeshList<mesh_type>::LoadMeshFromFile( hFile );

    return result;

}   // LoadMeshFromFile


/************************************************************************                                                             
*    DESCRIPTION:  Calling this function means the file only
*                  contains mesh data and we need to use this
*                  to calculate all our normals before we save
*                  the final file which is a combination of
*                  mesh and normal data.
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
************************************************************************/
template <class mesh_type>
bool CAnimMeshLst<mesh_type>::LoadMeshFromFile( char *filePath )
{    
    return CMeshList<mesh_type>::LoadMeshFromFile( filePath );

}   // LoadMeshFromFile


/************************************************************************
*    DESCRIPTION:  Save mesh and normal data to file. This is
*                  our own simple file for quickly loading
*                  vertices, face indexes, uv and texture indexes
*                  into the texture library.
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
************************************************************************/
template <class mesh_type>
bool CAnimMeshLst<mesh_type>::SaveToFile( FILE *hFile )
{
    char formatType[FORMAT_SPEC_STR_COUNT];
    bool result = false;

    memset( formatType, 0, sizeof(formatType) );
    strcpy( formatType, MESH_ANIM_TBL_FORMAT_STR_ID );

    // Write out the format ID
    if( fwrite( formatType, sizeof( formatType ), 1, hFile ) != 1 )
        return result;

    // Write out the vertex count
    if( fwrite( &animCount, sizeof( animCount ), 1, hFile ) != 1 )
        return result;

    // Write out the animation table
    if( fwrite( (CAnimTable *)pAnimTable, sizeof( CAnimTable ), animCount, hFile ) != animCount )
        return result;

    // Write out the rest of the file
    result = CMeshList<mesh_type>::SaveToFile( hFile );

    return result;

}   // SaveToFile


/************************************************************************
*    DESCRIPTION:  Calling this function means the file only
*                  contains mesh data and we need to use this
*                  to calculate all our normals before we save
*                  the final file which is a combination of
*                  mesh and normal data.
*
*    Input:    FILE *hFile - Handle to file
*
*    Output:   bool - true on success, false on fail
************************************************************************/
template <class mesh_type>
bool CAnimMeshLst<mesh_type>::SaveToFile( char *filePath )
{    
    return CMeshList<mesh_type>::SaveToFile( filePath );

}   // SaveToFile


/************************************************************************
*    DESCRIPTION:  Set the animation that is to run
*
*    Input:    char *animStr - Animation name
*              int twCount - number of tweens between key frames
*              EAPM_ANIM_PLAY_MODE playMode - playback mode
************************************************************************/
template <class mesh_type>
void CAnimMeshLst<mesh_type>::SetAnimation( char *animStr, int twCount, EAPM_ANIM_PLAY_MODE playMode )
{
    // Check that we are not trying to reset the same animation over
    if( (twCount != maxTweens - 1) || (animPlayMode != playMode) || (strcmp( curAnimNameStr, animStr ) != 0) )
	{
	    for( int i = 0; i < animCount; ++i )
	    {
	        // Go through the animation table and search for the animation
	        if( strcmp( pAnimTable[i].animNameStr, animStr ) == 0 )
	        {
	            // copy the name of the current animatin
	            strcpy( curAnimNameStr, animStr );

	            // check to be safe that we are not over running our animation frames
	            if( pAnimTable[i].frameStart + pAnimTable[i].frameCount <= meshCount )
	            {
	                frameOffset = pAnimTable[i].frameStart;
	                maxFrames = pAnimTable[i].frameCount;
	                
	                animPlayMode = playMode;
	                maxTweens = twCount + 1;
	                curAnimIndex = i;
	                tweenVector = 1.0f / (float)maxTweens;

	                if( playMode == EAPM_BACKWARDS_STOP )
	                {
	                    frameCounter = maxFrames-1;
	                    tweenCounter = twCount;
	                }
	                else
	                {
	                    frameCounter = 0;
	                    tweenCounter = 0;
	                }
	            }

	            break;
	        }   
	    }

	    // Set the frame index
	    frameIndex = frameOffset + frameCounter;
	}

}   // SetAnimation


/************************************************************************
*    DESCRIPTION:  Bind a given animation texture into memory
*
*    Input:    char *animStr - Animation name
************************************************************************/
template <class mesh_type>
void CAnimMeshLst<mesh_type>::BindAnimationText( char *animStr, unsigned int polyIndex )
{
	for( int i = 0; i < animCount; ++i )
	{
	    // Go through the animation table and search for the animation
	    if( strcmp( pAnimTable[i].animNameStr, animStr ) == 0 )
	    {
	        // check to be safe that we are not over running our animation frames
	        if( pAnimTable[i].frameStart + pAnimTable[i].frameCount <= meshCount )
	        {
	            pMesh[pAnimTable[i].frameStart].BindTexture( polyIndex );	            
	        }

	        break;
	    }   
	}

}   // BindAnimationText


/************************************************************************
*    DESCRIPTION:  Find the texture index of the given poly index
*
*    Input:    char *animStr - Animation name
************************************************************************/
template <class mesh_type>
int CAnimMeshLst<mesh_type>::GetTextIndex( char *animStr, unsigned int polyIndex )
{
	int textIndex = 0;

	for( int i = 0; i < animCount; ++i )
	{
	    // Go through the animation table and search for the animation
	    if( strcmp( pAnimTable[i].animNameStr, animStr ) == 0 )
	    {
	        // check to be safe that we are not over running our animation frames
	        if( ((pAnimTable[i].frameStart + pAnimTable[i].frameCount) <= meshCount) &&
	            polyIndex < (unsigned int)pMesh[pAnimTable[i].frameStart].GetPolyCount() )
	        {
	            textIndex = pMesh[pAnimTable[i].frameStart].GetPolyList()[ polyIndex ].GetTextIndex();	            
	        }

	        break;
	    }   
	}

	return textIndex;

}   // GetTextIndex


/************************************************************************
*    DESCRIPTION:  Inc the animation frame
************************************************************************/
template <class mesh_type>
void CAnimMeshLst<mesh_type>::IncFrame()
{
    if( animPlayMode == EAPM_FORWARD_STOP )
    {
        if( frameCounter < (maxFrames-1) )
        {
            tweenCounter = (tweenCounter + 1) % maxTweens;

            if( tweenCounter == 0 )
            {
                frameCounter = (frameCounter + 1) % maxFrames;
            }
        }
    }
    else if( animPlayMode == EAPM_BACKWARDS_STOP )
    {
        if( frameCounter > 0 )
        {
            --tweenCounter;

            if( tweenCounter == 0 )
            {
                --frameCounter;
            }
        }
    }
    else if( animPlayMode == EAPM_LOOP )
    {
        tweenCounter = (tweenCounter + 1) % maxTweens;

        if( tweenCounter == 0 )
        {
            frameCounter = (frameCounter + 1) % (maxFrames-1);
        }
    }

    // Set the frame index
    frameIndex = frameOffset + frameCounter;

}   // IncFrame


/************************************************************************                                                            
*    DESCRIPTION:  Do the world transformation
************************************************************************/
template <class mesh_type>
void CAnimMeshLst<mesh_type>::DoWorldTransformation()
{
	matrixMrg.InitilizeMatrix();
	matrixMrg.MergeMatrix( rotMatrix.matrix );
    matrixMrg.MergeMatrix( transMatrix.matrix );	

}	// DoLocalTransformation


/************************************************************************                                                            
*    DESCRIPTION:  Do the view transformation
************************************************************************/
template <class mesh_type>
void CAnimMeshLst<mesh_type>::DoViewTransformation( CMatrix &m )
{
	// Do the final transformation into view space    
    matrixMrg.MergeMatrix( m.matrix );

    // Must transform the center point first before we do anything
    // because many functions rely on the translated center point.
    pMesh[frameIndex].TransformCenterPt( matrixMrg );

}	// DoViewTransformation


/************************************************************************                                                            
*    DESCRIPTION:  Render the animation frame
*
*    Output:   int - polygon count of the mesh rendered
************************************************************************/
template <class mesh_type>
int CAnimMeshLst<mesh_type>::Render( CLightList *pLightList,
									 CMatrix &m,
									 CPoint &minPoint,
									 CPoint &maxPoint )
{    
    int result = 0;

    // Do the bounding sphere check and see if we should allow it to render
    if( !pMesh[frameIndex].CullMesh_BoundSphere( pMesh[frameIndex].GetRadius() * scale ) )
    {        
        mesh_type *pLastMesh;

        if( tweenCounter == 0 )
        {                        
            pMesh[frameIndex].SetLightList( pLightList );
            result = pMesh[frameIndex].Render( matrixMrg );
			pLastMesh = &pMesh[frameIndex];
        }
        else
        {
            pInterpMesh[curAnimIndex].InterpulateMesh( pMesh[frameIndex], 
                                                       pMesh[frameIndex + 1],
                                                       tweenCounter * tweenVector );
			pInterpMesh[curAnimIndex].SetLightList( pLightList );
            result = pInterpMesh[curAnimIndex].Render( matrixMrg );
			pLastMesh = &pInterpMesh[curAnimIndex];
        }

		// Are shadows enabled?
		if( shadow && pShadowMesh != NULL )
		{
			// Retransfor the mesh in to the world view
			DoWorldTransformation();
			pLastMesh->Transform( matrixMrg );

			CLight *light = pLastMesh->FindClosestLight( pLightList );

			if( light != NULL )
			{
				// Set the alpha channel shadow	image
				BindAnimationText( "shadowBox", shadowInten );

				// Transform the light position
				CPoint lightPt( light->GetPos() + lightOffsetPt );
				
				// Use the current mesh to build the shadow mesh
				pShadowMesh[curAnimIndex].ConvertToShadow( *pLastMesh,
														   matrixMrg,
														   lightPt,
														   shadowDirPt,
														   minPoint,
														   maxPoint );

				// Build a vector to offset each polygon to make use of the z buffer
				// otherwise the shadow will blend
				CPoint tmp(0.001f,0.001f,0.001f);
				CPoint offset = tmp * shadowDirPt;

				// Transform this vector so that it properly offsets each 
				// polygon based on the view rotation
				m.TransformVector(offset);
				offset.MakePermanent();
				result = pShadowMesh[curAnimIndex].RenderShadow( m, offset );
			}
		}
    }

    return result;

}   // Render


/************************************************************************                                                            
*    DESCRIPTION:  Render the animation frame
*
*    Output:   int - polygon count of the mesh rendered
************************************************************************/
template <class mesh_type>
int CAnimMeshLst<mesh_type>::Render( CLightList *pLightList,
                                     CMatrix &m,
									 float portRadius,
									 CPoint *pPortalCenter,
									 CPoint &minPoint,
									 CPoint &maxPoint )
{    
    int result = 0;

    // Do the bounding sphere check and see if we should allow it to render.
	// This object could have been scaled so we need to scale the radius
    if( !pMesh[frameIndex].CullMesh_BoundSphere( pMesh[frameIndex].GetRadius() * scale ) )
    {        
        mesh_type *pLastMesh;

        if( tweenCounter == 0 )
        {            
            pMesh[frameIndex].SetLightList( pLightList );
            result = pMesh[frameIndex].Render( matrixMrg, portRadius, pPortalCenter );
			pLastMesh = &pMesh[frameIndex];
        }
        else
        {
            pInterpMesh[curAnimIndex].InterpulateMesh( pMesh[frameIndex], 
                                                       pMesh[frameIndex + 1],
                                                       tweenCounter * tweenVector );
			pInterpMesh[curAnimIndex].SetLightList( pLightList );
            result = pInterpMesh[curAnimIndex].Render( matrixMrg, portRadius, pPortalCenter );
			pLastMesh = &pInterpMesh[curAnimIndex];
        }

		// Are shadows enabled?
		if( shadow && pShadowMesh != NULL )
		{
			// Retransfor the mesh in to the world view
			DoWorldTransformation();
			pLastMesh->Transform( matrixMrg );

			CLight *light = pLastMesh->FindClosestLight( pLightList );

			if( light != NULL )
			{
				// Set the alpha channel shadow	image
				BindAnimationText( "shadowBox", shadowInten );

				// Transform the light position
				CPoint lightPt( light->GetPos() + lightOffsetPt );
				
				// Use the current mesh to build the shadow mesh
				pShadowMesh[curAnimIndex].ConvertToShadow( *pLastMesh,
														   matrixMrg,
														   lightPt,
														   shadowDirPt,
														   minPoint,
														   maxPoint );

				// Build a vector to offset each polygon to make use of the z buffer
				// otherwise the shadow will blend
				CPoint tmp(0.001f,0.001f,0.001f);
				CPoint offset = tmp * shadowDirPt;

				// Transform this vector so that it properly offsets each 
				// polygon based on the view rotation
				m.TransformVector(offset);
				offset.MakePermanent();
				result = pShadowMesh[curAnimIndex].RenderShadow( m, offset );
			}
		}
    }

    return result;

}   // Render


/************************************************************************                                                            
*    DESCRIPTION:  Render the animation frame
*
*    Output:   int - polygon count of the mesh rendered
************************************************************************/
template <class mesh_type>
int CAnimMeshLst<mesh_type>::Render()
{    
    int result = 0;

    // Do the bounding sphere check and see if we should allow it to render
    if( !pMesh[frameIndex].CullMesh_BoundSphere( pMesh[frameIndex].GetRadius() * scale ) )
    {        
        if( tweenCounter == 0 )
        {            
            result = pMesh[frameIndex].Render( matrixMrg );
        }
        else
        {
            pInterpMesh[curAnimIndex].InterpulateMesh( pMesh[frameIndex], 
                                                       pMesh[frameIndex + 1],
                                                       tweenCounter * tweenVector );
            result = pInterpMesh[curAnimIndex].Render( matrixMrg );
        }
    }

    return result;

}   // Render


/************************************************************************                                                            
*    DESCRIPTION:  Render the animation frame
*
*    Output:   int - polygon count of the mesh rendered
************************************************************************/
template <class mesh_type>
int CAnimMeshLst<mesh_type>::Render( float portRadius, CPoint *pPortalCenter )
{    
    int result = 0;

    // Do the bounding sphere check and see if we should allow it to render.
	// This object could have been scaled so we need to scale the radius
    if( !pMesh[frameIndex].CullMesh_BoundSphere( pMesh[frameIndex].GetRadius() * scale ) )
    {        
        if( tweenCounter == 0 )
        {            
            result = pMesh[frameIndex].Render( matrixMrg, portRadius, pPortalCenter );
        }
        else
        {
            pInterpMesh[curAnimIndex].InterpulateMesh( pMesh[frameIndex], 
                                                       pMesh[frameIndex + 1],
                                                       tweenCounter * tweenVector );
            result = pInterpMesh[curAnimIndex].Render( matrixMrg, portRadius, pPortalCenter );
        }
    }

    return result;

}   // Render


/************************************************************************                                                             
*    DESCRIPTION:  Make the class ready for tweening
************************************************************************/
template <class mesh_type>
bool CAnimMeshLst<mesh_type>::SetUpTweens()
{
	// Allocate the interpolation mesh list
	// This function is only called if we are doing real mesh animation
    pInterpMesh = new mesh_type[ animCount ];
    if( pInterpMesh == NULL )
        return false;

    // Make a copy of the first mesh of each animation to be used for interpolation.
    // Each animation can be different in vertex and poly count
    for( int i = 0; i < animCount; ++i )
    {
        pInterpMesh[i].CopyMesh( pMesh[ pAnimTable[i].frameStart ] );   
    }

	return true;

}   // SetUpTweens


/************************************************************************                                                             
*    DESCRIPTION:  Make the class ready for shadows
************************************************************************/
template <class mesh_type>
bool CAnimMeshLst<mesh_type>::SetUpShadows()
{
	// Allocate the shadow mesh list
    pShadowMesh = new mesh_type[ animCount ];
    if( pShadowMesh == NULL )
        return false;

    // Make a copy of the first mesh of each animation to be used for shadows.
    // Each animation can be different in vertex and poly count
    for( int i = 0; i < animCount; ++i )
    {
        pShadowMesh[i].CopyMesh( pMesh[ pAnimTable[i].frameStart ] );
        pShadowMesh[i].ClearLightLst();
        pShadowMesh[i].SetAmbientLightLevel( 255, 255, 255 );   
    }

	return true;

}   // SetUpShadows


/************************************************************************                                                             
*    DESCRIPTION:  Set the size
************************************************************************/
template <class mesh_type>
void CAnimMeshLst<mesh_type>::SetScale( float x, float y, float z )
{
	transMatrix.Scale( x, y, z );

	// Save the average scale
	scale = (x + y + z) / 3;

}   // SetScale


/************************************************************************                                                             
*    DESCRIPTION:  Set the rotation
************************************************************************/
template <class mesh_type>
void CAnimMeshLst<mesh_type>::SetRot( float x, float y, float z )
{
	transMatrix.Rotate( x, y, z );

}   // SetRot


/************************************************************************                                                             
*    DESCRIPTION:  Set the position
************************************************************************/
template <class mesh_type>
void CAnimMeshLst<mesh_type>::SetPos( float x, float y, float z )
{
	transMatrix.Translate( x, y, z );

}   // SetPos


/************************************************************************                                                             
*    DESCRIPTION:  Set the position
************************************************************************/
template <class mesh_type>
void CAnimMeshLst<mesh_type>::SetPos( CPoint &point, TIntVector &intVector )
{
	CPoint objOffset;

	if( intVector.size() > 0 )
	{
		// Translate the min and max points of this mesh
		transMatrix.Transform( pMesh[frameIndex].GetMaxPoint() );
		transMatrix.Transform( pMesh[frameIndex].GetMinPoint() );

		// Check for X offset
		if( intVector[0] > 0 )
		{
			objOffset.x = -pMesh[frameIndex].GetMaxPoint().tx;
		}
		else if( intVector[0] < 0 )
		{
			objOffset.x = -pMesh[frameIndex].GetMinPoint().tx;
		}

		// Check for Y offset
		if( intVector[1] > 0 )
		{
			objOffset.y = -pMesh[frameIndex].GetMaxPoint().ty;
		}
		else if( intVector[1] < 0 )
		{
			objOffset.y = -pMesh[frameIndex].GetMinPoint().ty;
		}

		// Check for Z offset
		if( intVector[2] > 0 )
		{
			objOffset.z = -pMesh[frameIndex].GetMaxPoint().tz;
		}
		else if( intVector[2] < 0 )
		{
			objOffset.z = -pMesh[frameIndex].GetMinPoint().tz;
		}
	}

	transMatrix.Translate( point.x + objOffset.x,
						   point.y + objOffset.y, 
						   point.z + objOffset.z );

}   // SetPos


#endif  // _CANIM_MESH_LST_H_