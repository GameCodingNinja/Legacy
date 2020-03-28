/************************************************************************
*
*    PRODUCT:         LZW Coder & Decoder
*
*    FILE NAME:       TLZW.h
*
*    DESCRIPTION:     LZW Coder & Decoder
*
*    IMPLEMENTATION:  I didn't write this code. I found it as a set of C
*                     function calls. All I did was to move it into
*                     it's own object, replace all the library calls with API
*                     calls and clean it up a bit and make use of dynamic memory. 
*                     I added some comments but most of the comments came from
*                     the original code. I'm not able to comment most of the
*                     code because I simply don't understand it.
*
*    Copyright(c) by Howard C Rosenorn
*    All Rights Reserved
*
************************************************************************/


#ifndef __TLZW_H__
#define __TLZW_H__



/************************************************************************
*
*   Various constants used to define the compression parameters.  The
*   INDEX_BIT_COUNT tells how many bits we allocate to indices into the
*   text window.  This directly determines the WINDOW_SIZE.  The
*   LENGTH_BIT_COUNT tells how many bits we allocate for the length of
*   an encode phrase. This determines the size of the look ahead buffer.
*   The TREE_ROOT is a special node in the tree that always points to
*   the root node of the binary phrase tree.  END_OF_STREAM is a special
*   index used to flag the fact that the file has been completely
*   encoded, and there is no more data.  UNUSED is the null index for
*   the tree. MOD_WINDOW() is a macro used to perform arithmetic on tree
*   indices.
*
************************************************************************/
#define INDEX_BIT_COUNT      12
#define LENGTH_BIT_COUNT     4
#define WINDOW_SIZE          ( 1 << INDEX_BIT_COUNT )
#define RAW_LOOK_AHEAD_SIZE  ( 1 << LENGTH_BIT_COUNT )
#define BREAK_EVEN           ( ( 1 + INDEX_BIT_COUNT + LENGTH_BIT_COUNT ) / 9 )
#define LOOK_AHEAD_SIZE      ( RAW_LOOK_AHEAD_SIZE + BREAK_EVEN )
#define TREE_ROOT            WINDOW_SIZE
#define END_OF_STREAM        0
#define UNUSED               0
#define MOD_WINDOW( a )      ( ( a ) & ( WINDOW_SIZE - 1 ) )
#define PACIFIER_COUNT 2047
#ifndef SEEK_END
#define SEEK_END 2
#endif
#define TREE_SIZE            WINDOW_SIZE + 1
#define BUFFER_SIZE          1000000


struct TLZW_FileHeader
{
    WORD Type;
    DWORD CompSize;
    DWORD UncompSize;
    FILETIME FileTime;
    BYTE FutureData[ 20 ];  // Future data
};

/************************************************************************
*
*   These are the two data structures used in this program.
*   The window[] array is exactly that, the window of previously seen
*   text, as well as the current look ahead text.  The tree[] structure
*   contains the binary tree of all of the strings in the window sorted
*   in order.
*
************************************************************************/

struct BIT_FILE 
{
    unsigned char mask;
    int rack;
    int pacifier_counter;
};

struct TTree
{
    int parent;
    int smaller_child;
    int larger_child;
};


class TLZW
{
protected:

    inline void _fastcall InitTree( int r );
    inline void _fastcall ContractNode( int old_node, int new_node );
    inline void _fastcall ReplaceNode( int old_node, int new_node );
    inline int _fastcall FindNextNode( int node );
    inline void _fastcall DeleteString( int p );
    inline int _fastcall AddString( int new_node, int *match_position );
    inline void _fastcall OutputBit( int bit );
    inline void _fastcall OutputBits( unsigned long code, int count );
    inline int _fastcall InputBit();
    inline unsigned long _fastcall InputBits( int bit_count );
    void Init();
    void Compress();
    void Expand();

    BYTE window[ WINDOW_SIZE ];
    TTree tree[ TREE_SIZE ];
    TLZW_FileHeader FileHeader;

    DWORD InputFilePos, InputBufferPos;
    DWORD OutputBufferPos;
	BYTE *pInputBuffer, *pOutputBuffer;
	HANDLE hInputFile, hOutputFile;
    
    BIT_FILE bit_file;
    DWORD BufferSize;

public:

    TLZW();
    ~TLZW();
    BOOL CompressFile( char *InputFileName, char *OutputFileName, int FileType = 0, DWORD BufSize = BUFFER_SIZE );
    BOOL DecompressFile( char *InputFileName, char *OutputFileName, DWORD BufSize = BUFFER_SIZE );
    BOOL DecodeToBuffer( BYTE *pSource, BYTE *pDest, DWORD BufSize );
};


typedef TLZW *PTLZW;



#endif  // __TLZW_H__