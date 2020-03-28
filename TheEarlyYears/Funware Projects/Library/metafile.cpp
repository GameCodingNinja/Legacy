
/************************************************************************
*
*    PRODUCT:         Metafile Manipulation
*
*    FILE NAME:       metafile.cpp
*
*    DESCRIPTION:     Metafile Object for Window Metafiles
*
*    IMPLEMENTATION:  Load and palys metafiles
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#include "GDefines.h"        // Header file with #defines
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include "minTools.h"        // A misc group of useful functions
#include "Metafile.h"        // Header file for this *.cpp file.



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

TMetafile::TMetafile()
{
    // Null out all important variables
    hEnMetaFile = NULL;
    hMetaFile = NULL;
    // Active DC
    hDeviceDC = NULL;

    /*
    #define MM_TEXT             1
    #define MM_LOMETRIC         2
    #define MM_HIMETRIC         3
    #define MM_LOENGLISH        4
    #define MM_HIENGLISH        5
    #define MM_TWIPS            6
    #define MM_ISOTROPIC        7
    #define MM_ANISOTROPIC      8
    */

    // Maping Mode
    mappingMode = MM_ANISOTROPIC;

    // Init the metafile header data
    // Can't seem to get the proper width and height
    FillMemory( &enMetafileHeader, 0, sizeof(ENHMETAHEADER) );
}


