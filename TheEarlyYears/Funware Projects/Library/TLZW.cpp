/************************************************************************
*
*    PRODUCT:         LZW Coder & Decoder
*
*    FILE NAME:       TLZW.cpp
*
*    DESCRIPTION:     LZW Coder & Decoder
*
*    IMPLEMENTATION:  I didn't write this code. I found it as a set of C
*                     function calls. All I did was to move it into
*                     it's own object, replace all the library calls with API
*                     calls and clean it up a bit and make use of dynamic memory. 
*                     I added some comments but most of the comments came from
*                     the original code. I'm not able to comment most of the
*                     because I simply don't understand it.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/

                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size                             
#define WIN32_LEAN_AND_MEAN  // Barebones the windows.h header file. Doesn't reduce code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.

#include "TLZW.h"            // Header file for this *.cpp file.


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
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TLZW::TLZW()
{
    hInputFile = NULL;
    hOutputFile = NULL;
    BufferSize = 0;
}

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
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

TLZW::~TLZW()
{

}


/************************************************************************
*    FUNCTION NAME:         CompressFile                                                             
*
*    DESCRIPTION:           Compresses the file
*
*    FUNCTION PARAMETERS:
*    Input:    char *InputFileName  - Path to file to be compressed
*              char *OutputFileName - Path to file to recieve the
*                                     compressed data
*              int FileType         - User defined file type. For the purpose
*                                     of setting a value you can check later.
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TLZW::CompressFile( char *InputFileName, char *OutputFileName, int FileType, DWORD BufSize )
{
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesRead, bytesWritten;
    char ErrorMsg[MAX_PATH*2];
    BOOL Result = FALSE;

    BufferSize = BufSize;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    Init(); // Init memory

    // Open the file on the hard drive
    hInputFile = CreateFile( InputFileName, GENERIC_READ, FILE_SHARE_READ,
                           &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hInputFile == INVALID_HANDLE_VALUE )
    {
        wsprintf( ErrorMsg, "Error opening file: %s", InputFileName );
        MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
        goto FREE_MEMORY;
    }

    // Create the file on the hard drive to write the compressed data
    hOutputFile = CreateFile( OutputFileName, GENERIC_WRITE, FILE_SHARE_WRITE,
                           &FileSecurity, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hOutputFile == INVALID_HANDLE_VALUE )
    {
        wsprintf( ErrorMsg, "Error opening file: %s", OutputFileName );
        MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
        goto FREE_MEMORY;
    }

    // Get the file size of the bitmap
    FileHeader.UncompSize = GetFileSize( hInputFile, NULL );
    // Add in the file type
    FileHeader.Type = (WORD)FileType;
    // Save the file time of the source file so that it can be compared later
    GetFileTime( hInputFile, NULL, NULL, &FileHeader.FileTime );

    // Write the file header now
    WriteFile( hOutputFile, &FileHeader, sizeof(TLZW_FileHeader), &bytesWritten, NULL );

    // Allocate memory buffers input and output buffers
    pInputBuffer  = new BYTE[ BufferSize ];
    pOutputBuffer = new BYTE[ BufferSize ];

    // Make sure we have our memory
    if( !pInputBuffer || !pOutputBuffer )
    {
        wsprintf( ErrorMsg, "Unable to allocate memory for bitmap file: %s", InputFileName );
        MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
        goto FREE_MEMORY;
    }

    // Read contents of file into allocated memory
    ReadFile( hInputFile, pInputBuffer, BufferSize, &bytesRead, NULL );

    if( !bytesRead )
    {
        wsprintf( ErrorMsg, "Unable to load file: %s", InputFileName );
        MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
        goto FREE_MEMORY;
    }

    // Compress it
    Compress();


    // If the buffer is holding data, write it to the hard drive
    if( OutputBufferPos )
        WriteFile( hOutputFile, pOutputBuffer, OutputBufferPos, &bytesWritten, NULL );


    if ( bit_file.mask != 0x80 )
    {
        WriteFile( hOutputFile, &bit_file.rack, 1, &bytesWritten, NULL );
        ++FileHeader.CompSize;
    }

    // Write the file header again to update the header information
    SetFilePointer( hOutputFile, 0, 0, FILE_BEGIN );
    WriteFile( hOutputFile, &FileHeader, sizeof(TLZW_FileHeader), &bytesWritten, NULL );


    Result = TRUE;

FREE_MEMORY:;

    // Close the file handles
    if( hInputFile )
        CloseHandle( hInputFile );

    if( hOutputFile )
        CloseHandle( hOutputFile );

    if( pInputBuffer )
        delete [] pInputBuffer;

    if( pOutputBuffer )
        delete [] pOutputBuffer;


    return Result;

}   // CompressFile


/************************************************************************
*    FUNCTION NAME:         DecompressFile                                                             
*
*    DESCRIPTION:           Uncompresses the file
*
*    FUNCTION PARAMETERS:
*    Input:    char *InputFileName  - Path to file to be DecompressFile
*              char *OutputFileName - Path to file to recieve the
*                                     uncompressed data
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TLZW::DecompressFile( char *InputFileName, char *OutputFileName, DWORD BufSize )
{
    SECURITY_ATTRIBUTES FileSecurity;
    unsigned long bytesRead, bytesWritten;
    char ErrorMsg[MAX_PATH*2];
    BOOL Result = FALSE;

    BufferSize = BufSize;

    FileSecurity.nLength=sizeof(SECURITY_ATTRIBUTES);
    FileSecurity.lpSecurityDescriptor=NULL;
    FileSecurity.bInheritHandle=FALSE;

    Init(); // Init memory

    // Open the file on the hard drive
    hInputFile = CreateFile( InputFileName, GENERIC_READ, FILE_SHARE_READ,
                           &FileSecurity, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hInputFile == INVALID_HANDLE_VALUE )
    {
        wsprintf( ErrorMsg, "Error opening file: %s", InputFileName );
        MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
        goto FREE_MEMORY;
    }

    // Create the file on the hard drive to write the compressed data
    hOutputFile = CreateFile( OutputFileName, GENERIC_WRITE, FILE_SHARE_WRITE,
                           &FileSecurity, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );

    // Chech to see that we have a valid file handle
    if( hOutputFile == INVALID_HANDLE_VALUE )
    {
        wsprintf( ErrorMsg, "Error opening file: %s", OutputFileName );
        MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
        goto FREE_MEMORY;
    }

    // Read the file header
    ReadFile( hInputFile, &FileHeader, sizeof(TLZW_FileHeader), &bytesRead, NULL );

    // Allocate memory buffers input and output buffers
    pInputBuffer  = new BYTE[ BufferSize ];
    pOutputBuffer = new BYTE[ BufferSize ];

    // Make sure we have our memory
    if( !pInputBuffer || !pOutputBuffer )
    {
        wsprintf( ErrorMsg, "Unable to allocate memory for buffers: %s", InputFileName );
        MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
        goto FREE_MEMORY;
    }

    // Read contents of file into allocated memory
    ReadFile( hInputFile, pInputBuffer, BufferSize, &bytesRead, NULL );

    if( !bytesRead )
    {
        wsprintf( ErrorMsg, "Unable to load file: %s", InputFileName );
        MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
        goto FREE_MEMORY;
    }

    Expand();

    // If the buffer is holding data, write it to the hard drive
    if( OutputBufferPos )
        WriteFile( hOutputFile, pOutputBuffer, OutputBufferPos, &bytesWritten, NULL );

    Result = TRUE;

FREE_MEMORY:;

    // Close the file handles
    if( hInputFile )
        CloseHandle( hInputFile );

    if( hOutputFile )
        CloseHandle( hOutputFile );

    if( pInputBuffer )
        delete [] pInputBuffer;

    if( pOutputBuffer )
        delete [] pOutputBuffer;

    return Result;

}   // DecompressFile


/************************************************************************
*    FUNCTION NAME:         DecodeToBuffer                                                             
*
*    DESCRIPTION:           Decodes from a buffer to a buffer
*
*    FUNCTION PARAMETERS:
*    Input:    BYTE *pSource - Source buffer
*              BYTE *pDest   - Destination buffer
*              DWORD BufSize - Size of the destination buffer
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

BOOL TLZW::DecodeToBuffer( BYTE *pSource, BYTE *pDest, DWORD BufSize )
{
    // Make sure the buffer size is just a little bigger
    // than needed because otherwise it will
    BufferSize = BufSize;
    Init(); // Init memory

    // Init to the internal buffer pointers
    pInputBuffer  = pSource;
    pOutputBuffer = pDest;

    Expand();

    return TRUE;

}   // DecodeToBuffer


/************************************************************************
*    FUNCTION NAME:         Init                                                             
*
*    DESCRIPTION:           Inits data
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TLZW::Init()
{
    // init the bitfile structure
    bit_file.rack = 0;
    bit_file.mask = 0x80;
    bit_file.pacifier_counter = 0;

    ZeroMemory( &window, sizeof(window) );
    ZeroMemory( &tree, sizeof(tree) );
    ZeroMemory( &FileHeader, sizeof(FileHeader) );

    InputFilePos = 0;
    OutputBufferPos = 0;
    InputBufferPos = 0;

    pInputBuffer = NULL;
    pOutputBuffer = NULL;

    hInputFile = NULL;
    hOutputFile = NULL;

}   // Free


/************************************************************************
*    FUNCTION NAME:         Compress                                                             
*
*    DESCRIPTION:
*    This is the compression routine.  It has to first load up the look
*    ahead buffer, then go into the main compression loop.  The main loop
*    decides whether to output a single character or an index/length
*    token that defines a phrase.  Once the character or phrase has been
*    sent out, another loop has to run.  The second loop reads in new
*    characters, deletes the strings that are overwritten by the new
*    character, then adds the strings that are created by the new
*    character.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TLZW::Compress()
{
    int i;
    int c;
    int look_ahead_bytes;
    int current_position;
    int replace_count;
    int match_length;
    int match_position;
    unsigned long bytesRead;

    current_position = 1;
    for ( i = 0 ; i < LOOK_AHEAD_SIZE; ++i )
    {
        if( InputFilePos < FileHeader.UncompSize )
        {
            if( InputBufferPos >= BufferSize && hInputFile )
            {
                ReadFile( hInputFile, pInputBuffer, BufferSize, &bytesRead, NULL );
                InputBufferPos = 0;
            }

            c = pInputBuffer[InputBufferPos];
            ++InputFilePos;
            ++InputBufferPos;
        }
        else
            break;

        window[ current_position + i ] = (unsigned char) c;
    }

    look_ahead_bytes = i;
    InitTree( current_position );
    match_length = 0;
    match_position = 0;

    while ( look_ahead_bytes > 0 )
    {
        if ( match_length > look_ahead_bytes )
            match_length = look_ahead_bytes;

        if ( match_length <= BREAK_EVEN )
        {
            replace_count = 1;
            OutputBit( 1 );
            OutputBits( (unsigned long) window[ current_position ], 8 );
        }
        else
        {
            OutputBit( 0 );
            OutputBits( (unsigned long) match_position, INDEX_BIT_COUNT );
            OutputBits( (unsigned long) ( match_length - ( BREAK_EVEN + 1 ) ),
            LENGTH_BIT_COUNT );
            replace_count = match_length;
        }

        for ( i = 0 ; i < replace_count ; i++ ) {
            DeleteString( MOD_WINDOW( current_position + LOOK_AHEAD_SIZE ) );

        if( InputFilePos >= FileHeader.UncompSize )
            look_ahead_bytes--;
        else
        {
            if( InputBufferPos >= BufferSize && hInputFile )
            {
                ReadFile( hInputFile, pInputBuffer, BufferSize, &bytesRead, NULL );
                InputBufferPos = 0;
            }

            c = pInputBuffer[InputBufferPos];
            ++InputFilePos;
            ++InputBufferPos;

            window[ MOD_WINDOW( current_position + LOOK_AHEAD_SIZE ) ] = (unsigned char) c;
        }

        current_position = MOD_WINDOW( current_position + 1 );

        if ( look_ahead_bytes )
            match_length = AddString( current_position, &match_position );
        }
    }

    OutputBit( 0 );
    OutputBits( (unsigned long) END_OF_STREAM, INDEX_BIT_COUNT );

}   // Compress


/************************************************************************
*    FUNCTION NAME:         Expand                                                             
*
*    DESCRIPTION:
*    This is the expansion routine for the LZSS algorithm.  All it has
*    to do is read in flag bits, decide whether to read in a character or
*    a index/length pair, and take the appropriate action.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void TLZW::Expand()
{
    int i;
    int current_position;
    char c;
    int match_length;
    int match_position;
    unsigned long bytesWritten;

    current_position = 1;

    for ( ; ; )
    {
        if ( InputBit() )
        {
            //c = (int) InputBits( 8 );  was
            c = (char) InputBits( 8 );

            // If the buffer is full, empty it out and fill it up again
            if( OutputBufferPos >= BufferSize && hOutputFile )
            {
                WriteFile( hOutputFile, pOutputBuffer, BufferSize, &bytesWritten, NULL );
                OutputBufferPos = 0;
            }

            pOutputBuffer[OutputBufferPos] = c;
            ++OutputBufferPos;

            window[ current_position ] = (unsigned char) c;
            current_position = MOD_WINDOW( current_position + 1 );
        }
        else
        {
            match_position = (int) InputBits( INDEX_BIT_COUNT );

            if ( match_position == END_OF_STREAM )
                break;

            match_length = (int) InputBits( LENGTH_BIT_COUNT );
            match_length += BREAK_EVEN;

            for ( i = 0 ; i <= match_length; ++i )
            {
                c = window[ MOD_WINDOW( match_position + i ) ];

                // If the buffer is full, empty it out and fill it up again
                if( OutputBufferPos >= BufferSize && hOutputFile )
                {
                    WriteFile( hOutputFile, pOutputBuffer, BufferSize, &bytesWritten, NULL );
                    OutputBufferPos = 0;
                }

                pOutputBuffer[OutputBufferPos] = c;
                ++OutputBufferPos;

                window[ current_position ] = (unsigned char) c;
                current_position = MOD_WINDOW( current_position + 1 );
            }
        }
    }

}   // Expand


/************************************************************************
*    FUNCTION NAME:         OutputBit                                                             
*
*    DESCRIPTION:
*    
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall TLZW::OutputBit( int bit )
{
    unsigned long bytesWritten;

    if ( bit )
        bit_file.rack |= bit_file.mask;

    bit_file.mask >>= 1;

    if ( bit_file.mask == 0 )
    {
        // If the buffer is full, empty it out and fill it up again
        if( OutputBufferPos >= BufferSize && hOutputFile )
        {
            WriteFile( hOutputFile, pOutputBuffer, BufferSize, &bytesWritten, NULL );
            OutputBufferPos = 0;
        }

        //pOutputBuffer[OutputBufferPos] = bit_file.rack;  was
        pOutputBuffer[OutputBufferPos] = (BYTE)bit_file.rack;
        ++OutputBufferPos;
        ++FileHeader.CompSize;

        bit_file.rack = 0;
        bit_file.mask = 0x80;
    }
}   // OutputBit


/************************************************************************
*    FUNCTION NAME:         OutputBits                                                             
*
*    DESCRIPTION:
*    
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall TLZW::OutputBits( unsigned long code, int count )
{
    unsigned long mask;
    unsigned long bytesWritten;

    mask = 1L << ( count - 1 );

    while ( mask != 0)
    {
        if ( mask & code )
            bit_file.rack |= bit_file.mask;

        bit_file.mask >>= 1;
        if ( bit_file.mask == 0 )
        {
            // If the buffer is full, empty it out and fill it up again
            if( OutputBufferPos >= BufferSize && hOutputFile )
            {
                WriteFile( hOutputFile, pOutputBuffer, BufferSize, &bytesWritten, NULL );
                OutputBufferPos = 0;
            }

            //pOutputBuffer[OutputBufferPos] = bit_file.rack;  was
            pOutputBuffer[OutputBufferPos] = (BYTE)bit_file.rack;
            ++OutputBufferPos;
            ++FileHeader.CompSize;

            bit_file.rack = 0;
            bit_file.mask = 0x80;
        }

        mask >>= 1;
    }
}   // OutputBits


/************************************************************************
*    FUNCTION NAME:         InputBit                                                             
*
*    DESCRIPTION:
*    
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int _fastcall TLZW::InputBit()
{
    int value;
    unsigned long bytesRead;

    if ( bit_file.mask == 0x80 )
    {
        if( InputBufferPos >= BufferSize && hInputFile )
        {
            ReadFile( hInputFile, pInputBuffer, BufferSize, &bytesRead, NULL );
            InputBufferPos = 0;
        }

        bit_file.rack = pInputBuffer[InputBufferPos];
        ++InputFilePos;
        ++InputBufferPos;
    }

    value = bit_file.rack & bit_file.mask;
    bit_file.mask >>= 1;

    if ( bit_file.mask == 0 )
        bit_file.mask = 0x80;

    return( value ? 1 : 0 );

}   // InputBit


/************************************************************************
*    FUNCTION NAME:         InputBits
*
*    DESCRIPTION:
*    
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

unsigned long _fastcall TLZW::InputBits( int bit_count )
{
    unsigned long mask, bytesRead;
    unsigned long return_value;

    mask = 1L << ( bit_count - 1 );
    return_value = 0;

    while ( mask != 0)
    {
        if ( bit_file.mask == 0x80 )
        {
            if( InputBufferPos >= BufferSize && hInputFile )
            {
                ReadFile( hInputFile, pInputBuffer, BufferSize, &bytesRead, NULL );
                InputBufferPos = 0;
            }

            bit_file.rack = pInputBuffer[InputBufferPos];
            ++InputFilePos;
            ++InputBufferPos;
        }

        if ( bit_file.rack & bit_file.mask )
            return_value |= mask;

        mask >>= 1;
        bit_file.mask >>= 1;

        if ( bit_file.mask == 0 )
            bit_file.mask = 0x80;
    }
    return( return_value );

}   // InputBits


/************************************************************************
*    FUNCTION NAME:         InitTree                                                             
*
*    DESCRIPTION:
*    Since the tree is static data, it comes up with every node
*    initialized to 0, which is good, since 0 is the UNUSED code.
*    However, to make the tree really usable, a single phrase has to be
*    added to the tree so it has a root node.  That is done right here.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall TLZW::InitTree( int r )
{
    tree[ TREE_ROOT ].larger_child = r;
    tree[ r ].parent = TREE_ROOT;
    tree[ r ].larger_child = UNUSED;
    tree[ r ].smaller_child = UNUSED;

}   // InitTree


/************************************************************************
*    FUNCTION NAME:         ContractNode                                                             
*
*    DESCRIPTION:
*    This routine is used when a node is being deleted.  The link to
*    its descendant is broken by pulling the descendant in to overlay
*    the existing link.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall TLZW::ContractNode( int old_node, int new_node )
{
    tree[ new_node ].parent = tree[ old_node ].parent;

    if ( tree[ tree[ old_node ].parent ].larger_child == old_node )
        tree[ tree[ old_node ].parent ].larger_child = new_node;
    else
        tree[ tree[ old_node ].parent ].smaller_child = new_node;

    tree[ old_node ].parent = UNUSED;

}   // ContractNode


/************************************************************************
*    FUNCTION NAME:         ReplaceNode                                                             
*
*    DESCRIPTION:
*    This routine is used when a node is being deleted.  The link to
*    its descendant is broken by pulling the descendant in to overlay
*    the existing link.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall TLZW::ReplaceNode( int old_node, int new_node )
{
    int parent;

    parent = tree[ old_node ].parent;

    if ( tree[ parent ].smaller_child == old_node )
        tree[ parent ].smaller_child = new_node;
    else
        tree[ parent ].larger_child = new_node;

    tree[ new_node ] = tree[ old_node ];
    tree[ tree[ new_node ].smaller_child ].parent = new_node;
    tree[ tree[ new_node ].larger_child ].parent = new_node;
    tree[ old_node ].parent = UNUSED;

}   // ReplaceNode


/************************************************************************
*    FUNCTION NAME:         FindNextNode                                                             
*
*    DESCRIPTION:
*    This routine is used to find the next smallest node after the node
*    argument.  It assumes that the node has a smaller child.  We find
*    the next smallest child by going to the smaller_child node, then
*    going to the end of the larger_child descendant chain.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int _fastcall TLZW::FindNextNode( int node )
{
    int next;

    next = tree[ node ].smaller_child;

    while ( tree[ next ].larger_child != UNUSED )
        next = tree[ next ].larger_child;

    return( next );

}   // FindNextNode


/************************************************************************
*    FUNCTION NAME:         DeleteString                                                             
*
*    DESCRIPTION:
*    This routine performs the classic binary tree deletion algorithm.
*    If the node to be deleted has a null link in either direction, we
*    just pull the non-null link up one to replace the existing link.
*    If both links exist, we instead delete the next link in order, which
*    is guaranteed to have a null link, then replace the node to be deleted
*    with the next link.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

void _fastcall TLZW::DeleteString( int p )
{
    int  replacement;

    if ( tree[ p ].parent == UNUSED )
        return;

    if ( tree[ p ].larger_child == UNUSED )
        ContractNode( p, tree[ p ].smaller_child );
    else if ( tree[ p ].smaller_child == UNUSED )
        ContractNode( p, tree[ p ].larger_child );
    else
    {
        replacement = FindNextNode( p );
        DeleteString( replacement );
        ReplaceNode( p, replacement );
    }

}   // DeleteString


/************************************************************************
*    FUNCTION NAME:         AddString                                                             
*
*    DESCRIPTION:
*    This where most of the work done by the encoder takes place.  This
*    routine is responsible for adding the new node to the binary tree.
*    It also has to find the best match among all the existing nodes in
*    the tree, and return that to the calling routine.  To make matters
*    even more complicated, if the new_node has a duplicate in the tree,
*    the old_node is deleted, for reasons of efficiency.
*
*    FUNCTION PARAMETERS:
*    Input:    var1type Var1Name - Description
*              var2type Var2Name - Description
*
*    Output:   returntype - True or False depending on the outcome
*
*    HISTORY:
*    Name          Date          Comment
*    -------------------------------------------------------------------
*    h rosenorn    03/28/00       Function Created
*    -------------------------------------------------------------------
*    UserID        MM/DD/YY      Description of any changes
************************************************************************/

