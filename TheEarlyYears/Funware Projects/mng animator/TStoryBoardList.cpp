
/************************************************************************
*
*    PRODUCT:         Storyboard list
*
*    FILE NAME:       TStoryBoardList.cpp
*
*    DESCRIPTION:     Object for holding lists of storyboards
*
*    IMPLEMENTATION:  
*
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.
#include <windowsx.h>        // Macros the makes windows programming easer and more readable. Doesn't add to code size

#include "TStoryBoardList.h" // Header file for this *.cpp file.
#include <stdlib.h>          // Declares several commonly used routines such as atoi, random, abs, malloc etc.
#include "gtools.h"          // Group of useful functions


/************************************************************************
*
*                         Sound list object
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         Constructor                                                             
*
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CSoundListAry::CSoundListAry()
{
    // Set the format ID for this object so that we can tell
    // when we are trying to load a file that is not ours.
    ObjFileHeader.ObjForamtID = 0x001DFD91;

}   // Constructor


/************************************************************************
*    FUNCTION NAME:         Destructor                                                             
*
*    DESCRIPTION:           Free up all allocated resources.
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

CSoundListAry::~CSoundListAry()
{
                
}   // Destructor


/************************************************************************
*    FUNCTION NAME:         Add                                                             
*
*    DESCRIPTION:           Allocates more memory and copies the int.
*
*    FUNCTION PARAMETERS:
*    Input:    char *t           - String to be added
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL CSoundListAry::Add()
{
    PSSoundList temp;

    // Return if there has been an allocation error
    if( fAllocationError )
        return FALSE;

    // Allocate the string to the size we need
    temp = new SSoundList;

    // Check the allocation and copy the new string
    if( temp )
    {
        // Add the string to the object list
        AddObj( (LPVOID)temp );
    }
    else
        fAllocationError = TRUE;

    return (fAllocationError == FALSE);

}   // Add


/************************************************************************
*    FUNCTION NAME:         Delete                                                             
*
*    DESCRIPTION:           Since this class allocats anothe object, we
*                           free up that classes data because the destructor
*                           is not called.
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the requested object
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CSoundListAry::Delete( int Index )
{
    PSSoundList Temp;

    // Free the allocated story boards
    Temp = (PSSoundList)GetObj( Index );

    // Free the media player
    Temp->MediaPlayer.Free();

    // Let the parent class handle the rest
    TObjList::Delete( Index );

}   // Delete


/************************************************************************
*    FUNCTION NAME:         Free                                                             
*
*    DESCRIPTION:           Free up all allocated resources.
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void CSoundListAry::Free()
{
    PSSoundList Temp;

    // Delete all the allocated pointers
    for( int i = 0; i < count; ++i )
    {
        // Free the allocated story boards
        Temp = (PSSoundList)GetObj( i );

        // Free the media player
        Temp->MediaPlayer.Free();
    }

    // Free everything
    FreeObj();

}   // Free



/************************************************************************
*
*                        Storyboard list object
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         Constructor                                                             
*
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TStoryList::TStoryList()
{
    // Set the format ID for this object so that we can tell
    // when we are trying to load a file that is not ours.
    ObjFileHeader.ObjForamtID = 0x001DFD91;

}   // Constructor


/************************************************************************
*    FUNCTION NAME:         Destructor                                                             
*
*    DESCRIPTION:           Free up all allocated resources.
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TStoryList::~TStoryList()
{
                
}   // Destructor



/************************************************************************
*    FUNCTION NAME:         Add                                                             
*
*    DESCRIPTION:           Allocates more memory and copies the int.
*
*    FUNCTION PARAMETERS:
*    Input:    char *t           - String to be added
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TStoryList::Add()
{
    PTStoryBoard temp;

    // Return if there has been an allocation error
    if( fAllocationError )
        return FALSE;

    // Allocate the string to the size we need
    temp = new TStoryBoard;

    // Check the allocation and copy the new string
    if( temp )
    {
        // Add the string to the object list
        AddObj( (LPVOID)temp );
    }
    else
        fAllocationError = TRUE;

    return (fAllocationError == FALSE);

}   // Add


/************************************************************************
*    FUNCTION NAME:         Free                                                             
*
*    DESCRIPTION:           Free up all allocated resources.
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TStoryList::Free()
{
    PTStoryBoard Temp;

    // Delete all the allocated pointers
    for( int i = 0; i < count; ++i )
    {
        // Free the allocated story boards
        Temp = (PTStoryBoard)GetObj( i );

        // Free the sound list
        Temp->SndLst.Free();
    }

    // Free everything
    FreeObj();

}   // Free


/************************************************************************
*    FUNCTION NAME:         Delete                                                             
*
*    DESCRIPTION:           Since this class allocats anothe object, we
*                           free up that classes data because the destructor
*                           is not called.
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the requested object
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TStoryList::Delete( int Index )
{
    PTStoryBoard Temp;

    // Free the allocated story boards
    Temp = (PTStoryBoard)GetObj( Index );

    // Free the sound list
    Temp->SndLst.Free();

    // Let the parent class handle the rest
    TObjList::Delete( Index );

}   // Delete


/************************************************************************
*
*                        Storyboard array object
*
************************************************************************/