/************************************************************************
*    FUNCTION NAME:         LoadFromResource()                                                             
*
*    DESCRIPTION:           Load metafile from resource file.
*
*    FUNCTION PARAMETERS:
*    Input:    HINSTANCE hInst   - Program Instance
*              char *name        - Name of the resource
*              char *type        - Resource Type Id
*              int PointerType   - Flag to hold resource pointer or
*                                  allocate memory for the bitmap
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TMetafile::LoadFromResource( HINSTANCE hInst, char *name, char *type, BOOL convertToEnhance )
{
    TLoadRes LoadRes;
    BOOL Result = FALSE;

    Free(); // Free memory if need be

    if( !LoadTheResource( hInst, name, type, &LoadRes ) )
        goto HANDLE_ERROR;

    // Get the handle to the new metafile
    if( !ProcessMetafileData( (BYTE *)LoadRes.pData, LoadRes.Size, convertToEnhance ) )
        goto HANDLE_ERROR;

    // If we got rhis far, everythings OK
    Result = TRUE;

HANDLE_ERROR:;

    return Result;

} // LoadFromResource


/************************************************************************
*    FUNCTION NAME:         LoadFromFile()                                                             
*
*    DESCRIPTION:           Load metafile from file
*
*    FUNCTION PARAMETERS:
*    Input:    char *FileName    - Path to file name
*              var2type Var2Name - Description
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TMetafile::LoadFromFile( char *FileName, BOOL convertToEnhance )
{ 
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesRead;
    HANDLE hfile;
    DWORD size;
    BYTE *pData = NULL;
    BOOL Result = FALSE;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    Free(); // Free memory if need be

    // Open the file on the hard drive
    hfile = CreateFile( FileName, GENERIC_READ, FILE_SHARE_READ,
                           &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hfile == INVALID_HANDLE_VALUE )
    {
        PostMsg( "Error", "Can't open metafile (%s) to display.", FileName );
        goto HANDLE_ERROR;
    }

    // Get the file size of the bitmap so that we can allocate enough memory for it
    size = GetFileSize( hfile, NULL );

    // Allocate memory for metafile
    pData = new BYTE[ size ];
    if( pData == NULL )
    {
        PostMsg( "Error", "Unable to allocate memory for metafile (%s).", FileName );
        goto HANDLE_ERROR;
    }

    // Read contents of file into allocated memory
    ReadFile( hfile, pData, size, &bytesRead, NULL );

    if( bytesRead != size )
    {
        PostMsg( "Error", "Unable to load metafile (%s).", FileName );
        goto HANDLE_ERROR;
    }

    // Get the handle to the new metafile
    if( !ProcessMetafileData( pData, size, convertToEnhance ) )
        goto HANDLE_ERROR;

    // If we got rhis far, everythings OK
    Result = TRUE;

HANDLE_ERROR:;

    // We're done with the data so delete it
    if( pData )
        delete [] pData;

    if( hfile )
        CloseHandle( hfile );

    return Result;

} // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         Free                                                             
*
*    DESCRIPTION:           Free allocated memory
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TMetafile::Free()
{
    // We allocated this memory and now it has to be freed
    if( hEnMetaFile )
    {
       DeleteEnhMetaFile( hEnMetaFile );
       hEnMetaFile = NULL;
    }

    if( hMetaFile )
    {
       DeleteMetaFile( hMetaFile );
       hMetaFile = NULL;
    }

}  // Free


/************************************************************************
*    FUNCTION NAME:         ProcessMetafileData                                                             
*
*    DESCRIPTION:           Check the data to see what type of metafile
*                           it is and create the metafile. If it's a 
*                           Windows metafile, transform it to an
*                           enhanced metafile
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TMetafile::ProcessMetafileData( BYTE *pMetaData, DWORD size, BOOL convertToEnhance )
{
    BYTE *pDataTmp = pMetaData;
    DWORD AldusKey, MetaType, SizeTmp = size;
    HDC hDC;
    METAFILEPICT MetaPict;
    BOOL Result = FALSE;
    //PTAldusFileHeader pAldusHeader;

    // Check to see if this is an Aldus meta file
    AldusKey = *((DWORD *)pDataTmp);

    // If we have an aldus key, we just want to jump over it
    // because we're not going to use it
    if( AldusKey == ALDUS_KEY )
    {
        //pAldusHeader = PTAldusFileHeader( pDataTmp );
        pDataTmp += sizeof( TAldusFileHeader );
        SizeTmp -= sizeof( TAldusFileHeader );
    }

    // What type of metafile is this? Is it an enhanced metafile
    // or a windows metafile. The first two fields of the header structure
    // is type and size but the enhanced metafile uses DWORDs while the 
    // Windows metafile uses WORDs. If reading in the first DWORD is greater
    // than 2, then it is a Windows metafile because that would be the type
    // and the size together which I'm sure is much greater than two.
    MetaType = *((DWORD *)pDataTmp);

    // EMR_HEADER = 1
    if( MetaType == EMR_HEADER )
    {
        // Copy the metafile header data
        // Can't seem to get the proper width and height
        CopyMemory( &enMetafileHeader, pDataTmp, sizeof(ENHMETAHEADER) );

        // This is an enhanced metafile data
        hEnMetaFile = SetEnhMetaFileBits( SizeTmp, pDataTmp );
    }
    else
    {
        // Do we convert to an enhanced meta file or leave it as a wmf?
        if( convertToEnhance )
        {
            // zero out the metafile picture structure
            memset( &MetaPict, 0, sizeof(METAFILEPICT) );

            // Try using a DC we already have first.
            if( hDeviceDC )
                hDC = hDeviceDC;
            else
            {
                // Get the handle to the active window
                // Even if the GetActiveWindow function fails, it returns 0 which is
                // the desktop window and that will work too.
                hDC = GetDC( GetActiveWindow() );
            }

            // Convert the Windows metafile to an enhanced metafile
            MetaPict.mm = mappingMode;
            hEnMetaFile = SetWinMetaFileBits( SizeTmp, pDataTmp, hDC, &MetaPict );

            // Get the information of our new metafile
            if( hEnMetaFile )
                GetEnhMetaFileHeader( hEnMetaFile, sizeof(ENHMETAHEADER), &enMetafileHeader );

            // Only release the DC if we had to go get it here.
            if( !hDeviceDC && hDC )
                ReleaseDC( GetActiveWindow(), hDC );
        }
        else
        {
            // Copy the metafile header data
            // Can't seem to get the proper width and height
            CopyMemory( &metafileHeader, pDataTmp, sizeof(METAHEADER) );

            // This is an enhanced metafile data
            hMetaFile = SetMetaFileBitsEx( SizeTmp, pDataTmp );
        }
    }

    // Do we have a handle to a metafile?
    if( hEnMetaFile || hMetaFile )
        Result = TRUE;

    return Result;

}   // ProcessMetafileData


/************************************************************************
*    FUNCTION NAME:         IsEmpty()                                                             
*
*    DESCRIPTION:           Do we have a graphic sitting in here?
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
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TMetafile::IsEmpty()
{
    BOOL Result = TRUE;

    if( hEnMetaFile || hMetaFile )
        Result = FALSE;

    return Result;

}   // IsEmpty


/************************************************************************
*    FUNCTION NAME:         Draw()                                                             
*
*    DESCRIPTION:           Draw the metafile to a dc
*
*    FUNCTION PARAMETERS:
*    Input:    HWND hDest - Handle to a device (window or printer) 
*                           where we get the DC
*              RECT rect  - Output rectangle
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TMetafile::Draw( HDC hDC, RECT *rect )
{
    BOOL Result = FALSE;

    if( hEnMetaFile )
        Result = PlayEnhMetaFile( hDC, hEnMetaFile, rect );

    else if( hMetaFile )
        Result = PlayMetaFile( hDC, hMetaFile );

    return Result;

}   // Draw


/************************************************************************
*    FUNCTION NAME:         Draw()                                                             
*
*    DESCRIPTION:           Draw the metafile to a dc
*
*    FUNCTION PARAMETERS:
*    Input:    HDC DestDC - DC of output device (window or printer) 
*                           where we get the DC
*              RECT rect  - Output rectangle
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/15/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TMetafile::Draw( HWND hDest, RECT *rect )
{
    BOOL Result = FALSE;

    if( hEnMetaFile )
    {
        // Get the DC from the supplied handle
        HDC DC = GetDC( hDest );

        // Draw the metafile
        Result = PlayEnhMetaFile( DC, hEnMetaFile, rect );

        // Release the handle
        ReleaseDC( hDest, DC  );
    }
    else if( hEnMetaFile )
    {
        // Get the DC from the supplied handle
        HDC DC = GetDC( hDest );

        // Draw the metafile
        Result = PlayMetaFile( DC, hMetaFile );

        // Release the handle
        ReleaseDC( hDest, DC  );
    }

    return Result;

}   // Draw