
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

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "GDefines.h"        // Header file with #defines
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

#include "CAnimMeshLst.h"    // Header file for this *.cpp file
#include "CMatrix.h"         // Matrix class



/************************************************************************
*    FUNCTION NAME:              Constructer                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
CAnimMeshLst<mesh_type>::CAnimMeshLst()
{
    pAnimTable = NULL;
    animCount = 0;
    frameCounter = 0;
    frameOffset = 0;
    tweenVector = 0;
    animPlayMode = EAPM_LOOP;
    pInterpMesh = NULL;
    curAnimIndex = 0;
    tweenCounter = 0;
    maxTweens = 0;
    maxFrames = 0;
}


/************************************************************************
*    FUNCTION NAME:         Free()                                                             
*
*    DESCRIPTION:           Free the allocations
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
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
void CAnimMeshLst<mesh_type>::Free()
{
    if( pAnimTable != NULL )
    {
        delete [] pAnimTable;
        pAnimTable = NULL;
        animCount = 0;
    }

    if( pInterpMesh != NULL )
    {
        delete [] pInterpMesh;
        pInterpMesh = NULL;
    }

}   // Free


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Load an object from a PLY binary file. This is
*                           our own simple file format for quickly loading
*                           vertices, face indexes, uv and texture indexes
*                           into the texture library.
*
*    FUNCTION PARAMETERS:
*    Input:    char *filePath - Path to file to load
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
bool CAnimMeshLst<mesh_type>::LoadFromFile( char *filePath )
{
    return CMeshList<mesh_type>::LoadFromFile( filePath );

}   // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Load an object from a PLY binary file. This is
*                           our own simple file for quickly loading
*                           vertices, face indexes, uv and texture indexes
*                           into the texture library.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - Handle to file
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
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

    // Allocate the interpolation mesh list
    pInterpMesh = new mesh_type[ animCount ];
    if( pInterpMesh == NULL )
        return result;

    // Read in the animation count
    if( fread( pAnimTable, sizeof( CAnimTable ), animCount, hFile ) != animCount )
        return result;
    
    result = CMeshList<mesh_type>::LoadFromFile( hFile );

    return result;

HANDLE_ERROR:;
    
    return result;

}   // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         LoadMeshFromFile()                                                             
*
*    DESCRIPTION:           Calling this function means the file only
*                           contains mesh data and we need to use this
*                           to calculate all our normals before we save
*                           the final file which is a combination of
*                           mesh and normal data.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - Handle to file
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
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

    // Allocate the interpolation mesh list
    pInterpMesh = new mesh_type[ animCount ];
    if( pInterpMesh == NULL )
        return result;

    // Read in the animation count
    if( fread( pAnimTable, sizeof( CAnimTable ), animCount, hFile ) != animCount )
        return result;
    
    result = CMeshList<mesh_type>::LoadMeshFromFile( hFile );

    return result;

}   // LoadMeshFromFile


/************************************************************************
*    FUNCTION NAME:         LoadMeshFromFile()                                                             
*
*    DESCRIPTION:           Calling this function means the file only
*                           contains mesh data and we need to use this
*                           to calculate all our normals before we save
*                           the final file which is a combination of
*                           mesh and normal data.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - Handle to file
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
bool CAnimMeshLst<mesh_type>::LoadMeshFromFile( char *filePath )
{    
    return CMeshList<mesh_type>::LoadMeshFromFile( filePath );

}   // LoadMeshFromFile


/************************************************************************
*    FUNCTION NAME:         SaveToFile()                                                             
*
*    DESCRIPTION:           Save mesh and normal data to file. This is
*                           our own simple file for quickly loading
*                           vertices, face indexes, uv and texture indexes
*                           into the texture library.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - Handle to file
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
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
*    FUNCTION NAME:         SaveToFile()                                                             
*
*    DESCRIPTION:           Calling this function means the file only
*                           contains mesh data and we need to use this
*                           to calculate all our normals before we save
*                           the final file which is a combination of
*                           mesh and normal data.
*
*    FUNCTION PARAMETERS:
*    Input:    FILE *hFile - Handle to file
*              var2type Var2Name - Description
*
*    Output:   bool - true on success, false on fail
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
bool CAnimMeshLst<mesh_type>::SaveToFile( char *filePath )
{    
    return CMeshList<mesh_type>::SaveToFile( filePath );

}   // SaveToFile


/************************************************************************
*    FUNCTION NAME:         SetAnimation()                                                             
*
*    DESCRIPTION:           Set the animation that is to run
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
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
void CAnimMeshLst<mesh_type>::SetAnimation( char *animStr, int twCount, EAPM_ANIM_PLAY_MODE playMode )
{
    for( int i = 0; i < animCount; ++i )
    {
        // Go through the animation table and search for the animation
        if( strcmp( pAnimTable[i].animNameStr, animStr ) == 0 )
        {
            // check to be safe that we are not over running our animation frames
            if( pAnimTable[i].frameStart + pAnimTable[i].frameCount <= meshCount )
            {
                frameOffset = pAnimTable[i].frameStart;
                maxFrames = pAnimTable[i].frameCount;
                frameCounter = 0;
                tweenCounter = 0;
                animPlayMode = playMode;
                maxTweens = twCount + 1;
                curAnimIndex = i;
                tweenVector = 1.0 / (float)maxTweens;
            }

            break;
        }   
    }
}   // SetAnimation


/************************************************************************
*    FUNCTION NAME:         IncFrame()                                                             
*
*    DESCRIPTION:           Inc the animation frame
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
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
void CAnimMeshLst<mesh_type>::IncFrame()
{
    if( animPlayMode == EAPM_START_END )
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
    else if( animPlayMode == EAPM_LOOP )
    {
        tweenCounter = (tweenCounter + 1) % maxTweens;

        if( tweenCounter == 0 )
        {
            frameCounter = (frameCounter + 1) % (maxFrames-1);
        }
    }
}   // IncFrame


/************************************************************************
*    FUNCTION NAME:         RenderFrame()                                                             
*
*    DESCRIPTION:           Render the animation frame
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
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
void CAnimMeshLst<mesh_type>::RenderFrame( CMatrix &m )
{    
    if( tweenCounter == 0 )
    {
        pMesh[frameOffset + frameCounter].RenderFrame( m );
    }
    else
    {
        pInterpMesh[curAnimIndex].InterpulateMesh( pMesh[frameOffset + frameCounter], 
                                                   pMesh[frameOffset + frameCounter + 1],
                                                   tweenCounter * tweenVector );
        pInterpMesh[curAnimIndex].RenderFrame( m );
    }

}   // RenderFrame


/************************************************************************
*    FUNCTION NAME:         SetUpTweens()                                                             
*
*    DESCRIPTION:           Make the class ready for tweening
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
*    H Rosenorn    11/22/04      Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

template <class mesh_type>
void CAnimMeshLst<mesh_type>::SetUpTweens()
{
    // Make a copy of the first mesh of each animation to be used for interpolation.
    // Each animation can be different in vertex and poly count
    for( int i = 0; i < animCount; ++i )
    {
        pInterpMesh[i].CopyMesh( pMesh[ pAnimTable[i].frameStart ] );   
    }
}   // SetUpTweens