//-----------------------------------------------------
// Program: Imager
// File: complzss.h
// Copyright (c) 1998 howies funware
// Description: Program for compressing any file
//-----------------------------------------------------

#ifndef __COMPLZSS_H__
#define __COMPLZSS_H__


/*
 * Various constants used to define the compression parameters.  The
 * INDEX_BIT_COUNT tells how many bits we allocate to indices into the
 * text window.  This directly determines the WINDOW_SIZE.  The
 * LENGTH_BIT_COUNT tells how many bits we allocate for the length of
 * an encode phrase. This determines the size of the look ahead buffer.
 * The TREE_ROOT is a special node in the tree that always points to
 * the root node of the binary phrase tree.  END_OF_STREAM is a special
 * index used to flag the fact that the file has been completely
 * encoded, and there is no more data.  UNUSED is the null index for
 * the tree. MOD_WINDOW() is a macro used to perform arithmetic on tree
 * indices.
 *
 */
#define INDEX_BIT_COUNT      12
#define LENGTH_BIT_COUNT     4
//#define INDEX_BIT_COUNT      12
//#define LENGTH_BIT_COUNT     5
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


typedef struct bit_file 
{
	 HFILE file;
	 unsigned char mask;
	 int rack;
	 int pacifier_counter;
} BIT_FILE;



void Compress( HFILE input, BIT_FILE *output);
int CompressFile(char *sourceFile, char *destFile);
int DecompressFile( char *sourceFile, char *destFile );
void ExpandFile( BIT_FILE *input,HFILE output);
void wprintf( char *fmt, ... );
unsigned long GetChoice(char *fmt, ... );

void PostInfo( char *fmt, ... );
void PostWarning( char *fmt, ... );

void fatal_error( char *fmt, ... );

void ClearScreen(COLORREF Color);
int ExtractFileNameMinusExtension(char *DestinationFileName,char *SourceFileName);
int ExtractDirectoryPathName(char * PathNameDestBuff , char * FileAndPathName );//returns the string length on sucess
int CropFileExtAndCopyFileName(char * CropedNameDestBuff , char * FileName );//returns the string length on sucess
int AddExt(char * FileName , char * Extension );//returns 1 on sucess
unsigned long InputBits( BIT_FILE *bit_file, int bit_count );


BIT_FILE     *OpenInputBitFile( char *name );
BIT_FILE     *OpenOutputBitFile( char *name );
void          OutputBit( BIT_FILE *bit_file, int bit );
void          OutputBits( BIT_FILE *bit_file,
								  unsigned long code, int count );
int           InputBit( BIT_FILE *bit_file );
unsigned long InputBits( BIT_FILE *bit_file, int bit_count );
void          CloseInputBitFile( BIT_FILE *bit_file );
void          CloseOutputBitFile( BIT_FILE *bit_file );

void ExpandFile( BIT_FILE *input, HFILE output);

void print_ratios( char *input, char *output );
long file_size( char *name );

void InitTree( int r );
void ContractNode( int old_node, int new_node );
void ReplaceNode( int old_node, int new_node );
int FindNextNode( int node );
void DeleteString( int p );
int AddString( int new_node, int *match_position );


#endif  /* __COMPLZSS_H__ */