int _fastcall TLZW::AddString( int new_node, int *match_position )
{
    int i;
    int test_node;
    int delta;
    int match_length;
    int *child;

    if ( new_node == END_OF_STREAM )
        return( 0 );

    test_node = tree[ TREE_ROOT ].larger_child;
    match_length = 0;

    for ( ; ; )
    {
        for ( i = 0 ; i < LOOK_AHEAD_SIZE ; i++ ) 
        {
            delta = window[ MOD_WINDOW( new_node + i ) ] -
              window[ MOD_WINDOW( test_node + i ) ];

            if ( delta != 0 )
                break;
        }

        if ( i >= match_length )
        {
            match_length = i;
            *match_position = test_node;

            if ( match_length >= LOOK_AHEAD_SIZE ) 
            {
                ReplaceNode( test_node, new_node );
                return( match_length );
            }
        }

        if ( delta >= 0 )
            child = &tree[ test_node ].larger_child;
        else
            child = &tree[ test_node ].smaller_child;

        if ( *child == UNUSED )
        {
            *child = new_node;
            tree[ new_node ].parent = test_node;
            tree[ new_node ].larger_child = UNUSED;
            tree[ new_node ].smaller_child = UNUSED;

            return( match_length );
        }

    test_node = *child;

    }
}   // AddString