/************************************************************************
*    FILE NAME:       genfunc.cpp
*
*    DESCRIPTION:     General mutipurpose functions
************************************************************************/           

// Windows OS dependencies
                             // Enables strict compiler error checking. Without this, the compiler doesn't
#define STRICT               // know the difference between many Windows data types. Very bad to leave out. Doesn't add to code size
#include <windows.h>         // Windows header file for creating windows programs. This is a must have.


// Physical component dependency
#include <utilities/genfunc.h>

// Standard lib dependencies
#include <stdlib.h>  
#include <stdio.h>
#include <iostream>
#include <algorithm>

// Boost lib dependencies
#include <boost/scoped_array.hpp>

// Required namespace(s)
using namespace std;

// disable warning about unsafe functions. Can't use allocated arrays with the safe functions
#pragma warning(disable : 4996)

namespace NGenFunc
{

    /************************************************************************
    *    desc:  Count the number of occurrences of sub string
    *
    *    param: int
    ************************************************************************/
    int CountStrOccurrence( const string & searchStr, const string & subStr )
    {
        int result = 0;
        size_t found = string::npos;

        do
        {
            found = searchStr.find(subStr, found+1);

            if( found != string::npos )
                ++result;
        }
        while( found != string::npos );

        return result;

    }	// CountStrOccurrence


    /************************************************************************
    *    desc: Translates into a windows messagebox with sprintf capabilities.
    *
    *    param: char *title - Title of the windows message box
    *           char *format - Used with vprintf to get the argument list
    *                            for wvsprintf.
    ************************************************************************/
    void PostMsg( const char *title, const char *format, ... )
    {
        va_list args;

        va_start( args, format );

        const int TERMINATOR_FLUFF(5);

        // Get the number of characters in this arguement list. Add some fluff for terminator
        int length = _vscprintf( format, args ) + TERMINATOR_FLUFF;
        assert( (length > TERMINATOR_FLUFF) && "Parameter length not valid. NGenFunc::PostMsg" );
        
        // Allocate based on character length
        boost::scoped_array<char> strBuffer( new char[length] );

        // Combine into one string
        vsprintf( strBuffer.get(), format, args );

        #ifdef UNICODE
        cout << title << ": " << strBuffer.get() << endl;
        #else
        MessageBox( GetActiveWindow(), strBuffer.get(), title, MB_ICONINFORMATION );
        #endif

        va_end( args );
        
    }   // PostMsg


    /************************************************************************
    *    desc: Translates into a debug string to your VC++ Output window with
    *          sprintf capabilities.
    *
    *		example: PostDebugMsg( "%d", 7 );
    *
    *    param: char *fmt   - Used with vprintf to get the argument list
    *                         for vsprintf_s.
    ************************************************************************/
    void PostDebugMsg( const char *format, ... )
    {
        va_list args;

        va_start( args, format );

        const int TERMINATOR_FLUFF(5);

        // Get the number of characters in this arguement list. Add some fluff for terminator
        int length = _vscprintf( format, args ) + TERMINATOR_FLUFF;
        assert( (length > TERMINATOR_FLUFF) && "Parameter length not valid. NGenFunc::PostDebugMsg" );

        // Allocate based on character length
        boost::scoped_array<char> strBuffer( new char[length] );

        // Combine into one string
        vsprintf( strBuffer.get(), format, args );
        strcat( strBuffer.get(), "\n" );

        #ifdef UNICODE
        string s(strBuffer.get());
        wstring temp(s.length(),L' ');  
        copy(s.begin(), s.end(), temp.begin());
        OutputDebugString( temp.c_str() );
        cout << strBuffer << endl;
        #else
        OutputDebugString( strBuffer.get() );
        #endif

        va_end( args );

    }   // PostDebugMsg


    /************************************************************************
    *    desc: Get the path to default windows directories. For Vista and greater
    *
    *    param: rfid - DEFINE_KNOWN_FOLDER
    ************************************************************************/
    string GetWindowsFolderPath( __in REFKNOWNFOLDERID rfid )
    {
        string result;
        wchar_t* wpath = NULL;

        if( SUCCEEDED(SHGetKnownFolderPath(rfid, 0, NULL, &wpath)) )
        {
            // Convert to byte character string
            char charPath[MAX_PATH];
            wcstombs( charPath, wpath, sizeof(charPath) );
            // mbstowcs - Convert multibyte string to wide-character string

            #ifdef UNICODE
            DWORD ftyp = GetFileAttributes(wpath);
            #else
            DWORD ftyp = GetFileAttributes(charPath);
            #endif

            // Copy it to our string if the directory is really there 
            if( ftyp & FILE_ATTRIBUTE_DIRECTORY )
                result = charPath;

            // Free the memory returned from windows SHGetKnownFolderPath function
            CoTaskMemFree(wpath);
        }

        return result;

    }	// GetWindowsFolderPath

}	// NGenFunc