/************************************************************************
*    FUNCTION NAME:         Constructor                                                             
*
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TStoryListArry::TStoryListArry()
{
    // Set the format ID for this object so that we can tell
    // when we are trying to load a file that is not ours.
    ObjFileHeader.ObjForamtID = 0xFE421802;

}   // Constructor


/************************************************************************
*    FUNCTION NAME:         Destructor                                                             
*
*    DESCRIPTION:           Free up all allocated resources.
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TStoryListArry::~TStoryListArry()
{
    PTStoryBoardArry Temp;

    // Delete all the allocated pointers
    for( int i = 0; i < count; ++i )
    {
        // Free the allocated story boards
        Temp = (PTStoryBoardArry)GetObj( i );

        // Free the storyboards
        Temp->StoryList.Free();
    }
                    
}   // Destructor



/************************************************************************
*    FUNCTION NAME:         Add                                                             
*
*    DESCRIPTION:           Allocates more memory and copies the int.
*
*    FUNCTION PARAMETERS:
*    Input:    char *t           - String to be added
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TStoryListArry::Add()
{
    PTStoryBoardArry Temp;

    // Return if there has been an allocation error
    if( fAllocationError )
        return FALSE;

    // Allocate the object
    Temp = new TStoryBoardArry;

    // Check the allocation and copy the new string
    if( Temp )
    {
        // Add the object to the object list
        AddObj( (LPVOID)Temp );
    }
    else
        fAllocationError = TRUE;

    return (fAllocationError == FALSE);

}   // Add


/************************************************************************
*    FUNCTION NAME:         Free                                                             
*
*    DESCRIPTION:           Free up all allocated resources.
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
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TStoryListArry::Free()
{
    PTStoryBoardArry Temp;

    // Delete all the allocated pointers
    for( int i = 0; i < count; ++i )
    {
        // Free the allocated story boards
        Temp = (PTStoryBoardArry)GetObj( i );

        // Free the storyboards
        Temp->StoryList.Free();

        // Free the mng bitmaps
        Temp->mng.Free();

        // Free the sound list
        Temp->soundList.Free();
    }

    // Free the pointer array
    FreeObj();

}   // Free


/************************************************************************
*    FUNCTION NAME:         SaveToFile                                                             
*
*    DESCRIPTION:           Save the object data to a file. We need our own
*                           for this class.
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TStoryListArry::SaveToFile( int index )
{
    SECURITY_ATTRIBUTES FileSec;
    HANDLE hFile;
    BOOL Result = FALSE;
    char pathToFile[MAX_PATH*2], mngFileName[MAX_PATH*2], tmpPath[MAX_PATH*2];
    int length, i, animsDirOffset=0;

    FileSec.nLength = sizeof( SECURITY_ATTRIBUTES );
    FileSec.lpSecurityDescriptor = NULL;
    FileSec.bInheritHandle = FALSE;

    // Create the file
    hFile = CreateFile( GetPtrToScriptName( index ),
                        GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ,
                        &FileSec, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

    if( hFile == INVALID_HANDLE_VALUE )
        goto HANDLE_ERROR;

    // temporary copy the name of the MNG file
    strcpy( mngFileName, GetPtrToMngName( index ) );

    // Copy over the path
    CropOutPath( GetPtrToScriptName( index ), tmpPath );

    // Cap off the end of the directory
    strcat( tmpPath, "\\" );

    // Terminate the string
    pathToFile[0] = 0;

    // Get the length of the path
    length = strlen( tmpPath ) - 6;

    // Search the path for the anims folder
    for( i = 0; i < length; ++i )
    {
        // Check for the '\ANIM\' directory
        if( tmpPath[i] == '\\' && 
            (tmpPath[i+1] == 'A' || tmpPath[i+1] == 'a') &&
            (tmpPath[i+2] == 'N' || tmpPath[i+2] == 'n') && 
            (tmpPath[i+3] == 'I' || tmpPath[i+3] == 'i') &&
            (tmpPath[i+4] == 'M' || tmpPath[i+4] == 'm') &&
            (tmpPath[i+5] == 'S' || tmpPath[i+5] == 's') &&
            tmpPath[i+6] == '\\' )
        {
            // Save the offset to the animsDirOffset directory
            animsDirOffset = i + 1;

            // Copy over the path
            strcpy( pathToFile, (tmpPath + animsDirOffset) );

            break;
        }
    }

    // Check that we founf the path
    if( !pathToFile[0] )
    {
        PostMsg( "Error", "Could not find the ANIMS directory. The MNG file must be located in this directory or a sub-directory of ANIMS." );
    }
    else
    {
        // Convert the dos '\\' to the unix '/'
        ConvertToDosUnixPath( pathToFile );

        // Build the path to the file
        wsprintf( GetPtrToMngName( index ), "%s%s", pathToFile, mngFileName );
    }

    // Save all this object's data
    if( !SaveToFile( hFile, index, animsDirOffset ) )
    {
        PostMsg( "Error", "Error saving animation file." );
        goto HANDLE_ERROR;
    }

    Result = TRUE;

HANDLE_ERROR:;

    // Copy the file name back
    strcpy( GetPtrToMngName( index ), mngFileName );

    if( hFile )
        CloseHandle( hFile );

    return Result;

}   // SaveToFile


/************************************************************************
*    FUNCTION NAME:         SaveToFile                                                             
*
*    DESCRIPTION:           Save the object data to a file. We need our own
*                           for this class.
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TStoryListArry::SaveToFile( HANDLE hFile, int index, int animsDirOffset )
{
    BOOL Result = FALSE;
    char OutPutStr[500], soundPath[MAX_PATH*2];
    BOOL FileReadWriteResult, baseOffsetWriten = FALSE;
    unsigned long bytesWritten;
    int i, j, frm, snd, id;
    TStringList strList;

    // Build the script header
    strcpy( OutPutStr, "<AnimationScript>\n\n" );

    // Write the storyboard
    FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
    if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
        goto HANDLE_ERROR;

    // Build the MNG file name tags
    wsprintf( OutPutStr, "    <MNG_File> %s </MNG_File>\n\n", GetPtrToMngName( index ) );

    // Write the storyboard
    FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
    if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
        goto HANDLE_ERROR;

    // Do we have a sound list?
    if( GetSoundListStr( index )->Count() )
    {
        // Go through each sound and check to see if it is in use
        // Save a list to ensure only the sounds being used get saved to the script
        for( i = 0; i < GetSoundListStr( index )->Count(); ++i )
        {
            for( frm = 0; frm < GetStoryList( index )->Count(); ++frm )
            {
                for( snd = 0; snd < GetStoryList( index )->GetSndLst( frm )->Count(); ++snd )
                {
                    // If we have a sound ID that equils the string index, that sound is being used
                    // so save it to the list
                    if( GetStoryList( index )->GetSndLst( frm )->GetID( snd ) == i )
                    {
                        // Add the string to the list
                        strList.Add( GetSoundListStr( index )->GetString( i ) );

                        // We found what we are looking for so jump out of the double for loop
                        goto JUMP_FROM_DOUBLE_FOR_LOOP;
                    }
                }
            }

            JUMP_FROM_DOUBLE_FOR_LOOP:;
        }

        // Do we have any strings in the list?
        // Then write it to the script
        if( strList.Count() )
        {
            // Just for fun, lets sort it
            strList.Sort();
            
            strcpy( OutPutStr, "    <SoundList>\n" );

            // Write to the script
            FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
            if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
                goto HANDLE_ERROR;

            // Write out each sound path minus the path before the sounds folder
            for( i = 0; i < strList.Count(); ++i )
            {
                // Copy over the path
                strcpy( soundPath, strList.GetString( i ) );

                // Convert the path to a unix path
                ConvertToDosUnixPath( soundPath );

                wsprintf( OutPutStr, "        %s\n", (soundPath + animsDirOffset) );

                // Write to the script
                FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
                if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
                    goto HANDLE_ERROR;
            }

            strcpy( OutPutStr, "    </SoundList>\n\n" );

            // Write to the script
            FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
            if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
                goto HANDLE_ERROR;
        }
    }

    if( GetBaseXOffset( index ) )
    {
        // Build the X base offsets tag
        wsprintf( OutPutStr, "    ; <baseX> %d </baseX>\n", GetBaseXOffset( index ) );

        // Write the storyboard
        FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
        if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
            goto HANDLE_ERROR;

        baseOffsetWriten = TRUE;
    }

    if( GetBaseYOffset( index ) )
    {
        // Build the X base offsets tag
        wsprintf( OutPutStr, "    ; <baseY> %d </baseY>\n", GetBaseYOffset( index ) );

        // Write the storyboard
        FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
        if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
            goto HANDLE_ERROR;

        baseOffsetWriten = TRUE;
    }

    if( GetBaseZOffset( index ) )
    {
        // Build the X base offsets tag
        wsprintf( OutPutStr, "    ; <baseZ> %d </baseZ>\n", GetBaseZOffset( index ) );

        // Write the storyboard
        FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
        if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
            goto HANDLE_ERROR;

        baseOffsetWriten = TRUE;
    }

    // If we wrote a base offset, add a return
    if( baseOffsetWriten )
    {
        // Build the return
        strcpy( OutPutStr, "\n" );

        // Write the storyboard
        FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
        if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
            goto HANDLE_ERROR;
    }

    // Build the script element tag
    strcpy( OutPutStr, "    <ScriptElementSet>\n\n" );

    // Write the storyboard
    FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
    if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
        goto HANDLE_ERROR;

    // Lay down all the scripts
    for( frm = 0; frm < GetStoryList( index )->Count(); ++frm )
    {
        // Build the script element tag
        strcpy( OutPutStr, "        <ScriptElement>\n" );

        // Write the storyboard
        FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
        if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
            goto HANDLE_ERROR;

        // See if the setting is something other then a default setting
        if( GetStoryList( index )->GetXOffset( frm ) )
        {            
            // Build the script element tag
            wsprintf( OutPutStr, "            <X> %d </X>\n", GetStoryList( index )->GetXOffset( frm ) + GetBaseXOffset( index ) );

            // Write the storyboard
            FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
            if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
                goto HANDLE_ERROR;
        }

        // See if the setting is something other then a default setting
        if( GetStoryList( index )->GetYOffset( frm ) )
        {
            // Build the script element tag
            wsprintf( OutPutStr, "            <Y> %d </Y>\n", GetStoryList( index )->GetYOffset( frm ) + GetBaseYOffset( index ) );

            // Write the storyboard
            FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
            if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
                goto HANDLE_ERROR;
        }

        // See if the setting is something other then a default setting
        if( GetStoryList( index )->GetZOrder( frm ) )
        {
            // Build the script element tag
            wsprintf( OutPutStr, "            <zOrder> %d </zOrder>\n", GetStoryList( index )->GetZOrder( frm ) + GetBaseZOffset( index ) );

            // Write the storyboard
            FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
            if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
                goto HANDLE_ERROR;
        }

        // Build the script element tag
        wsprintf( OutPutStr, "            <FrameIndex> %d </FrameIndex>\n", GetStoryList( index )->GetFrameIndex( frm ) );

        // Write the storyboard
        FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
        if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
            goto HANDLE_ERROR;

        // See if the setting is something other then a default setting
        if( GetStoryList( index )->GetFrameRepeat( frm ) )
        {
            // Build the script element tag
            wsprintf( OutPutStr, "            <FrameRepeat> %d </FrameRepeat>\n", GetStoryList( index )->GetFrameRepeat( frm ) );

            // Write the storyboard
            FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
            if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
                goto HANDLE_ERROR;
        }

        // Do we have any sounds related to this frame?
        for( i = 0; i < GetStoryList( index )->GetSndLst( frm )->Count(); ++i )
        {
            // Build the script element tag
            strcpy( OutPutStr, "            <Sound>\n" );

            // Write the storyboard
            FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
            if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
                goto HANDLE_ERROR;

            // get the id for the sound
            id = GetStoryList( index )->GetSndLst( frm )->GetID( i );

            // Search for the path in the list
            for( j = 0; j < strList.Count(); ++j )
            {
                if( strcmp( strList.GetString( j ), GetSoundListStr( index )->GetString( id ) ) == 0 )
                {
                    // reset the id and break out
                    id = j;
                    break;
                }
            }

            // Build the script element tag
            wsprintf( OutPutStr, "                <SoundID> %d </SoundID>\n", id );

            // Write the storyboard
            FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
            if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
                goto HANDLE_ERROR;

            // Only save if it's not the default
            if( GetStoryList( index )->GetSndLst( frm )->GetVolume( i ) != DEF_SND_VOLUME )
            {
                // Build the script element tag
                wsprintf( OutPutStr, "                <Volume> %d </Volume>\n", GetStoryList( index )->GetSndLst( frm )->GetVolume( i ) );

                // Write the storyboard
                FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
                if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
                    goto HANDLE_ERROR;
            }

            // Only save if it's not the default
            if( GetStoryList( index )->GetSndLst( frm )->GetChannel( i ) != DEF_SND_CHANNEL )
            {
                // Build the script element tag
                wsprintf( OutPutStr, "                <Channel> %d </Channel>\n", GetStoryList( index )->GetSndLst( frm )->GetChannel( i ) );

                // Write the storyboard
                FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
                if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
                    goto HANDLE_ERROR;
            }

            // Build the script element tag
            strcpy( OutPutStr, "            </Sound>\n" );

            // Write the storyboard
            FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
            if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
                goto HANDLE_ERROR;
        }

        // Build the script element tag
        strcpy( OutPutStr, "        </ScriptElement>\n\n" );

        // Write the storyboard
        FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
        if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
            goto HANDLE_ERROR;
    }

    // Build the script element tag
    strcpy( OutPutStr, "    </ScriptElementSet>\n\n" );

    // Write the storyboard
    FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
    if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
        goto HANDLE_ERROR;

    // Build the script header
    strcpy( OutPutStr, "</AnimationScript>" );

    // Write the storyboard
    FileReadWriteResult = WriteFile( hFile, OutPutStr, strlen(OutPutStr), &bytesWritten, NULL );
    if( !FileReadWriteResult || bytesWritten != strlen(OutPutStr) )
        goto HANDLE_ERROR;


    Result = TRUE;

HANDLE_ERROR:;

    return Result;

}   // SaveToFile


/************************************************************************
*    FUNCTION NAME:         LoadFromFile                                                             
*
*    DESCRIPTION:           Load the object data from a file. We need our own
*                           for this class.
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TStoryListArry::LoadFromFile( char *FileName )
{
    SECURITY_ATTRIBUTES FileSec;
    HANDLE hFile;
    BOOL Result = FALSE;

    FileSec.nLength = sizeof( SECURITY_ATTRIBUTES );
    FileSec.lpSecurityDescriptor = NULL;
    FileSec.bInheritHandle = FALSE;

    // Create the file
    hFile = CreateFile( FileName, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ,
                        &FileSec, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    if( hFile == INVALID_HANDLE_VALUE )
        goto HANDLE_ERROR;

    // Load the object's data
    if( !LoadFromFile( hFile, FileName ) )
    {
        PostMsg( "Error", "Error loading animation script." );
        goto HANDLE_ERROR;
    }   
        
    Result = TRUE;  

HANDLE_ERROR:;

    if( hFile )
        CloseHandle( hFile );

    return Result;

}   // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         LoadFromFile                                                             
*
*    DESCRIPTION:           Load the object data from a file. We need our own
*                           for this class.
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TStoryListArry::LoadFromFile( HANDLE hFile, char *ScriptName )
{
    BOOL Result = FALSE;
    int fileSize;
    char *pData = NULL;
    unsigned long bytesRead;
    BOOL FileReadResult;

    // Get the file size so we know how much memory to allocate
    fileSize = GetFileSize( hFile, NULL );

    // Make sure the file has a size
    if( fileSize < 1 )
    {
        PostMsg( "Error", "File has no size." );

        goto HANDLE_ERROR;
    }

    // Allocate a character array
    pData = new char[ fileSize ];

    // Test all allocations
    if( !pData )
    {
        PostMsg( "Error", "Memory allocation failure." );

        goto HANDLE_ERROR;
    }

    // Read in the file
    FileReadResult = ReadFile( hFile, pData, fileSize, &bytesRead, NULL );

    if( !FileReadResult || bytesRead != fileSize )
    {
        PostMsg( "Error", "Error reading in script file" );

        goto HANDLE_ERROR;
    }

    // Create an object
    Add();

    // Save the script name
    strcpy( GetPtrToScriptName( Count()-1 ), ScriptName );

    // Parse the data to fill out the entries
    ParseData( pData, fileSize, ScriptName );
        
    Result = TRUE;  

HANDLE_ERROR:;

    if( pData )
        delete [] pData;

    return Result;

}   // LoadFromFile


/************************************************************************
*    FUNCTION NAME:         ParseData                                                             
*
*    DESCRIPTION:           Parse the data from the loaded file
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TStoryListArry::ParseData( char *pData, int size, char *fileName )
{
    int index=0, i, j, entryIndex=0, length, sndLstIndex;
    char tag[200], data[500], *pStr, soundPath[MAX_PATH*2], soundFile[MAX_PATH*2];
    TStringList stringList; 

    // NULL the tag
    tag[0] = 0;

    //Get the path before the anima directory

    length = strlen(fileName) - 6;
    soundPath[0] = 0;

    // Search the path for the anims folder to get the path before that
    for( i = 0; i < length; ++i )
    {
        // Check for the '\ANIMS\' directory
        if( fileName[i] == '\\' && 
            (fileName[i+1] == 'A' || fileName[i+1] == 'a') &&
            (fileName[i+2] == 'N' || fileName[i+2] == 'n') && 
            (fileName[i+3] == 'I' || fileName[i+3] == 'i') &&
            (fileName[i+4] == 'M' || fileName[i+4] == 'm') &&
            (fileName[i+5] == 'S' || fileName[i+5] == 's') &&
            fileName[i+6] == '\\' )
        {
            // Copy over the path
            strcpy( soundPath, fileName );

            // terminate the path
            soundPath[i + 1] = 0;

            break;
        }
    }

    do
    {
        // Search for a tag
        if( pData[ index ] == '<' )
        {
            // init the tag index
            i = 0;
            
            while( pData[ index ] != '>' )
                tag[ i++ ] = pData[ index++ ];
                
            // Save the end if the tag
            tag[ i++ ] = pData[ index++ ];
            
            // Terminate it
            tag[ i ] = 0;      
        }

        // Do we have a tag?
        // Do something depending on the tag
        if( tag[0] )
        {
            // Is this a read in data tag?
            if( (tag[1] != '/') &&
                (strcmp( "<MNG_File>", tag ) == 0) ||

                (strcmp( "<baseX>", tag ) == 0) ||
                (strcmp( "<baseY>", tag ) == 0) ||
                (strcmp( "<baseZ>", tag ) == 0) ||

                (strcmp( "<X>", tag ) == 0) ||
                (strcmp( "<Y>", tag ) == 0) ||
                (strcmp( "<zOrder>", tag ) == 0) ||
                (strcmp( "<FrameIndex>", tag ) == 0) ||
                (strcmp( "<FrameRepeat>", tag ) == 0) ||
                (strcmp( "<SoundID>", tag ) == 0) ||
                (strcmp( "<Volume>", tag ) == 0) ||
                (strcmp( "<Channel>", tag ) == 0)  )
            {
                i = 0;
                
                // Read in up to the next tag
                while( pData[ index ] != '<' )
                {
                    // Don't read in any space characters
                    if( pData[ index ] != ' ' )
                        data[ i++ ] = pData[ index ];

                    ++index;
                }
                
                // terminate it
                data[ i ] = 0;  
            }

            // We need to read in a list of items
            else if( (tag[1] != '/') && (strcmp( "<SoundList>", tag ) == 0) )
            {
                i = 0;
                
                // Clear out any previous lists
                stringList.Free();

                // We need to read past the first return character
                while( pData[ index++ ] != '\n' )
                {
                    // Zoom past multiple returns if they are there
                    while( pData[ index ] == '\n' )
                        ++index;
                }
                
                // Read in up to the next tag
                while( pData[ index ] != '<' )
                {
                    // Don't read in any space characters
                    if( pData[ index ] != ' ' && pData[ index ] != ';' && pData[ index ] != '\n' )
                        data[ i++ ] = pData[ index ];

                    // If we hit these codes, it's time to save to the list
                    else if( pData[ index ] == ';' || pData[ index ] == '\n' )
                    {
                        // terminate it
                        data[ i ] = 0;

                        // Add the string to the list
                        stringList.Add( data );

                        // search for the next item in the list
                        i = 0;

                        // if we hit a comment, zoom past it
                        if( pData[ index ] == ';' )
                        {
                            while( pData[ index ] != '\n' )
                                ++index;
                        }
                    }

                    ++index;
                }
            }

            ///////////////////////////
            // Act out on an action tag
            ///////////////////////////

            // Record the MNG file name
            else if( strcmp( "</MNG_File>", tag ) == 0 )
            {
                // Seperate the file name from the unix path
                CropFileNameFromPath( data, GetPtrToMngName( Count()-1 ) );
            }

            // Record the base X Offset
            else if( strcmp( "</baseX>", tag ) == 0 )
            {                
                SetBaseXOffset( Count()-1, atoi( data ) );
            }

            // Record the base Y Offset
            else if( strcmp( "</baseY>", tag ) == 0 )
            {                
                SetBaseYOffset( Count()-1, atoi( data ) );
            }

            // Record the base Z Offset
            else if( strcmp( "</baseZ>", tag ) == 0 )
            {                
                SetBaseZOffset( Count()-1, atoi( data ) );
            }

            // Add a new record the storyboard list
            else if( strcmp( "<ScriptElement>", tag ) == 0 )
            {
                // Add a new entry
                GetStoryList( Count()-1 )->Add();

                // Get the index to save the new entries
                entryIndex = GetStoryList( Count()-1 )->Count() - 1;
            }

            // Record the X Offset
            else if( strcmp( "</X>", tag ) == 0 )
            {                
                GetStoryList( Count()-1 )->SetXOffset( entryIndex, atoi( data ) - GetBaseXOffset( Count()-1 ) );
            }

            // Record the Y Offset
            else if( strcmp( "</Y>", tag ) == 0 )
            {
                GetStoryList( Count()-1 )->SetYOffset( entryIndex, atoi( data ) - GetBaseYOffset( Count()-1 ) );
            }
            
            // Record the Z Order
            else if( strcmp( "</zOrder>", tag ) == 0 )
            {
                GetStoryList( Count()-1 )->SetZOrder( entryIndex, atoi( data ) - GetBaseZOffset( Count()-1 ) );
            }

            // Record the Frame Index
            else if( strcmp( "</FrameIndex>", tag ) == 0 )
            {
                GetStoryList( Count()-1 )->SetFrameIndex( entryIndex, atoi( data ) );
            }
            
            // Record the Frame Repeat
            else if( strcmp( "</FrameRepeat>", tag ) == 0 )
            {
                GetStoryList( Count()-1 )->SetFrameRepeat( entryIndex, atoi( data ) );
            }
            
            // Record all the Sound list items
            else if( strcmp( "</SoundList>", tag ) == 0 )
            {
                for( j = 0; j < stringList.Count(); ++j )
                {
                    // Convert all the / slashes to \ for our needs
                    ConvertToDosUnixPath( stringList.GetString( j ) );

                    // Build the path to the file
                    wsprintf( soundFile, "%s%s", soundPath, stringList.GetString( j ) );

                    // Save the path
                    GetSoundListStr( Count()-1 )->Add( soundFile );
                }    
            }

            // Add a new record the sound list
            else if( strcmp( "<Sound>", tag ) == 0 )
            {
                // Add a new entry
                GetStoryList( Count()-1 )->GetSndLst( entryIndex )->Add();

                // Get the index to save the new entries
                sndLstIndex = GetStoryList( Count()-1 )->GetSndLst( entryIndex )->Count() - 1;
            }

            // Record the sound ID
            else if( strcmp( "</SoundID>", tag ) == 0 )
            {                
                GetStoryList( Count()-1 )->GetSndLst( entryIndex )->SetID( sndLstIndex, atoi( data ) );
            }

            // Record the Volume
            else if( strcmp( "</Volume>", tag ) == 0 )
            {                
                GetStoryList( Count()-1 )->GetSndLst( entryIndex )->SetVolume( sndLstIndex, atoi( data ) );
            }

            // Record the Volume
            else if( strcmp( "</Channel>", tag ) == 0 )
            {                
                GetStoryList( Count()-1 )->GetSndLst( entryIndex )->SetChannel( sndLstIndex, atoi( data ) );
            }
            
            // Terminate the tag because it is no longer needed
            tag[0] = 0; 
        }
        else
            ++index;

    }
    while( index < size );

}   // ParseData


/************************************************************************
*    FUNCTION NAME:         Delete                                                             
*
*    DESCRIPTION:           Since this class allocats anothe object, we
*                           free up that classes data because the destructor
*                           is not called.
*
*    FUNCTION PARAMETERS:
*    Input:    int Index         - Index of the requested object
*              var2type Var2Name - Description 
*
*    Output:   returntype - Description
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*                  6/17/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TStoryListArry::Delete( int Index )
{
    PTStoryBoardArry Temp;

    // Free the allocated story boards
    Temp = (PTStoryBoardArry)GetObj( Index );

    // Free the storyboards
    Temp->StoryList.Free();

    // Free the mng bitmaps
    Temp->mng.Free();

    // Free the sound list
    Temp->soundList.Free();

    // Let the parent class handle the rest
    TObjList::Delete( Index );

}   // Delete


/************************************************************************
*    FUNCTION NAME:         ConvertToDosUnixPath                                                             
*
*    DESCRIPTION:           Convert the path to a unix or a dos path
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
*    HCR           9/27/99       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TStoryListArry::ConvertToDosUnixPath( char *path )
{
    int i, length = strlen( path );

    for( i = 0; i < length; ++i )
    {
        if( path[i] == '\\' )
            path[i] = '/';

        else if( path[i] == '/' )
            path[i] = '\\';
    }

}   // ConvertToDosUnixPath
