//-----------------------------------------------------
// Program: Imager
// File: complzss.cpp
// Copyright (c) 1998 howies funware
// Description: Program for compressing any file
//-----------------------------------------------------

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <STDARG.H>

#include "complzss.h"

char textBuff[128];
char UtilityBuffer[512];
char *CompressionName = "LZSS Encoder";
char *Usage           = "in-file out-file\n\n";
unsigned int comptype, x, y, dataSize;

/*
 * These are the two global data structures used in this program.
 * The window[] array is exactly that, the window of previously seen
 * text, as well as the current look ahead text.  The tree[] structure
 * contains the binary tree of all of the strings in the window sorted
 * in order.
*/

unsigned char far window[ WINDOW_SIZE ];

struct {
	 int parent;
	 int smaller_child;
	 int larger_child;
} tree[ WINDOW_SIZE + 1 ];


void FilePrintBinary( FILE *file, unsigned int code, int bits );



int CompressFile( char *sourceFile,char *destFile )
{
	 OFSTRUCT  OFData;
	 BIT_FILE *output;
	 HFILE  input;
	 char ErrorMsg[MAX_PATH*2];
	 /*
	 input = OpenFile( sourceFile,&OFData, OF_READ );
	 if ( input == HFILE_ERROR  )
	 {
		  wsprintf( ErrorMsg, "Error opening %s for input\n", sourceFile );
		  MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
		  return 0;
	  }
			_lread(input , &comptype, 2);
			_lread(input , &x, 2);
			_lread(input , &y, 2);
			_lread(input , &dataSize, 2);

	 _lclose( input );
	 */

	 input = OpenFile( sourceFile,&OFData, OF_READ );
	 if ( input == HFILE_ERROR  )
	 {
		  wsprintf( ErrorMsg, "Error opening %s for input\n", sourceFile );
		  MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
		  return 0;
	  }

	 output = OpenOutputBitFile( destFile );
	 if ( output == 0 )
	 {
		  wsprintf( "Error opening %s for output\n",destFile );
		  MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
		  return 0;
	  }

	 Compress( input, output);
	 CloseOutputBitFile( output );
	 _lclose( input );

	 /*
	 HFILE expandoutput;
	 BIT_FILE *expandinput;
	 expandinput = OpenInputBitFile(destFile );

	 expandoutput = OpenFile( "uncomp.gex",&OFData, OF_CREATE|OF_READWRITE );
	 if ( expandoutput == HFILE_ERROR  )
	 {
		  wsprintf( "Error opening %s for output\n", destFile );
		  MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
		  return 0;
	  }

	 ExpandFile( expandinput, expandoutput);

	 _lclose( expandoutput );
	 CloseInputBitFile( expandinput );	*/

	 return( 0 );
}


int DecompressFile( char *sourceFile, char *destFile )
{
     char ErrorMsg[MAX_PATH*2];
	 HFILE expandoutput;
	 BIT_FILE *expandinput;
	 OFSTRUCT  OFData;

     expandinput = OpenInputBitFile( sourceFile );
	 if( expandinput == NULL )
	 {
		  wsprintf( "Error opening %s for output\n", sourceFile );
		  MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
		  return 0;
	 }

	 expandoutput = OpenFile( destFile, &OFData, OF_CREATE|OF_READWRITE );
	 if( expandoutput == HFILE_ERROR  )
	 {
		  wsprintf( "Error opening %s for output\n", destFile );
		  MessageBox( GetActiveWindow(), ErrorMsg, "File Error", MB_ICONINFORMATION );
		  return 0;
	 }

	 ExpandFile( expandinput, expandoutput);

	 _lclose( expandoutput );
	 CloseInputBitFile( expandinput );

	return 1;
}


//==========================================================================//

/*
 * This routine just wants to print out the usage message that is
 * called for when the program is run with no parameters.  The first
 * part of the Usage statement is supposed to be just the program
 * name.  argv[ 0 ] generally holds the fully qualified path name
 * of the program being run.  I make a half-hearted attempt to strip
 * out that path info and file extension before printing it.  It should
 * get the general idea across.
 */
