//
// File name: TextType.h
//
// Description:
//
// Author: John De Goes
//
// Project: Cutting Edge 3D Game Programming
//

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

#include <StdIO.H>
#include "TextType.h"

// Turn off warnings for deprecated calls
#pragma warning(disable : 4996)

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
ImageData::ImageData()
{
    TCount = 0;
    TMap = NULL;
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
ImageData::~ImageData()
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

void ImageData::Free()
{
    if( TMap )
    {
        delete [] TMap;
        TMap = NULL;
        TCount = 0;
    }

}   // Free


/************************************************************************
*    FUNCTION NAME:         LoadINI()                                                             
*
*    DESCRIPTION:           Load the text file of BMP names to convert
*                           to native format
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

BOOL ImageData::LoadINI( char *FileName )
{
    int ErrCode, FileCount = 0, i;
    char String[ MAX_PATH ];
    FILE *InFile;
    BOOL Result = FALSE;

    // Free any allocated memory
    Free();

    // Check to see if the file is there
    if( ( InFile = fopen ( FileName, "rt" ) ) == 0 )
        goto ERROR_JUMP;

    // Count all the strings in the file to see if we get an error
    // and jump out if we do or reach the end of the file
    for( ;; )
    {
        ErrCode = fscanf( InFile, "%s", String );

        if( ( ErrCode != 0 ) && ( ErrCode != EOF ) )
            ++FileCount;
        else
            break;
    }

    // Rewind
    rewind( InFile );

    // Record the number of strings
    TCount = FileCount;

    // Allocate an array to hold the textures
    if( ( TMap = new LoadTexture [ TCount ] ) == 0 )
        goto ERROR_JUMP;

    // Load each texture by file name
    for( i = 0; i < TCount; ++i )
    {
        // Read in the file name
        fscanf( InFile, "%s", String );

        // Load the texture
        if( !TMap[ i ].Load( String, i ) )
            goto ERROR_JUMP;
    }

    // If we made it this far, we are OK
    Result = TRUE;

    ERROR_JUMP:;

    // close the file if it has been opened
    if( InFile )
        fclose( InFile );

    return Result;

}   // LoadINI


/************************************************************************
*    FUNCTION NAME:         AllocateTextLibrary()                                                             
*
*    DESCRIPTION:           Allocate the texture library
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

BOOL ImageData::AllocateTextLibrary( int Count )
{
    BOOL Result = FALSE;

    // Free any allocated memory
    Free();

    // Make sure we have some files in the list
    if( Count )
    {
        // Record the number of textures to allocate
        TCount = Count;

        // Allocate an array to hold the textures
        if( ( TMap = new LoadTexture [ TCount ] ) == 0 )
		{
            TCount = 0;
            goto ERROR_JUMP;
		}

	    // If we made it this far, we are OK
	    Result = TRUE;
    }

    ERROR_JUMP:;

    return Result;

}   // AllocateTextLibrary


/************************************************************************
*    FUNCTION NAME:         BuildTextLibrary()                                                             
*
*    DESCRIPTION:           Build native texture library file
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

BOOL ImageData::BuildTextLibrary( int Index, char *FileName )
{
    BOOL Result = FALSE;

    // Make sure we have an allocated list
    if( Index < TCount )
    {
        // Load the texture by file name 
        if( !TMap[ Index ].Load( FileName, Index ) )
            goto ERROR_JUMP;

	    // If we made it this far, we are OK
	    Result = TRUE;
    }

    ERROR_JUMP:;

    return Result;

}   // BuildTextLibrary


/************************************************************************
*    FUNCTION NAME:         LoadBT()                                                             
*
*    DESCRIPTION:           Load native format of bitmap textures
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

BOOL ImageData::LoadBT( FILE *InFile )
{
    int i;
    BOOL Result = FALSE;

    // Free any allocated memory
    Free();

    // Load the number of textures
    fread( &TCount, sizeof TCount, 1, InFile );

    if( TCount )
    {
        // Allocate memory for bitmap images
        if( ( TMap = new LoadTexture[ TCount ] ) == 0 )
            goto LOAD_ERROR;

        // Load palette of the first image only
        TMap[ 0 ].LoadPal( InFile );

        // Load the textures:
        for( i = 0; i < TCount; ++i )
        {
            if( !TMap[ i ].LoadBT( InFile ) )
                goto LOAD_ERROR;
        }

        Result = TRUE;
    }

    LOAD_ERROR:;

    return Result;

}   // LoadBT


/************************************************************************
*    FUNCTION NAME:         LoadFromPointer()                                                             
*
*    DESCRIPTION:           Load native format of bitmap textures from a pointer
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

BOOL ImageData::LoadFromPointer( BYTE *pBTdata )
{
    int i;
    BOOL Result = FALSE;

    // Free any allocated memory
    Free();

    // Load the number of textures
    TCount = *(WORD *)pBTdata;

    if( TCount )
    {
        // Inc the pointer
        pBTdata += sizeof( WORD );

        // Allocate memory for bitmap images
        if( ( TMap = new LoadTexture[ TCount ] ) == 0 )
            goto LOAD_ERROR;

        // Load palette of the first image only
        pBTdata = TMap[ 0 ].LoadPalfromPtr( pBTdata );

        // Load the textures:
        for( i = 0; i < TCount; ++i )
        {
            // Returns the inc pointer
            pBTdata = TMap[ i ].LoadBTfromPtr( pBTdata );

			if( !pBTdata )
                goto LOAD_ERROR;
        }

        Result = TRUE;
    }

    LOAD_ERROR:;

    return Result;

}   // LoadFromPointer


/************************************************************************
*    FUNCTION NAME:         SaveBT()                                                             
*
*    DESCRIPTION:           Save native format of bitmap textures
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

BOOL ImageData::SaveBT( FILE *OutFile )
{
    int i;

    // Save the number of texture maps:
    fwrite ( &TCount, sizeof TCount, 1, OutFile );

    // Save the palette:
    TMap[ 0 ].SavePal( OutFile );

    // Save the texture data:
    for( i = 0; i < TCount; ++i )
    {
        if( TMap[ i ].SaveBT( OutFile ) == 0 )
            return FALSE;
    }

    return TRUE;

}   // SaveBT



/************************************************************************
*    FUNCTION NAME:         ReadTexture()                                                             
*
*    DESCRIPTION:           Loads native bitmap texture format file
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

BOOL ImageData::ReadTexture( char *FileName )
{
    FILE *InFile;

    if( ( InFile = fopen ( FileName, "rb" ) ) == 0 )
        return FALSE;

    // Load the textures
    if( !LoadBT( InFile ) )
        return FALSE;

    // close the file
    fclose( InFile );

    return TRUE;

}   // ReadTexture


/************************************************************************
*    FUNCTION NAME:         WriteTexture()                                                             
*
*    DESCRIPTION:           Save native bitmap texture format file
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

BOOL ImageData::WriteTexture( char *FileName )
{
    FILE *OutFile;

    if( ( OutFile = fopen ( FileName, "wb" ) ) == 0 )
        return FALSE;

    // Save the textures
    SaveBT( OutFile );

    // close the file
    fclose( OutFile );

    return TRUE;

}   // WriteTexture


/************************************************************************
*    FUNCTION NAME:         LoadPal()                                                             
*
*    DESCRIPTION:           Load the palette
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

BOOL ImageData::LoadPal( FILE *InFile )
{
    return TMap[ 0 ].LoadPal( InFile );

}   // ImageData


/************************************************************************
*    FUNCTION NAME:         SavePal()                                                             
*
*    DESCRIPTION:           Save the palette
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

BOOL ImageData::SavePal( FILE *OutFile )
{
    return TMap[ 0 ].SavePal( OutFile );

}   // SavePal