
/************************************************************************
*
*    PRODUCT:         3D Engine texture library
*
*    FILE NAME:       CTextLib.cpp
*
*    DESCRIPTION:     Creating and loading a texture library
*
*    Copyright(c) John De Goes & Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include "CTextLib.h"        // This objects header file

#include "gl/gl.h"           // standard OpenGL include
#include "gl/glu.h"          // OpenGL utilities

extern unsigned int gTextureCurrentID;

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
CTextureLib::CTextureLib()
{
    count = 0;
    pTMap = NULL;
    autoMipmap = false;
    textureFilter = GL_NEAREST;
    mipmapFilter = GL_NEAREST_MIPMAP_NEAREST;
}


/************************************************************************
*    FUNCTION NAME:              Destructor                                                             
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  10/22/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/
CTextureLib::~CTextureLib()
{
    Free();
}


/************************************************************************
*    FUNCTION NAME:         Free()                                                             
*
*    DESCRIPTION:           Free allocated data
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

void CTextureLib::Free()
{
    if( pTMap != NULL )
    {
        delete [] pTMap;
        pTMap = NULL;
        count = 0;
    }

}   // Free


/************************************************************************
*    FUNCTION NAME:         AllocateTextLibrary()                                                             
*
*    DESCRIPTION:           Allocate the texture library.
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

bool CTextureLib::AllocateTextLibrary( int aCount )
{
    bool result = true;

    // Free any allocated memory
    Free();

    // Make sure we have some files in the list
    if( aCount )
    {
        // Record the number of textures to allocate
        count = aCount;

        // Allocate an array to hold the textures
        pTMap = new CTextureMap[ count ];

        // Check all allocations
        if( pTMap == NULL )
        {
            count = 0;
            result = false;
        }
    }

    return result;

}   // AllocateTextLibrary


/************************************************************************
*    FUNCTION NAME:         AddTextureToLib()                                                             
*
*    DESCRIPTION:           Add a texture file to the library
*
*    FUNCTION PARAMETERS:
*    Input:    int index - Index into texture library to load the BMP
*              char *fileName - Path to load bitmap file
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

bool CTextureLib::AddTextureToLib( int index, char *fileName, EBT_FLIP_FLOP flipFlop )
{
    bool result = true;

    // Make sure we have an allocated list
    if( index < count )
    {
        // Load the texture by file name 
        if( !pTMap[ index ].LoadFromFile( fileName, flipFlop ) )
            result = false;
    }
    else
        result = false; 

    return result;

}   // AddTextureToLib


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Loads native bitmap texture library file
*
*    FUNCTION PARAMETERS:
*    Input:    char *fileName    - File path to texture library file
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

bool CTextureLib::LoadFromFile( char *fileName )
{
    int i;
    bool result = false;
    FILE *hFile;

    // Free any allocated memory
    Free();

    // Open the file
    hFile = fopen( fileName, "rb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    // Load the number of textures
    if( fread( &count, sizeof( count ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Make sure it's not a negitive number
    if( count > 0 )
    {
        // Allocate the array of load classes
        pTMap = new CTextureMap[ count ];
        if( pTMap == NULL )
            goto HANDLE_ERROR;

        // Load the textures
        for( i = 0; i < count; ++i )
        {
            if( !pTMap[ i ].LoadFromLibraryFile( hFile, autoMipmap, textureFilter, mipmapFilter ) )
                goto HANDLE_ERROR;
        }

        // If we got this far, everything's ok
        result = true;
    }

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
    return result;

}   // LoadFromFile_16


/************************************************************************
*    FUNCTION NAME:         LoadFromPointer_32()                                                             
*
*    DESCRIPTION:           Load native bitmap texture library format from
*                           a pointer. Mostly used for loading a texture
*                           library from resource.
*
*                           With the passed pointer, we load the data and
*                           inc the pointer and send it back. Similar to
*                           loading data via a file handle but we need to
*                           move the point position ourselves.
*
*    FUNCTION PARAMETERS:
*    Input:    unsigned char *pData - Pointer to texture library.
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

bool CTextureLib::LoadFromPointer( unsigned char *pData )
{
    int i;
    bool result = false;

    // Free any allocated memory
    Free();

    // Load the number of textures
    count = *(int *)pData;

    // Make sure it's not a negitive number
    if( count > 0 )
    {
        // Inc the pointer
        pData += sizeof( count );

        // Allocate the array of load classes
        pTMap = new CTextureMap[ count ];
        if( pTMap == NULL )
            goto HANDLE_ERROR;

        // Load the textures:
        for( i = 0; i < count; ++i )
        {
            // Returns the inc pointer
            pData = pTMap[ i ].LoadFromPointer( pData, autoMipmap, textureFilter, mipmapFilter );

            if( pData == NULL )
                goto HANDLE_ERROR;
        }

        // If we got this far, everything's ok
        result = true;        
    }

HANDLE_ERROR:;
    
    return result;

}   // LoadFromPointer


/************************************************************************
*    FUNCTION NAME:         SaveToFile()                                                             
*
*    DESCRIPTION:           Save texture library to file
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

bool CTextureLib::SaveToFile( char *fileName )
{
    int i;
    bool result = false;
    FILE *hFile;

    // Open the file
    hFile = fopen( fileName, "wb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    // Save the number of texture maps:
    if( fwrite ( &count, sizeof( count ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Save the texture data to the library file
    for( i = 0; i < count; ++i )
    {
        if( !pTMap[ i ].SaveToLibraryFile( hFile ) )
            goto HANDLE_ERROR;
    }

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
    return result;

}   // SaveToFile


/************************************************************************
*    FUNCTION NAME:         SetActiveTexture()                                                             
*
*    DESCRIPTION:           Set the active texture
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

void CTextureLib::SetActiveTexture( int index )
{
	// Only switch states selecting a new texture if the needed texture is different
    if( gTextureCurrentID != pTMap[index].textureID )
    {
        glBindTexture( GL_TEXTURE_2D, pTMap[index].textureID );
        gTextureCurrentID = pTMap[index].textureID;
    }

}	// SetActiveTexture