void usage_exit( char *prog_name )
{
	 char *short_name;
	 char *extension;

	 short_name = strrchr( prog_name, '\\' );
	 if ( short_name == NULL )
		  short_name = strrchr( prog_name, '/' );
	 if ( short_name == NULL )
		  short_name = strrchr( prog_name, ':' );
    if ( short_name != NULL )
        short_name++;
    else
		  short_name = prog_name;
    extension = strrchr( short_name, '.' );
    if ( extension != NULL )
		  *extension = '\0';
    //printf( "\nUsage:  %s %s\n", short_name, Usage );
	 exit( 0 );
}

//==========================================================================//

/*
 * This routine is used by main to print out get the size of a file after
 * it has been closed.  It does all the work, and returns a long.  The
 * main program gets the file size for the plain text, and the size of
 * the compressed file, and prints the ratio.
 */

long file_size( char *name )
{
	 long eof_ftell;
	 FILE *file;

	 file = fopen( name, "r" );
	 if ( file == NULL )
        return( 0L );
	 fseek( file, 0L, SEEK_END );
	 eof_ftell = ftell( file );
    fclose( file );
    return( eof_ftell );
}

//==========================================================================//

/*
 * This routine prints out the compression ratios after the input
 * and output files have been closed.
 */

void print_ratios( char *input, char *output )
{
	 long input_size;
    long output_size;
	 int ratio;

	 input_size = file_size( input );
	 if ( input_size == 0 )
        input_size = 1;
	 output_size = file_size( output );
    ratio = 100 - (int) ( output_size * 100L / input_size );
	 printf( "\nInput bytes:        %ld\n", input_size );
    printf( "Output bytes:       %ld\n", output_size );
	 if ( output_size == 0 )
        output_size = 1;
	 printf( "Compression ratio:  %d%%\n", ratio );
}

/************************** End of MAIN-C.C **************************/

/************************** Start of LZSS.C *************************
 *
 * This is the LZSS module, which implements an LZ77 style compression
 * algorithm.  As iplemented here it uses a 12 bit index into the sliding
 * window, and a 4 bit length, which is adjusted to reflect phrase lengths
 * of between 2 and 17 bytes.
 */



/*
 * Since the tree is static data, it comes up with every node
 * initialized to 0, which is good, since 0 is the UNUSED code.
 * However, to make the tree really usable, a single phrase has to be
 * added to the tree so it has a root node.  That is done right here.
*/

void InitTree( int r )
{
	 tree[ TREE_ROOT ].larger_child = r;
	 tree[ r ].parent = TREE_ROOT;
	 tree[ r ].larger_child = UNUSED;
    tree[ r ].smaller_child = UNUSED;
}

//==========================================================================//

/*
 * This routine is used when a node is being deleted.  The link to
 * its descendant is broken by pulling the descendant in to overlay
 * the existing link.
 */

void ContractNode( int old_node, int new_node )
{
	 tree[ new_node ].parent = tree[ old_node ].parent;
	 if ( tree[ tree[ old_node ].parent ].larger_child == old_node )
        tree[ tree[ old_node ].parent ].larger_child = new_node;
	 else
        tree[ tree[ old_node ].parent ].smaller_child = new_node;
    tree[ old_node ].parent = UNUSED;
}

//==========================================================================//

/*
 * This routine is also used when a node is being deleted.  However,
 * in this case, it is being replaced by a node that was not previously
 * in the tree.
 */

void ReplaceNode( int old_node, int new_node )
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
}

//==========================================================================//

/*
 * This routine is used to find the next smallest node after the node
 * argument.  It assumes that the node has a smaller child.  We find
 * the next smallest child by going to the smaller_child node, then
 * going to the end of the larger_child descendant chain.
*/

int FindNextNode( int node )
{
	 int next;

    next = tree[ node ].smaller_child;
	 while ( tree[ next ].larger_child != UNUSED )
		  next = tree[ next ].larger_child;
    return( next );
}

//==========================================================================//

/*
 * This routine performs the classic binary tree deletion algorithm.
 * If the node to be deleted has a null link in either direction, we
 * just pull the non-null link up one to replace the existing link.
 * If both links exist, we instead delete the next link in order, which
 * is guaranteed to have a null link, then replace the node to be deleted
 * with the next link.
 */

