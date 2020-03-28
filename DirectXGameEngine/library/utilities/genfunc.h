/************************************************************************
*    FILE NAME:       genfunc.h
*
*    DESCRIPTION:     General mutipurpose functions
************************************************************************/           

#ifndef __genfunc_h__
#define __genfunc_h__

// Windows OS dependencies
#include <ShlObj.h>

// Standard lib dependencies
#include <string>
#include <assert.h>

namespace NGenFunc
{
    // Translates into a windows messagebox with sprintf capabilities.
    void PostMsg( const char *title, const char *format, ... );

    // Translates into a debug string to your VC++ Output window with
    // sprintf capabilities.
    void PostDebugMsg( const char *format, ... );

    // Count the number of occurrences of sub string
    int CountStrOccurrence( const std::string & searchStr, const std::string & subStr );

    // dynamic_cast type with error checking
    template <class target, class source>
    inline target DynCast( source * ptr )
    {
        target result = dynamic_cast<target>(ptr);
        assert(result != NULL);
        return result;
    }

    // dynamic_cast type with error checking
    template <class target, class source>
    inline target ReintCast( source * ptr )
    {
        target result = reinterpret_cast<target>(ptr);
        assert(result != NULL);
        return result;
    }

    // Get the path to default windows directories
    std::string GetWindowsFolderPath( __in REFKNOWNFOLDERID rfid );
}

#endif  // __genfunc_h__
