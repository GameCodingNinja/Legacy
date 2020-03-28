//*****************************************************************************
/*!
 *  \file   tokenizer.cpp
 *
 *  \brief  This class is designed to break input into 'tokens'.  These tokens
 *          are delimited within the input by whitespace.  Will tokenize an
 *          entire file, or a single line of input.
 *
 *****************************************************************************/

#include "windows.h"
#include "tokenizer.h"
#include <sys/stat.h>
#include <cassert>
#include <string.h>
#include <iostream>


//*****************************************************************************
/*!
 *  \brief  Convert the specified file into tokens based on whitespace
 *          separation.  Always ignore comments.  Comments are any section
 *          of any line that follow a ';' character.
 *
 *  \param  TTokenList & tokens - the destination token storage.
 *  \param  const string & filename - the file to convert.
 *
 *****************************************************************************/
void CTokenizer::TokenizeFile(TTokenList & tokens,
                              const std::string & filename)
{
    // File handle
    HANDLE hFile = NULL;
	HANDLE hMap = NULL;
	char *mmap_ptr = NULL;

    // Open the file as read-only
	hFile = CreateFile( const_cast<char *>(filename.c_str()),
		                GENERIC_READ,
					    FILE_SHARE_READ,
                        NULL,
					    OPEN_ALWAYS,
					    FILE_ATTRIBUTE_NORMAL,
					    NULL );

    if( hFile == INVALID_HANDLE_VALUE )
	{
		std::cerr << "Error in CTokenizer::TokenizeFile: File " << filename << " could not be opened." << std::endl;
        assert(0);
		return;
	}

    // Map the file
    hMap = CreateFileMapping( hFile, 0, PAGE_READONLY, 0, 0, 0 );
    if( hMap == NULL )
	{
        CloseHandle(hFile);
        std::cerr << "Error in CTokenizer::TokenizeFile: File " << filename << " could not create file mapping." << std::endl;
        assert(0);
		return;
    }
    
	mmap_ptr = (char *)MapViewOfFile( hMap, FILE_MAP_READ, 0, 0, 0 );
    if( mmap_ptr == NULL )
    {
        CloseHandle(hMap);
        CloseHandle(hFile);

        std::cerr << "Error in CTokenizer::TokenizeFile: File " << filename << " could not map view of file." << std::endl;
        assert(0);
		return;
    }

    // local variable for the file size
    int fileSize = GetFileSize( hFile, NULL );
	if( fileSize <= 0 )
    {
        CloseHandle( hFile );    
        UnmapViewOfFile( mmap_ptr );
        CloseHandle( hMap );

        std::cerr << "Error in CTokenizer::TokenizeFile: File " << filename << " has no size." << std::endl;
        assert(0);
		return;
    }

    // Cache the current byte pointer
    const char * pCurrentByte = mmap_ptr;

    // This pointer is pointing past the last byte in the file
    const char * pEndByte = mmap_ptr + fileSize;

    // arrays of characters that the c-library search routines use
    // must have NUL char at end of these
    const char delimiters[] = { ' ', ',' ,'\t' ,'\r', '\n', 0x0D, '\0' };
    const char delimitersWithComment[] = { ' ', ',' ,'\t' ,'\r', '\n', 0x0D, ';', '\0' };
    const char endOfLineChars[] = { '\r', '\n', 0x0D, '\0' };

    // move past any delimiter characters at the beginning of the file
    pCurrentByte += strspn( pCurrentByte, delimiters );

    // Loop through the file
    while (pCurrentByte < pEndByte)
    {
        // is the current byte is a comment-begin character
        if (*pCurrentByte == ';')
        {
            // ignore this line and move the pointer to the end of the line
            pCurrentByte = strpbrk( pCurrentByte, endOfLineChars );

            // if no end of line characters were found, then move to the end of the file
            if ( pCurrentByte == NULL )
            {
                pCurrentByte = pEndByte;
            }
        }
        else
        {
            // find the number of characters that are NOT delimiters and NOT the comment character
            size_t nextDelimiter = strcspn( pCurrentByte, delimitersWithComment );
            
            // add the word to the token data
            tokens.push_back( std::string(pCurrentByte, nextDelimiter) );

            // move the pointer past this word
            pCurrentByte += nextDelimiter;
        }

        // skip past any of the dilimiter characters
        pCurrentByte += strspn( pCurrentByte, delimiters );
    }

    // unmap the file
    CloseHandle( hFile );    
    UnmapViewOfFile( mmap_ptr );
    CloseHandle( hMap );
}


//*****************************************************************************
/*!
 *  \brief  Walk through an inputstring, splitting it into tokens based on the
 *          delimiters supplied.
 *
 *  \param  TTokenList & tokens - storage for the tokens produced.
 *  \param  const char * inputString - the string to be tokenized.
 *  \param  const char * delimiters - the delimiting characters to split on.
 *
 *****************************************************************************/
void CTokenizer::TokenizeString(TTokenList & tokens,
                                const char * pStringBuffer,
                                const char * pDelimiters)
{
    // set the local pointer to the position of the next non-delimiter character
    const char *pNextWordStart = pStringBuffer + strspn( pStringBuffer, pDelimiters );

    // loop until we're at the end of the string
    while ( *pNextWordStart != '\0' )
    {
        // Retrieve the index of the first occurence of a delimiting character.
        const unsigned int wordEndIndex = strcspn( pNextWordStart, pDelimiters );

        // Store the token represented by a substring of the input line.
        tokens.push_back(std::string(pNextWordStart, wordEndIndex ));

        // get the next position by incrementing past the word and past the
        // next delimiter character
        pNextWordStart += wordEndIndex;
        pNextWordStart += strspn( pNextWordStart, pDelimiters );
    }
}