void DeleteString( int p )
{
	 int  replacement;

    if ( tree[ p ].parent == UNUSED )
		  return;
    if ( tree[ p ].larger_child == UNUSED )
		  ContractNode( p, tree[ p ].smaller_child );
	 else if ( tree[ p ].smaller_child == UNUSED )
		  ContractNode( p, tree[ p ].larger_child );
	 else {
		  replacement = FindNextNode( p );
		  DeleteString( replacement );
        ReplaceNode( p, replacement );
    }
}

//==========================================================================//

/*
 * This where most of the work done by the encoder takes place.  This
 * routine is responsible for adding the new node to the binary tree.
 * It also has to find the best match among all the existing nodes in
 * the tree, and return that to the calling routine.  To make matters
 * even more complicated, if the new_node has a duplicate in the tree,
 * the old_node is deleted, for reasons of efficiency.
 */

int AddString( int new_node, int *match_position )
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
	 for ( ; ; ) {
		  for ( i = 0 ; i < LOOK_AHEAD_SIZE ; i++ ) {
            delta = window[ MOD_WINDOW( new_node + i ) ] -
						  window[ MOD_WINDOW( test_node + i ) ];
				if ( delta != 0 )
					 break;
        }
		  if ( i >= match_length ) {
				match_length = i;
            *match_position = test_node;
            if ( match_length >= LOOK_AHEAD_SIZE ) {
                ReplaceNode( test_node, new_node );
					 return( match_length );
				}
        }
		  if ( delta >= 0 )
            child = &tree[ test_node ].larger_child;
        else
            child = &tree[ test_node ].smaller_child;
		  if ( *child == UNUSED ) {
				*child = new_node;
				tree[ new_node ].parent = test_node;
				tree[ new_node ].larger_child = UNUSED;
				tree[ new_node ].smaller_child = UNUSED;
            return( match_length );
        }
		  test_node = *child;
	 }
}

//==========================================================================//

/*
 * This is the compression routine.  It has to first load up the look
 * ahead buffer, then go into the main compression loop.  The main loop
 * decides whether to output a single character or an index/length
 * token that defines a phrase.  Once the character or phrase has been
 * sent out, another loop has to run.  The second loop reads in new
 * characters, deletes the strings that are overwritten by the new
 * character, then adds the strings that are created by the new
 * character.
 */
void Compress( HFILE  input, BIT_FILE *output)
{
	 int i;
	 int c;
	 int look_ahead_bytes;
	 int current_position;
	 int replace_count;
	 int match_length;
	 int match_position;

	//PostInfo("WINDOW_SIZE = %lu",WINDOW_SIZE);
	i=0;
	while(i< WINDOW_SIZE )
	{
		window[i++]=0;
	}
	i=0;
	while(i< WINDOW_SIZE + 1 )
	{
		tree[i].parent=0;
		tree[i].smaller_child=0;
		tree[i].larger_child=0;
		i++;
	}

		  _lread(input , &c, 1);
		  _lread(input , &c, 1);
		  _lread(input , &c, 1);
		  _lread(input , &c, 1);
		  _lread(input , &c, 1);
		  _lread(input , &c, 1);
		  _lread(input , &c, 1);
		  _lread(input , &c, 1);

	 current_position = 1;
	 for ( i = 0 ; i < LOOK_AHEAD_SIZE ; i++ ) {
		  if(!_lread(input , &c, 1))
				break;
		  window[ current_position + i ] = (unsigned char) c;
	 }
	 look_ahead_bytes = i;
	 InitTree( current_position );
	 match_length = 0;
	 match_position = 0;
	 while ( look_ahead_bytes > 0 ) {
		  if ( match_length > look_ahead_bytes )
				match_length = look_ahead_bytes;
		  if ( match_length <= BREAK_EVEN ) {
				replace_count = 1;
				OutputBit( output, 1 );
				OutputBits( output,
								(unsigned long) window[ current_position ], 8 );
		  } else {
				OutputBit( output, 0 );
				OutputBits( output,
								(unsigned long) match_position, INDEX_BIT_COUNT );
				OutputBits( output,
								(unsigned long) ( match_length - ( BREAK_EVEN + 1 ) ),
								LENGTH_BIT_COUNT );
				replace_count = match_length;
		  }
		  for ( i = 0 ; i < replace_count ; i++ ) {
				DeleteString( MOD_WINDOW( current_position + LOOK_AHEAD_SIZE ) );
				if(!_lread(input , &c, 1))
					 look_ahead_bytes--;
				else
					 window[ MOD_WINDOW( current_position + LOOK_AHEAD_SIZE ) ]
								= (unsigned char) c;
				current_position = MOD_WINDOW( current_position + 1 );
				if ( look_ahead_bytes )
					 match_length = AddString( current_position, &match_position );
		  }
	 };
	 OutputBit( output, 0 );
	 OutputBits( output, (unsigned long) END_OF_STREAM, INDEX_BIT_COUNT );
}

