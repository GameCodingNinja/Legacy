
/************************************************************************
*    FILE NAME:       megatexturemanager.h
*
*    DESCRIPTION:     Class that combines textures into larger ones
************************************************************************/

#ifndef __mega_texture_manager_h__
#define __mega_texture_manager_h__

// Standard lib dependencies
#include <string>

// Boost lib dependencies
#include <boost/ptr_container/ptr_map.hpp>

// Forward declaration(s)
class CMegaTextureComponent;
class CMegaTexture;

// Constants
const int MAX_SIZE = 4048;

class CMegaTextureManager
{
public:

    // Get the instance of the singleton class
    static CMegaTextureManager & Instance()
    {
        static CMegaTextureManager megaTextureManager;
        return megaTextureManager;
    }

    // Get a mega texture
    CMegaTexture * GetTexture( const std::string & group );

    // Create a mega texture using the group name passed in
    void CreateMegaTexture( const std::string & group, int wLimit = MAX_SIZE );

    // Render a specific group's mega texture
    void Render( const std::string & group );

    // Delete the texture and component data associated with a specific group
    void DeleteMegaTexture( const std::string & group );

    // Clear all the Mega Textures
    void Clear();

private:

    // Constructor
    CMegaTextureManager();

    // Destructor
    ~CMegaTextureManager();

    // Map to hold the mega textures
    boost::ptr_map< const std::string, CMegaTexture > spMegaTextureMap;
    boost::ptr_map< const std::string, CMegaTexture >::iterator spMegaTextureMapIter;

};

#endif  // __mega_texture_manager_h__


