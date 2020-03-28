
/************************************************************************
*
*    PRODUCT:         True color 3D Engine texture library
*
*    FILE NAME:       CTextLib.h
*
*    DESCRIPTION:     Creating and loading a texture library
*
*    Copyright(c) John De Goes & Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include "CTextLib.h"        // This objects header file


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
    if( pTMap )
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

	// Load the bit size
    if( fread( &bitSize, sizeof( bitSize ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Make sure it's not a negitive number
    if( count > 0 )
    {
        // Allocate the array of load classes
        pTMap = new CTextureMap[ count ];
        if( pTMap == NULL )
            goto HANDLE_ERROR;

        if( bitSize == 16 )
		{
	        // Load the textures
	        for( i = 0; i < count; ++i )
	        {
	            if( !pTMap[ i ].LoadBT_16( hFile ) )
	                goto HANDLE_ERROR;
	        }

	        // If we got this far, everything's ok
	        result = true;
		}
		else if( bitSize == 32 )
		{
			// Load the textures
	        for( i = 0; i < count; ++i )
	        {
	            if( !pTMap[ i ].LoadBT_32( hFile ) )
	                goto HANDLE_ERROR;
	        }

	        // If we got this far, everything's ok
	        result = true;
		}
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

		// Load the bit size
    	bitSize = *(int *)pData;

		// Inc the pointer
        pData += sizeof( bitSize );

        // Allocate the array of load classes
        pTMap = new CTextureMap[ count ];
        if( pTMap == NULL )
            goto HANDLE_ERROR;

        if( bitSize == 16 )
		{
	        // Load the textures:
	        for( i = 0; i < count; ++i )
	        {
	            // Returns the inc pointer
	            pData = pTMap[ i ].LoadFromPointer_16( pData );

	            if( pData == NULL )
	                goto HANDLE_ERROR;
	        }

			// If we got this far, everything's ok
        	result = true;
		}
		else if( bitSize == 32 )
		{
			// Load the textures:
	        for( i = 0; i < count; ++i )
	        {
	            // Returns the inc pointer
	            pData = pTMap[ i ].LoadFromPointer_32( pData );

	            if( pData == NULL )
	                goto HANDLE_ERROR;
	        }

			// If we got this far, everything's ok
        	result = true;
		}        
    }

HANDLE_ERROR:;
    
    return result;

}   // LoadFromPointer


/************************************************************************
*    FUNCTION NAME:         SaveToFile_32()                                                             
*
*    DESCRIPTION:           Save 32 bit bitmap texture library
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

bool CTextureLib::SaveToFile_32( char *fileName )
{
    int i, bitSize = 32;
    bool result = false;
    FILE *hFile;

    // Open the file
    hFile = fopen( fileName, "wb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    // Save the number of texture maps:
    if( fwrite ( &count, sizeof( count ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

	// Save the bit size
    if( fwrite ( &bitSize, sizeof( bitSize ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Save the texture data to the library file
    for( i = 0; i < count; ++i )
    {
        if( !pTMap[ i ].SaveBT_32( hFile ) )
            goto HANDLE_ERROR;
    }

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
    return result;

}   // SaveToFile_32


/************************************************************************
*    FUNCTION NAME:         SaveToFile_16()                                                             
*
*    DESCRIPTION:           Save 16 bit bitmap texture library
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

bool CTextureLib::SaveToFile_16( char *fileName )
{
    int i, bitSize = 16;
    bool result = false;
    FILE *hFile;

    // Open the file
    hFile = fopen( fileName, "wb" );
    if( hFile == NULL )
        goto HANDLE_ERROR;

    // Save the number of texture maps:
    if( fwrite ( &count, sizeof( count ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

	// Save the bit size
    if( fwrite ( &bitSize, sizeof( bitSize ), 1, hFile ) != 1 )
        goto HANDLE_ERROR;

    // Save the texture data to the library file
    for( i = 0; i < count; ++i )
    {
        if( !pTMap[ i ].SaveBT_16( hFile ) )
            goto HANDLE_ERROR;
    }

HANDLE_ERROR:;

    // Close the file
    if( hFile != NULL )
        fclose( hFile );
    
    return result;

}   // SaveToFile_16


/************************************************************************
*    FUNCTION NAME:         Create16BitLibFrom32()                                                             
*
*    DESCRIPTION:           Convert the 32 bit library to a 16 bit library
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

bool CTextureLib::Create16BitLibFrom32()
{
    bool result = false;
    int i;

    for( i = 0; i < count; ++i )
    {
        if( !pTMap[ i ].Create16BitTextFrom32() )
            goto HANDLE_ERROR;
    }

    // If we made it this far, we are OK
    result = true;

HANDLE_ERROR:;   

    return result;
      
}   // Create16BitLibFrom32