//==========================================================================//

/*
 * This is the expansion routine for the LZSS algorithm.  All it has
 * to do is read in flag bits, decide whether to read in a character or
 * a index/length pair, and take the appropriate action.
*/
void ExpandFile( BIT_FILE *input,HFILE output)
{
	 int i;
	 int current_position;
	 //unsigned char c;
    char c;
	 int match_length;
	 int match_position;

	 current_position = 1;
	 for ( ; ; ) {
		  if ( InputBit( input ) ) {
				c = (int) InputBits( input, 8 );
				_lwrite(output  , &c, 1);

				window[ current_position ] = (unsigned char) c;
				current_position = MOD_WINDOW( current_position + 1 );
		  } else
		  {
				match_position = (int) InputBits( input, INDEX_BIT_COUNT );
				if ( match_position == END_OF_STREAM )
					 break;
				match_length = (int) InputBits( input, LENGTH_BIT_COUNT );
				match_length += BREAK_EVEN;
				for ( i = 0 ; i <= match_length ; i++ ) {
					 c = window[ MOD_WINDOW( match_position + i ) ];
					 _lwrite(output  , &c, 1);
					 window[ current_position ] = (unsigned char) c;
					 current_position = MOD_WINDOW( current_position + 1 );
				}
		  }
	 }
}

//==========================================================================//

/************************** Start of BITIO.C *************************
 *
 * This utility file contains all of the routines needed to impement
 * bit oriented routines under either ANSI or K&R C.  It needs to be
 * linked with every program used in the entire book.
 *
 */

BIT_FILE *OpenOutputBitFile( char *name )
{
	 BIT_FILE *bit_file;
	 OFSTRUCT  OFData;

	 bit_file = (BIT_FILE *) calloc( 1, sizeof( BIT_FILE ) );
	 if ( bit_file == NULL )
		  return( bit_file );
	 //bit_file->file = fopen( name, "wb" );
	 bit_file->file = OpenFile( name,&OFData, OF_CREATE|OF_READWRITE );
	 if ( bit_file->file == HFILE_ERROR  )
		  return 0;
	  comptype=130;
	 //_lwrite(bit_file->file ,(unsigned char *) &comptype, 2);
	 //_lwrite(bit_file->file , (unsigned char *)&x, 2);
	 //_lwrite(bit_file->file , (unsigned char *)&y, 2);
	 //_lwrite(bit_file->file , (unsigned char *)&dataSize, 2);
    _lwrite(bit_file->file ,(char *) &comptype, 2);
	 _lwrite(bit_file->file , (char *)&x, 2);
	 _lwrite(bit_file->file , (char *)&y, 2);
	 _lwrite(bit_file->file , (char *)&dataSize, 2);


	 bit_file->rack = 0;
	 bit_file->mask = 0x80;
	 bit_file->pacifier_counter = 0;
	 return( bit_file );
}

//==========================================================================//

BIT_FILE *OpenInputBitFile( char *name )
{
	 BIT_FILE *bit_file;
	 OFSTRUCT OFData;
    ZeroMemory( &OFData, sizeof(OFData) );

	 bit_file = (BIT_FILE *) calloc( 1, sizeof( BIT_FILE ) );
	 if ( bit_file == NULL )
			return( bit_file );

	 bit_file->file = OpenFile( name,&OFData, OF_READ|OF_WRITE );
	 if ( bit_file->file == HFILE_ERROR  )
		  return 0;
	_lread(bit_file->file , &comptype, 2);
	_lread(bit_file->file, &x, 2);
	_lread(bit_file->file, &y, 2);
	_lread(bit_file->file , &dataSize, 2);
	//PostInfo("comptype %u",comptype);
	//PostInfo("x %u",x);
	//PostInfo("y %u",y);
	//PostInfo("dataSize %u",dataSize);


	 bit_file->rack = 0;
	 bit_file->mask = 0x80;
	 bit_file->pacifier_counter = 0;
	 return( bit_file );
}

//==========================================================================//

void CloseOutputBitFile( BIT_FILE *bit_file )
{
	 if ( bit_file->mask != 0x80 )
	 {
		  //if ( _lwrite(bit_file->file ,(unsigned char *) &bit_file->rack, 1)== HFILE_ERROR)
        if( _lwrite(bit_file->file ,(char *) &bit_file->rack, 1)== HFILE_ERROR )
		{
		   MessageBox( GetActiveWindow(), "Fatal error in CloseBitFile!", "Fatal error", MB_ICONINFORMATION );
		}

	  }
	 _lclose( bit_file->file );
	 free( (char *) bit_file );
}

//==========================================================================//

void CloseInputBitFile( BIT_FILE *bit_file )
{
	 _lclose( bit_file->file );
	 free( (char *) bit_file );
}

//==========================================================================//

void OutputBit( BIT_FILE *bit_file, int bit )
{
	 if ( bit )
		  bit_file->rack |= bit_file->mask;
	 bit_file->mask >>= 1;
	 if ( bit_file->mask == 0 )
	 {

	  //if ( _lwrite(bit_file->file ,(unsigned char *) &bit_file->rack, 1)== HFILE_ERROR)
     if( _lwrite(bit_file->file ,(char *) &bit_file->rack, 1)== HFILE_ERROR )
	 {
		MessageBox( GetActiveWindow(), "Fatal error in OutputBit!", "Fatal error", MB_ICONINFORMATION );
	 }

	  bit_file->rack = 0;
	  bit_file->mask = 0x80;
	 }
}

//==========================================================================//

void OutputBits( BIT_FILE *bit_file, unsigned long code, int count )
{
	 unsigned long mask;

	 mask = 1L << ( count - 1 );
	 while ( mask != 0)
	 {
		  if ( mask & code )
				bit_file->rack |= bit_file->mask;
		  bit_file->mask >>= 1;
		  if ( bit_file->mask == 0 )
		  {
			//if ( _lwrite(bit_file->file ,(unsigned char *) &bit_file->rack, 1)== HFILE_ERROR)
         if( _lwrite(bit_file->file ,(char *) &bit_file->rack, 1)== HFILE_ERROR )
		 {
			MessageBox( GetActiveWindow(), "Fatal error in OutputBit!", "Fatal error", MB_ICONINFORMATION );
		 }

			  bit_file->rack = 0;
				 bit_file->mask = 0x80;
		  }
		  mask >>= 1;
	 }
}

//==========================================================================//

int InputBit( BIT_FILE *bit_file )
{
	 int value;

	 if ( bit_file->mask == 0x80 )
	 {
		  if(!	 _lread(bit_file->file , &bit_file->rack, 1))
		  {
			MessageBox( GetActiveWindow(), "Fatal error in InputBit!", "Fatal error", MB_ICONINFORMATION );
		  }
	 }
	 value = bit_file->rack & bit_file->mask;
	 bit_file->mask >>= 1;
	 if ( bit_file->mask == 0 )
	bit_file->mask = 0x80;
	 return( value ? 1 : 0 );
}

//==========================================================================//

unsigned long InputBits( BIT_FILE *bit_file, int bit_count )
{
	 unsigned long mask;
	 unsigned long return_value;

	 mask = 1L << ( bit_count - 1 );
	 return_value = 0;
	 while ( mask != 0) {
	if ( bit_file->mask == 0x80 )
	{
		 if(!	 _lread(bit_file->file , &bit_file->rack, 1))
		 {
			MessageBox( GetActiveWindow(), "Fatal error in InputBit!", "Fatal error", MB_ICONINFORMATION );
		 }
	}
	if ( bit_file->rack & bit_file->mask )
				return_value |= mask;
		  mask >>= 1;
		  bit_file->mask >>= 1;
		  if ( bit_file->mask == 0 )
				bit_file->mask = 0x80;
	 }
	 return( return_value );
}



void FilePrintBinary( FILE *file, unsigned int code, int bits )
{
	 unsigned int mask;

	 mask = 1 << ( bits - 1 );
	 while ( mask != 0 ) {
		  if ( code & mask )
				fputc( '1', file );
		  else
				fputc( '0', file );
		  mask >>= 1;
	 }
}
